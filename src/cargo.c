#include "../include/cargo.h"

Cargo cargo_boolean_create(bool boolean){
    Cargo cargo = {.type = CARGO_TYPE_BOOL, .boolean = boolean};
    return cargo;
}