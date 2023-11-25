#ifndef HEADERTABLE_HPP
#define HEADERTABLE_HPP

#include <deque>
#include "hpack/tables.hpp"

constexpr size_t ENTRY_OVERHEAD = 32UL;
constexpr uint32_t STATIC_TABLE_LENGTH = 61UL;
constexpr size_t TABLE_DEFAULT_SIZE = 4096UL;

constexpr size_t compute_entry_overhead(const HeaderTablePair& entry) {
    return ENTRY_OVERHEAD + entry.get_name().length() + entry.get_value().length();
}

// A lookup table for compressing common headers by index.
const HeaderTablePair STATIC_HEADER_TABLE[] = {
    {":authority", ""},
    {":method", "GET"},
    {":method", "POST"},
    {":path", "/"},
    {":path", "/index.html"},
    {":scheme", "http"},
    {":scheme", "https"},
    {":status", "200"},
    {":status", "204"},
    {":status", "206"},
    {":status", "304"},
    {":status", "400"},
    {":status", "404"},
    {":status", "500"},
    {"accept-charset", ""},
    {"accept-encoding", "gzip, deflate"},
    {"accept-language", ""},
    {"accept-ranges", ""},
    {"accept", ""},
    {"access-control-allow-origin", ""},
    {"age", ""},
    {"allow", ""},
    {"authorization", ""},
    {"cache-control", ""},
    {"content-disposition", ""},
    {"content-encoding", ""},
    {"content-language", ""},
    {"content-length", ""},
    {"content-location", ""},
    {"content-range", ""},
    {"content-type", ""},
    {"cookie", ""},
    {"data", ""},
    {"etag", ""},
    {"expect", ""},
    {"expires", ""},
    {"from", ""},
    {"host", ""},
    {"if-match", ""},
    {"if-modified-since", ""},
    {"if-none-match", ""},
    {"if-range", ""},
    {"if-unmodified-since", ""},
    {"last-modified", ""},
    {"link", ""},
    {"location", ""},
    {"max-forwards", ""},
    {"proxy-authenticate", ""},
    {"proxy-authorization", ""},
    {"range", ""},
    {"referer", ""},
    {"refresh", ""},
    {"retry-after", ""},
    {"server", ""},
    {"set-cookie", ""},
    {"strict-transport-security", ""},
    {"transfer-encoding", ""},
    {"user-agent", ""},
    {"vary", ""},
    {"via", ""},
    {"www-authenticate", ""},
};

class HeaderIndexingTable {
private:
    std::deque<HeaderTablePair> dynamic_table; // dynamic header field table
    const HeaderTablePair* static_table; // ptr to static array of header field classes
    size_t table_capacity; // maximum dynamic entry memory in octets permitted
    size_t table_size; // current dynamic entry memory in octets
    uint32_t static_length; // item count of static table
    uint32_t dynamic_length; // item count of dynamic table

    bool has_entry(const std::string& name) const;
public:
    HeaderIndexingTable();
    bool is_full() const;
    void update_capacity(size_t new_capacity);
    const HeaderTablePair& get_entry(uint32_t index) const;
    void put_entry(const HeaderTablePair& entry);
};

#endif