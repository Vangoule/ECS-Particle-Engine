#include <SFML/System.hpp>
#include <iostream>

#include "../Manager/SceneManager.h"
#include "../Manager/WindowManager.h"
#include "../Kairos/Timestep.hpp"
#include "../Kairos/FpsLite.hpp"

namespace DOI {
	class Scheduler {

	public:
		Scheduler() {};
		~Scheduler() {};


		void Init();

		void Execute();

	private:
		kairos::Timestep* timestep;
		kairos::FpsLite fps;

		const float m_tickRate = 60.0f;

	};
}