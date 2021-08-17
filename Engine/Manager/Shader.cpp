#include "Shader.h"
#include <assert.h>
#include <iostream>
#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include "WindowManager.h"

#include <sstream>


using namespace std;

namespace DOI {

	Shader::Shader(void)
	{
	}


	Shader::~Shader(void)
	{
		glDetachShader(shaderProgram, vertexShader);
		glDetachShader(shaderProgram, pixelShader);
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
		glDeleteProgram(shaderProgram);
	}

	glm::mat4 Shader::GetViewProjection(ComponentHandle<CameraComponent> cam)
	{
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)WindowManager::Get().getSize().x / (float)WindowManager::Get().getSize().y, 0.01f, 1000.0f);

		glm::mat4 View = glm::mat4(1.0f);

		if (cam->lookAt == true) {
			View = glm::lookAt(
				cam->m_pos,
				cam->m_pos + cam->m_at,
				glm::vec3(0,1,0)
			);
		}
		else {
			View = glm::translate(View, -glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z));

		}

		return Projection * View;
	}

	void Shader::UpdateUniforms(ComponentHandle<TransformComponent> trans, ComponentHandle<CameraComponent> cam)
	{
		

		glm::mat4 translationMatrix = glm::mat4(1);
		glm::mat4 rotationMatrix = glm::mat4(1);
		glm::mat4 scaleMatrix = glm::mat4(1);

		translationMatrix = glm::translate(trans->m_pos);
		scaleMatrix = glm::scale(trans->m_scale);
		rotationMatrix = glm::toMat4(trans->m_rot);

		glm::mat4 Model = rotationMatrix * translationMatrix  * scaleMatrix;

		glm::mat4 MVP = GetViewProjection(cam) * Model;

		SetShaderMatrixParameter("MVP", (float*)&MVP[0][0]);
	}

	void Shader::UpdateUniforms(ComponentHandle<PhysicsComponent> phys, ComponentHandle<CameraComponent> cam)
	{
		glm::mat4 MVP = GetViewProjection(cam) * phys->m_modelMatrix;

		SetShaderMatrixParameter("MVP", (float*)&MVP[0][0]);
	}

	void Shader::UpdateUniforms(ComponentHandle<ParticleComponent> particles, ComponentHandle<CameraComponent> cam)
	{
		glm::mat4 VP = GetViewProjection(cam);
		SetShaderMatrixParameter("VP", (float*)&VP[0][0]);
		
		SetShaderVec3Parameter("CameraRight_worldspace", cam->m_right);
		SetShaderVec3Parameter("CameraUp_worldspace", cam->m_up);
	}

	void Shader::UpdateUniforms()
	{
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)WindowManager::Get().getSize().x / (float)WindowManager::Get().getSize().y, 0.01f, 1000.0f);

		glm::mat4 View = glm::lookAt(
			glm::vec3(0, 0, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		glm::mat4 Model = glm::mat4(1.0f);

		glm::mat4 MVP = Projection * View * Model;

		SetShaderMatrixParameter("MVP", (float*)&MVP[0][0]);
	}

	bool Shader::Initialize(const char* vsFileName, const char* psFileName)
	{
		char vs[128] = "../Resources/Shaders/";
		strcat_s(vs, vsFileName);

		char ps[128] = "../Resources/Shaders/";
		strcat_s(ps, psFileName);

		LOG("Initializing Shaders: ", vsFileName, " + ", psFileName);
		//std::cout << "Initializing Shaders: " << vsFileName << " + " << psFileName << std::endl;
		if (!InitializeShader(vs, ps))
		{
			cout << "" << vsFileName << " " << psFileName << " Couldn't be initialized!" << endl;

			return false;
		}

		LOG("Shader Initialied successfully");
		//std::cout << "Shader Initialied successfully" << std::endl;

		return true;
	}

	bool Shader::InitializeShader(const char* vsFileName, const char* psFileName)
	{
		const char* vertexShaderBuffer;
		const char* pixelShaderBuffer;
		int status;

		vertexShaderBuffer = LoadShaderSourceFile(vsFileName);
		if (!vertexShaderBuffer)
		{
			cout << "Shader vertex buffer - Couldn't be initialized!" << endl;
			return false;
		}

		pixelShaderBuffer = LoadShaderSourceFile(psFileName);
		if (!pixelShaderBuffer)
		{
			cout << "Pixel vertex buffer - Couldn't be initialized!" << endl;
			return false;
		}

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderBuffer, NULL);
		glShaderSource(pixelShader, 1, &pixelShaderBuffer, NULL);

		delete[] vertexShaderBuffer;
		vertexShaderBuffer = 0;

		delete[] pixelShaderBuffer;
		pixelShaderBuffer = 0;

		glCompileShader(vertexShader);
		glCompileShader(pixelShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != 1)
		{
			ShaderErrorMessage(vertexShader, vsFileName);
			return false;
		}

		glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &status);
		if (status != 1)
		{
			ShaderErrorMessage(pixelShader, vsFileName);
			return false;
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, pixelShader);

		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

		if (status != 1)
		{
			LinkerErrorMessage(shaderProgram);
			return false;
		}

		return true;
	}

	void Shader::BindAttributes(int index, const char* pName)
	{
		assert(shaderProgram);
		glBindAttribLocation(shaderProgram, index, pName);
	}

	char* Shader::LoadShaderSourceFile(const char* pFileName)
	{
		FILE* pFile = NULL;
		char* shaderBuffer = NULL;

		fopen_s(&pFile, pFileName, "rb");
		if (!pFile)
		{
			return NULL;
		}

		fseek(pFile, 0L, SEEK_END);

		unsigned int bufferSize = ftell(pFile);

		rewind(pFile);

		shaderBuffer = new char[bufferSize + 1];
		if (fread(shaderBuffer, bufferSize, 1, pFile) <= 0)
		{
			fclose(pFile);
			return NULL;
		}

		shaderBuffer[bufferSize] = '\0';
		fclose(pFile);

		if (DEBUGSHADERS)
			std::cout << shaderBuffer << std::endl;

		return shaderBuffer;
	}

	void Shader::ShaderErrorMessage(unsigned int pShaderID, const char* pShaderFileName)
	{
		int logSize;
		char* infoLog;
		ofstream fout;

		glGetShaderiv(pShaderID, GL_INFO_LOG_LENGTH, &logSize);

		infoLog = new char[logSize + 1];

		glGetShaderInfoLog(pShaderID, logSize, NULL, infoLog);

		fout.open("ShaderError.txt");
		for (int i = 0; i < logSize + 1; ++i)
		{
			fout << infoLog[i];
		}


		std::cout << "Error compiling shader! Check ShaderError.txt! Shader File Name: " << pShaderFileName << std::endl;
	}

	void Shader::LinkerErrorMessage(unsigned int pProgramID)
	{
		int logSize;
		char* infoLog;
		ofstream fout;

		glGetProgramiv(pProgramID, GL_INFO_LOG_LENGTH, &logSize);

		infoLog = new char[logSize + 1];

		glGetProgramInfoLog(pProgramID, logSize, NULL, infoLog);

		fout.open("LinkerError.txt");
		for (int i = 0; i < logSize + 1; ++i)
		{
			fout << infoLog[i];
		}

		std::cout << "Linking Error! Check LinkerError.txt!" << std::endl;
	}

	void Shader::BeginShader()
	{
		glUseProgram(shaderProgram);
	}

	void Shader::EndShader()
	{
		glUseProgram(NULL);
	}

	bool Shader::SetShaderMatrixParameter(const char* pName, float* pMatrix)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, pMatrix);

			return true;
		}

		return false;
	}

	bool Shader::SetShaderFloatParameter(const char* pName, float pValue)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glUniform1f(loc, pValue);
			return true;
		}

		return false;
	}

	bool Shader::SetShaderIntParameter(const char* pName, int pValue)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glUniform1i(loc, pValue);
			return true;
		}

		return false;
	}

	bool Shader::SetShaderVec2Parameter(const char* pName, glm::vec2 pValue)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glUniform2f(loc, pValue.x, pValue.y);
			return true;
		}

		return false;
	}

	bool Shader::SetShaderVec3Parameter(const char* pName, glm::vec3 pValue)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glUniform3f(loc, pValue.x, pValue.y, pValue.z);
			return true;
		}

		return false;
	}

	bool Shader::SetShaderSamplerParameter(const char* pName, int pSlot, int pHandle)
	{
		int loc = glGetUniformLocation(shaderProgram, pName);
		if (loc >= 0)
		{
			glActiveTexture(GL_TEXTURE0 + pSlot);
			glBindTexture(GL_TEXTURE_2D, pHandle);

			glUniform1i(loc, pSlot);

			return true;
		}

		return false;
	}

}