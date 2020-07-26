#include "Game.h"

bool Game::Initialize(const TCHAR* windowTitle) {
	if (m_system.Initialize(windowTitle)) return true;
	return false;
}
bool Game::Finalize() {
	return false;
}
void Game::ProcessLoop() {
	
	while (SystemUpdate()) {
		/*
		Update();
		Draw();
		*/

		DxLib::DrawFormatString(0, 16, 0xffff00, L"%f", GetFPS());
		DxLib::DrawFormatString(0, 0, 0xffffff, L"%d, %d", m_system.GetWindowWidth(), m_system.GetWindowHeight());
		DxLib::DrawFormatString(0, 32, 0xffffff, L"%d", DxLib::GetDrawScreen() == DX_SCREEN_BACK);

		if (GetInputChar(TRUE) == ' ') {
			m_system.ToggleFullScreen();
		}


	}
}

bool Game::SystemUpdate() {
	return (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen());
}