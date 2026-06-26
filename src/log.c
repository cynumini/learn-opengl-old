#include "log.h"

#include "sakana.h"
#include <stdio.h>

void on_glfw_error(int error_code, const char *description) {
    LOG_ERROR("%d %s", error_code, description);
}

void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
                 [[maybe_unused]] GLsizei length, const GLchar *msg,
                 [[maybe_unused]] const void *data) {
    const char *_source;
    const char *_type = NULL;
    const char *_severity;
    LogKind log_kind = LOG_KIND_INFO;

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
        log_kind = LOG_KIND_ERROR;
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        log_kind = LOG_KIND_ERROR;
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        log_kind = LOG_KIND_WARN;
        break;
    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;
    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
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

    if (_type == NULL) {
        log_msg(log_kind, true, "%s (%u) from %s: %s", _severity, id, _source, msg);
    } else {
        log_msg(log_kind, true, "%s - %s (%u) from %s: %s", _type, _severity, id,
                _source, msg);
    }
}
