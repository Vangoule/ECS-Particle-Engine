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

	}

	void PhysicsSystem::unconfigure(class World* world)
	{
		world->unsubscribeAll(this);
	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityCreated& event)
	{

	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityInit& event)
	{

	}

	void PhysicsSystem::receive(class World* world, const Events::OnEntityDestroyed& event)
	{
		std::cout << "An entity was destroyed!" << std::endl;
	}

	void PhysicsSystem::receive(class World* world, const Events::OnComponentRemoved<PhysicsComponent>& event)
	{

	}

	void PhysicsSystem::tick(class World* world, float deltaTime) {
		

	}

}

