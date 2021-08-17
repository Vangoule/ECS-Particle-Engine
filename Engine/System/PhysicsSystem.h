#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "../Manager/WindowManager.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"

#include "../Framework/PhysicsComponents.h"

#include "../Kairos/Timestep.hpp"
namespace DOI {

	class PhysicsSystem : public EntitySystem,
		public EventSubscriber<Events::OnEntityCreated>,
		public EventSubscriber<Events::OnEntityDestroyed>,
		public EventSubscriber<Events::OnEntityInit>,
		public EventSubscriber<Events::OnComponentRemoved<PhysicsComponent>>
	{
	public:
		virtual ~PhysicsSystem() {}

		virtual void configure(class World* world);

		virtual void unconfigure(class World* world);

		virtual void tick(class World* world, float deltaTime);

		virtual void receive(class World* world, const Events::OnEntityInit& event);

		virtual void receive(class World* world, const Events::OnEntityCreated& event);

		virtual void receive(class World* world, const Events::OnEntityDestroyed& event);

		virtual void receive(class World* world, const Events::OnComponentRemoved<PhysicsComponent>& event);

		btDiscreteDynamicsWorld* m_dynamicsWorld;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

		btRigidBody* AddDynamicBody(PhysicsType type, Entity* e, double mass, double friction, double restitution, double damping, glm::vec3 vel, glm::vec3 avel);
		void RemoveObject(btRigidBody* rb);
		glm::mat4 btScalar2glmMat4(btScalar* matrix);

		void RemoveComponent(Entity* e, ComponentHandle<PhysicsComponent> physics); 
		kairos::Timestep* timestep;
	
	};

}

