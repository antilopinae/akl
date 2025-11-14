#pragma once

#include <stdlib.h>

namespace akl {
    /// an integer value padded to 64 bytes
    struct padded_integer {
        size_t val;
        char __pad__[64 - sizeof(size_t)];
    };
}
