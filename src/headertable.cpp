/**
 * @file headertable.cpp
 * @author Derek Tan
 * @brief Implements header indexing table for HPACK.
 * @date 2023-11-25
 */

#include "hpack/headertable.hpp"

/* Public HeaderIndexingTable Impl. */

HeaderIndexingTable::HeaderIndexingTable() : dynamic_table {} {
    this->static_table = STATIC_HEADER_TABLE;
    this->table_capacity = TABLE_DEFAULT_SIZE;
    this->table_size = 0UL;
    this->static_length = STATIC_TABLE_LENGTH;
    this->dynamic_length = 0U;
}

bool HeaderIndexingTable::is_full() const {
    return this->table_size >= table_capacity;
}

void HeaderIndexingTable::update_capacity(size_t new_capacity) {
    /// @todo 1: The risk is that the new_capacity may be too large from the client's wishes. I should put an implementation defined hard limit on this value.
    /// @todo 2: The other code smell here is the repeated entry eviction code. I should pull that out into a method.
 
    uint32_t temp_dyntable_length = this->static_length + this->dynamic_length; // temp copy of dynamic entry count
    size_t temp_dyntable_size = this->table_size; // copy of total table size
    size_t temp_entry_size = 0UL; // temp size of evicting entry

    // Update table size
    this->table_capacity = new_capacity;

    // Handle special case of clear dynamic table
    if (this->table_capacity == 0UL) {
        this->dynamic_table.clear();
        this->table_size = 0UL;
        this->dynamic_length = 0U;
        return;
    }

    // Evict entries only if current memory size exceeds the new limit
    if (this->is_full()) {
        while (temp_dyntable_size > this->table_capacity) {
            const HeaderTablePair& old_entry = this->dynamic_table.back();
            temp_entry_size = compute_entry_overhead(old_entry);
            this->dynamic_table.pop_back();
    
            temp_dyntable_size -= temp_entry_size;
            temp_dyntable_length--;
        }

        this->table_size = temp_dyntable_size;
        this->dynamic_length = temp_dyntable_length;
    }
}

const HeaderTablePair& HeaderIndexingTable::get_entry(uint32_t index) const {
    uint32_t curr_static_length = this->static_length;
    uint32_t real_index = (index != 0U) ? index - 1U : 0U;

    if (real_index < curr_static_length) {
        return this->static_table[real_index];
    } else if (real_index < curr_static_length + this->dynamic_length) {
        return this->dynamic_table.at(real_index - curr_static_length);
    }

    return this->static_table[0];
}

void HeaderIndexingTable::put_entry(const HeaderTablePair& entry) {
    size_t entry_overhead = compute_entry_overhead(entry);

    this->dynamic_table.push_front(entry);
    this->table_size += entry_overhead;

    // If the table passes its memory capacity again, evict entries until size is OK
    while (this->is_full()) {
        const HeaderTablePair& temp_back_entry = this->dynamic_table.back();
        entry_overhead = compute_entry_overhead(temp_back_entry);
        this->dynamic_table.pop_back();

        this->table_size -= entry_overhead;
        this->dynamic_length--;
    }
}

/* Private HeaderIndexingTable Impl. */

bool HeaderIndexingTable::has_entry(const std::string& name) const {
    for (auto& item : this->dynamic_table) {
        if (item.get_name() == name) {
            return true;
        }
    }

    return false;
}
