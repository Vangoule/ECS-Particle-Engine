#pragma once
#include <iostream>
#include <GL\glew.h>

#define LOGGING_LEVEL_1
#include "Utils\Logger.h"

#include "Framework\Scheduler.h"

#include "Manager\WindowManager.h"
#include "Manager/SceneManager.h"


namespace DOI {

	class App;
	class Engine;

	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init(App* app);

		SceneManager* GetSceneManager() { return &SceneManager::Get(); };

	private:
		void Cleanup();

		void Loop();

		void Stop();

		

		App* m_app;

		bool m_isRunning = false;

		Scheduler* m_scheduler;
	};

	class App {

	public:
		void Run() {
			m_engine = new Engine();
			m_engine->Init(this);
		};

		virtual void Init() = 0;

		virtual void Update(float dt) = 0;

		virtual void Cleanup() = 0;

	protected:
		
		Engine* m_engine;
	private:

	};
}