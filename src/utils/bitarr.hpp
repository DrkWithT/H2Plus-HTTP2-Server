#ifndef BITARR_HPP
#define BITARR_HPP

#include <cstdint>
#include <cstring>

/**
 * @brief Simple wrapper for a raw bit sequence.
 */
class BitArray {
private:
    uint32_t bit_capacity; // allocated bits: `octet_capacity / 8`
    uint32_t bit_length; // used bit count: `octet_length / 8`
    uint8_t* octets; // raw unsigned byte block
public:
    BitArray();
    BitArray(uint32_t size);
    ~BitArray();

    uint32_t length() const;
    const uint8_t* get_octets() const;

    bool at(uint32_t bit_pos);
    void put(uint32_t bit_pos, bool bit);
    bool append(bool bit);
    bool append(uint8_t octet);
    void clear();
};

#endif
