#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <stb/stb_image.h>

class Texture {
public:
	Texture();
	Texture(std::string dir, std::string path, aiTextureType type);


	void generate();
	void load(bool flip = true);

	/*void setFilters(GLenum all);
	void setFilters(GLenum mag, GLenum min);

	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);*/

	void bind();

	unsigned int id;
	aiTextureType type;
	std::string dir;
	std::string path;

	//texture object

	/*int id;
	unsigned int tex;
	const char* name;
private:
	const char* path;
	static int currentId;
	int width, height, nChannels;*/
};


#endif