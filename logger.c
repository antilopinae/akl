#ifdef __KERNEL__
#include <linux/kernel.h>
#else
#include <stdio.h>
#include <stdarg.h>
#endif

#include "akl/logger.h"

void kern_log(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
#ifdef __KERNEL__
    vprintk(fmt, args);
#else
    printf(fmt, args);
#endif

    va_end(args);
}