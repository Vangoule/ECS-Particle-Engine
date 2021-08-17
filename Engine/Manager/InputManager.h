#pragma once
#include <iostream>

#include "Manager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../Framework/ECS.h"
#include "glm\glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Texture.h"
#include "WindowManager.h"


namespace DOI {

	

	class InputManager : public Manager<InputManager>
	{
	friend class Manager<InputManager>;
	
	public:
		InputManager(void) {};
		~InputManager(void) {};

		static bool GetKey(int keyCode);
		static bool GetKeyDown(int keyCode);
		static bool GetKeyUp(int keyCode);

		static bool GetMouse(int button);
		static bool GetMouseDown(int button);
		static bool GetMouseUp(int button);

		static glm::vec2 GetMousePosition();

		static void SetCursor(bool value);
		static void SetMousePosition(glm::vec2 pos);

		static void Update();

		static void Clean();


		bool mouseLocked = false;
		bool firstMouse = false;
	
	private:
		static void GetInput(sf::Event event);


		

		

		
	};
}