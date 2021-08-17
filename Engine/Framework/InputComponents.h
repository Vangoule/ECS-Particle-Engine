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


	struct CameraComponent
	{
		ECS_DECLARE_TYPE;


		//Connect to object to add system data to object
		CameraComponent(glm::vec3 p) : m_pos(p) {}
		CameraComponent(glm::vec3 p, glm::vec3 at, glm::vec3 up) : m_pos(p), m_up(up), m_at(at) { lookAt = true; m_sensitivity = 10.0f; m_speed = 15.0f, m_defaultSpeed = m_speed, m_boostSpeed = 30.0f, m_forwardKey = sf::Keyboard::W, m_backKey = sf::Keyboard::S, m_leftKey = sf::Keyboard::A, m_rightKey = sf::Keyboard::D; }
		CameraComponent(glm::vec3 p, glm::vec3 at, glm::vec3 up, float sens, float speed, float boost) : m_pos(p), m_up(up), m_at(at) { lookAt = true; m_sensitivity = sens; m_speed = speed, m_defaultSpeed = m_speed, m_boostSpeed = boost, m_forwardKey = sf::Keyboard::W, m_backKey = sf::Keyboard::S, m_leftKey = sf::Keyboard::A, m_rightKey = sf::Keyboard::D; }



		glm::vec3 m_pos;
		glm::vec3 m_up;
		glm::vec3 m_at;

		glm::vec3 m_dir;
		glm::vec3 m_right;

		float m_sensitivity;

		float m_speed;
		float m_boostSpeed;
		float m_defaultSpeed;

		int m_forwardKey;
		int m_backKey;
		int m_leftKey;
		int m_rightKey;

		float horizontalAngle = 0.0f;
		float verticalAngle = 0.0f;

		bool lookAt = false;

	};

	ECS_DEFINE_TYPE(CameraComponent);

}