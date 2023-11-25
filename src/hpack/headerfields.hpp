#ifndef HEADERFIELDS_HPP
#define HEADERFIELDS_HPP

#include <cstdint>

/**
 * @brief Tagging enum value for HTTP/2 header fields.
 */
enum HeaderFieldType : uint8_t {
    HFIELD_ALL_INDEX = 0,            // name and value is entirely indexable
    HFIELD_NAME_INDEX,               // only name is indexable
    HFIELD_NAME_NEW,                 // header by RFC 7541 6.2.1
    HFIELD_NAME_INDEX_VALUE_LITERAL, // header by RFC 7541 6.2.2a
    HFIELD_NAME_NEW_VALUE_LITERAL,   // header by RFC 7541 6.2.2b
    HFIELD_NEVER_INDEX_NAME_INDEX,   // header by RFC 7541 6.2.3a
    HFIELD_NEVER_INDEX_NAME_NEW      // header by RFC 7541 6.2.3b
};

/**
 * @brief Tagged struct for any HTTP/2 header field block.
 */
struct HeaderField {
    union HeaderFieldContents {
        struct {
            uint8_t index;
        } field_ai; // name and value is entirely indexable

        struct {
            uint8_t* value;
            uint16_t index;
            uint16_t value_length;
        } field_nmi; // only name is indexable

        struct {
            uint8_t* name;
            uint8_t* value;
            uint16_t name_length;
            uint16_t value_length;
        } field_nmn; // header by RFC 7541 6.2.1

        struct {
            uint8_t* value;
            uint16_t index;
            uint16_t value_length;
        } field_nmi_vl; // header by RFC 7541 6.2.2a

        struct {
            uint8_t* name;
            uint8_t* value;
            uint16_t name_length;
            uint16_t value_length;
        } field_nmn_vl; // header by RFC 7541 6.2.2b

        struct {
            uint8_t* value;
            uint16_t value_length;
        } field_nvi_nmi; // header by RFC 7541 6.2.3a

        struct {
            uint8_t* name;
            uint8_t* value;
            uint16_t name_length;
            uint16_t value_length;
        } field_nvi_nmn; // header by RFC 7541 6.2.3b
    } data;

    HeaderFieldType type;
};

/**
 * @brief For fully indexable header fields.
 * 
 * @param field 
 * @param index 
 */
void header_field_init(HeaderField* field, uint8_t index);

/**
 * @brief For indexed header fields where only name is indexable.
 * 
 * @param field 
 * @param name_index 
 * @param value_length 
 * @param value 
 */
void header_field_init(HeaderField* field, uint16_t name_index, uint16_t value_length, uint8_t* value);

/**
 * @brief For the following header field types: indexed and new name, unindexed and literal value, and then never indexed and literal value. See _RFC 7541_ Sections 6.2.1, 6.2.2b, 6.2.3b.
 * 
 * @param field 
 * @param name 
 * @param name_length 
 * @param value 
 * @param value_length 
 * @param sub_type 
 */
void header_field_init(HeaderField* field, uint8_t* name, uint16_t name_length, uint8_t* value, uint16_t value_length, HeaderFieldType sub_type);

/**
 * @brief For unindexed header fields with indexed names and literal values.
 * 
 * @param field 
 * @param name_index 
 * @param value 
 * @param value_length 
 */
void header_field_init(HeaderField* field, uint16_t name_index, uint8_t* value, uint16_t value_length);

/**
 * @brief For never indexed header fields with indexed values.
 * 
 * @param field 
 * @param value 
 * @param value_length 
 */
void header_field_init(HeaderField* field, uint8_t* value, uint16_t value_length);

#endif