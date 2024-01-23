#include <iostream>

//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtx/transform.hpp"


int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 960;

bool recompileShaders = false;

glm::vec3 camPos = glm::vec3(0, 0, -5);
glm::vec3 camRot = glm::vec3(0, 270, 0);
glm::vec2 mousePos = glm::vec2(0, 0);

bool cameraButtonHeld = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        recompileShaders = true;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        camPos.z += 0.5f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        camPos.z -= 0.5f;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camPos.x += 0.5f;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camPos.x -= 0.5f;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camPos.y += 0.5f;
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        camPos.y -= 0.5f;
    }

    
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2) {
        if (action == GLFW_PRESS) {
            std::cout << "MOUSE PRESSED" << std::endl;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cameraButtonHeld = true;
        }

        if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cameraButtonHeld = false;
        }
    }
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
"layout (location = 1) in vec3 in_Normals;\n"
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

float cubeVerts[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                                              
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                                              
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                              
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                              
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                                              
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};

//
//const float cubeVerts[] = {
//    -0.5f, -0.5f, -0.5f,  
//     0.5f, -0.5f, -0.5f,  
//     0.5f,  0.5f, -0.5f,  
//     0.5f,  0.5f, -0.5f,  
//    -0.5f,  0.5f, -0.5f,  
//    -0.5f, -0.5f, -0.5f,  
//
//    -0.5f, -0.5f,  0.5f,  
//     0.5f, -0.5f,  0.5f,  
//     0.5f,  0.5f,  0.5f,  
//     0.5f,  0.5f,  0.5f,  
//    -0.5f,  0.5f,  0.5f,  
//    -0.5f, -0.5f,  0.5f,  
//
//    -0.5f,  0.5f,  0.5f,  
//    -0.5f,  0.5f, -0.5f,  
//    -0.5f, -0.5f, -0.5f,  
//    -0.5f, -0.5f, -0.5f,  
//    -0.5f, -0.5f,  0.5f,  
//    -0.5f,  0.5f,  0.5f,  
//
//     0.5f,  0.5f,  0.5f,  
//     0.5f,  0.5f, -0.5f,  
//     0.5f, -0.5f, -0.5f,  
//     0.5f, -0.5f, -0.5f,  
//     0.5f, -0.5f,  0.5f,  
//     0.5f,  0.5f,  0.5f,  
//
//    -0.5f, -0.5f, -0.5f,  
//     0.5f, -0.5f, -0.5f,  
//     0.5f, -0.5f,  0.5f,  
//     0.5f, -0.5f,  0.5f,  
//    -0.5f, -0.5f,  0.5f,  
//    -0.5f, -0.5f, -0.5f,  
//
//    -0.5f,  0.5f, -0.5f,  
//     0.5f,  0.5f, -0.5f,  
//     0.5f,  0.5f,  0.5f,  
//     0.5f,  0.5f,  0.5f,  
//    -0.5f,  0.5f,  0.5f,  
//    -0.5f,  0.5f, -0.5f,  
//};
//


