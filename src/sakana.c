#include "sakana.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((format(printf, 3, 4))) void log_msg(LogKind kind, bool new_line,
                                                   const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    switch (kind) {
    case LOG_KIND_INFO: {
        fprintf(stderr, GREEN "INFO" RESET ": ");
        break;
    }
    case LOG_KIND_WARN: {
        fprintf(stderr, YELLOW "WARN" RESET ": ");
        break;
    }
    case LOG_KIND_ERROR: {
        fprintf(stderr, RED "ERROR" RESET ": ");
        break;
    }
    default: {
        UNREACHABLE;
    };
    }
    vfprintf(stderr, fmt, args);
    if (new_line)
        fprintf(stderr, "\n");

    va_end(args);
}

_Noreturn void skn_assert_fail(const char *expr, const char *file, int line, const char *func)
{
    fprintf(stderr, "%s:%d:0: assertion failed: %s in function %s\n", file, line, expr, func);
    abort();
}

_Noreturn void skn_unreachable(const char *file, int line, const char *func) {
    fprintf(stderr, "%s:%d:0: unreachable: in function %s\n", file, line, func);
    abort();
}
