#include <sstream>
#include "SceneManager.h"

//Systems
#include "../System/InputSystem.h"
#include "../System/GraphicsSystem.h"
#include "../System/PhysicsSystem.h"
#include "../System/ParticleSystem.h"


//Components
#include "../Framework/GraphicsComponents.h"
#include "../Framework/TransformComponents.h"
#include "../Framework/PhysicsComponents.h"
#include "../Framework/ParticleComponents.h"
namespace DOI {
	ECS_TYPE_IMPLEMENTATION;
	using namespace ECS;

	World* SceneManager::CreateScene()
	{
		m_scene = World::createWorld();
	}

	void SceneManager::DestroyScene()
	{
		m_scene->cleanup();
		m_scene->destroyWorld();
	}

	void SceneManager::InitEntity(Entity* entity)
	{
		m_scene->emit<Events::OnEntityInit>({ entity });
	}

	void SceneManager::LoadScene()
	{
		//Create Scene
		CreateScene();

		EntitySystem* graphicsSystem = m_scene->registerSystem(new GraphicsSystem());
		EntitySystem* inputSystem = m_scene->registerSystem(new InputSystem());
		EntitySystem* particleSystem = m_scene->registerSystem(new ParticleSystem());
		EntitySystem* physicsSystem = m_scene->registerSystem(new PhysicsSystem());
		
		
		

		//Define a cube
		std::vector<glm::vec3> cube;
		std::vector<unsigned int> cubeIndices = { 0, 1, 2, 0, 2, 3, //front
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
		std::vector<glm::vec2> uvs;
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

		std::vector<glm::vec3> triangle;
		triangle.push_back(glm::vec3(-1.0, 1.0, 0.0));
		triangle.push_back(glm::vec3(0.0, 2.0, 0.0));
		triangle.push_back(glm::vec3(1.0, 1.0, 0.0));

		std::vector<glm::vec3> plane;
		std::vector<unsigned int> planeIndices = { 0, 1, 2, 0, 2, 3 };
		plane.push_back(glm::vec3(1.0, 0.0, 1.0));
		plane.push_back(glm::vec3(-1.0, 0.0, 1.0));
		plane.push_back(glm::vec3(-1.0, 0.0, -1.0));
		plane.push_back(glm::vec3(1.0, 0.0, -1.0));
		std::vector<glm::vec2> planeUvs;
		planeUvs.push_back(glm::vec2(0.0f, 0.0f));
		planeUvs.push_back(glm::vec2(1.0f, 0.0f));
		planeUvs.push_back(glm::vec2(1.0f, 1.0f));
		planeUvs.push_back(glm::vec2(0.0f, 1.0f));

		int uvScale = 50;
		for (int i = 0; i < planeUvs.size(); i++)
		{
			planeUvs[i].x *= uvScale;
			planeUvs[i].y *= uvScale;
		}

		//Texture* woodenBox = new Texture("box.jpg");
		//Entity* obj = m_scene->createEntity();
		//obj->assign<RenderComponent>(cube, cubeIndices, uvs, woodenBox);
		//obj->assign<TransformComponent>(glm::vec3(0,0,0), glm::quat(), glm::vec3(10, 10, 10));
		//obj->assign<PhysicsComponent>(PhysicsType::Box, 10, 0.5, 0.5);
		//InitEntity(obj);

		Entity* cam = m_scene->createEntity();
		cam->assign<CameraComponent>(glm::vec3(0, 5, -30), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 10, 30, 60);
		InitEntity(cam);

		//LoadScene
		Texture* metalBox = new Texture("box.png");
		Texture* woodenBox = new Texture("box.jpg");
		Texture* fragileTexture = new Texture("fragile.png");
		Texture* planeTexture = new Texture("grid.png");

		Entity* obj = m_scene->createEntity();
		obj->assign<RenderComponent>(cube, cubeIndices, uvs, fragileTexture);
		obj->assign<TransformComponent>(glm::vec3(-10,300,0), glm::quat(), glm::vec3(10, 10, 10));
		obj->assign<PhysicsComponent>(PhysicsType::Box, 10, 0.5, 0.5);
		InitEntity(obj);

		int num = 4;
		float gap = 5;

		//for (int i = 0; i < num; i++) {
		//	for (int j = 0; j < num; j++) {
		//		for (int k = 0; k < num; k++) {
		//			Entity* obj = m_scene->createEntity();
		//			obj->assign<RenderComponent>(cube, cubeIndices, uvs, woodenBox);
		//			obj->assign<TransformComponent>(glm::vec3((num / 2 * 2) - i * (2 + (gap*2)), 5 + k * (2 + (gap * 2)), (num / 2 * 2) -  j * (2 + (gap * 2)) ));
		//			obj->assign<PhysicsComponent>(PhysicsType::Box, 1, 0.5, 0.5);
		//			InitEntity(obj);
		//		}
		//	}k
		//}

		int height = 40;
		float scale = 2.0f;
		for (int i = 0; i < height; i++)
		{
			int mass = 1;
			Entity* obj = m_scene->createEntity();
			obj->assign<RenderComponent>(cube, cubeIndices, uvs, woodenBox);
			obj->assign<TransformComponent>(glm::vec3(0, (scale/2) + scale + i * (scale * 2), 0), glm::quat(), glm::vec3(scale, scale, scale));
			if (i == 0) { mass = 1; }
			obj->assign<PhysicsComponent>(PhysicsType::Box, mass, 1.0, 0);
			InitEntity(obj);
		}

		//Texture* particleTexture = new Texture("Particles/genericParticle.jpg");
		//Texture* particleTexture = new Texture("Particles/snowflake.png");
		//Texture* particleTexture = new Texture("Particles/particle2.jpg");
		//scale = 0.1;
		//Entity* particles = m_scene->createEntity();
		//particles->assign<RenderComponent>(cube, cubeIndices, uvs, woodenBox);
		//particles->assign<TransformComponent>(glm::vec3(-10,10,0), glm::quat(), glm::vec3(scale, scale, scale));
		//particles->assign<PhysicsComponent>(PhysicsType::Box, 0, 0.5, 0.5);

		//ComponentHandle<ParticleComponent> pc = particles->assign<ParticleComponent>(particleTexture, 1000, 10, 6);

		//InitEntity(particles);

		//pc->m_mainDir = glm::vec3(0, 0, 0);
		//pc->m_r = 255;
		//pc->m_g = 255;
		//pc->m_b = 255;
		//pc->m_a = 1;
		//pc->m_size = 5;
		//pc->m_randomSizeScale = 4;
		//pc->m_useRandomScale = true;
		//pc->m_spread = 0.5;
		//pc->m_gravity = glm::vec3(0, 0, 0);


		//Floor
		uvScale = 4;
		for (int i = 0; i < uvs.size(); i++)
		{
			uvs[i].x *= uvScale;
			uvs[i].y *= uvScale;
		}



		Entity* floor = m_scene->createEntity();
		floor->assign<RenderComponent>(cube, cubeIndices, uvs, planeTexture);
		floor->assign<TransformComponent>(glm::vec3(0, -0.5, 0), glm::quat(), glm::vec3(200, 1, 200));
		floor->assign<PhysicsComponent>(PhysicsType::Box, 0, 1.0, 0.5, 0.0);
		InitEntity(floor);



		bool walls = true;

		if (walls) {
			//Walls
			Entity* rightWall = m_scene->createEntity();
			rightWall->assign<RenderComponent>(cube, cubeIndices, uvs, planeTexture);
			rightWall->assign<TransformComponent>(glm::vec3(210, -100, 0), glm::quat(), glm::vec3(10, 200, 200));
			rightWall->assign<PhysicsComponent>(PhysicsType::Box, 0, 0.5, 0.5);
			InitEntity(rightWall);

			Entity* leftWall = m_scene->createEntity();
			leftWall->assign<RenderComponent>(cube, cubeIndices, uvs, planeTexture);
			leftWall->assign<TransformComponent>(glm::vec3(-210, -100, 0), glm::quat(), glm::vec3(10, 200, 200));
			leftWall->assign<PhysicsComponent>(PhysicsType::Box, 0, 0.5, 0.5);
			InitEntity(leftWall);


			Entity* topWall = m_scene->createEntity();
			topWall->assign<RenderComponent>(cube, cubeIndices, uvs, planeTexture);
			topWall->assign<TransformComponent>(glm::vec3(0, -100, 210), glm::quat(), glm::vec3(200, 200, 10));
			topWall->assign<PhysicsComponent>(PhysicsType::Box, 0, 0.5, 0.5);
			InitEntity(topWall);

			Entity* bottomWall = m_scene->createEntity();
			bottomWall->assign<RenderComponent>(cube, cubeIndices, uvs, planeTexture);
			bottomWall->assign<TransformComponent>(glm::vec3(0, -100, -210), glm::quat(), glm::vec3(200, 200, 10));
			bottomWall->assign<PhysicsComponent>(PhysicsType::Box, 0, 0.5, 0.5);
			InitEntity(bottomWall);
		}


		//cam->assign<CameraComponent>(glm::vec3(-2, 1, 9));

	}
}