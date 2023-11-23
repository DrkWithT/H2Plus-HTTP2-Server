/**
 * @file pairs.cpp
 * @author Derek Tan
 * @brief Implements simple structures that store items within `huffcodes.cpp`.
 * @date 2023-11-22
 */

#include "hpack/pairs.hpp"

/* HeaderTablePair Impl. */

HeaderTablePair::HeaderTablePair(const char* name_cstr, const char* value_cstr): name {name_cstr}, value {value_cstr} {}

const std::string& HeaderTablePair::get_name() const {
    return this->name;
}

const std::string& HeaderTablePair::get_value() const {
    return this->value;
}
