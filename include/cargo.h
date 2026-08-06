#pragma once

#include <stdbool.h>
#include "render/render_types.h"

typedef enum CARGO_TYPE {
    CARGO_TYPE_NONE,
    CARGO_TYPE_BOOL,
    CARGO_TYPE_INT,
    CARGO_TYPE_FONT_HASHMAP
} CARGO_TYPE;

typedef struct Cargo {
    CARGO_TYPE type;
    union {
        bool boolean;
        int integer;
        font_hash* font_hashmap;
    };
} Cargo;

static const Cargo NULL_CARGO = {.type = CARGO_TYPE_NONE};

Cargo cargo_boolean_create(bool boolean);
Cargo cargo_integer_create(int integer);
Cargo cargo_font_hashmap_create(font_hash *font_hashmap);