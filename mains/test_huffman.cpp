/**
 * @file test_huffman.cpp
 * @author Derek Tan
 * @brief Implements some unit tests for `HuffmanEncoder` & `HuffmanDecoder`.
 * @date 2023-11-23
 */

#include <iostream>
#include "hpack/huffxcoders.hpp"

int main() {
    // Test data
    std::string text1 {"text/html"}; // fake header value to encode
    std::string text2 {}; // resulting decoded fake header
    BitArray encoded_bits {}; // empty bitarr
    OctetArray encoded_octets {}; // padded and Huffman coded fake header

    // Initialize Huffman utils
    HuffmanEncoder encoder {STATIC_HUFFMAN_CODES};
    HuffmanDecoder decoder {STATIC_HUFFMAN_CODES};

    // Test if decoder is ready
    if (!decoder.setup_valid()) {
        std::cerr << "Decoder failed to load static codes!" << std::endl;
        return 1;
    }

    uint32_t encode_bit_count = encoder.encode(encoded_bits, text1);

    // Test if encode completed with no error
    if (encode_bit_count < 1) {
        std::cerr << "Encoder failed: the bitarray failed to append." << std::endl;
        return 1;
    }

    encoded_octets << encoded_bits; // Copy bit-encoded octets from encoder to  raw unsigned octet sequence

    // Test simple decode
    if (!decoder.decode(text2, encoded_octets)) {
        std::cerr << "Decoder failed: bitarray failed to append." << std::endl;
        return 1;
    }

    if (text1 != text2)
    {
        std::cerr << "Decoder failed: incorrect text1 vs. expected text2." << std::endl;
        return 1;
    }

    return 0;
}
