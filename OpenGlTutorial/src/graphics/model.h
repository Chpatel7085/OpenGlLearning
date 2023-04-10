#ifndef MODEL_H
#define MODEL_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "mesh.h"

class Model {
public:
	//generalize the pos and size for the models
	glm::vec3 pos;
	glm::vec3 size;


	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void init();
	void loadModel(std::string path);

	void render(Shader shader, bool setModel = true);
	void cleanup();

protected:
	std::vector<Mesh> meshes;
	std::string directory;

	bool noTex;
	//Textures that have been loaded
	std::vector<Texture> textures_loaded;
	//node - > each node in assimp library 
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};



#endif