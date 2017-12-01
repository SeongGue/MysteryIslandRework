#pragma once
#include "DirectXManager.h"
#include "Timer.h"

struct CVertex
{
	XMFLOAT3 Pos;
};

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;

	int		  m_nClientWidth;
	int		  m_nClientHeight;

	CTimer	  m_timer;
	CDirectXManager m_d3dMgr;


	XMFLOAT4X4 mWorld;
	//XMFLOAT4X4 mView;
	//XMFLOAT4X4 mProj;

	ID3D11Buffer* m_BoxVB;
	ID3D11Buffer* m_BoxIB;

	ID3D11VertexShader				*m_pd3dVertexShader;
	ID3D11InputLayout				*m_pd3dVertexLayout;
	ID3D11PixelShader				*m_pd3dPixelShader;

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
	// ���̴� �ҽ� �ڵ� ���Ͽ��� ���� - ���̴��� �����ϴ� �Լ��̴�.
	void CreateVertexShaderFromFile();

	//���̴� �ҽ� �ڵ� ���Ͽ��� �ȼ�-���̴��� �����ϴ� �Լ��̴�. 
	void CreatePixelShaderFromFile();
};

