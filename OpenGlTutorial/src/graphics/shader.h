#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>
#include<assimp/scene.h>

class Shader {
public :
	unsigned int id;
	//Constructor
	Shader();
	Shader(const char* vertexSHaderPath, const char* fragmentShaderPath);

	void generate(const char* vertexShaderPath, const char* fragmentShaerPath);

	void activate();

	//utility functions
	std::string loadShaderSrc(const char* filepath);
	GLuint compileShader(const char* filepath, GLenum type);

	//uniform functions
	void setBool(const std::string& name, bool value);
	void setFloat(const std::string& name, float value);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void set4Float(const std::string& name, aiColor4D color);
	void set4Float(const std::string& name, glm::vec4 v);
	void set3Float(const std::string& name, glm::vec3 v);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void setMat4(const std::string& name, glm::mat4 val);
	void setInt(const std::string& name, int value);


};
#endif
