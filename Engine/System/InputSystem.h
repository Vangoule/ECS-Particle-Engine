#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "../Manager/WindowManager.h"
#include "../Manager/Texture.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


#include "../Framework/InputComponents.h"
#include "../Framework/GraphicsComponents.h"

namespace DOI {

	ECS_TYPE_IMPLEMENTATION;
	using namespace ECS;


	class InputSystem : public EntitySystem,
		public EventSubscriber<Events::OnEntityCreated>,
		public EventSubscriber<Events::OnEntityDestroyed>,
		public EventSubscriber<Events::OnEntityInit>
	{


	public:
		virtual ~InputSystem() {}

		virtual void configure(class World* world);

		virtual void unconfigure(class World* world);

		virtual void tick(class World* world, float deltaTime);

		virtual void receive(class World* world, const Events::OnEntityCreated& event);

		virtual void receive(class World* world, const Events::OnEntityInit& event);

		virtual void receive(class World* world, const Events::OnEntityDestroyed& event);

		glm::vec2 centerPosition;

		inline glm::vec3 GetDirection(glm::vec3 rot)
		{
			float horizontalAngle = glm::radians(rot.y);
			float verticalAngle = -glm::radians(rot.x);


			glm::vec3 direction;
			direction.x = cos(horizontalAngle) * cos(verticalAngle);
			direction.y = sin(verticalAngle);
			direction.z = sin(horizontalAngle) * cos(verticalAngle);

			//glm::vec3 direction(
			//	cos(verticalAngle) * sin(horizontalAngle),
			//	sin(verticalAngle),
			//	cos(verticalAngle) * cos(horizontalAngle)
			//);

			return glm::normalize(direction);
		}
	};




}