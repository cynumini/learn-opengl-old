#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>

#include "error_callback.h"

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

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    glClearColor(1, 0, 0, 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
