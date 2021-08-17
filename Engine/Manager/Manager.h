#pragma once
#include <iostream>
#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#define LOGGING_LEVEL_1
#include "../Utils/Logger.h"

namespace DOI {
	template <typename T>
	class Manager
	{
	public:

		static T& Get() {
			static T m_instance;

			return m_instance;
		};

		Manager(Manager const &) = delete;
		Manager& operator=(Manager const &) = delete;

	protected:
		Manager() {}
		~Manager() {}
		
	};

}

