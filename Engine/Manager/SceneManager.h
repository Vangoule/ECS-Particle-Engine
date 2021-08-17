#pragma once
#include <iostream>

#include "Manager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Texture.h"


namespace DOI {

	

	class SceneManager : public Manager<SceneManager>
	{
	friend class Manager<SceneManager>;
	
	public:
		SceneManager(void) {};
		~SceneManager(void) {};

		ECS::World* CreateScene();

		ECS::World* GetScene() { return m_scene; }

	//	void SetScene(Scene* scene) { m_scene = scene;  }

		void DestroyScene();

		void InitEntity(ECS::Entity* entity);

		void LoadScene();

	
	private:
		

		ECS::World* m_scene;

		
	};
}