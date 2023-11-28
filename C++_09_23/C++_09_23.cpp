#include <iostream>

//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
    if (!glfwInit())
    {
        // Initialization failed
    }

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();

    while (!glfwWindowShouldClose(window))
    {
        //glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}