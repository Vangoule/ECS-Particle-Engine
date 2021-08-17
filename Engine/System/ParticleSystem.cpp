#pragma once

#pragma once
#include "ParticleSystem.h"
#include <sstream>

#include "../Manager/WindowManager.h"


#include <algorithm>
namespace DOI {

	void ParticleSystem::configure(class World* world)
	{
		world->subscribe<Events::OnEntityDestroyed>(this);
		world->subscribe<Events::OnEntityInit>(this);
	}

	void ParticleSystem::unconfigure(class World* world)
	{
		world->unsubscribeAll(this);
	}

	void ParticleSystem::receive(class World* world, const Events::OnEntityCreated& event)
	{

	}

	const GLfloat m_vertexData[] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.5f, 0.f,
	};


	void ParticleSystem::receive(class World* world, const Events::OnEntityInit& event)
	{

		ComponentHandle<CameraComponent> cam = event.entity->get<CameraComponent>();
		if (cam)
		{
			m_mainCamera = cam;
		}

		ComponentHandle<ParticleComponent> particles = event.entity->get<ParticleComponent>();

		if (particles)
		{
			particles->m_spread = 1.0f;
			particles->m_mainDir = glm::vec3(0, 0, 0);
			particles->m_useRandomDir = true;
			particles->m_useRandomColour = false;
			particles->m_useRandomScale = true;
			particles->m_gravity = glm::vec3(0.0f, -9.81f, 0.0f);
			particles->m_followCamera = false;
			particles->m_rot = glm::vec3(0, 0, 0);
			particles->m_r = 255;
			particles->m_g = 255;
			particles->m_b = 255;
			particles->m_a = 255;
			particles->m_size = 0.5f;

			particles->m_randomSizeScale = 0.25;
			particles->m_randomMax = 2000;
			particles->m_randomMinus = 1000;

			particles->m_simulationSpeed = 0.5;

			particles->m_particles = new Particle[particles->m_maxParticles];
			particles->m_positions = new GLfloat[particles->m_maxParticles * 4];
			particles->m_colours = new GLubyte[particles->m_maxParticles * 4];

			for (int i = 0; i < particles->m_maxParticles; i++) {
				particles->m_particles[i].life = -1.0f;
				particles->m_particles[i].cameradistance = -1.0f;
			}
			glGenBuffers(1, &particles->m_vbo);
			glGenBuffers(1, &particles->m_offsetBuffer);
			glGenBuffers(1, &particles->m_colourBuffer);
			glGenVertexArrays(1, &particles->m_vao);
			glBindVertexArray(particles->m_vao);

			glBindBuffer(GL_ARRAY_BUFFER, particles->m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData), m_vertexData, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, particles->m_offsetBuffer);
			glBufferData(GL_ARRAY_BUFFER, particles->m_maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

			
			glBindBuffer(GL_ARRAY_BUFFER, particles->m_colourBuffer);
			glBufferData(GL_ARRAY_BUFFER, particles->m_maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

			glBindVertexArray(0);
		}

	
		//m_texture = tex;
		//m_particlesPerFrame = particlesPerFrame;
		//m_posOffset = posOffset;
		//m_maxLife = maxLife;
		//m_spread = 1.0f;
		//m_maxParticles = particleCount;
		//ParticlesContainer = new Particle[particleCount];
		//m_mainDir = glm::vec3(0, 0, 0);
		//m_useRandomDir = true;
		//m_useRandomColour = false;
		//m_useRandomScale = true;
		//m_gravity = glm::vec3(0.0f, -9.81f, 0.0f);
		//m_followCamera = false;

		//m_rot = glm::vec3(0, 0, 0);

		//m_r = 255;
		//m_g = 255;
		//m_b = 255;
		//m_a = 255;
		//m_size = 0.5f;

		//m_positionData = new GLfloat[m_maxParticles * 4];
		//m_colourData = new GLubyte[m_maxParticles * 4];

		//m_randomSizeScale = 0.25;
		//m_randomMax = 2000;
		//m_randomMinus = 1000;

		//m_objectPos = glm::vec3(0, 0, 0);

		//m_simulationSpeed = 0.5;
	}

	void ParticleSystem::receive(class World* world, const Events::OnEntityDestroyed& event)
	{
		ComponentHandle<ParticleComponent> comp = event.entity->get<ParticleComponent>();
		if (comp.isValid()) {
			delete comp->m_particles;
		if (comp->m_vbo) glDeleteBuffers(1, &comp->m_vbo);
		if (comp->m_offsetBuffer) glDeleteBuffers(1, &comp->m_offsetBuffer);
		if (comp->m_colourBuffer) glDeleteBuffers(1, &comp->m_colourBuffer);
		if (comp->m_positions) delete comp->m_positions;
		if (comp->m_colours) delete comp->m_colours;
		}
	}

	void ParticleSystem::receive(class World* world, const Events::OnComponentRemoved<ParticleComponent>& event)
	{

	}

	int ParticleSystem::FindUnusedParticle(ComponentHandle<ParticleComponent> emitter) {

		for (int i = emitter->m_lastParticle; i < emitter->m_maxParticles; i++) {
			if (emitter->m_particles[i].life < 0) {
				emitter->m_lastParticle = i;
				return i;
			}
		}

		for (int i = 0; i < emitter->m_lastParticle; i++) {
			if (emitter->m_particles[i].life < 0) {
				emitter->m_lastParticle = i;
				return i;
			}
		}

		return 0;
	}

	void ParticleSystem::SortParticles(ComponentHandle<ParticleComponent> emitter) {
		std::sort(&(emitter->m_particles[0]), &(emitter->m_particles[emitter->m_maxParticles]));
	}

	void ParticleSystem::tick(class World* world, float deltaTime) {

		for (Entity* entity : world->each<ParticleComponent>())
		{
			ComponentHandle<ParticleComponent> particles = entity->get<ParticleComponent>();

			if (particles)
			{
				int newparticles = (int)(deltaTime * particles->m_particlesPerFrame);
				if (newparticles > (int)(0.016f * particles->m_particlesPerFrame))
					newparticles = (int)(0.016f * particles->m_particlesPerFrame);

				ComponentHandle<PhysicsComponent> physics = entity->get<PhysicsComponent>();
				glm::vec3 position;
				if (physics) {
					float x = physics->m_body->getWorldTransform().getOrigin().x();
					float y = physics->m_body->getWorldTransform().getOrigin().y();
					float z = physics->m_body->getWorldTransform().getOrigin().z();
					position = glm::vec3(x, y, z);
				}

				//Create new particles
				for (int i = 0; i < newparticles; i++) {
					int particleIndex = FindUnusedParticle(particles);
					Particle& p = particles->m_particles[particleIndex];
					p.life = particles->m_maxLife;
					p.pos = position + particles->m_posOffset;

					if (particles->m_useRandomDir)
					{
						particles->m_randomDir = glm::vec3(
							(rand() % particles->m_randomMax - particles->m_randomMinus) / 1000.0f,
							(rand() % particles->m_randomMax - particles->m_randomMinus) / 1000.0f,
							(rand() % particles->m_randomMax - particles->m_randomMinus) / 1000.0f
						);

						p.speed = particles->m_mainDir + particles->m_randomDir * particles->m_spread;
					}
					else {
						p.speed = particles->m_mainDir * particles->m_spread;
					}

					if (particles->m_useRandomColour)
					{
						p.r = rand() % 256;
						p.g = rand() % 256;
						p.b = rand() % 256;
						p.a = 0.5;
					}
					else {
						p.r = particles->m_r;
						p.g = particles->m_g;
						p.b = particles->m_b;
						p.a = particles->m_a;
					}

					if (particles->m_useRandomScale)
					{
						p.size = ((rand() % 1000) / 2000.0f + 0.1f) * particles->m_randomSizeScale;
					}
					else {
						p.size = particles->m_size;
					}

				}

				//Simulate Current Particles
				particles->m_particlesCount = 0;
				for (int i = 0; i < particles->m_maxParticles; i++) {
					
					Particle& p = particles->m_particles[i]; 
					
					if (p.life > 0.0f) {
						// Decrease life
						p.life -= deltaTime;

						float ratio = (p.life / particles->m_maxLife);
						p.a = ratio * 255;

						if (p.life > 0.0f) {
							// Simulate simple physics : gravity only, no collisions
							p.speed += particles->m_gravity * deltaTime * particles->m_simulationSpeed;
							p.pos += p.speed * deltaTime;
							p.cameradistance = glm::length2(p.pos - (m_mainCamera->m_pos));

							//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

							// Fill the GPU buffer
							particles->m_positions[4 * particles->m_particlesCount + 0] = p.pos.x;
							particles->m_positions[4 * particles->m_particlesCount + 1] = p.pos.y;
							particles->m_positions[4 * particles->m_particlesCount + 2] = p.pos.z;

							particles->m_positions[4 * particles->m_particlesCount + 3] = p.size;

							particles->m_colours[4 * particles->m_particlesCount + 0] = p.r;
							particles->m_colours[4 * particles->m_particlesCount + 1] = p.g;
							particles->m_colours[4 * particles->m_particlesCount + 2] = p.b;
							particles->m_colours[4 * particles->m_particlesCount + 3] = p.a;

						}
						else {
							// Particles that just died will be put at the end of the buffer in SortParticles();
							p.cameradistance = -1.0f;
						}

						particles->m_particlesCount++;

					}
				}

				SortParticles(particles);
			}
		}
	}

}

