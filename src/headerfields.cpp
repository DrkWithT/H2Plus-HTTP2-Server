/**
 * @file headerfields.cpp
 * @author Derek Tan
 * @brief Implements initializers for tagged struct of header field.
 * @date 2023-11-25
 */

#include "hpack/headerfields.hpp"

/* HeaderField Impl. */

void header_field_init(HeaderField* field, uint8_t index) {
    field->type = HFIELD_ALL_INDEX;

    field->data.field_ai.index = index;    
}

void header_field_init(HeaderField* field, uint16_t name_index, uint16_t value_length, uint8_t* value) {
    field->type = HFIELD_NAME_INDEX;

    field->data.field_nmi = {.value = value, .index = name_index, .value_length = value_length};
}

void header_field_init(HeaderField* field, uint8_t* name, uint16_t name_length, uint8_t* value, uint16_t value_length, HeaderFieldType sub_type) {
    field->type = sub_type;

    switch (field->type)
    {
    case HFIELD_NAME_NEW:
        field->data.field_nmn = {.name = name, .name_length = name_length, .value = value, .value_length = value_length};
        break;
    case HFIELD_NAME_NEW_VALUE_LITERAL:
        field->data.field_nmn_vl = {.name = name, .name_length = name_length, .value = value, .value_length = value_length};
        break;
    case HFIELD_NEVER_INDEX_NAME_NEW:
        field->data.field_nvi_nmn = {.name = name, .name_length = name_length, .value = value, .value_length = value_length};
        break;
    default:
        break;
    }
}

void header_field_init(HeaderField* field, uint16_t name_index, uint8_t* value, uint16_t value_length) {
    field->type = HFIELD_NAME_INDEX_VALUE_LITERAL;

    field->data.field_nmi_vl = {.value = value, .index = name_index, .value_length = value_length};
}

void header_field_init(HeaderField* field, uint8_t* value, uint16_t value_length) {
    field->type = HFIELD_NEVER_INDEX_NAME_INDEX;

    field->data.field_nvi_nmi = {.value = value, .value_length = value_length};
}
