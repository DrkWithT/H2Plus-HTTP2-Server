#ifndef HUFFXCODERS_HPP
#define HUFFXCODERS_HPP

#include "hpack/tables.hpp"
#include "utils/octarr.hpp"
#include "utils/symtree.hpp"

/**
 * @brief Encodes an ASCII string into a 1-padded bit sequence according to static Huffman codes. The bits are stored in a BitArray object.
 * @note For HTTP/2 header Huffman compression.
 */
class HuffmanEncoder {
private:
    const HuffmanCodePair* huffcodes_ptr; // pointer to statically created huffman code structs
public:
    HuffmanEncoder(const HuffmanCodePair* huffcodes);
    uint32_t encode(BitArray& result, const std::string& text);
};

/**
 * @brief Decodes contents from a 1-padded bit sequence according to the static Huffman codes. The refernced bit sequence is a BitArray.
 * @note For HTTP/2 header Huffman decompression.
 * @todo Check Tippy2_Server/hpack/huffxcoders.py for general logic.
 */
class HuffmanDecoder {
private:
    SymbolTree huffcode_tree;
    const SymbolNode* tree_cursor;
    bool is_ready;

    void reset_cursor();
    bool load_huffcode(const HuffmanCodePair* huffcode, uint8_t symbol);
public:
    HuffmanDecoder(const HuffmanCodePair* huffcodes);
    bool setup_valid() const;
    bool decode(std::string& result, const OctetArray& raw_octets);
};

#endif