#include "Scheduler.h"
#include <string>
#include "../Manager/InputManager.h"


#define USE_SYSTEM_THREADS false

namespace DOI {

	void Scheduler::Init()
	{
		timestep = new kairos::Timestep;

		timestep->setStep(1.0f / 60.0f);
		timestep->setMaxAccumulation(0.25);
	}

	void Scheduler::Execute()
	{
		fps.update();
		timestep->addFrame();

		InputManager::Get().Update();
		SceneManager::Get().GetScene()->tick(timestep->getStepAsFloat());

		
		std::string title{ "Engine" };
		title += " - FPS: " + std::to_string(fps.getFps());
		WindowManager::Get().getHandle()->setTitle(title);
			
	}



	
}