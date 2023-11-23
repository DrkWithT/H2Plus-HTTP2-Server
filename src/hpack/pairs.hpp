#ifndef PAIRS_HPP
#define PAIRS_HPP

#include <cstdint>
#include <string>

/**
 * @brief Simple class for the static header table of an HPACK context.
 */
class HeaderTablePair {
private:
    std::string name;
    std::string value;
public:
    HeaderTablePair(const char* name_cstr, const char* value_cstr);
    const std::string& get_name() const;
    const std::string& get_value() const;
};

/**
 * @brief Simple data struct to store a static Huffman code entry.
 */
struct HuffmanCodePair {
    uint32_t code_number; // bits encoded as `uint32_t`
    uint32_t code_length; // bit count of static Huffman code
};

#endif