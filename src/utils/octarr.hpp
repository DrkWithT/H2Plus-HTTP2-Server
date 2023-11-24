#ifndef OCTARR_HPP
#define OCTARR_HPP

#include <cstdint>
#include <cstring>
#include "utils/bitarr.hpp"

/**
 * @brief A flexible vector of raw, unsigned octet values aka extended ASCII.
 */
class OctetArray
{
private:
    uint8_t* octets;
    int32_t length;
    int32_t capacity;
public:
    OctetArray();
    ~OctetArray();
    OctetArray(const OctetArray& other);
    OctetArray& operator=(const OctetArray& other);
    OctetArray& operator<<(const BitArray& bitarr);
    void clear();
    const uint8_t* get_octets() const;
    int32_t get_length() const;
    uint8_t get_octet(int32_t index) const;
    void set_octet(int32_t index, uint8_t value);
};

#endif