/**
 * @file octarr.cpp
 * @author Derek Tan
 * @brief Implements the dynamic octet array.
 * @date 2023-11-22
 */

#include <algorithm>
#include "utils/octarr.hpp"

/* Constexprs */

constexpr uint32_t OCTET_ARRAY_DEFAULT_CAPACITY = 1024;

/* OctetArray Impl. */

OctetArray::OctetArray() {
    this->octets = new uint8_t[OCTET_ARRAY_DEFAULT_CAPACITY];

    if (this->octets != nullptr) {
        this->length = static_cast<uint32_t>(OCTET_ARRAY_DEFAULT_CAPACITY);
        std::fill_n(this->octets, this->length, 0);
    } else {
        this->length = 0;
    }
}

OctetArray::OctetArray(uint32_t capacity)
{
    uint32_t checked_capacity = (capacity >= OCTET_ARRAY_DEFAULT_CAPACITY)
        ? capacity
        : OCTET_ARRAY_DEFAULT_CAPACITY;

    this->octets = new uint8_t[checked_capacity];

    if (this->octets != nullptr) {
        this->length = checked_capacity;
        std::fill_n(this->octets, this->length, 0);
    } else {
        this->length = 0;
    }
}

OctetArray::~OctetArray() {
    if (this->octets != nullptr) {
        delete[] this->octets;
        this->octets = nullptr;
    }
}

OctetArray::OctetArray(const OctetArray& other) {
    if (this == &other || !other.get_octets()) {
        return;
    }

    if (this->octets != nullptr) {
        delete[] this->octets;
        this->octets = nullptr;
    }

    int32_t other_capacity = other.get_length();
    const uint8_t* other_data = other.get_octets();
    uint8_t* new_buffer = new uint8_t[other_capacity];

    if (new_buffer != nullptr) {
        std::copy_n(other_data, other_capacity, new_buffer);

        this->octets = new_buffer;
        this->length = other.get_length();
    }
}

OctetArray& OctetArray::operator=(const OctetArray& other) {
    if (this == &other) {
        return *this;
    }

    if (this->octets != nullptr) {
        delete[] this->octets;
        this->octets = nullptr;
    }

    int32_t other_capacity = other.get_length();
    uint8_t* new_buffer = new uint8_t[other_capacity];

    if (new_buffer != nullptr) {
        for (int i = 0; i < other_capacity; i++) {
            new_buffer[i] = other.get_octet(i);
        }

        this->octets = new_buffer;
        this->length = other.get_length();
    }

    return *this;
}

OctetArray& OctetArray::operator<<(const BitArray& bitarr) {
    if (bitarr.length() < 1U || !bitarr.get_octets()) {
        return *this;
    }

    // Reload contents of this OctetArray with the encoded octets from bit array
    delete[] this->octets;
    this->octets = nullptr;

    const uint8_t* other_data = bitarr.get_octets();
    int32_t new_octet_capacity = bitarr.length() / 8;
    uint8_t* new_buffer = new uint8_t[new_octet_capacity];

    if (new_buffer != nullptr) {
        std::copy_n(other_data, new_octet_capacity, new_buffer);

        this->octets = new_buffer;
        this->length = new_octet_capacity;
    }

    return *this;
}

void OctetArray::clear() {
    if (this->octets != nullptr) {
        std::memset(this->octets, 0, static_cast<size_t>(this->length));
    }
}

const uint8_t* OctetArray::get_octets() const {
    return this->octets;
}

uint32_t OctetArray::get_length() const {
    return this->length;
}

uint8_t OctetArray::get_octet(uint32_t index) const {
    return this->octets[index];
}

void OctetArray::set_octet(uint32_t index, uint8_t value) {
    if (index >= 0 && index < this->get_length()) {
        this->octets[index] = value;
    }
}
