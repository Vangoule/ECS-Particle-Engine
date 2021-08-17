#include "WindowManager.h"
#include <sstream>

namespace DOI {

	void WindowManager::Create(int width, int height, std::string title)
	{
		m_width = width;
		m_height = height;
		m_title = title;

		sf::ContextSettings settings;
		settings.depthBits = 24;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 4;
		settings.majorVersion = 3;
		settings.minorVersion = 2;

		m_windowHndl.create(sf::VideoMode(1500, 1500), "OpenGL", sf::Style::Default, settings);


		sf::ContextSettings settingsCheck = m_windowHndl.getSettings();
		LOG("Initiating Window");
		LOG("Your computer supports: ");
		LOG("Depth Bits:", settingsCheck.depthBits);
		LOG("Stencil Bits:", settingsCheck.stencilBits);
		LOG("Antialiasing Level:", settingsCheck.antialiasingLevel);
		LOG("Version:", settingsCheck.majorVersion, ".", settingsCheck.minorVersion);


		//	m_windowHndl.setFramerateLimit(100);
		m_windowHndl.setFramerateLimit(144);
		//m_windowHndl.setVerticalSyncEnabled(true);

	}


	sf::Vector2i WindowManager::getMousePos()
	{
		sf::Vector2i localPosition = sf::Mouse::getPosition(m_windowHndl);
		return localPosition;
	}

	void WindowManager::setMousePos(int posx, int posy)
	{
		sf::Mouse::setPosition(sf::Vector2i(posx, posy), m_windowHndl);
	}


	sf::Vector2u WindowManager::getSize()
	{
		return m_windowHndl.getSize();
	}

	void WindowManager::Close() {
		m_shouldClose = true;
	}

	bool WindowManager::shouldClose()
	{
		return m_shouldClose;
	}

	void WindowManager::BindAsRenderTarget()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, getSize().x, getSize().y);
	}

}