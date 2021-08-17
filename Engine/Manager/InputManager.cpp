#include <sstream>
#include "InputManager.h"

//Systems
#include "../System/GraphicsSystem.h"
#include "../System/PhysicsSystem.h"
#include "../System/ParticleSystem.h"


//Components
#include "../Framework/GraphicsComponents.h"
#include "../Framework/TransformComponents.h"
#include "../Framework/PhysicsComponents.h"
#include "../Framework/ParticleComponents.h"
namespace DOI {

	const static int NUM_KEYS = 512;
	const static int NUM_MOUSEBUTTONS = 256;

	static bool inputs[NUM_KEYS];
	static bool downKeys[NUM_KEYS];
	static bool upKeys[NUM_KEYS];

	static bool mouseInput[NUM_MOUSEBUTTONS];
	static bool downMouse[NUM_MOUSEBUTTONS];
	static bool upMouse[NUM_MOUSEBUTTONS];

	static double mouseX = 0;
	static double mouseY = 0;


	void InputManager::Clean()
	{
		for (int i = 0; i < NUM_MOUSEBUTTONS; i++)
		{
			downMouse[i] = false;
			upMouse[i] = false;
		}

		for (int i = 0; i < NUM_KEYS; i++)
		{
			downKeys[i] = false;
			upKeys[i] = false;
		}

	}

	void InputManager::GetInput(sf::Event event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			int value = event.key.code;

			if (value >= 0 && value <= 512)
			{
				inputs[value] = true;
				downKeys[value] = true;
			}
		}else if (event.type == sf::Event::KeyReleased)
		{
			int value = event.key.code;

			if (value >= 0 && value <= 512)
			{
				inputs[value] = false;
				upKeys[value] = true;
			}
		}else if (event.type == sf::Event::MouseButtonReleased)
		{
			int value = event.mouseButton.button;

			mouseInput[value] = false;
			upMouse[value] = true;
		}else if (event.type == sf::Event::MouseButtonPressed)
		{
			int value = event.mouseButton.button;

			mouseInput[value] = true;
			downMouse[value] = true;
			
		}else if (event.type == sf::Event::MouseMoved)
		{
			mouseX = (double)event.mouseMove.x;
			mouseY = (double)event.mouseMove.y;
		}

	}

	bool InputManager::GetKey(int keyCode)
	{
		return inputs[keyCode];
	}

	bool InputManager::GetKeyDown(int keyCode)
	{
		return downKeys[keyCode];
	}

	bool InputManager::GetKeyUp(int keyCode)
	{
		return upKeys[keyCode];
	}

	bool InputManager::GetMouse(int button)
	{
		return mouseInput[button];
	}

	bool InputManager::GetMouseDown(int button)
	{
		return downMouse[button];	
	}

	bool InputManager::GetMouseUp(int button)
	{
		return upMouse[button];
	}

	glm::vec2 InputManager::GetMousePosition()
	{
		glm::vec2 pos((float)mouseX, (float)mouseY);
		return pos;
	}

	void InputManager::SetCursor(bool visible)
	{
		if (visible)
			WindowManager::Get().getHandle()->setMouseCursorVisible(true);
		else
			WindowManager::Get().getHandle()->setMouseCursorVisible(false);
	}

	void InputManager::SetMousePosition(glm::vec2 pos)
	{
		WindowManager::Get().setMousePos((float)pos.x, (float)pos.y);
	}

	void InputManager::Update()
	{
		Clean();

		sf::Event event;

		while (WindowManager::Get().getHandle()->pollEvent(event))
		{
			GetInput(event);

			if (event.type == sf::Event::Closed) {
				WindowManager::Get().Close();
			}
			else if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
				WindowManager::Get().setSize(event.size.width, event.size.height);


			}

		}
	}


}