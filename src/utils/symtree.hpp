#ifndef SYMTREE_HPP
#define SYMTREE_HPP

#include <string>
#include "utils/bitarr.hpp"

/**
 * @brief A binary tree node to store an ASCII symbol (code 0-255) at the end of a left-right path. Also contains other info to discern between an `EOS` and `\0` symbol.
 */
struct SymbolNode {
    struct SymbolNode* left;
    struct SymbolNode* right;
    uint8_t symbol; // letter in extended ASCII
    bool is_eos;    // end marker of HPACK Huffman code
};

SymbolNode* symbol_node_create(uint8_t symbol_char, bool is_eos, SymbolNode* left_ptr, SymbolNode* right_ptr);

void symbol_node_destroy(SymbolNode* root);

bool is_leaf(const SymbolNode* node);

bool is_eos(const SymbolNode* node);

/**
 * @brief A container that stores a binary tree mapping bitstrings as tree paths to reach any statically Huffman coded symbol. 
 */
class SymbolTree {
private:
    SymbolNode* root;
public:
    SymbolTree();
    ~SymbolTree();
    const SymbolNode* get_root_symbol_node() const;
    bool put_symbol(uint32_t code, bool is_eos, uint32_t code_length, uint8_t symbol_octet);
};

#endif