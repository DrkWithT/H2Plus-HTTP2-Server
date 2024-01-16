#ifndef INTXCODER_HPP
#define INTXCODER_HPP

#include "utils/octarr.hpp"

/**
 * @brief Helper function to generate a first octet bitmask for HPACK encoded integers.
 * 
 * @param prefix_n  
 * @returns int8_t
 */
constexpr uint8_t get_prefix_mask(uint8_t prefix_n);

/**
 * @brief Encodes integer values in HPACK encoding.
 */
class IntegerEncoder {
private:
    uint32_t prefix;        // stores bit count of integer prefix
    uint32_t encoding_count; // stores total offset of octets from start of overall encoding
public:
    IntegerEncoder();
    void set_prefix(uint32_t prefix_n);
    void set_offset(uint32_t offset);
    uint32_t encode_int(OctetArray& buffer, uint32_t target);
    void reset();
};

/**
 * @brief Decodes HPACKed integer values based on a given n-bit prefix.
 */
class IntegerDecoder {
private:
    uint32_t decoding_offset;  // offset from 1st octet in decoding
    uint8_t prefix;          // N-bit prefix of integer to decode
public:
    IntegerDecoder();
    void set_offset(uint32_t offset);
    void set_prefix(uint8_t prefix_n);
    uint32_t decode_int(const OctetArray& buffer);
    void reset();
};

#endif