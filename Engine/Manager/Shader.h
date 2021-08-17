#pragma once

#include <fstream>
#include <glm/glm.hpp>

#define LOGGING_LEVEL_1
#include "../Utils/Logger.h"

#include "../Framework/GraphicsComponents.h"
#include "../Framework/TransformComponents.h"
#include "../Framework/InputComponents.h"
#include "../Framework/PhysicsComponents.h"
#include "../Framework/ParticleComponents.h"

#include "../Framework/ECS.h"
#define DEBUGSHADERS false
namespace DOI {
	class Shader
	{
	public:
		Shader(void);
		~Shader(void);

		bool Initialize(const char* vsFileName, const char* psFileName);

		void BindAttributes(int index, const char* pName);

		void BeginShader();

		void EndShader();

		void UpdateUniforms();

		void UpdateUniforms(ComponentHandle<TransformComponent> trans, ComponentHandle<CameraComponent> cam);

		void UpdateUniforms(ComponentHandle<PhysicsComponent> phys, ComponentHandle<CameraComponent> cam);

		void UpdateUniforms(ComponentHandle<ParticleComponent> particles, ComponentHandle<CameraComponent> cam);

		glm::mat4 GetViewProjection(ComponentHandle<CameraComponent> cam);

		bool SetShaderMatrixParameter(const char* pName, float* pMatrix);

		bool SetShaderFloatParameter(const char* pName, float pValue);

		bool SetShaderIntParameter(const char* pName, int pValue);

		bool SetShaderVec2Parameter(const char* pName, glm::vec2 pValue);

		bool SetShaderVec3Parameter(const char* pName, glm::vec3 pValue);

		bool SetShaderSamplerParameter(const char* pName, int pSlot, int pHandle);
	private:

		bool InitializeShader(const char* vsFileName, const char* psFileName);

		char* LoadShaderSourceFile(const char* pFilename);

		void ShaderErrorMessage(unsigned int pShaderID, const char* pShaderFileName);

		void LinkerErrorMessage(unsigned int pProgramID);

		unsigned int vertexShader;
		unsigned int pixelShader;
		unsigned int shaderProgram;
	};
}
