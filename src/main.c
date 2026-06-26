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

    int result;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
        char *msg = (char *)calloc((size_t)len, sizeof(char));
        log_msg(LOG_KIND_ERROR, false, "Program link: %s", msg);
        glGetProgramInfoLog(id, len, &len, msg);
        UNREACHABLE;
    }

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

const int screen_width = 800;
const int screen_height = 600;

const char vertex_shader[] = {
#embed "basic.vert.glsl"
    , 0};

const char fragment_shader[] = {
#embed "basic.frag.glsl"
    , 0};

int main(void) {
    ASSERT(glfwSetErrorCallback(on_glfw_error) == NULL);

    ASSERT(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height,
                                          "LearnOpenGL", NULL, NULL);
    ASSERT(window != NULL);

    glfwMakeContextCurrent(window);

    ASSERT(glfwSetFramebufferSizeCallback(window, framebuffer_size_callback) ==
           NULL);

    ASSERT(gladLoadGL(glfwGetProcAddress) != 0);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(on_gl_error, NULL);
    }

    LOG_INFO("OpenGL version is %s", glGetString(GL_VERSION));

    GLfloat vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, // top left
    };

    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint program = create_program(vertex_shader, fragment_shader);

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
