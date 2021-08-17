#pragma once

#pragma once
#include "PhysicsSystem.h"
#include <sstream>

#include "../Manager/WindowManager.h"
#include "../Framework/TransformComponents.h"
#include "../Framework/InputComponents.h"
#include "../Framework/PhysicsComponents.h"

namespace DOI {

	void PhysicsSystem::configure(class World* world)
	{
		//world->subscribe<Events::OnEntityCreated>(this);
		world->subscribe<Events::OnEntityDestroyed>(this);
		world->subscribe<Events::OnEntityInit>(this);
		world->subscribe<Events::OnComponentRemoved<PhysicsComponent>>(this);

		LOG("Physics System has been initiated!");

		m_collisionConfiguration = new btDefaultCollisionConfiguration();

		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

		m_overlappingPairCache = new btDbvtBroadphase();

		m_solver = new btSequentialImpulseConstraintSolver;
		

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

		//btContactSolverInfo& info = m_dynamicsWorld->getSolverInfo();
		//info.m_numIterations = 10;

		timestep = new kairos::Timestep;

		timestep->setStep(1.0 / 120);
		timestep->setMaxAccumulation(0.25);
	}

	void PhysicsSystem::unconfigure(class World* world)
	{
		world->unsubscribeAll(this);
		//remove the rigidbodies from the dynamics world and delete them
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//delete collision shapes
		for (int j = 0; j < m_collisionShapes.size(); j++)
		{
			btCollisionShape* shape = m_collisionShapes[j];
			m_collisionShapes[j] = 0;
			delete shape;
		}

		//delete dynamics world
		delete m_dynamicsWorld;

		//delete solver
		delete m_solver;

		//delete broadphase
		delete m_overlappingPairCache;

		//delete dispatcher
		delete m_dispatcher;

		delete m_collisionConfiguration;

		//next line is optional: it will be cleared by the destructor when the array goes out of scope
		m_collisionShapes.clear();
	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityCreated& event)
	{

	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityInit& event)
	{
		ComponentHandle<PhysicsComponent> physics = event.entity->get<PhysicsComponent>();

		if (physics) {
			
			physics->m_body = AddDynamicBody(physics->m_type, event.entity, physics->m_mass, physics->m_friction, physics->m_restitution, physics->m_damping, physics->m_startLinearVel, physics->m_startAngularVel);
			physics->m_body->setUserPointer(event.entity);

		}
	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityDestroyed& event)
	{
		ComponentHandle<PhysicsComponent> physics = event.entity->get<PhysicsComponent>();

		if (physics) {
			m_dynamicsWorld->removeRigidBody(physics->m_body);
		}


	}

	void PhysicsSystem::receive(class World* world, const Events::OnComponentRemoved<PhysicsComponent>& event)
	{
		RemoveObject(event.component->m_body);
		event.component->m_body = NULL;
	}

	void PhysicsSystem::tick(class World* world, float deltaTime) {
		m_dynamicsWorld->stepSimulation(deltaTime, 10, 1.0f/120.0f);
		for (Entity* entity : world->each<PhysicsComponent>())
		{
			entity->with<PhysicsComponent>([&](ComponentHandle<PhysicsComponent> physics) {

				if (physics->m_destroy == true)
				{
					if (physics->m_body != nullptr) {
						//RemoveComponent(entity, physics);
						//entity->remove<RenderComponent>();
						//entity->removeAll();
						world->destroy(entity, false);
						
					}
				}
				else {

					ComponentHandle<TransformComponent> objTrans = entity->get<TransformComponent>();
					btTransform trans;

					if (physics->m_body && physics->m_body->getMotionState())
					{
						btScalar btModelMatrix[16];
						btTransform btTrans;
						glm::mat4 matrix;

						if (physics->m_temp == true)
							physics->m_duration -= 1;

						if (physics->m_duration <= 0)
							physics->m_destroy = true;

						physics->m_body->getMotionState()->getWorldTransform(btTrans);

						if (btTrans.getOrigin().y() < -200) {
							btTrans.setOrigin(btVector3(0, 50, 0));
							physics->m_body->setLinearVelocity(btVector3(0, 0, 0));
							physics->m_body->setCenterOfMassTransform(btTrans);
						}

						btTrans.getOpenGLMatrix(btModelMatrix);
						matrix = btScalar2glmMat4(btModelMatrix);

						glm::mat4 scaleMatrix = glm::mat4(1);
						glm::mat4 scale = glm::scale(objTrans->m_scale);
						physics->m_modelMatrix = matrix * scale;
					}
					else {

					}
				}
			});
		}

	}


	btRigidBody* PhysicsSystem::AddDynamicBody(PhysicsType type, Entity* e, double mass, double friction, double restitution, double damping, glm::vec3 vel, glm::vec3 avel)
	{

		ComponentHandle<PhysicsComponent> physics = e->get<PhysicsComponent>();

		btCollisionShape* Shape = new btBoxShape(btVector3(1, 1, 1));
		ComponentHandle<TransformComponent> trans = e->get<TransformComponent>();
		if (trans) {

			if (type == PhysicsType::Sphere)
			{
				Shape = new btSphereShape(trans->m_scale.y);
			}
			else if (type == PhysicsType::Box)
			{
				Shape = new btBoxShape(btVector3(trans->m_scale.x, trans->m_scale.y, trans->m_scale.z));
			}
			else {
				Shape = new btBoxShape(btVector3(1, 1, 1));
			}
		}

		btRigidBody* body;
		physics->m_motionState = new btDefaultMotionState(btTransform(btQuaternion(trans->m_rot.y, trans->m_rot.x, trans->m_rot.z), btVector3(trans->m_pos.x, trans->m_pos.y, trans->m_pos.z)));
		btScalar Mass = mass;
		btVector3 Inertia(0, 0, 0);
		Shape->calculateLocalInertia(mass, Inertia);
		btRigidBody::btRigidBodyConstructionInfo bodyCI(Mass, physics->m_motionState, Shape, Inertia);

		bodyCI.m_linearDamping = damping; // 0
		bodyCI.m_friction = friction; // 0.5
		bodyCI.m_restitution = restitution; // 0

		body = new btRigidBody(bodyCI);
		body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
		body->setAngularVelocity(btVector3(avel.x, avel.y, avel.z));
		m_dynamicsWorld->addRigidBody(body);
		return body;
	}

	void PhysicsSystem::RemoveObject(btRigidBody* rb)
	{
		m_dynamicsWorld->removeRigidBody(rb);
		delete rb->getMotionState();
		delete rb;
	}

	void PhysicsSystem::RemoveComponent(Entity* e, ComponentHandle<PhysicsComponent> physics)
	{
		RemoveObject(physics->m_body);
		physics->m_body = NULL;
		e->remove<PhysicsComponent>();
	}


	glm::mat4 PhysicsSystem::btScalar2glmMat4(btScalar* matrix) {
		return glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
	}

}

