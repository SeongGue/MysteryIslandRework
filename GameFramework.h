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
	// 쉐이더 소스 코드 파일에서 정점 - 쉐이더를 생성하는 함수이다.
	void CreateVertexShaderFromFile();

	//쉐이더 소스 코드 파일에서 픽셀-쉐이더를 생성하는 함수이다. 
	void CreatePixelShaderFromFile();
};

