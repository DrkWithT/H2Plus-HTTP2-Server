#ifndef BITARR_HPP
#define BITARR_HPP

#include <cstdint>
#include <cstring>

constexpr int OCTET_BITS = 8; // bits per octet
constexpr int MAX_OCTET_N = 7; // max bit place in octet
constexpr int DEFAULT_BIT_CAPACITY = 4096; // bits for a default `BitArray`

/**
 * @brief Simple wrapper for a raw bit sequence.
 */
class BitArray {
private:
    int32_t bit_capacity; // allocated bits: `octet_capacity / 8`
    int32_t bit_length; // used bit count: `octet_length / 8`
    uint8_t* octets; // raw unsigned byte block
public:
    BitArray();
    BitArray(int32_t size);
    ~BitArray();

    int32_t length() const;
    const uint8_t* get_octets() const;

    bool at(int32_t bit_pos);
    void put(int32_t bit_pos, bool bit);
    bool append(bool bit);
    bool append(uint8_t octet);
    void clear();
};

#endif
