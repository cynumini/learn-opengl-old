#ifndef ERROR_H
#define ERROR_H

#include <glad/gl.h>

void on_glfw_error(int error_code, const char *description);
void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                 [[maybe_unused]] GLsizei length, const GLchar *msg,
                 [[maybe_unused]] const void *data);

#endif // ERROR_H
