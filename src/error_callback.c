#include "error_callback.h"

#include <stdio.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1b[33m"
#define RESET "\x1B[0m"

void on_glfw_error(int error_code, const char *description) {
    fprintf(stderr, RED "ERROR" RESET ": %d %s\n", error_code, description);
}

void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                 [[maybe_unused]] GLsizei length, const GLchar *msg,
                 [[maybe_unused]] const void *data) {
    const char *_source;
    const char *_type;
    const char *_severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;
    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = RED "ERROR" RESET;
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = YELLOW "DEPRECATED BEHAVIOR" RESET;
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = RED "UDEFINED BEHAVIOR" RESET;
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        _type = YELLOW "PORTABILITY" RESET;
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = YELLOW "PERFORMANCE" RESET;
        break;
    case GL_DEBUG_TYPE_OTHER:
        _type = GREEN "OTHER" RESET;
        break;
    case GL_DEBUG_TYPE_MARKER:
        _type = GREEN "MARKER" RESET;
        break;
    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = RED "HIGH" RESET;
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = YELLOW "MEDIUM" RESET;
        break;
    case GL_DEBUG_SEVERITY_LOW:
        _severity = GREEN "LOW" RESET;
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;
    default:
        _severity = "UNKNOWN";
        break;
    }

    fprintf(stderr, "%s (%u): %s, raised from %s: %s\n", _type, id, _severity,
            _source, msg);
}
