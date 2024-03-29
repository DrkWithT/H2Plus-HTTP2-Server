/**
 * @file test_intxcoder.cpp
 * @author Derek Tan
 * @brief Implements unit test for HPACK integer utilities.
 * @date 2023-12-01
 */

#include "hpack/intxcoder.hpp"
#include <iostream>

int main() {
    // Create testable encoder and decoder
    IntegerEncoder encoder {};
    IntegerDecoder decoder {};

    // Create octet buffers for testing
    OctetArray tiny_int_buffer {};
    OctetArray bigger_int_buffer {};

    // Define test inputs
    uint32_t tiny_int = 10U;
    uint32_t bigger_int = 1337U;
    uint32_t prefix = 5U;

    // Encode and decode tinier integer within 5 bit prefix... check results!
    encoder.set_prefix(prefix);
    decoder.set_prefix(prefix);

    if (encoder.encode_int(tiny_int_buffer, tiny_int) != 1U) {
        std::cerr << "IntegerEncoder gave invalid encoding offset for tiny_int." << std::endl;
        return 1;
    }

    uint32_t temp = decoder.decode_int(tiny_int_buffer);

    if (temp != tiny_int) {
        std::cerr << "IntegerDecoder gave invalid decoded value from tiny_int_buffer: " << temp << std::endl;
        return 1;
    }

    // Encode and decode bigger integer with prefix 5... check results!
    encoder.reset();
    decoder.reset();
    encoder.set_prefix(prefix);
    decoder.set_prefix(prefix);

    if (encoder.encode_int(bigger_int_buffer, bigger_int) != 3U) {
        std::cerr << "IntegerEncoder gave invalid encoding offset for bigger_int." << std::endl; 
        return 1;
    }

    temp = decoder.decode_int(bigger_int_buffer);

    if (temp != bigger_int) {
        std::cerr << "IntegerDecoder gave invalid decoded value from bigger_int_buffer: " << temp << std::endl;
        return 1;
    }

    return 0;
}
