#pragma once

#include <stdbool.h>

typedef enum CARGO_TYPE {
    CARGO_TYPE_NONE,
    CARGO_TYPE_BOOL,
} CARGO_TYPE;

typedef struct Cargo {
    CARGO_TYPE type;
    union {
        bool boolean;
    };
} Cargo;

static const Cargo NULL_CARGO = {.type = CARGO_TYPE_NONE};

Cargo cargo_boolean_create(bool boolean);