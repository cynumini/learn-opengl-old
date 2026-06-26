#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "sakana.h"

static GLuint compile_shader(GLenum kind, const char *source) {
    GLuint id = glCreateShader(kind);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)calloc((size_t)length, sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        const char *type;
        if (kind == GL_VERTEX_SHADER) {
            type = "vertex";
        } else if (kind == GL_FRAGMENT_SHADER) {
            type = "fragment";
        } else {
            UNREACHABLE;
        }
        log_msg(LOG_KIND_ERROR, false,
                "Failed to compile %s shader with error:\n%s", type, message);
        free(message);
        UNREACHABLE;
    }
    return id;
}

static GLuint create_shader(const char *vertex_shader,
                            const char *fragment_shader) {
    GLuint program = glCreateProgram();

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {
    assert(glfwSetErrorCallback(on_glfw_error) == NULL);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    assert(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, GLFW", NULL, NULL);
    assert(window != NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    assert(gladLoadGL(glfwGetProcAddress) != 0);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(on_gl_error, NULL);
    }

    LOG_INFO("OpenGL version is %s", glGetString(GL_VERSION));

    GLfloat positions[] = {
        -0.5, -0.5, //
        0.5,  -0.5, //
        0.5,  0.5,  //
        -0.5, 0.5   //
    };

    GLuint indices[] = {
        0, 1, 2, //
        2, 3, 0  //
    };

    GLuint vao, buffer, ibo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    const char vertex_shader[] = {
#embed "basic.vert.glsl"
        , 0};

    const char fragment_shader[] = {
#embed "basic.frag.glsl"
        , 0};

    GLuint shader = create_shader(vertex_shader, fragment_shader);

    glUseProgram(shader);

    GLint location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLfloat r = 0.f;
    GLfloat increment = 0.02f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glUniform4f(location, r, 0.3f, 0.8f, 1.f);

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, ARRAY_LEN(indices), GL_UNSIGNED_INT, 0);

        if (r > 1.0f || r < 0.f)
            increment *= -1;

        r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
