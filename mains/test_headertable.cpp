/**
 * @file test_headertable.cpp
 * @author Derek Tan
 * @brief Implements unit test for full header indexing table.
 * @date 2023-11-26
 */

#include <iostream>
#include "hpack/headertable.hpp"

int main() {
    HeaderTablePair mock_headers[]{
        {"server", "TestServer/1.0"},
        {"content-type", "text/javascript"},
        {"content-length", "1234"},
        {"date", "Mon, 27 Nov 2023 05:10:30 GMT"}
    };

    HeaderIndexingTable table {};

    // Trivial empty table test
    if (table.get_size() != 0UL && table.get_total_length() != 0U) {
        std::cerr << "Table was not marked as empty!" << std::endl;
        return 1;
    }

    for (auto& mock_item : mock_headers) {
        table.put_entry(mock_item);
    }

    // Test if table expanded to correct length in under-capacity.
    if (table.get_total_length() != 65U || table.get_size() != 128UL) {
        std::cerr << "Table expanded to incorrect length or size!" << std::endl;
        return 1;
    }

    // Test if table can evict (all) entries on a big size shrink.
    table.update_capacity(0);

    if (table.get_total_length() != 0U || table.get_size() != 0UL) {
        std::cerr << "Table shrinkage failed by incorrect length or overhead size." << std::endl;
        return 1;
    }

    std::cout << "Test OK!" << std::endl;

    return 0;
}
