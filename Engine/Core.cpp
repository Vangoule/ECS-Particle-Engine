#include "Core.h"


namespace DOI {

	Engine::Engine()
	{
	}


	Engine::~Engine()
	{
	}


	void Engine::Init(App* app)
	{
		LOG("Initialized!");
		m_app = app;
		m_app->Init();

		WindowManager::Get().Create(800, 600, "Game");

		SceneManager::Get().LoadScene();

		m_scheduler = new Scheduler();
		m_scheduler->Init();

		m_isRunning = true;

		Loop();

		Cleanup();
	}

	void Engine::Cleanup()
	{
		m_app->Cleanup();
		SceneManager::Get().DestroyScene();
		LOG("Shutdown!");

	}

	void Engine::Loop()
	{

		while (m_isRunning) {
			if (WindowManager::Get().shouldClose())
				Stop();

			//Execute Scheduler
			m_scheduler->Execute();

			//sf::Event event;

			//while (WindowManager::Get().getHandle()->pollEvent(event))
			//{

			//	switch (event.type)
			//	{

			//	case sf::Event::Closed:
			//		Stop();
			//		break;

			//	default:
			//		break;
			//	}
			//}
	
		}



	}

	void Engine::Stop()
	{
		m_isRunning = false;
	}


}