int main()
{
    using namespace Rendering;
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
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //float vertices[] = { //triangle 
    //-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //down left, uv 0,0
    // 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //down right, uv 1,0
    // 0.0f,  0.5f, 0.0f, 0.5f, 1.0f, //up, uv .5, 1
    //}; //counter-clockwise notation

    //per coordinate we have 3 floats, so multiply byte size by 3
    unsigned int sizePerCoordinate = sizeof(float) * 3;
    //unsigned int sizePerUV = sizeof(float) * 2;
    unsigned int sizePerNormal = sizeof(float) * 3;
    unsigned int sizePerUV = sizeof(float) * 2;
    unsigned int sizePerVertex = sizePerCoordinate + sizePerNormal + sizePerUV;
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

    unsigned int normalLocationIndex = 1;
    glVertexAttribPointer(normalLocationIndex, 3, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)sizePerCoordinate);
    glEnableVertexAttribArray(normalLocationIndex);

    unsigned int uvLocationIndex = 2;
    glVertexAttribPointer(uvLocationIndex, 2, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)(sizePerCoordinate + sizePerNormal));
    glEnableVertexAttribArray(uvLocationIndex);


    //load texture
    Texture brickTex("textures/brick.jpg");
    Texture brickNormalsTex("textures/brickNormals.png");

    //start shader
    bool success;

    Shader* simpleShader = new Shader("simpleShader");;
    Shader lightShader("lightShader");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_BACK);

    glBindVertexArray(vertexArrayObjectId); //bind our vertex data + attribute settings
    simpleShader->setFloat("time", 1);

    glActiveTexture(GL_TEXTURE0);
    brickTex.Use();
    glActiveTexture(GL_TEXTURE1);
    brickNormalsTex.Use();


    auto lightPosition = glm::vec3(0, 0, -2);

    while (!glfwWindowShouldClose(window))
    {
        //1 frame of the game per loop
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec2 mouseDelta = glm::vec2(xpos - mousePos.x, ypos - mousePos.y);

        mousePos.x = xpos;
        mousePos.y = ypos;

        const float cameraRotationSpeed = .2f;
        if (cameraButtonHeld) {
            camRot.y += mouseDelta.x * cameraRotationSpeed;
            camRot.x += mouseDelta.y * cameraRotationSpeed;
        }

        glm::vec3 cameraForward(0, 0, 0);
        cameraForward.x = glm::cos(glm::radians(camRot.y)) * glm::cos(glm::radians(camRot.x));
        cameraForward.y = glm::sin(glm::radians(camRot.x));
        cameraForward.z = glm::sin(glm::radians(camRot.y)) * glm::cos(glm::radians(camRot.x));

        cameraForward = normalize(cameraForward);


        //std::cout << "Delta: x" << mouseDelta.x << " y" << mouseDelta.y << std::endl;

        if (recompileShaders) {

            delete simpleShader;
            simpleShader = new Shader("simpleShader");
            recompileShaders = false;
            std::cout << "Shaders recompiled!" << std::endl;
        }

        simpleShader->Use();

        simpleShader->setVec("lightPosition", lightPosition);


        float time = glfwGetTime();

        simpleShader->setFloat("time", time);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 proj = glm::ortho(-(float)WINDOW_WIDTH, (float)WINDOW_WIDTH, -(float)WINDOW_HEIGHT, (float)WINDOW_HEIGHT, .1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //m = glm::translate(glm::vec3(0, 0, -5.0f) - camPos);
        //model = glm::rotate(model, glm::radians(90.0f + time * 10), glm::vec3(0, 1, 0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 15), glm::vec3(0, 0, 1));

        auto viewMatrix = glm::lookAtLH(camPos, camPos + cameraForward, glm::vec3(0, 1, 0));
        //auto viewMatrix = glm::mat4(1.0f);
        //viewMatrix = glm::lookAt(camPos, camPos + camRot, glm::vec3(0, 1, 0));
        //viewMatrix = glm::translate(glm::vec3(camPos.x, camPos.y, -camPos.z));
        //viewMatrix = glm::inverse(viewMatrix);

        //m = glm::rotate(m, glm::radians(45.0f), glm::vec3(1, 0, 0));

        simpleShader->setMat4("modelMatrix", model);
        simpleShader->setMat4("viewMatrix", viewMatrix);
        simpleShader->setMat4("projectionMatrix", proj);

        //simpleShader->Use();

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::mat4(1.0f);
        //model = glm::translate(glm::vec3(3,0,0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 5), glm::vec3(0, 1, 0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 2), glm::vec3(0, 0, 1));

        //simpleShader->setMat4("modelMatrix", model);

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::mat4(1.0f);
        //model = glm::translate(glm::vec3(-2, .5, 0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 2), glm::vec3(0, 1, 0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 2.5f), glm::vec3(0, 0, 1));

        //simpleShader->setMat4("modelMatrix", model);

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::mat4(1.0f);
        //model = glm::translate(glm::vec3(1.5, 0, 1));
        //model = glm::rotate(model, glm::radians(90.0f + time * 7), glm::vec3(0, 1, 0));
        //model = glm::rotate(model, glm::radians(90.0f + time * 3), glm::vec3(0, 0, 1));

        //simpleShader->setMat4("modelMatrix", model);

        //glDrawArrays(GL_TRIANGLES, 0, 36);


        //drawing light cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(.1f, .1f, .1f));

        lightShader.Use();

        lightShader.setMat4("modelMatrix", model);
        lightShader.setMat4("viewMatrix", viewMatrix);
        lightShader.setMat4("projectionMatrix", proj);

        glDrawArrays(GL_TRIANGLES, 0, 36);






        //vsync would wait here till next frame
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}