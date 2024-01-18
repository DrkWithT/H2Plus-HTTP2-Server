#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <map>
#include "utils/octarr.hpp"

enum HeaderFieldSuperType {
    field_supertype_pseudo,
    field_supertype_normal
};

enum HeaderFieldType {
    field_type_int,
    field_type_str
};

struct HeaderField {
    std::string literal;
    HeaderFieldType semantic_type;    // type to interpret header literal as
    HeaderFieldSuperType super_type;  // more type info: pseudo-header or not?

    HeaderField();
};

class HttpXMessage {
private:
    std::map<std::string, HeaderField> headers;
    OctetArray content;
    HeaderField nil_field;
public:
    HttpXMessage();
    HttpXMessage(uint32_t content_presize);

    [[nodiscard]] const HeaderField& peek_header(const std::string& name) const;
    void put_header(const std::string& name, const HeaderField& new_field);
    [[nodiscard]] bool remove_header(const std::string& name);

    [[nodiscard]] bool set_octet_in_content(uint32_t index, uint8_t octet);
    [[nodicard]] bool put_octets_to_content(const OctetArray& octets);

    void reset_data();
};

#endif