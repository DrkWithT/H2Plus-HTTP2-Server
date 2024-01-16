/**
 * @file intxcoder.cpp
 * @author Derek Tan
 * @brief Implements HPACK integer encoder and decoder.
 * @date 2023-11-27
 */

#include "hpack/intxcoder.hpp"

/* Constants */

/// @brief Special constant for encoding any "larger" integer in HPACK.
constexpr uint32_t HPACK_TERMINAL_TINY_INT = 128U;
constexpr uint32_t HPACK_INT_CHUNK_VALUE = 127U;
constexpr uint32_t HPACK_MAX_INT_OCTETS = 8U;

/* Helper Impl. */

constexpr uint8_t get_prefix_mask(uint8_t prefix_n) {
    return (1 << prefix_n) - 1;
}

/* IntegerEncoder Impl. */

IntegerEncoder::IntegerEncoder() {
    prefix = 0U;
    encoding_count = 0U;
}

uint32_t IntegerEncoder::get_relative_offset() const
{
    return encoding_count;
}

void IntegerEncoder::set_prefix(uint32_t prefix_n) {
    this->prefix = prefix_n;
}

void IntegerEncoder::set_offset(uint32_t offset) {
    this->encoding_count = offset;
}

void IntegerEncoder::reset() {
    set_prefix(0U);
    encoding_count = 0U;
}

uint32_t IntegerEncoder::encode_int(OctetArray& buffer, uint32_t target) {
    /// @todo Review RFC 7541 Section 5.1 for pseudocode!
    uint32_t target_accumulator = target;
    uint8_t prefix_mask = get_prefix_mask(prefix);
    uint8_t temp_octet = 0;

    if (target < prefix_mask) {
        temp_octet = target;
        buffer.set_octet(0, temp_octet);        
        encoding_count++;

        return encoding_count;
    }

    /// @note Unconditionally encode the 1st octet of the integer in an N bit prefix to follow the RFC.
    temp_octet = prefix_mask;
    buffer.set_octet(encoding_count, temp_octet);
    encoding_count++;

    target_accumulator -= prefix_mask;

    /// @note Follows RFC 7541 5.1 pseudocode for correct encoding of middle octets in the encoding.
    while (target_accumulator >= HPACK_TERMINAL_TINY_INT) {
        if (encoding_count >= buffer.get_length()) {
            // Reject excessive octet counts in encoding to avoid buffer overflow.
            encoding_count = 0U;
            break;
        }

        temp_octet = HPACK_TERMINAL_TINY_INT | (target_accumulator & HPACK_INT_CHUNK_VALUE);

        buffer.set_octet(encoding_count, temp_octet);

        target_accumulator /= HPACK_TERMINAL_TINY_INT;

        encoding_count++;
    }

    /// @note Put and then skip the last encoded octet of integer since loop terminates 1 iteration early.
    buffer.set_octet(encoding_count, (target_accumulator & HPACK_INT_CHUNK_VALUE));
    encoding_count++;

    return encoding_count;
}

/* IntegerDecoder Impl. */

IntegerDecoder::IntegerDecoder() {
    decoding_offset = 0U;
    prefix = 0U;
}

uint32_t IntegerDecoder::get_relative_offset() const
{
    return decoding_offset;
}

void IntegerDecoder::set_offset(uint32_t offset) {
    this->decoding_offset = offset;
}

void IntegerDecoder::set_prefix(uint8_t prefix_n) {
    this->prefix = prefix_n;
}

uint32_t IntegerDecoder::decode_int(const OctetArray& buffer) {
    uint32_t result = 0U;
    uint8_t prefix_mask = get_prefix_mask(prefix);
    uint8_t temp_chunk = buffer.get_octet(decoding_offset) & prefix_mask;

    if (temp_chunk != prefix_mask) {
        /// @note Handles 1 octet integers where no additional chunks need to be decoded!
        result = temp_chunk;
        decoding_offset++;
        return result;
    }

    uint8_t temp_octet = 0; // raw octet from buffer
    uint32_t chunk_count = 0U;
    uint32_t multiplier = 1U; // power of 2 to multiply with result accumulator term
    result = temp_chunk;
    decoding_offset++;

    while (decoding_offset < buffer.get_length() && chunk_count <= HPACK_MAX_INT_OCTETS) {
        temp_octet = buffer.get_octet(decoding_offset);
        temp_chunk = temp_octet & HPACK_INT_CHUNK_VALUE;

        result += temp_chunk * multiplier;

        if ((temp_octet & HPACK_TERMINAL_TINY_INT) == 0) {
            /// @note Handle flag 0 of encoding chunk as END so that H(un)PACK is correct.
            decoding_offset++;
            chunk_count++;
            break;
        }

        if ((multiplier << 7) < multiplier) {
            // Handle overflowing multiplier with decoding error.
            result = 0U;
            break;
        }

        multiplier <<= 7;
        decoding_offset++;
    }

    return result;
}

void IntegerDecoder::reset() {
    set_offset(0U);
    set_prefix(0U);
}
