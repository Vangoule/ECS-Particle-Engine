#pragma once
#include <iostream>
#include "ECS.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "../Manager/Texture.h"

namespace DOI {
	ECS_TYPE_IMPLEMENTATION;
	using namespace ECS;

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life;
		float cameradistance;
		bool operator<(const Particle& that) const {
			return this->cameradistance > that.cameradistance;
		}
	};

	struct ParticleComponent
	{
		ECS_DECLARE_TYPE;

		//Connect to object to add system data to object
		ParticleComponent(Texture* texture, int maxParticles, int particlesPerFrame, float maxLife) { m_texture = texture; m_maxParticles = maxParticles; m_particlesPerFrame = particlesPerFrame; m_maxLife = maxLife; }
		ParticleComponent() {}

		int m_particlesCount;
		int m_particlesPerFrame;
		float m_maxLife;
		float m_spread;
		glm::vec3 m_posOffset;
		int m_maxParticles;

		glm::vec3 m_mainDir;
		glm::vec3 m_randomDir;

		int m_a;
		int m_r;
		int m_g;
		int m_b;
		float m_size;
		glm::vec3 m_rot;

		bool m_useRandomDir;
		bool m_useRandomColour;
		bool m_useRandomScale;
		bool m_followCamera;

		bool m_inRange;

		float m_randomSizeScale;
		int m_randomMax;
		float m_randomMinus;

		float m_simulationSpeed;

		Particle* m_particles;

		glm::vec3 m_gravity;

		Texture* m_texture;

		GLuint m_vao;
		GLuint m_vbo;
		GLuint m_offsetBuffer;
		GLuint m_colourBuffer;

		int m_lastParticle = 0;

		GLfloat* m_positions;
		GLubyte* m_colours;

	
	};

	ECS_DEFINE_TYPE(ParticleComponent);
}