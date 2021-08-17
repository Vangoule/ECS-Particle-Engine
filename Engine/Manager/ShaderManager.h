#pragma once
#include "Manager.h"
#include "Shader.h"

#define DEBUGSHADERS false
namespace DOI {
	class ShaderManager : public Manager<ShaderManager>
	{
	public:
		ShaderManager(void);
		~ShaderManager(void);

		enum ShaderType
		{
			COLOUR,
			TEXTURE,
			PARTICLE,
			COUNT,
		};

		void Initialize();

		static Shader* GetShader(ShaderType pType);
	private:
		static Shader** shaders;
	};
}


