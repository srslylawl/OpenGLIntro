#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image/stb_image.h";
#include <string>
#include <glad/glad.h>
#include <iostream>

class Texture {
	int width, height, nrChannels;

	unsigned int textureId = 0;

public:
	Texture(std::string path) {
		LoadTexture(path);
	}

	void Use() {
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void LoadTexture(std::string path) {
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "ERROR: unable to load texture: '" << path << "'" << std::endl;
			return;
		}

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		int pixelDataChannelFormat = 0;
		if (nrChannels == 3) {
			pixelDataChannelFormat = GL_RGB;
		}
		else if (nrChannels == 4) {
			pixelDataChannelFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixelDataChannelFormat, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);


		stbi_image_free(data);
	}

};