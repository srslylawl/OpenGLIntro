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

static void window_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //std::cout << "Window resized, new size:" << width << " x " << height << std::endl;
}

const char* vertexShaderSource = "#version 460\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 vColor;\n"
"out vec3 frag_in_vColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   frag_in_vColor = vColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 460\n"
"out vec4 FragColor;\n"
"in vec3 frag_in_vColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(frag_in_vColor, 1.0f);\n"
"}\n\0";

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to init glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
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


    float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //down left, color red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //down right, color green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f //up, color blue
    }; //counter-clockwise notation

    //per coordinate we have 3 floats, so multiply byte size by 3
    unsigned int sizePerCoordinate = sizeof(float) * 3;
    unsigned int sizePerColor = sizeof(float) * 3;
    unsigned int sizePerVertex = sizePerCoordinate + sizePerColor;
    unsigned int sizeOfTriangleData = 3 * sizePerVertex;

    unsigned int vertexArrayObjectId; //combines the FOLLOWING VBO and attribute pointer calls
    glGenVertexArrays(1, &vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    unsigned int vertexBufferObjectId;
    glGenBuffers(1, &vertexBufferObjectId); //generates vertex buffer and fills id into our pointed to variable
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectId); //bind this buffer so we can modify/use it

    glBufferData(GL_ARRAY_BUFFER, sizeOfTriangleData, vertices, GL_STATIC_DRAW); //fill buffer with our data

    //tell opengl how to interpret our data
    unsigned int coordinateLocationIndex = 0;
    glVertexAttribPointer(coordinateLocationIndex, 3, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)0);
    //at index/location 0, we have 3 floats that make up one element, a float3 or vec3
    glEnableVertexAttribArray(coordinateLocationIndex);
    //enable this element, so it gets passed to the shader

    unsigned int colorLocationIndex = 1;
    glVertexAttribPointer(colorLocationIndex, 3, GL_FLOAT, GL_FALSE, sizePerVertex, (void*)sizePerCoordinate);
    glEnableVertexAttribArray(colorLocationIndex);


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates shader object, similar to buffer and returns id
    //vertexShader is 0 if there was an error creating the shader object

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //pass shader data/string to shader object
    glCompileShader(vertexShader);


    const unsigned int bufferSize = 512;
    char infoLog[bufferSize]; //char buffer that opengl fills with error message
    int success;//get shader information, in this case we want to query the compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); 
    if (!success) {
        glGetShaderInfoLog(vertexShader, bufferSize, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //exactly the same for fragment shader, except for different argument to create shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, bufferSize, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram(); //creates a shader program that combines both shaders
    glAttachShader(shaderProgram, vertexShader); //attach both vertex and fragment shaders
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); //link them together
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader); //the separate shaders are no longer needed since we have our completed shader program
    glDeleteShader(fragmentShader);




    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {
        //1 frame of the game per loop
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        glUseProgram(shaderProgram); //use our shader
        glBindVertexArray(vertexArrayObjectId); //bind our vertex data + attribute settings
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //vsync would wait here till next frame
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}