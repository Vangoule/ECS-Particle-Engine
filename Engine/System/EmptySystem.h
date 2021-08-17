#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "../Manager/WindowManager.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "../Framework/PhysicsComponents.h"
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
	};

}
