#pragma once

#pragma once
#include "GraphicsSystem.h"
#include <sstream>

#include "../Manager/WindowManager.h"
#include "../Framework/TransformComponents.h"
#include "../Framework/InputComponents.h"
#include "../Framework/PhysicsComponents.h"

namespace DOI {

	void GraphicsSystem::configure(class World* world)
	{
		//world->subscribe<Events::OnEntityCreated>(this);
		world->subscribe<Events::OnEntityDestroyed>(this);
		world->subscribe<Events::OnEntityInit>(this);

		m_scene = world;

		glewInit();

		ilInit();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_isRendering = true;

		m_shaders = &ShaderManager::Get();
		m_shaders->Initialize();

		//glGenVertexArrays(1, &m_vao);
		//glBindVertexArray(m_vao);

		LOG("Graphics System has been initiated successfully!");

		WindowManager::Get().getHandle()->setActive(true);
	}

	void GraphicsSystem::unconfigure(class World* world)
	{
		world->unsubscribeAll(this);
	}

	void GraphicsSystem::receive(class World* world, const Events::OnEntityCreated& event)
	{

	}

	void GraphicsSystem::receive(class World* world, const Events::OnEntityInit& event)
	{
		ComponentHandle<RenderComponent> graphics = event.entity->get<RenderComponent>();
		if (graphics != NULL) {
			glGenVertexArrays(1, &graphics->m_vao);
			glBindVertexArray(graphics->m_vao);
			
			graphics->m_size = graphics->m_vertices.size();

			glGenBuffers(1, &(graphics->m_vbo));
			glBindBuffer(GL_ARRAY_BUFFER, graphics->m_vbo);
			glBufferData(GL_ARRAY_BUFFER, graphics->m_vertices.size() * sizeof(glm::vec3), &graphics->m_vertices[0], GL_STATIC_DRAW);


			if (graphics->m_indexed == true) {
				glGenBuffers(1, &(graphics->m_ibo));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphics->m_ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, graphics->m_indices.size() * sizeof(unsigned int), &graphics->m_indices[0], GL_STATIC_DRAW);
				graphics->m_size = graphics->m_indices.size();
			}


			glGenBuffers(1, &(graphics->m_nbo));
			glGenBuffers(1, &(graphics->m_ubo));
			glGenBuffers(1, &(graphics->m_tbo));

			if (graphics->m_uvs.size() == 0)
			{
				std::vector<glm::vec2> uvs;
				for (int i = 0; i < graphics->m_vertices.size(); i++)
				{
					glm::vec3 v1 = graphics->m_vertices[i];


					uvs.push_back(glm::vec2(v1.x, v1.y));

				}
				graphics->m_uvs = uvs;

			}

			glBindBuffer(GL_ARRAY_BUFFER, graphics->m_ubo);
			glBufferData(GL_ARRAY_BUFFER, graphics->m_uvs.size() * sizeof(glm::vec2), &graphics->m_uvs[0], GL_STATIC_DRAW);

			if (graphics->m_normals.size() == 0)
			{
				CalcNormals(graphics->m_vertices, graphics->m_normals, graphics->m_indices);

				graphics->m_tangentNormals = graphics->m_normals;
			}

			glBindBuffer(GL_ARRAY_BUFFER, graphics->m_nbo);
			glBufferData(GL_ARRAY_BUFFER, graphics->m_normals.size() * sizeof(glm::vec3), &graphics->m_normals[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, graphics->m_tbo);
			glBufferData(GL_ARRAY_BUFFER, graphics->m_tangentNormals.size() * sizeof(glm::vec3), &graphics->m_tangentNormals[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		ComponentHandle<CameraComponent> cam = event.entity->get<CameraComponent>();
		if (cam)
		{
			m_mainCamera = cam;
		}
	}

	void GraphicsSystem::receive(class World* world, const Events::OnEntityDestroyed& event)
	{
		ComponentHandle<RenderComponent> comp = event.entity->get<RenderComponent>();
		if (comp.isValid()) {
			if (comp->m_vao) glDeleteBuffers(1, &comp->m_vao);
			if (comp->m_vbo) glDeleteBuffers(1, &comp->m_vbo);
			if (comp->m_ibo) glDeleteBuffers(1, &comp->m_ibo);
			if (comp->m_ubo) glDeleteBuffers(1, &comp->m_ubo);
			if (comp->m_nbo) glDeleteBuffers(1, &comp->m_nbo);
			if (comp->m_tbo) glDeleteBuffers(1, &comp->m_tbo);
		}


	}

	void GraphicsSystem::receive(class World* world, const Events::OnComponentRemoved<RenderComponent>& event)
	{

	}

	void GraphicsSystem::tick(class World* world, float deltaTime) {
		if (m_isRendering)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_shaders->GetShader(ShaderManager::TEXTURE)->BeginShader();

			for (Entity* entity : world->each<RenderComponent>())
			{


				//Get all entities with graphics components and loop through them.
				ComponentHandle<RenderComponent> comp = entity->get<RenderComponent>();

				if (comp->m_visible)
				{

					//Texture
					if (comp->m_texture)
						comp->m_texture->Bind();
					m_shaders->GetShader(ShaderManager::TEXTURE)->SetShaderIntParameter("diffuse", 0);

					glBindVertexArray(comp->m_vao);

					//Vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, comp->m_vbo);
					glVertexAttribPointer(
						0,
						3,
						GL_FLOAT,
						GL_FALSE,
						0,
						(void*)0
					);

					//UVs
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, comp->m_ubo);
					glVertexAttribPointer(
						1,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						(void*)0
					);

					//Normal
					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, comp->m_nbo);
					glVertexAttribPointer(
						2,
						3,
						GL_FLOAT,
						GL_FALSE,
						0,
						(void*)0
					);

					//Tangent Normals
					glEnableVertexAttribArray(3);
					glBindBuffer(GL_ARRAY_BUFFER, comp->m_tbo);
					glVertexAttribPointer(
						3,
						3,
						GL_FLOAT,
						GL_FALSE,
						0,
						(void*)0
					);

					//ComponentHandle<TransformComponent> trans = entity->get<TransformComponent>();
					//m_shaders->GetShader(ShaderManager::TEXTURE)->UpdateUniforms();
					//m_shaders->GetShader(ShaderManager::TEXTURE)->UpdateUniforms(trans, m_mainCamera);

					bool render = true;

					ComponentHandle<PhysicsComponent> physics = entity->get<PhysicsComponent>();
					//bool render = true;

					if (!physics) {
						//Get the transform, send the uniforms to the shader including the camera
						ComponentHandle<TransformComponent> trans = entity->get<TransformComponent>();
						m_shaders->GetShader(ShaderManager::TEXTURE)->UpdateUniforms(trans, m_mainCamera);

					}
					else {
						if (physics->m_body != nullptr) {
							m_shaders->GetShader(ShaderManager::TEXTURE)->UpdateUniforms(physics, m_mainCamera);

						}
					}



					if (render == true) {
						//Draw with indices or basic vertex array
						if (comp->m_indexed == true) {
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp->m_ibo);
							glDrawElements(GL_TRIANGLES, comp->m_size, GL_UNSIGNED_INT, 0);
						}
						else {
							glBindBuffer(GL_ARRAY_BUFFER, comp->m_vbo);
							glDrawArrays(GL_TRIANGLES, 0, comp->m_size);
						}
					}


					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);
					glDisableVertexAttribArray(3);

					glBindVertexArray(0);
				}


			}
			//Unbind the shader
			m_shaders->GetShader(ShaderManager::TEXTURE)->EndShader();


