#pragma once
#include "System.h"

class Game {
public:
	bool Initialize(const TCHAR* windowTitle);
	bool Finalize();
	void ProcessLoop();
private:
	bool SystemUpdate();
	bool Update();
	void Draw();

	CSystem m_system;
};