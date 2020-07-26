#pragma once
#include "DxLib.h"

struct EnableSizeChange_t {
	bool enable = true;
	bool toFit = true;
};

struct RectSize_t {
	int width;
	int height;
};

class Window {
public:
	Window();
	int PosInit();
	int SetSize(int width, int height, int colorBitDepth = 32);
	int SetPosition(int x, int y);
	int SetEnableSizeChange(bool enable, bool toFit = true);
	int SetStyleMode(int styleMode);
	int SetFullScreenMode(bool isFullScreen);
	int ToggleFullScreenMode();
	int SetUseVirtualFullScreenFlag(bool useVirtualFullScreen);
	
	RectSize_t GetSize();
	int GetWidth();
	int GetHeight();
private:
	int  m_width, m_height, m_colorBitDepth;
	int  m_posX, m_posY;
	RectSize_t m_desktopSize;
	EnableSizeChange_t m_enableSizeChange;
	bool m_useVirtualFullScreen;
	bool m_isFullScreen;
	bool m_hasPosInitialized;
	int  m_styleMode;  // 非フルスクリーン時のため (仮想フルスクリーン)
};


class CSystem {
public:
	CSystem();
	~CSystem();

	bool Initialize(const TCHAR* windowTitle);
	bool Finalize();
	bool SetAlwaysRunFlag(bool doAlwaysRun);

	int  ToggleFullScreen() { return m_window.ToggleFullScreenMode(); }
	int  GetWindowWidth() {
		return m_window.GetWidth();
	}
	int  GetWindowHeight() {
		return m_window.GetHeight();
	}

private:
	bool m_hasInitialized = false;
	bool m_doAlwaysRun = true;
	Window m_window;
};

void ErrMsgBx(const TCHAR* ErrorMsg);