/**
 * @file octarr.cpp
 * @author Derek Tan
 * @brief Implements the dynamic octet array.
 * @date 2023-11-22
 */

#include "utils/octarr.hpp"

/* Constexprs */

constexpr int32_t OCTET_ARRAY_DEFAULT_CAPACITY = 1024;

/* OctetArray Impl. */

OctetArray::OctetArray() {
    this->octets = new uint8_t[OCTET_ARRAY_DEFAULT_CAPACITY];

    if (this->octets != nullptr) {
        this->capacity = OCTET_ARRAY_DEFAULT_CAPACITY;
        this->length = 0;
    } else {
        this->capacity = -1;
        this->length = -1;
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

    int32_t other_capacity = other.capacity;
    uint8_t* new_buffer = new uint8_t[other_capacity];

    if (new_buffer != nullptr) {
        for (int i = 0; i < other_capacity; i++) {
            new_buffer[i] = other.get_octet(i);
        }

        this->octets = new_buffer;
        this->capacity = other_capacity;
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

    int32_t other_capacity = other.capacity;
    uint8_t* new_buffer = new uint8_t[other_capacity];

    if (new_buffer != nullptr) {
        for (int i = 0; i < other_capacity; i++) {
            new_buffer[i] = other.get_octet(i);
        }

        this->octets = new_buffer;
        this->capacity = other_capacity;
        this->length = other.get_length();
    }

    return *this;
}

OctetArray& OctetArray::operator<<(const BitArray& bitarr) {
    if (bitarr.length() < 1 || !bitarr.get_octets()) {
        return *this;
    }

    // Reload contents of this OctetArray with the encoded octets from bit array
    if (this->octets != nullptr) {
        delete[] this->octets;
        this->octets = nullptr;
    }

    const uint8_t* octet_ptr = bitarr.get_octets();
    int32_t new_octet_capacity = bitarr.length() / 8;
    uint8_t* new_buffer = new uint8_t[new_octet_capacity];

    if (new_buffer != nullptr) {
        for (int i = 0; i < new_octet_capacity; i++) {
            new_buffer[i] = octet_ptr[i];
        }

        this->octets = new_buffer;
        this->capacity = new_octet_capacity;
        this->length = new_octet_capacity;
    }

    return *this;
}

void OctetArray::clear() {
    if (this->octets != nullptr && this->capacity > 0) {
        std::memset(this->octets, 0, static_cast<size_t>(this->capacity));
    }
}

const uint8_t* OctetArray::get_octets() const {
    return this->octets;
}

int32_t OctetArray::get_length() const {
    return this->length;
}

uint8_t OctetArray::get_octet(int32_t index) const {
    return this->octets[index];
}

void OctetArray::set_octet(int32_t index, uint8_t value) {
    if (index >= 0 && index < this->get_length()) {
        this->octets[index] = value;
    }
}
