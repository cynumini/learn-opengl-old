#ifndef LOG_H
#define LOG_H

#include <glad/gl.h>

typedef enum {
    LOG_KIND_INFO,
    LOG_KIND_ERROR,
    LOG_KIND_WARN,
} LogKind;

void log_msg(LogKind kind, const char *fmt, ...);

#define LOG_ERROR(fmt, ...) log_msg(LOG_KIND_ERROR, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) log_msg(LOG_KIND_INFO, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) log_msg(LOG_KIND_WARN, fmt, __VA_ARGS__)

void on_glfw_error(int error_code, const char *description);
void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                 [[maybe_unused]] GLsizei length, const GLchar *msg,
                 [[maybe_unused]] const void *data);

#endif // LOG_H
