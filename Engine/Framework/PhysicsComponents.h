#pragma once
#include <iostream>
#include "ECS.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "btBulletDynamicsCommon.h"

namespace DOI {
	ECS_TYPE_IMPLEMENTATION;
	using namespace ECS;

	enum PhysicsType {
		Mesh,
		Box,
		Sphere
	};

	struct PhysicsComponent
	{
		PhysicsComponent() {
			m_type = PhysicsType::Box;  m_isDynamic = true; m_localInertia = btVector3(0, 0, 0); m_mass = btScalar(1.f); m_startLinearVel = glm::vec3(0, 0, 0);
			m_startAngularVel = glm::vec3(0, 0, 0);

		}

		PhysicsComponent(PhysicsType type, double mass = 1, double friction = 0.5, double restitution = 0.0, double damping = 0, glm::vec3 linearVel = glm::vec3(0, 0, 0), glm::vec3 angularVel = glm::vec3(0, 0, 0)) {
			m_type = type;
			m_localInertia = btVector3(0, 0, 0);
			m_friction = friction;
			m_mass = mass;
			m_restitution = restitution;
			m_damping = damping;
			m_startLinearVel = linearVel;
			m_startAngularVel = angularVel;
			m_destroy = false;

			m_duration = 100;
			m_temp = false;
		}

		bool m_destroy;

		int m_duration;
		bool m_temp;

		bool m_isDynamic;
		PhysicsType m_type;
		btVector3 m_localInertia;
		double m_mass;
		double m_friction;
		double m_restitution;
		btScalar m_damping;
		glm::vec3 m_startLinearVel;
		glm::vec3 m_startAngularVel;
		btRigidBody* m_body;
		btDefaultMotionState* m_motionState;
		glm::mat4 m_modelMatrix;
	};
	ECS_DEFINE_TYPE(PhysicsComponent);
}