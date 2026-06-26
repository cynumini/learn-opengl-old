#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>

#include "log.h"
#include "sakana.h"

int main(void) {
    assert(glfwSetErrorCallback(on_glfw_error) == NULL);

    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
    assert(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, GLFW", NULL, NULL);
    assert(window != NULL);

    glfwMakeContextCurrent(window);

    assert(gladLoadGL(glfwGetProcAddress) != 0);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(on_gl_error, NULL);
    }

    LOG_INFO("OpenGL version is %s", glGetString(GL_VERSION));

    float positions[] = {
        -0.5, -0.5, //
        0.0,  0.5,  //
        0.5,  -0.5  //
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, ARRAY_LEN(positions) / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
