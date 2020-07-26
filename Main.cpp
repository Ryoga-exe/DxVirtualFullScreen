#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Game game;
	if (game.Initialize(L"Text")) return -1;
	game.ProcessLoop();
	game.Finalize();
	return 0;
}