			ShaderManager::GetShader(ShaderManager::PARTICLE)->BeginShader();
			//Start Particles
			for (Entity* entity : world->each<ParticleComponent>())
			{
				ComponentHandle<ParticleComponent> particles = entity->get<ParticleComponent>();

				if (particles)
				{
					glBindVertexArray(particles->m_vao);


					ShaderManager::GetShader(ShaderManager::PARTICLE)->UpdateUniforms(particles, m_mainCamera);

					if (particles->m_texture)
						particles->m_texture->Bind();
					m_shaders->GetShader(ShaderManager::PARTICLE)->SetShaderIntParameter("diffuse", 0);

					glBindBuffer(GL_ARRAY_BUFFER, particles->m_offsetBuffer);
					glBufferData(GL_ARRAY_BUFFER, particles->m_maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
					glBufferSubData(GL_ARRAY_BUFFER, 0, particles->m_particlesCount * sizeof(GLfloat) * 4, particles->m_positions);

					glBindBuffer(GL_ARRAY_BUFFER, particles->m_colourBuffer);
					glBufferData(GL_ARRAY_BUFFER, particles->m_maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
					glBufferSubData(GL_ARRAY_BUFFER, 0, particles->m_particlesCount * sizeof(GLubyte) * 4, particles->m_colours);

					glDepthMask(GL_FALSE);
					glEnable(GL_BLEND);
					//glBlendFunc(GL_ONE, GL_ONE); //CHAOTIC, Maybe rockets
					glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Above but has alphaz
					//glBlendFunc(GL_SRC_COLOR, GL_ONE); // Mid Range
					//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // more controlled but has artefacts
					//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR); // SMOKEY?
					//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Terrible


					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, particles->m_vbo);
					glVertexAttribPointer(
						0,                  
						3,                 
						GL_FLOAT,           
						GL_FALSE,          
						0,                 
						(void*)0            
					);

	
					glEnableVertexAttribArray(1);
					glBindBuffer(GL_ARRAY_BUFFER, particles->m_offsetBuffer);
					glVertexAttribPointer(
						1,                        
						4,                        
						GL_FLOAT,                       
						GL_FALSE,                      
						0,                           
						(void*)0                        
					);


					glEnableVertexAttribArray(2);
					glBindBuffer(GL_ARRAY_BUFFER, particles->m_colourBuffer);
					glVertexAttribPointer(
						2,                          
						4,                              
						GL_UNSIGNED_BYTE,            
						GL_TRUE,                         
						0,                         
						(void*)0                       
					);

					glVertexAttribDivisor(0, 0); // vert
					glVertexAttribDivisor(1, 1); // pos
					glVertexAttribDivisor(2, 1); // color

					glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles->m_particlesCount);

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);

					glBindVertexArray(0);

					glVertexAttribDivisor(0, 0); // vert
					glVertexAttribDivisor(1, 0); // pos
					glVertexAttribDivisor(2, 0); // color

					glDisable(GL_BLEND);
					glDepthMask(GL_TRUE);



				}
			}
			ShaderManager::GetShader(ShaderManager::PARTICLE)->EndShader();

			WindowManager::Get().getHandle()->display();


		}

	}


	void GraphicsSystem::CalcNormals(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices)
	{
		//std::cout << "CALC NORMALS" << std::endl;

		normals.clear();
		for (int i = 0; i < indices.size() / 3; i += 3)
		{

			glm::vec3 p1 = vertices[indices[i + 0]];
			glm::vec3 p2 = vertices[indices[i + 1]];
			glm::vec3 p3 = vertices[indices[i + 2]];

			glm::vec3 v1 = p2 - p1;
			glm::vec3 v2 = p3 - p1;
			glm::vec3 normal = glm::cross(v1, v2);

			glm::normalize(normal);

			// Store the face's normal for each of the vertices that make up the face.
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
		}

		for (int i = 0; i < normals.size(); i++)
			glm::normalize(normals[i]);


	}


}
