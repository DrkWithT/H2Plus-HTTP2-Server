/**
 * @file test_bitarr.cpp
 * @author Derek Tan
 * @brief Unit test for my own BitArray class.
 * @date 2023-11-20
 */

#include "utils/bitarr.hpp"
#include <string>
#include <iostream>

int main () {
    std::string text_blob {"ABCD"};
    const size_t text_blob_len = text_blob.size();
    BitArray text_bits {};

    // Test append and put methods!
    for (size_t txt_idx = 0; txt_idx < text_blob_len; txt_idx++) {
        if (!text_bits.append(static_cast<uint8_t>(text_blob[txt_idx]))) {
            std::cerr << "Failed to put byte at pos = " << txt_idx << '\n';
            break;
        }
    }

    // Test length() method!
    const int32_t text_bits_length = text_bits.length();

    if (text_bits_length != OCTET_BITS * static_cast<int32_t>(text_blob_len)) {
        std::cerr << "Incorrect length found vs 8 * text_blob_len: " << text_bits_length << '\n';
    }

    // Test at() method!
    for (int32_t bit_idx = 0; bit_idx < text_bits_length; bit_idx++) {
        if (text_bits.at(bit_idx)) {
            std::cout << '1';
        } else {
            std::cout << '0';
        }
    }

    std::cout << std::endl;

    return 0;
}
