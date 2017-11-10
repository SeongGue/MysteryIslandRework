#pragma once
#include "DirectXManager.h"
#include "Timer.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;

	int		  m_nClientWidth;
	int		  m_nClientHeight;

	CTimer	  m_timer;
	CDirectXManager m_d3dMgr;

public:
	static CGameFramework* m_framework;

private:
	//윈도우 클래스 등록 함수
	BOOL InitRegisterClass();
	//콘솔 생성 함수
	void CreateConsole();
	void DestroyConsole();

public:
	CGameFramework(HINSTANCE hInstance);
	~CGameFramework();

	bool Init();
	bool InitWindow();

	int	 Run();
	void Update();
	void Draw();

	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

