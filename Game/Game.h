#pragma once

#include <Engine\Core.h>
#include <Engine\Utils\Logger.h>

class Game : public DOI::App {

public:
	Game();
	~Game();

	void Init();
	void Update(float dt);
	void Cleanup();
};