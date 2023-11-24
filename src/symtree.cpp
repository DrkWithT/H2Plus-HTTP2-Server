/**
 * @file symtree.cpp
 * @author Derek Tan
 * @brief Implements the bit-sequence to symbol tree. Used for HPACK.
 * @date 2023-11-22
 */

#include "utils/symtree.hpp"

/* Constants */

constexpr uint32_t HPACK_HUFFCODE_EOS = 0x3fffffff;

/* SymbolNode Impl. */

SymbolNode* symbol_node_create(uint8_t symbol_char, bool is_eos, SymbolNode* left_ptr, SymbolNode* right_ptr) {
    SymbolNode* node = new SymbolNode;

    if (node != nullptr) {
        node->symbol = symbol_char;
        node->is_eos = is_eos;
        node->left = left_ptr;
        node->right = right_ptr;
    }

    return node;
}

void symbol_node_destroy(SymbolNode* root) {
    if (!root) {
        return;
    }

    symbol_node_destroy(root->left);
    symbol_node_destroy(root->right);
    delete root;
}

bool is_leaf(const SymbolNode* node) {
    return !node->left && !node->right;
}

bool is_eos(const SymbolNode* node) {
    return node->is_eos;
}

/* SymbolTree Impl. */

SymbolTree::SymbolTree() {
    this->root = nullptr;
}

SymbolTree::~SymbolTree() {
    symbol_node_destroy(this->root);
    this->root = nullptr;
}

SymbolNode* SymbolTree::get_root_symbol_node() const {
    return this->root;
}

bool SymbolTree::put_symbol(uint32_t code, uint32_t code_length, uint8_t symbol_octet) {
    bool has_placed = true;
    SymbolNode* temp_cursor = this->root;
    bool has_new_eos = code == HPACK_HUFFCODE_EOS;
    uint32_t code_bitmask = 1 << (code_length - 1);

    for (uint32_t i = 0; i < code_length; i++){
        // Track current position in bitstring of Huffman code.
        bool bit_flag = 1 == ((code & code_bitmask) >> (code_length - (i + 1)));
        uint8_t symbol = 0;

        if (i == code_length - 1U) {
            // Prepare to place symbol at end of path.
            symbol = symbol_octet;
        }

        if (bit_flag) {
            if (!temp_cursor->left) {
                temp_cursor->left = symbol_node_create(symbol, has_new_eos, nullptr, nullptr);
            }

            temp_cursor = temp_cursor->left;
        } else {
            if (!temp_cursor->right) {
                temp_cursor->right = symbol_node_create(symbol, has_new_eos, nullptr, nullptr);
            }

            temp_cursor = temp_cursor->right;
        }

        if (!temp_cursor) {
            // Exit early if allocation of node still fails.
            has_placed = false;
            break;
        } 

        code_bitmask >>= 1;
    }

    return has_placed;
}
