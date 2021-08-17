#pragma once
#include <iostream>
#include "ECS.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace DOI {
	ECS_TYPE_IMPLEMENTATION;
	using namespace ECS;

	struct TransformComponent
	{
		//Connect to object to add system data to object
		TransformComponent(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : m_pos(pos), m_rot(rot), m_scale(scale), m_oldPos(m_pos) {}
		TransformComponent(glm::vec3 pos, glm::quat rot) : m_pos(pos), m_rot(rot), m_oldPos(m_pos) { m_scale = glm::vec3(1, 1, 1); }
		TransformComponent(glm::vec3 pos) : m_pos(pos), m_oldPos(m_pos) { m_rot = glm::quat(); m_scale = glm::vec3(1, 1, 1); }
		TransformComponent() { m_pos = glm::vec3(0, 0, 0); m_rot = glm::quat(); m_scale = glm::vec3(1, 1, 1); }

		glm::vec3 m_pos;
		glm::quat m_rot;
		glm::vec3 m_scale;

		glm::vec3 m_oldPos;

	};

	ECS_DEFINE_TYPE(TransformComponent);
}