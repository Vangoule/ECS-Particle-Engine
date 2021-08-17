#include "ShaderManager.h"

namespace DOI {
	Shader** ShaderManager::shaders = NULL;

	ShaderManager::ShaderManager(void)
	{
	}


	ShaderManager::~ShaderManager(void)
	{
		for (int i = 0; i < COUNT; ++i)
		{
			delete shaders[i];
		}

		delete[] shaders;
	}

	void ShaderManager::Initialize()
	{
		shaders = new Shader*[COUNT];

		shaders[COLOUR] = new Shader();
		shaders[COLOUR]->Initialize("colour.vs", "colour.fs");

		shaders[TEXTURE] = new Shader();
		shaders[TEXTURE]->Initialize("texture.vs", "texture.fs");

		shaders[PARTICLE] = new Shader();
		shaders[PARTICLE]->Initialize("particle.vs", "particle.fs");
	}

	Shader* ShaderManager::GetShader(ShaderType pType)
	{
		return shaders[pType];
	}
}