#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

void setOptions();
unsigned genTexture(unsigned shader, const char* path, const char* name, unsigned val, bool alpha);

auto texture_init(unsigned shader) {
	return std::pair<unsigned, unsigned> { genTexture(shader, "C:/Users/Aman/source/repos/LearnGL/1.Basics/lib/container.jpg", "texture1", 0, false),
										   genTexture(shader, "C:/Users/Aman/source/repos/LearnGL/1.Basics/lib/awesomeface.png", "texture2", 1, true) };
}

unsigned genTexture(unsigned shader, const char* path, const char* name, unsigned val, bool alpha) {
	// load texture
	int w, h, nrChannels;
	unsigned char* data = stbi_load(path, &w, &h, &nrChannels, 0);

	// generate texture
	unsigned texture;
	glGenTextures(1, &texture);

	// set active
	glBindTexture(GL_TEXTURE_2D, texture);

	setOptions();

	// bind data with texture and generate mipmap for current active
	glTexImage2D(GL_TEXTURE_2D, 0, alpha?GL_RGBA:GL_RGB, w, h, 0, alpha?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glUseProgram(shader); // don't forget to activate/use the shader before setting uniforms!

	glUniform1i(glGetUniformLocation(shader, name), val);

	return texture; 
}

void setOptions() {
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}