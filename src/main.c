#include <glad/gl.h>

#include <GLFW/glfw3.h>
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
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char *msg = (char *)calloc((size_t)len, sizeof(char));
        glGetShaderInfoLog(id, len, &len, msg);
        const char *type;
        if (kind == GL_VERTEX_SHADER) {
            type = "vertex";
        } else if (kind == GL_FRAGMENT_SHADER) {
            type = "fragment";
        } else {
            UNREACHABLE;
        }
        log_msg(LOG_KIND_ERROR, false,
                "Failed to compile %s shader with error:\n%s", type, msg);
        free(msg);
        UNREACHABLE;
    }
    return id;
}

static GLuint create_program(const char *vert_code, const char *frag_code) {
    GLuint id = glCreateProgram();
    GLuint vert_shader = compile_shader(GL_VERTEX_SHADER, vert_code);
    GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, frag_code);
    glAttachShader(id, vert_shader);
    glAttachShader(id, frag_shader);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glLinkProgram(id);
    glValidateProgram(id);
    return id;
}

static void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

static void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                      int width, int height) {
    glViewport(0, 0, width, height);
}

int main(void) {
    ASSERT(glfwSetErrorCallback(on_glfw_error) == NULL);

    ASSERT(glfwInit() == GLFW_TRUE);

    int width = 640, height = 480;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window =
        glfwCreateWindow(width, height, "Hello, GLFW", NULL, NULL);
    ASSERT(window != NULL);

    ASSERT(glfwSetFramebufferSizeCallback(window, framebuffer_size_callback) ==
           NULL);

    glfwMakeContextCurrent(window);

    ASSERT(gladLoadGL(glfwGetProcAddress) != 0);

    glViewport(0, 0, width, height);

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

    GLuint program = create_program(vertex_shader, fragment_shader);

    glUseProgram(program);

    GLint location = glGetUniformLocation(program, "u_Color");
    ASSERT(location != -1);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLfloat r = 0.f;
    GLfloat increment = 0.02f;

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform4f(location, r, 0.3f, 0.8f, 1.f);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, ARRAY_LEN(indices), GL_UNSIGNED_INT, 0);

        if (r > 1.f || r < 0.f)
            increment *= -1;

        r += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
