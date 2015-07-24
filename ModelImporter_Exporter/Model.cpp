#include "Model.h"

GLint TextureFromFile(const char* path, std::string &directory);

Model::Model(GLchar* path)
	: mPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	, mRotation(glm::vec3(0.0f, 0.0f, 0.0f))
	, mRotAngle(0.0f)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	glm::mat4 model;
	model = glm::translate(model, mPosition);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	model = glm::rotate(model, mRotAngle, mRotation);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	for (GLuint i = 0; i < mMeshes.size(); ++i)
	{
		mMeshes[i].Draw(shader);
	}
}

void Model::Translate(glm::vec3 transBy)
{
	mPosition += transBy;
}

void Model::SetPosition(glm::vec3 transTo)
{
	mPosition = transTo;
}

void Model::Rotate(float angle, glm::vec3 rot)
{
	mRotation = rot;
	mRotAngle = angle;
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR: Assimp-> %s\n", importer.GetErrorString());
		return;
	}

	// Retrieve directory from path and process nodes
	mDirectory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}
	// Do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// Process vertex positions
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// Texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];		
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());		
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < mTexturesLoaded.size(); ++j)
		{
			if (mTexturesLoaded[j].path == str)
			{
				textures.push_back(mTexturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			// If texture hasn't been loaded already, load it
			// TODO::create memory management system for resource sharing
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), mDirectory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			mTexturesLoaded.push_back(texture);
		}
	}
	return textures;
}

GLint TextureFromFile(const char* path, std::string &directory)
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}