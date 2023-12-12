#include <iostream>

//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtx/transform.hpp"


int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void window_resize_callback(GLFWwindow* window, int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
    //std::cout << "Window resized, new size:" << width << " x " << height << std::endl;
}

const char* vertexShaderSource = "#version 460\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 in_UV;\n"
"out vec2 uvCoords;\n"
"uniform mat4 modelMatrix;\n"
"void main()\n"
"{\n"
"   gl_Position = modelMatrix * vec4(aPos, 1.0);\n"
"   uvCoords = in_UV;\n"
"}\0";

const char* fragmentShaderSource = "#version 460\n"
"out vec4 FragColor;\n"
"in vec2 uvCoords;\n"
"uniform float time;\n"
"uniform sampler2D inputTexture;\n"
"void main()\n"
"{\n"
"   float modTime = mod(time, 1); \n"
"   FragColor = texture(inputTexture, uvCoords);\n"
"}\n\0";

const float cubeVerts[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to init glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Title", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //float vertices[] = {
    //-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //down left, uv 0,0
    // 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //down right, uv 1,0
    // 0.0f,  0.5f, 0.0f, 0.5f, 1.0f, //up, uv .5, 1
    //}; //counter-clockwise notation

    //per coordinate we have 3 floats, so multiply byte size by 3
    unsigned int sizePerCoordinate = sizeof(float) * 3;
    unsigned int sizePerUV = sizeof(float) * 2;
    unsigned int sizePerVertex = sizePerCoordinate + sizePerUV;
    unsigned int sizeOfTriangleData = 3 * sizePerVertex;
    unsigned int sizeOfCubeData = 6 * 6 * sizePerVertex;

    unsigned int vertexArrayObjectId; //combines the FOLLOWING VBO and attribute pointer calls
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    unsigned int vertexBufferObjectId;
    glGenBuffers(1, &vertexBufferObjectId); //generates vertex buffer and fills id into our pointed to variable
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId); //bind this buffer so we can modify/use it

    glBufferData(GL_ARRAY_BUFFER, sizeOfCubeData, cubeVerts, GL_STATIC_DRAW); //fill buffer with our data

    //tell opengl how to interpret our data
    unsigned int coordinateLocationIndex = 0;
    glVertexAttribPointer(coordinateLocationIndex, 3, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)0);
    //at index/location 0, we have 3 floats that make up one element, a float3 or vec3
    glEnableVertexAttribArray(coordinateLocationIndex);
    //enable this element, so it gets passed to the shader

    unsigned int uvLocationIndex = 1;
    glVertexAttribPointer(uvLocationIndex, 2, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)sizePerCoordinate);
    glEnableVertexAttribArray(uvLocationIndex);


    //load texture
    Texture catPicture("textures/cat.png");

    //start shader
    bool success;
    Shader shader = Shader::TryCompileShader(vertexShaderSource, fragmentShaderSource, &success);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_BACK);

    shader.Use(); //use our shader
    glBindVertexArray(vertexArrayObjectId); //bind our vertex data + attribute settings
    shader.SetFloat("time", 1);
    catPicture.Use();

    while (!glfwWindowShouldClose(window))
    {
        //1 frame of the game per loop
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        float time = glfwGetTime();

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        auto m = glm::mat4(1.0f);
        m = glm::translate(glm::vec3(0, 0, -5.0f));
        m = glm::rotate(m, glm::radians(90.0f + time * 10), glm::vec3(0, 1, 0));
        m = glm::rotate(m, glm::radians(90.0f + time * 15), glm::vec3(0, 0, 1));

        m = proj * m;


        shader.SetMat4("modelMatrix", m);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //vsync would wait here till next frame
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}