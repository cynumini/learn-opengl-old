#ifndef SAKANA_H
#define SAKANA_H

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1b[33m"
#define RESET "\x1B[0m"

typedef enum {
    LOG_KIND_INFO,
    LOG_KIND_ERROR,
    LOG_KIND_WARN,
} LogKind;

void log_msg(LogKind kind, bool new_line, const char *fmt, ...);

#define LOG_ERROR(fmt, ...) log_msg(LOG_KIND_ERROR, true, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) log_msg(LOG_KIND_INFO, true, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) log_msg(LOG_KIND_WARN, true, fmt, __VA_ARGS__)
#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))
_Noreturn void skn_assert_fail(const char *expr, const char *file, int line,
                             const char *func);
_Noreturn void skn_unreachable(const char *file, int line, const char *func);
#define ASSERT(x)                                                              \
    ((void)((x) || (skn_assert_fail(#x, __FILE__, __LINE__, __func__), 0)))
#define UNREACHABLE skn_unreachable(__FILE__, __LINE__, __func__)

#endif // SAKANA_H
