/**
 * @file message.cpp
 * @author Derek Tan
 * @brief Implements high-level, reusable HTTP message ADT.
 * @date 2024-01-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "protocol/message.hpp"

/* HeaderField impl. */

HeaderField::HeaderField() : literal {""}, semantic_type {field_type_str}, super_type {field_supertype_normal}
{}

/* HttpXMessage impl. */

HttpXMessage::HttpXMessage() : headers {}, content {}, nil_field {}
{}

HttpXMessage::HttpXMessage(uint32_t content_presize) : headers {}, content {content_presize}, nil_field {}
{}

[[nodiscard]] const HeaderField& HttpXMessage::peek_header(const std::string& name) const {
    if (headers.find(name) == headers.end()) {
        return nil_field;
    }

    return headers.at(name);
}

void HttpXMessage::put_header(const std::string& name, const HeaderField& new_field) {
    headers[name] = new_field;
}

[[nodiscard]] bool HttpXMessage::remove_header(const std::string& name) {
    if (headers.find(name) == headers.end())
        return false;

    headers.erase(name);
    return true;
}

[[nodiscard]] bool HttpXMessage::set_octet_in_content(uint32_t index, uint8_t octet) {
    if (index >= content.get_length())
        return false;

    content.set_octet(index, octet);
    return true;
}

[[nodicard]] bool HttpXMessage::put_octets_to_content(const OctetArray& octets) {
    if (octets.get_length() < 1 || octets.get_length() > content.get_length())
        return false;

    content = octets;

    return true;
}

void HttpXMessage::reset_data() {
    headers.clear();
    content.clear();
}
