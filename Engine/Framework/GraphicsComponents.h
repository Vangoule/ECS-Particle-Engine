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


	struct RenderComponent
	{
		ECS_DECLARE_TYPE;

		//Connect to object to add system data to object
		RenderComponent(std::vector < glm::vec3 > v) : m_vertices(v), m_indexed(false), m_visible(true) {}
		RenderComponent(std::vector < glm::vec3 > v, std::vector<unsigned int> i) : m_vertices(v), m_indices(i), m_indexed(true), m_visible(true) {}
		RenderComponent(std::vector < glm::vec3 > v, std::vector<unsigned int> i, Texture* texture) : m_vertices(v), m_indices(i), m_indexed(true), m_texture(texture), m_visible(true) {}
		RenderComponent(std::vector < glm::vec3 > v, std::vector<unsigned int> i, std::vector < glm::vec2 > uvs, Texture* texture) : m_vertices(v), m_indices(i), m_indexed(true), m_texture(texture), m_uvs(uvs), m_visible(true) {}
		RenderComponent() {}

		unsigned int m_vbo;
		unsigned int m_ibo;
		unsigned int m_nbo;
		unsigned int m_tbo;
		unsigned int m_ubo;
		unsigned int m_vao;

		std::vector<glm::vec3> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec2>  m_uvs;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec3>  m_tangentNormals;

		Texture* m_texture;

		int m_size;
		bool m_indexed;

		bool m_visible;
	};

	ECS_DEFINE_TYPE(RenderComponent);
}