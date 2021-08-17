#pragma once
#include "InputSystem.h"
#include <sstream>

#include "../Manager/WindowManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"

#include "../Framework/TransformComponents.h"
#include "../Framework/InputComponents.h"
#include "../Framework/PhysicsComponents.h"
#include "../Framework/ParticleComponents.h"




namespace DOI {
	std::vector<glm::vec3> cube;
	std::vector<unsigned int> cubeIndices;

	std::vector<glm::vec2> uvs;
	Texture* woodenBox;
	Texture* metalBox;
	Texture* fireTexture;
	Texture* iceTexture;


	void InputSystem::configure(class World* world)
	{
		//	world->subscribe<Events::OnEntityCreated>(this);
		//	world->subscribe<Events::OnEntityDestroyed>(this);


		cubeIndices = { 0, 1, 2, 0, 2, 3, //front
		4, 5, 6, 4, 6, 7, //right
		8, 9, 10, 8, 10, 11, //back
		12, 13, 14, 12, 14, 15, //left
		16, 17, 18, 16, 18, 19, //upper
		20, 21, 22, 20, 22, 23 }; //bottom


		//front
		cube.push_back(glm::vec3(-1.0, -1.0, 1.0));
		cube.push_back(glm::vec3(1.0, -1.0, 1.0));
		cube.push_back(glm::vec3(1.0, 1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, 1.0));

		//right
		cube.push_back(glm::vec3(1.0, 1.0, 1.0));
		cube.push_back(glm::vec3(1.0, 1.0, -1.0));
		cube.push_back(glm::vec3(1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(1.0, -1.0, 1.0));

		//back
		cube.push_back(glm::vec3(-1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(1.0, 1.0, -1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, -1.0));

		//left
		cube.push_back(glm::vec3(-1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(-1.0, -1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, -1.0));

		//upper
		cube.push_back(glm::vec3(1.0, 1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, 1.0, -1.0));
		cube.push_back(glm::vec3(1.0, 1.0, -1.0));

		//bottom
		cube.push_back(glm::vec3(-1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(1.0, -1.0, -1.0));
		cube.push_back(glm::vec3(1.0, -1.0, 1.0));
		cube.push_back(glm::vec3(-1.0, -1.0, 1.0));

		//UV's

		uvs.push_back(glm::vec2(0.0f, 0.0f));
		uvs.push_back(glm::vec2(0.0f, 1.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));
		uvs.push_back(glm::vec2(1.0f, 0.0f));

		uvs.push_back(glm::vec2(0.0f, 1.0f));
		uvs.push_back(glm::vec2(0.0f, 0.0f));
		uvs.push_back(glm::vec2(1.0f, 0.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));

		uvs.push_back(glm::vec2(1.0f, 0.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));
		uvs.push_back(glm::vec2(0.0f, 1.0f));
		uvs.push_back(glm::vec2(0.0f, 0.0f));

		uvs.push_back(glm::vec2(0.0f, 0.0f));
		uvs.push_back(glm::vec2(0.0f, 1.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));
		uvs.push_back(glm::vec2(1.0f, 0.0f));

		uvs.push_back(glm::vec2(0.0f, 0.0f));
		uvs.push_back(glm::vec2(1.0f, 0.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));
		uvs.push_back(glm::vec2(0.0f, 1.0f));

		uvs.push_back(glm::vec2(1.0f, 0.0f));
		uvs.push_back(glm::vec2(0.0f, 0.0f));
		uvs.push_back(glm::vec2(0.0f, 1.0f));
		uvs.push_back(glm::vec2(1.0f, 1.0f));

		woodenBox = new Texture("box.jpg");
		metalBox = new Texture("box.png");
		fireTexture = new Texture("Particles/particle2.jpg");
		iceTexture = new Texture("Particles/snowflake.png");

		world->subscribe<Events::OnEntityInit>(this);
	}

	void InputSystem::unconfigure(class World* world)
	{
		world->unsubscribeAll(this);
	}

	void InputSystem::receive(class World* world, const Events::OnEntityCreated& event)
	{

	}

	void InputSystem::receive(class World* world, const Events::OnEntityDestroyed& event)
	{
		std::cout << "An entity was destroyed!" << std::endl;
	}

	void InputSystem::receive(class World* world, const Events::OnEntityInit& event)
	{

	}

	void InputSystem::tick(class World* world, float deltaTime) {
		InputManager* input = &InputManager::Get();

		if (input->GetKeyDown(sf::Keyboard::LAlt))
		{
			std::cout << "Hello World" << std::endl;
			input->SetCursor(true);
			input->mouseLocked = false;

		}

		if (input->GetMouseDown(sf::Mouse::Left))
		{
			centerPosition = glm::vec2(WindowManager::Get().getSize().x / 2, (int)WindowManager::Get().getSize().y / 2);
			input->SetCursor(false);
			input->SetMousePosition(centerPosition);
			input->mouseLocked = true;
			input->firstMouse = true;
		}

		if (input->GetMouseDown(sf::Mouse::Right))
		{

			//centerPosition = glm::vec2((float)WindowManager::Get().getSize().x / 2.0f, (float)WindowManager::Get().getSize().y / 2.0f);
			centerPosition = glm::vec2((int)WindowManager::Get().getSize().x / 2, (int)WindowManager::Get().getSize().y / 2);
			input->SetCursor(false);
			input->SetMousePosition(centerPosition);
			input->mouseLocked = true;
			input->firstMouse = true;
		}

		if (input->GetKeyDown(sf::Keyboard::Escape))
		{
			WindowManager::Get().Close();
		}

		for (Entity* entity : world->each<CameraComponent>())
		{

			ComponentHandle<CameraComponent> cam = entity->get<CameraComponent>();

			if (input->mouseLocked)
			{
				if (input->firstMouse == true) {
					input->SetMousePosition(centerPosition);
				}

				if (input->firstMouse == false) {
					glm::vec2 deltaPos = input->GetMousePosition() - centerPosition;
					bool rotY = deltaPos.x != 0;
					bool rotX = deltaPos.y != 0;

					if (cam->verticalAngle + (cam->m_sensitivity * deltaPos.y) * deltaTime > 89.0f)
						cam->verticalAngle = 89.0f;
					else if (cam->verticalAngle + (cam->m_sensitivity * deltaPos.y) * deltaTime < -89.0f)
						cam->verticalAngle = -89.0f;
					else {
						cam->verticalAngle += (cam->m_sensitivity * deltaPos.y) * deltaTime;
					}

					cam->horizontalAngle += (cam->m_sensitivity * deltaPos.x) * deltaTime;
					
					if (rotY || rotX)
						input->SetMousePosition(centerPosition);

					float speed = cam->m_speed * deltaTime;

					glm::vec3 direction = GetDirection(glm::vec3(cam->verticalAngle, cam->horizontalAngle, 0));
					cam->m_dir = direction;
					glm::vec3 globalRight = glm::cross(glm::normalize(direction), glm::vec3(0, 1, 0));
					cam->m_up = -glm::cross(glm::normalize(direction), glm::normalize(globalRight));
					cam->m_right = glm::cross(glm::normalize(direction), glm::normalize(cam->m_up));

					cam->m_at = direction;

					if (input->GetKey(sf::Keyboard::W))
					{
						cam->m_pos += speed * cam->m_at;
					}

					if (input->GetKey(sf::Keyboard::S))
					{
						cam->m_pos -= speed * cam->m_at;
					}

					if (input->GetKey(sf::Keyboard::D))
					{
						cam->m_pos += glm::normalize(glm::cross(cam->m_at, cam->m_up)) * speed;

					}

					if (input->GetKey(sf::Keyboard::A))
					{
						cam->m_pos -= glm::normalize(glm::cross(cam->m_at, cam->m_up)) * speed;

					}

					if (input->GetKeyDown(sf::Keyboard::LShift))
					{
						cam->m_speed = cam->m_boostSpeed;
					}

					if (input->GetKeyUp(sf::Keyboard::LShift))
					{
						cam->m_speed = cam->m_defaultSpeed;
					}

					if (input->GetKey(sf::Keyboard::Space))
					{
						cam->m_pos += glm::vec3(0, 1, 0) * glm::vec3(speed);
					}

					if (input->GetKey(sf::Keyboard::X))
					{
						cam->m_pos -= glm::vec3(0, 1, 0) * glm::vec3(speed);
					}

					if (input->GetMouseUp(sf::Mouse::Left))
					{


						//float speed = 60;
						//float distanceFromCamera = 2;
						//SceneManager* scn = &SceneManager::Get();
						//Entity* box = world->createEntity();
						//box->assign<RenderComponent>(cube, cubeIndices, uvs, metalBox);
						//glm::vec3 position = glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z) + (direction * glm::vec3(distanceFromCamera));
						//glm::vec3 direction = GetDirection(glm::vec3(cam->verticalAngle, cam->horizontalAngle, 0));
						//box->assign<TransformComponent>(position, glm::quat(), glm::vec3(2, 2, 2));

						//PhysicsComponent comp(PhysicsType::Box, 2, 0.9, 0.2, 0.1, direction * glm::vec3(speed));
						//comp.m_temp = false;
						//comp.m_duration = 5;
						//box->assign<PhysicsComponent>(comp);
						//scn->InitEntity(box);

						float speed = 60;
						float distanceFromCamera = 6;
						float sideDistance = 0;
						Entity* box = world->createEntity();
						//box->assign<RenderComponent>(cube, cubeIndices, uvs, metalBox);
						//glm::vec3 position = glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z) + (glm::normalize(direction) * glm::vec3(distanceFromCamera));
						glm::vec3 position = glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z) + (glm::normalize(direction) * glm::vec3(distanceFromCamera)) + (glm::normalize(cam->m_right) * glm::vec3(sideDistance));
						glm::vec3 direction = GetDirection(glm::vec3(cam->verticalAngle, cam->horizontalAngle, 0));
						box->assign<TransformComponent>(position, glm::quat(), glm::vec3(0.4, 0.4, 0.4));

						PhysicsComponent comp(PhysicsType::Sphere, 10, 0.8, 0.9, 0.1, glm::normalize(direction) * glm::vec3(speed));
						comp.m_temp = true;
						comp.m_duration = 600;
						box->assign<PhysicsComponent>(comp);

						ComponentHandle<ParticleComponent> pc = box->assign<ParticleComponent>(fireTexture, 7000, 500, 15);
						SceneManager::Get().InitEntity(box);
						pc->m_mainDir = glm::vec3(0, 0, 0);
						pc->m_r = 255;
						pc->m_g = 255;
						pc->m_b = 255;
						pc->m_a = 1;
						pc->m_size = 6;
						pc->m_randomSizeScale = 6;
						pc->m_useRandomScale = true;
						pc->m_spread = 1.5;
						pc->m_gravity = glm::vec3(0, 2, 0);

					}

					if (input->GetMouseUp(sf::Mouse::Right))
					{
						float speed = 60;
						float distanceFromCamera = 6;
						float sideDistance = 0;
						Entity* box = world->createEntity();
						//box->assign<RenderComponent>(cube, cubeIndices, uvs, metalBox);
						glm::vec3 position = glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z) + (glm::normalize(direction) * glm::vec3(distanceFromCamera)) + (glm::normalize(cam->m_right) * glm::vec3(sideDistance));
						glm::vec3 direction = GetDirection(glm::vec3(cam->verticalAngle, cam->horizontalAngle, 0));
						box->assign<TransformComponent>(position, glm::quat(), glm::vec3(0.4, 0.4, 0.4));

						PhysicsComponent comp(PhysicsType::Sphere, 2, 0.8, 0.9, 0.1, glm::normalize(direction) * glm::vec3(speed));
						comp.m_temp = true;
						comp.m_duration = 600;
						box->assign<PhysicsComponent>(comp);

						ComponentHandle<ParticleComponent> pc = box->assign<ParticleComponent>(iceTexture, 3000, 200, 5);
						SceneManager::Get().InitEntity(box);
						pc->m_mainDir = glm::vec3(0, 0, 0);
						pc->m_r = 255;
						pc->m_g = 255;
						pc->m_b = 255;
						pc->m_a = 1;
						pc->m_size = 1.5;
						pc->m_randomSizeScale = 2;
						pc->m_useRandomScale = true;
						pc->m_spread = 1.5;
						pc->m_gravity = glm::vec3(0, 0, 0);

						//float speed = 60;
						//float distanceFromCamera = 8;
						//SceneManager* scn = &SceneManager::Get();

						//Entity* box = world->createEntity();
						//box->assign<RenderComponent>(cube, cubeIndices, uvs, woodenBox);
						//glm::vec3 position = glm::vec3(cam->m_pos.x, cam->m_pos.y, cam->m_pos.z) + (direction * glm::vec3(distanceFromCamera));
						//box->assign<TransformComponent>(position, glm::quat(), glm::vec3(1, 1, 1));
						//glm::vec3 direction = GetDirection(glm::vec3(cam->verticalAngle, cam->horizontalAngle, 0));
						//PhysicsComponent comp(PhysicsType::Box, 1, 0.9, 0.2, 0.1, direction * glm::vec3(speed));
						//comp.m_temp = true;
						//comp.m_duration = 500;
						//box->assign<PhysicsComponent>(comp);
						//scn->InitEntity(box);


					}




				}
			}
			input->firstMouse = false;



		}
	}
}