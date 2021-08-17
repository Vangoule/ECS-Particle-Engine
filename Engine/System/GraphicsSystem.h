#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "../Manager/WindowManager.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "../Manager/Texture.h"
#include "../Manager/ShaderManager.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


#include "../Framework/GraphicsComponents.h"
#include "../Framework/ParticleComponents.h"
namespace DOI {

	class GraphicsSystem : public EntitySystem,
		public EventSubscriber<Events::OnEntityCreated>,
		public EventSubscriber<Events::OnEntityDestroyed>,
		public EventSubscriber<Events::OnEntityInit>,
		public EventSubscriber<Events::OnComponentRemoved<RenderComponent>>
	{
	public:
		virtual ~GraphicsSystem() {}

		virtual void configure(class World* world);

		virtual void unconfigure(class World* world);

		virtual void tick(class World* world, float deltaTime);

		virtual void receive(class World* world, const Events::OnEntityInit& event);

		virtual void receive(class World* world, const Events::OnEntityCreated& event);

		virtual void receive(class World* world, const Events::OnEntityDestroyed& event);

		virtual void receive(class World* world, const Events::OnComponentRemoved<RenderComponent>& event);

		void CalcNormals(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices);

		bool m_isRendering = false;
		ShaderManager* m_shaders;
		ComponentHandle<CameraComponent> m_mainCamera;
		GLuint m_vao;
		World* m_scene;
	};

}



//
//#include "ISystem.h"
//#include <glm\glm.hpp>
//
//#include "../Manager/StateManager.h"
//#include "../Manager/ShaderManager.h"
//#include "../Manager/WindowManager.h"
//
//#include "GraphicsSystemComponents.h"
//
//#include <IL/il.h>
//#include <IL/ilu.h>
//#include <IL/ilut.h>
//namespace DOI {
//
//	class GraphicsSystemTask;
//	class GraphicsSystem;
//
//
//	
//
//	class GraphicsSystemTask : public ISystemTask
//	{
//	public:
//		//Execute System on System Data connected to Scenes / Objects
//		GraphicsSystemTask(GraphicsSystem* system) { m_system = system; m_systemType = System::GraphicsSystem; };
//
//		void Run(float dt);
//
//		GraphicsSystem* m_system;
//
//		
//	
//	};
//
//	class GraphicsSystem : public ISystem, public EventSubscriber<Event::OnEntityInit>, public EventSubscriber<Event::OnComponentRemoved<RenderComponent>>
//	{
//	
//	public:
//		//Initialize System Settings
//		GraphicsSystem() {
//			
//			m_task = new GraphicsSystemTask(this); 
//			StateManager::Get().Subscribe<Event::OnEntityInit>(this); 
//			StateManager::Get().Subscribe<Event::OnComponentRemoved<RenderComponent>>(this); 
//		};
//
//		virtual ~GraphicsSystem() {}
//
//		virtual void Receive(const Event::OnEntityInit& event);
//
//		virtual void Receive(const Event::OnComponentRemoved<RenderComponent>& event);
//
//		void CalcNormals(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices);
//		
//		void Init();
//
//		ISystemTask* GetTask() { return m_task;  };
//		bool GetParallel() { return true; };
//		System::Type GetType() { return System::GraphicsSystem; }
//		
//		bool m_isRendering = false;
//		ShaderManager* m_shaders;
//		ComponentHandle<CameraComponent> m_mainCamera;
//		GLuint m_vao;
//
//	};
// 
//}