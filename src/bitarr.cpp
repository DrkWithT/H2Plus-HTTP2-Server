/**
 * @file bitarr.cpp
 * @author Derek Tan
 * @brief Implements custom bit array wrapper.
 * @date 2023-11-18
 */

#include "utils/bitarr.hpp"

BitArray::BitArray() {
    const int32_t default_capacity = DEFAULT_BIT_CAPACITY / OCTET_BITS;
    
    this->octets = new uint8_t[default_capacity];

    if (this->octets != nullptr) {
        std::memset(this->octets, 0, default_capacity);
        this->bit_capacity = default_capacity;
        this->bit_length = 0;
    } else {
        this->bit_capacity = -1;
        this->bit_length = -1;
    }
}

BitArray::BitArray(int32_t bit_count) {
    int32_t checked_capacity = (bit_count < DEFAULT_BIT_CAPACITY)
        ? DEFAULT_BIT_CAPACITY
        : bit_count;
    
    checked_capacity /= OCTET_BITS;

    this->octets = new uint8_t[checked_capacity];

    if (this->octets != nullptr) {
        std::memset(this->octets, 0, checked_capacity);
        this->bit_capacity = checked_capacity;
        this->bit_length = 0;
    } else {
        this->bit_capacity = -1;
        this->bit_length = -1;
    }
}

BitArray::~BitArray() {
    if (this->octets != nullptr) {
        delete[] this->octets;
        this->octets = nullptr;
    }
}

int32_t BitArray::length() const {
    return this->bit_length;
}

bool BitArray::at(int32_t bit_pos) {
    int32_t octet_pos = bit_pos / OCTET_BITS;
    uint32_t bit_offset = bit_pos - (OCTET_BITS * octet_pos);
    uint8_t octet = this->octets[octet_pos];

    return 1 == ((octet & (128 >> bit_offset)) >> (MAX_OCTET_N - bit_offset));
}

void BitArray::put(int32_t bit_pos, bool bit)
{
    int32_t octet_pos = bit_pos / OCTET_BITS;
    uint8_t bit_offset = static_cast<uint8_t>(bit_pos - (OCTET_BITS * octet_pos));

    if (bit) {
        this->octets[octet_pos] |= (1 << (MAX_OCTET_N - bit_offset));
    }
}

bool BitArray::append(bool bit) {
    if (!this->octets) {
        return false;
    }

    int32_t curr_bit_length = this->bit_length;
    int32_t curr_bit_capacity = this->bit_capacity;
    int32_t new_bit_capacity = 2 * curr_bit_capacity;

    if (curr_bit_length >= curr_bit_capacity) {
        int32_t new_octet_capacity = new_bit_capacity / OCTET_BITS;
        int32_t old_octet_capacity = curr_bit_capacity / OCTET_BITS;
        uint8_t* new_bitbuf = new uint8_t[new_octet_capacity];

        if (new_bitbuf != nullptr) {
            std::memcpy(new_bitbuf, this->octets, old_octet_capacity);
            std::memset(new_bitbuf + old_octet_capacity, 0, new_octet_capacity - old_octet_capacity);
            
            delete[] this->octets;
            this->octets = new_bitbuf;
            this->bit_capacity = new_bit_capacity;
        } else {
            return false;
        }
    }

    put(curr_bit_length, bit);
    this->bit_length++;

    return true;
}

bool BitArray::append(uint8_t octet) {
    uint8_t bitmask = 128;

    for (int put_count = 0; put_count < OCTET_BITS; put_count++) {
        if (!append(1 == ((octet & bitmask) >> (MAX_OCTET_N - put_count)))) {
            return false;
        }

        bitmask >>= 1;
    }

    return true;
}
