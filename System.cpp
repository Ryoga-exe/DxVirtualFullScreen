#include "System.h"

Window::Window() : 
	m_width(640), m_height(480), m_hasPosInitialized(false),
	m_enableSizeChange({ true, true }), m_isFullScreen(true), m_useVirtualFullScreen(false), m_styleMode(7)
{
	DxLib::GetDefaultState(&m_desktopSize.width, &m_desktopSize.height, &m_colorBitDepth);
}

int Window::PosInit() {
	m_hasPosInitialized = true;
	return DxLib::GetWindowPosition(&m_posX, &m_posY);
}

int Window::SetSize(int width, int height, int colorBitDepth) {
	if (!m_enableSizeChange.toFit) return -1;
	m_width = width; m_height = height; m_colorBitDepth = colorBitDepth;
	return DxLib::SetGraphMode(width, height, colorBitDepth);
}
int Window::SetPosition(int x, int y) {
	if (m_isFullScreen) return -1;
	m_posX = x; m_posY = y;
	return DxLib::SetWindowPosition(x, y);
}
int Window::SetEnableSizeChange(bool enable, bool toFit) {
	if (m_useVirtualFullScreen) return -1;
	m_enableSizeChange = { enable, toFit };
	if (!toFit) {
		DxLib::SetGraphMode(m_desktopSize.width, m_desktopSize.height, m_colorBitDepth);
		DxLib::SetWindowSize(m_width, m_height);
	}
	return DxLib::SetWindowSizeChangeEnableFlag(enable, toFit);
}
int Window::SetStyleMode(int styleMode) {
	m_styleMode = styleMode;
	return DxLib::SetWindowStyleMode(styleMode);
}
int Window::SetFullScreenMode(bool isFullScreen) {
	m_isFullScreen = isFullScreen;
	if (m_useVirtualFullScreen) {
		if (isFullScreen) {
			int leftWidth, topWidth;

			GetWindowEdgeWidth(&leftWidth, NULL, &topWidth, NULL);

			//SetWindowZOrder(DX_WIN_ZTYPE_TOPMOST);

			if (DxLib::DxLib_IsInit() == TRUE) SetWindowPosition(-leftWidth, -topWidth);

			SetWindowSize(m_desktopSize.width, m_desktopSize.height);
		}
		else {
			//SetWindowZOrder(DX_WIN_ZTYPE_NORMAL);

			if (m_hasPosInitialized) SetWindowPosition(m_posX, m_posY);

			SetWindowSize(m_width, m_height);
		}
		return 0;
	}
	return DxLib::ChangeWindowMode(!isFullScreen);
}

int Window::ToggleFullScreenMode() {
	m_isFullScreen ^= true;
	return SetFullScreenMode(m_isFullScreen);
}

int Window::SetUseVirtualFullScreenFlag(bool useVirtualFullScreen) {
	// 同じなら何もしない
	if ((!m_useVirtualFullScreen && !useVirtualFullScreen) || (m_useVirtualFullScreen && useVirtualFullScreen)) return 0;
	// false -> true
	else if (!m_useVirtualFullScreen && useVirtualFullScreen) {

		if (m_isFullScreen) ChangeWindowMode(TRUE);

		SetGraphMode(m_desktopSize.width, m_desktopSize.height, m_colorBitDepth);

		m_enableSizeChange = { false, false };

		SetWindowSizeChangeEnableFlag(FALSE, FALSE);

		SetWindowSize(m_width, m_height);


	}
	// false -> true ( 2^2 なので else でOK )
	else {

	}
	// 変更
	m_useVirtualFullScreen = useVirtualFullScreen;
	return 0;
}

RectSize_t Window::GetSize() {
	if (!m_enableSizeChange.toFit) {
		int width, height;
		DxLib::GetWindowSize(&width, &height);
		if (m_useVirtualFullScreen) {
			return { width, height };
		}
		m_width = width;
		m_height = height;
	}
	return { m_width, m_height };
}
int Window::GetWidth() {
	RectSize_t winSize;
	winSize = GetSize();
	return winSize.width;
}
int Window::GetHeight() {
	RectSize_t winSize;
	winSize = GetSize();
	return winSize.height;
}

CSystem::CSystem() {

}
CSystem::~CSystem() {
	Finalize();
}
bool CSystem::Finalize() {
	if (m_hasInitialized) {
		DxLib::DxLib_End();
		m_hasInitialized = false;
	}
	return false;
}
bool CSystem::SetAlwaysRunFlag(bool doAlwaysRun) {
	if (m_hasInitialized) return true;
	m_doAlwaysRun = doAlwaysRun;
	return false;
}

bool CSystem::Initialize(const TCHAR* windowTitle) {
	if (m_hasInitialized) return true;
	DxLib::SetOutApplicationLogValidFlag(FALSE);
	DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	m_window.SetSize(1280, 720);
	m_window.SetUseVirtualFullScreenFlag(true);
	
	m_window.SetStyleMode(7);
	m_window.SetFullScreenMode(false);
	DxLib::SetAlwaysRunFlag(m_doAlwaysRun);
	DxLib::SetMainWindowText(windowTitle);

	if (DxLib::DxLib_Init() != 0) {
		ErrMsgBx(L"エラーが発生しました。\nウィンドウの生成に失敗しました。"); // language
		return true;
	}
	if (DxLib::SetDrawScreen(DX_SCREEN_BACK) != 0) {
		ErrMsgBx(L"エラーが発生しました。\nウィンドウの設定に失敗しました。");
		DxLib::DxLib_End();
		return true;
	}
	m_window.PosInit();
	m_hasInitialized = true;
	return false;
}

void ErrMsgBx(const TCHAR* ErrorMsg) {
	MessageBox(
		NULL,
		ErrorMsg,
		L"ERROR",
		MB_OK | MB_ICONERROR
	);
}