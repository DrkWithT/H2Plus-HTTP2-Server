#ifndef INTXCODER_HPP
#define INTXCODER_HPP

#include "utils/octarr.hpp"

/**
 * @brief Helper function to determine if an encoded integer is a tiny integer, specifically if it fits in an n-bit prefix. 
 * 
 * @param prefix_n 
 * @param head_octet 
 * @return true 
 * @return false 
 */
constexpr bool is_target_tiny(uint8_t prefix_n, uint8_t head_octet);

/**
 * @brief Encodes integer values in HPACK encoding.
 */
class IntegerEncoder {
private:
    uint32_t prefix;        // stores bit count of integer prefix
    uint32_t encoding_offset; // stores total offset of octets from start of overall encoding

    bool encode_tiny_int(OctetArray& buffer, uint32_t target);
    bool encode_big_int(OctetArray& buffer, uint32_t target);
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
    uint32_t encoding_offset;  // offset from overall encoding in octets
    uint16_t bit_count;      // bit count in decoded value
    uint16_t bit_limit;      // bit count maximum of decoded value
    uint8_t prefix;          // N-bit prefix of integer to decode

    uint32_t decode_tiny_int(const OctetArray& buffer);
    uint32_t decode_big_int(const OctetArray& buffer);
public:
    IntegerDecoder();
    void set_offset(uint32_t offset);
    void set_prefix(uint8_t prefix_n);
    uint32_t decode_int(const OctetArray& buffer);
    void reset();
};

#endif