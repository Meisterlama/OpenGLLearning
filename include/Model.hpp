#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.hpp"
#include <vector>
#include <string>

#include "Mesh.hpp"

class Model
{
public:
	Model(std::string path);
	void Draw(Shader &shader);

private:
	std::vector<Texture> m_texturesLoaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory);
};
