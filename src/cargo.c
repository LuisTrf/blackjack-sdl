#include "../include/cargo.h"

Cargo cargo_boolean_create(bool boolean){
    Cargo cargo = {.type = CARGO_TYPE_BOOL, .boolean = boolean};
    return cargo;
}

Cargo cargo_integer_create(int integer){
    Cargo cargo = {.type = CARGO_TYPE_INT, .integer = integer};
    return cargo;
}

Cargo cargo_font_hashmap_create(font_hash *font_hashmap){
    Cargo cargo = {.type = CARGO_TYPE_FONT_HASHMAP, .font_hashmap = font_hashmap};
    return cargo;
}