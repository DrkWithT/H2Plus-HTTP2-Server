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

/* Helper Impl. */

constexpr bool is_target_tiny(uint8_t prefix_n, uint8_t head_octet) {
    return head_octet < (1 << prefix_n) - 1;
}

/* IntegerEncoder Impl. */

IntegerEncoder::IntegerEncoder() {
    this->prefix = 0U;
    this->encoding_offset = 0U;
}

void IntegerEncoder::set_prefix(uint32_t prefix_n) {
    this->prefix = prefix_n;
}

void IntegerEncoder::set_offset(uint32_t offset) {
    this->encoding_offset = offset;
}

void IntegerEncoder::reset() {
    this->set_prefix(0U);
    this->encoding_offset = 0U;
}

uint32_t IntegerEncoder::encode_int(OctetArray& buffer, uint32_t target) {
    /// @todo Review RFC 7541 Section 5.1 for pseudocode!
    if (!this->encode_tiny_int(buffer, target)) {
        this->encode_big_int(buffer, target);
    }

    return this->encoding_offset;
}

/* IntegerEncoder Private Impl. */

bool IntegerEncoder::encode_tiny_int(OctetArray& buffer, uint32_t target) {
    uint8_t tiny_int = target & ((1 << this->prefix) - 1);

    if (!is_target_tiny(this->prefix, tiny_int)) {
        return false;
    }

    buffer.set_octet(this->encoding_offset, tiny_int);
    this->encoding_offset++;

    return true;
}

bool IntegerEncoder::encode_big_int(OctetArray& buffer, uint32_t target) {
    // Do not accept tiny integers that fill 2^N - 1!
    uint8_t tiny_int = target & ((1 << this->prefix) - 1);

    if (is_target_tiny(this->prefix, tiny_int)) {
        return false;
    }

    uint32_t temp_encoding_span = this->encoding_offset;
    uint32_t target_accumulator = target;
    uint8_t temp_octet = 0U;

    /// @note Put filled prefix of larger integer first... reuse tiny_int because it's guaranteed to exceed 2^N - 1 by this point.
    buffer.set_octet(temp_encoding_span, tiny_int);
    temp_encoding_span++;

    // Put rest of target's encoding as octets
    target_accumulator -= static_cast<uint32_t>(tiny_int);

    while (target_accumulator >= 128U) {
        temp_octet = (target_accumulator & HPACK_TERMINAL_TINY_INT) + HPACK_TERMINAL_TINY_INT;
        buffer.set_octet(temp_encoding_span, temp_octet);

        target_accumulator /= HPACK_TERMINAL_TINY_INT;
        temp_encoding_span++;
    }

    // Encode last octet for integer...
    tiny_int = target_accumulator & ((1 << 8) - 1U);
    buffer.set_octet(temp_encoding_span, tiny_int);
    temp_encoding_span++;

    // Update total encoding span
    this->encoding_offset = temp_encoding_span;

    return false;
}

/* IntegerDecoder Impl. */

IntegerDecoder::IntegerDecoder() {
    this->encoding_offset = 0U;

}

void IntegerDecoder::set_offset(uint32_t offset) {
    this->encoding_offset = offset;
}

void IntegerDecoder::set_prefix(uint8_t prefix_n) {
    this->prefix = prefix_n;
}

uint32_t IntegerDecoder::decode_int(const OctetArray& buffer) {
    uint32_t result = 0U;
    uint32_t checked_tiny_int = this->decode_tiny_int(buffer);

    if (buffer.get_octet(this->encoding_offset) != 0 && checked_tiny_int == 0U) {
        result = this->decode_big_int(buffer);
    }

    return result;
}

void IntegerDecoder::reset() {
    this->set_offset(0U);
    this->set_prefix(0U);
}

/* IntegerDecoder Private Impl. */

uint32_t IntegerDecoder::decode_tiny_int(const OctetArray& buffer) {
    uint8_t tiny_int = buffer.get_octet(this->encoding_offset);
    tiny_int &= ((1 << this->prefix) - 1);

    if (!is_target_tiny(this->prefix, tiny_int)) {
        return 0U;
    }

    this->encoding_offset++;

    return tiny_int;
}

uint32_t IntegerDecoder::decode_big_int(const OctetArray& buffer) {
    uint32_t int_accumulator = 0U;
    uint32_t main_bit_count = 0U;
    uint8_t temp_octet = 0;

    do {
        temp_octet = buffer.get_octet(this->encoding_offset);
        int_accumulator += (temp_octet & 127) * (1 << main_bit_count);
        main_bit_count += 7U;
    } while (temp_octet & HPACK_TERMINAL_TINY_INT == HPACK_TERMINAL_TINY_INT);

    return int_accumulator;
}
