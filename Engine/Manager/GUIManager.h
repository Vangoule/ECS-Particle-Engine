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

	

	class GUIManager : public Manager<GUIManager>
	{
	friend class Manager<GUIManager>;
	
	public:
		GUIManager(void) {};
		~GUIManager(void) {};
		
	
	private:
		

		
	};
}