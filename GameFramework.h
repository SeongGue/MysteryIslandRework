#pragma once
#include "Shader.h"
#include "Timer.h"
#include "Camera.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;

	int		  m_nClientWidth;
	int		  m_nClientHeight;

	CTimer	  m_timer;
	//CDirectXManager m_d3dMgr;


	//XMFLOAT4X4 mWorld;
	//XMFLOAT4X4 mView;
	//XMFLOAT4X4 mProj;

	CShader* m_pShader;
	CCamera* m_pCamera;


public:
	static CGameFramework* m_framework;

private:
	//������ Ŭ���� ��� �Լ�
	BOOL InitRegisterClass();
	//�ܼ� ���� �Լ�
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

