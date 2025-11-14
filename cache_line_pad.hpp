#pragma once

namespace akl {
    /**
     * Used to prevent false cache sharing by padding T
     */
    template<typename T>
    struct cache_line_pad {
        T value;
        char pad[64 - (sizeof(T) % 64)];

        cache_line_pad(const T &value = T()) : value(value) {
        }

        T &operator=(const T &other) { return value = other; }
        operator T() const { return value; }
    }; // end of cache_line_pad
}
