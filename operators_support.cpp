#include "akl/kern_lib.h"
#include "akl/logger.h"

#include <cstddef>

void* operator new(size_t sz) throw() {
    return akl_cmalloc(sz);
}

void* operator new[](size_t sz) throw() {
    return akl_cmalloc(sz);
}

void operator delete(void* p) {
    akl_cfree(p);
}

void operator delete(void* p, std::size_t sz) {
    akl_cfree(p);
}

void operator delete[](void* p) {
    akl_cfree(p);
}

void terminate() {
    akl_kern_log("terminate requested\n");
}

extern "C" void __cxa_pure_virtual() {
    akl_kern_log("cxa_pure_virtual error handler\n");
}