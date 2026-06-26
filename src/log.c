#include "log.h"

#include "sakana.h"
#include <stdio.h>

void on_glfw_error(int error_code, const char *description) {
    LOG_ERROR("%d %s", error_code, description);
}

void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                 [[maybe_unused]] GLsizei length, const GLchar *msg,
                 [[maybe_unused]] const void *data) {
    const char *source_str;
    const char *type_str = NULL;
    const char *severity_str;
    LogKind log_kind = LOG_KIND_INFO;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        source_str = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "WINDOW SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "SHADER COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "THIRD PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "UNKNOWN";
        break;
    default:
        source_str = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        log_kind = LOG_KIND_ERROR;
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "DEPRECATED BEHAVIOR";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "UDEFINED BEHAVIOR";
        log_kind = LOG_KIND_ERROR;
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "PORTABILITY";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "PERFORMANCE";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "OTHER";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "MARKER";
        break;
    default:
        type_str = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = RED "HIGH" RESET;
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = YELLOW "MEDIUM" RESET;
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = GREEN "LOW" RESET;
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_str = "NOTIFICATION";
        break;
    default:
        severity_str = "UNKNOWN";
        break;
    }

    if (type_str == NULL) {
        log_msg(log_kind, true, "%s (%u) from %s: %s", severity_str, id,
                source_str, msg);
    } else {
        log_msg(log_kind, true, "%s - %s (%u) from %s: %s", type_str,
                severity_str, id, source_str, msg);
    }
}
