#pragma once
#include <iostream>
#include <glad/glad.h>
#include "libs/glm/ext/matrix_float4x4.hpp"

class Shader {
public:
	unsigned int id = 0;

    void Use() {
        glUseProgram(id);
    }

    void SetFloat(std::string name, float f) {
        glUniform1f(glGetUniformLocation(id, &name[0]), f);
    }

    void SetMat4(std::string name, glm::mat4x4 mat) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

	static Shader TryCompileShader(const char* vertex, const char* fragment, bool* out_success) {
        Shader s;

        *out_success = true;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates shader object, similar to buffer and returns id
        //vertexShader is 0 if there was an error creating the shader object

        glShaderSource(vertexShader, 1, &vertex, NULL); //pass shader data/string to shader object
        glCompileShader(vertexShader);


        const unsigned int bufferSize = 512;
        char infoLog[bufferSize]; //char buffer that opengl fills with error message
        int success;//get shader information, in this case we want to query the compilation status
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, bufferSize, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

            *out_success = false;
        }

        //exactly the same for fragment shader, except for different argument to create shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragment, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, bufferSize, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

            *out_success = false;
        }

        s.id = glCreateProgram(); //creates a shader program that combines both shaders
        glAttachShader(s.id, vertexShader); //attach both vertex and fragment shaders
        glAttachShader(s.id, fragmentShader);
        glLinkProgram(s.id); //link them together
        glGetProgramiv(s.id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(s.id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;

            *out_success = false;
        }

        glDeleteShader(vertexShader); //the separate shaders are no longer needed since we have our completed shader program
        glDeleteShader(fragmentShader);

		return s;
	}

};