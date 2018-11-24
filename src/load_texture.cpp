#include <string>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "load_texture.hpp"
#include "stb_image.h"

unsigned int loadTexture(std::string file, std::string path) {
	std::string filename = path + file;
	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
	
	if (data == NULL) {
		std::cout << "failed to load image " << filename << '\n';
		exit(1);
	}
	
	// create openGL texture
	unsigned int texture;
	glGenTextures(1, &texture);
	
	// bind texture for future operations
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// give image to openGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(data);
	return texture;
}