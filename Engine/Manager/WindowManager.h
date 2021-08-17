#pragma once
#include <iostream>
#include <GL\glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include "Manager.h"

#define LOGGING_LEVEL_1
#include "..\Utils\Logger.h"
namespace DOI {
	class WindowManager : public Manager<WindowManager>
	{

	public:
		WindowManager() { m_shouldClose = false; };
		~WindowManager() {};

		void Create(int width, int height, std::string title);

		sf::Window* getHandle() {
			return &m_windowHndl;
		}

		void setMousePos(int posx, int posy);

		sf::Vector2i getMousePos();
		
		sf::Vector2u getSize();

		void setSize(int x, int y) {
			m_width = x;
			m_height = y;
		}

		void Close();
		bool shouldClose();

		void BindAsRenderTarget();

	private:
		sf::Window m_windowHndl;
		float m_width, m_height;
		std::string m_title;
		bool m_shouldClose;
	};
}