/**
 * @file huffxcoders.cpp
 * @author Derek Tan
 * @brief Implements Huffman utils for my HPACK implementation.
 * @date 2023-11-23
 */

#include "hpack/huffxcoders.hpp"

constexpr uint32_t HUFFCODE_PAIR_COUNT = 256U;
constexpr size_t HPACK_TEXT_MAX_LEN = 1UL << 16;

/* HuffmanEncoder Impl. */

HuffmanEncoder::HuffmanEncoder(const HuffmanCodePair* huffcodes) {
    this->huffcodes_ptr = huffcodes;
}

uint32_t HuffmanEncoder::encode(BitArray& result, const std::string& text) {
    uint32_t encode_count = 0U; // bits encoded
    size_t text_length = text.length();

    // Check size validity of header string to compress.
    if (text_length >= HPACK_TEXT_MAX_LEN) {
        return encode_count; // Do not encode string if the size is invalid (too large).
    }

    uint32_t checked_text_length = static_cast<uint32_t>(text_length);

    for (uint32_t i = 0U; i < checked_text_length; i++) {
        uint8_t symbol = static_cast<uint8_t>(text[i]);
        uint32_t huffcode = this->huffcodes_ptr[symbol].code_number;
        uint32_t huffbits = this->huffcodes_ptr[symbol].code_length - 1;
        uint32_t huffcode_bitmask = 1U << huffbits;

        for (uint32_t bitmask_i = 0; bitmask_i <= huffbits; bitmask_i++) {
            bool bit_flag = 1U == ((huffcode & huffcode_bitmask) >> (huffbits - bitmask_i));
    
            if (!result.append(bit_flag)) {
                encode_count = 0U; // On allocation error in append, exit early with failed encode count 0. 
                return encode_count;
            }

            huffcode_bitmask >>= 1;
            encode_count++;
        }
    }

    uint32_t padding_count = 8U - (encode_count & 7U);

    /// @note If encode_count is on an octet boundary, place padding of 1's until so. This caps an "EOS" symbol (all ones) to the Huffman encoded bitstring.
    if (padding_count != OCTET_BITS) {
        for (int32_t eos_i = 0U; eos_i < padding_count; eos_i++) {
            if (!result.append(true)) {
                encode_count = 0U;
                return encode_count;
            }
        }

        encode_count++;
    }
    
    return encode_count;
}

/* Huffman Decoder Public Impl. */

HuffmanDecoder::HuffmanDecoder(const HuffmanCodePair* huffcodes) : huffcode_tree {} {
    this->is_ready = true;

    for (uint8_t huffcode_i = 0; huffcode_i < HUFFCODE_PAIR_COUNT; huffcode_i++) {
        if (!load_huffcode(&huffcodes[huffcode_i], huffcode_i)) {
            this->is_ready = false;
            break;
        }
    }

    if (this->is_ready) {
        this->tree_cursor = huffcode_tree.get_root_symbol_node();
    } else {
        this->tree_cursor = nullptr;
    }
}

bool HuffmanDecoder::setup_valid() const {
    return this->is_ready;
}

bool HuffmanDecoder::decode(std::string& result, const OctetArray& raw_octets) {
    uint32_t data_length = raw_octets.get_length();
    BitArray result_bitstr {OCTET_BITS * data_length};
    bool curr_bit = false;
    bool decode_ok = true;

    // Copy all bits of `raw_octets` into `result_bitstr`
    for (uint32_t copy_i = 0U; copy_i < data_length; copy_i++) {
        uint8_t bitmask_n = 128;
        const uint8_t temp_octet = raw_octets.get_octet(copy_i);

        // Append each octet as 8 bit chunk
        if (!result_bitstr.append(temp_octet)) {
            decode_ok = false;
            return decode_ok;
        }
    }
    
    // Decode raw octets by bitstring to plaintext
    int32_t result_bit_span = result_bitstr.length();
    uint8_t node_symbol = 0;

    for (uint32_t peek_i = 0; peek_i < result_bit_span; peek_i++) {
        if (!this->tree_cursor) {
            break;
        }

        curr_bit = result_bitstr.at(peek_i);

        if (is_leaf(this->tree_cursor)) {
            node_symbol = tree_cursor->symbol;

            if (!tree_cursor->is_eos) {
                result += node_symbol;
                reset_cursor();
            } else {
                reset_cursor();
                break;
            }
        }

        if (curr_bit) {
            tree_cursor = tree_cursor->right;
        } else {
            tree_cursor = tree_cursor->left;
        }
    }

    return decode_ok;
}

/* HuffmanDecoder Private Impl. */

void HuffmanDecoder::reset_cursor() {
    this->tree_cursor = huffcode_tree.get_root_symbol_node();
}

bool HuffmanDecoder::load_huffcode(const HuffmanCodePair* huffcode, uint8_t symbol) {
    SymbolNode* temp_cursor = this->huffcode_tree.get_root_symbol_node();
    uint32_t temp_huffcode = huffcode->code_number;
    uint32_t code_span = huffcode->code_length - 1;
    uint32_t bitmask = 1U << code_span;
    bool has_eos = symbol == static_cast<uint8_t>(HUFFCODE_PAIR_COUNT - 1U);

    for (int32_t bit_i = 0; bit_i <= code_span; bit_i++) {
        // Decodes bit flag from `temp_huffcode`
        bool bit_flag = (temp_huffcode & bitmask) >> (code_span - bit_i);
        uint8_t curr_symbol = 0;
        
        // When the path finishes, prepare to insert symbol
        if (bit_i == code_span) {
            curr_symbol = symbol;
        }

        // Expand the tree with each code-path
        if (bit_flag) {
            if (!temp_cursor->right) {
                temp_cursor->right = symbol_node_create(curr_symbol, has_eos, nullptr, nullptr);
            }

            temp_cursor = temp_cursor->right;
        } else {
            if (!temp_cursor->left) {
                temp_cursor->left = symbol_node_create(curr_symbol, has_eos, nullptr, nullptr);
            }

            temp_cursor = temp_cursor->left;
        }

        // Advance forward on bitstring for path
        bitmask >>= 1;
    }
}
