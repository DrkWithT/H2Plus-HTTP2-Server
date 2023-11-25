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
        uint32_t huffbits = this->huffcodes_ptr[symbol].code_length;
        uint32_t huffcode_bitmask = 1U << (huffbits - 1);

        for (uint32_t bitmask_i = 0; bitmask_i < huffbits; bitmask_i++) {
            bool bit_flag = 1U == ((huffcode & huffcode_bitmask) >> (huffbits - (bitmask_i + 1)));
    
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
        for (uint32_t eos_i = 0U; eos_i < padding_count; eos_i++) {
            if (!result.append(true)) {
                encode_count = 0U;
                return encode_count;
            }

            encode_count++;
        }
    }
    
    return encode_count;
}

/* Huffman Decoder Public Impl. */

HuffmanDecoder::HuffmanDecoder(const HuffmanCodePair* huffcodes) : huffcode_tree {} {
    this->is_ready = true;

    for (uint32_t huffcode_i = 0U; huffcode_i < HUFFCODE_PAIR_COUNT; huffcode_i++) {
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
    int32_t data_length = raw_octets.get_length();

    // Do not attempt to decode into invalid OctetArray data e.g length = -1
    if (data_length < 1) {
        return false;
    }

    BitArray result_bitstr {OCTET_BITS * data_length};
    bool curr_bit = false;
    bool decode_ok = true;

    // Copy all bits of `raw_octets` into `result_bitstr`
    for (int32_t copy_i = 0; copy_i < data_length; copy_i++) {
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

    for (int32_t peek_i = 0; peek_i < result_bit_span; peek_i++) {
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
    uint32_t temp_huffcode = huffcode->code_number;
    uint32_t code_span = huffcode->code_length;

    return this->huffcode_tree.put_symbol(temp_huffcode, code_span, symbol);
}
