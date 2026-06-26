#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

const char frag_code[] = {
#embed "basic.frag.glsl"
};

const char vert_code[] = {
#embed "basic.vert.glsl"
};

static void on_error(int error_code, const char *description) {
    fprintf(stderr, RED "ERROR" RESET ": %d %s\n", error_code, description);
}

static void on_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity,
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
        _type = "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
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
        _severity = "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;
    default:
        _severity = "UNKNOWN";
        break;
    }

    printf("%u: %s of %s severity, raised from %s: %s\n", id, _type, _severity,
           _source, msg);
}

static GLuint create_shader(GLenum type, const char *src) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    return id;
}

int main(void) {
    assert(glfwSetErrorCallback(on_error) == NULL);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    assert(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, GLFW", NULL, NULL);
    assert(window != NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    assert(gladLoadGL(glfwGetProcAddress) != 0);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(on_gl_error, NULL);
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    float position[] = {
        -0.5f, -0.5f, //
        0.5f,  -0.5f, //
        0.5f,  0.5f,  //
        -0.5f, 0.5f   //
    };

    float indices[] = {
        0, 1, 2, //
        2, 3, 0  //
    };

    GLuint va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    GLuint vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, ARRAY_LENGTH(position) * sizeof(float),
                 position, GL_STATIC_DRAW);

    size_t offset = 0;
    for (GLuint i = 0; i < 3; i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(float),
                              (void *)offset);
        offset += sizeof(float) * 3;
    }

    GLuint ib;
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ARRAY_LENGTH(indices) * sizeof(float),
                 indices, GL_STATIC_DRAW);

    GLuint program = glCreateProgram();

    GLuint vert_shader = create_shader(GL_VERTEX_SHADER, vert_code);
    GLuint frag_shader = create_shader(GL_FRAGMENT_SHADER, frag_code);

    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    // deinit
    glDeleteBuffers(1, &ib);
    glDeleteBuffers(1, &vb);
    glDeleteVertexArrays(1, &va);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
