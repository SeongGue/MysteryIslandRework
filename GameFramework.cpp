#include "GameFramework.h"

CGameFramework* CGameFramework::m_framework = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return CGameFramework::m_framework->MainWndProc(hwnd, msg, wParam, lParam);
}

CGameFramework::CGameFramework(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = NULL;
	m_nClientWidth = 1024;
	m_nClientHeight = 700;

	m_framework = this;

	//XMStoreFloat4x4(&mView, I);
	//XMStoreFloat4x4(&mProj, I);
	m_pShader = NULL;
	m_pCamera = NULL;
}

CGameFramework::~CGameFramework()
{
	SafeDelete(m_pShader);
	SafeDelete(m_pCamera);
}

BOOL CGameFramework::InitRegisterClass()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("MysteryClassName");

	return RegisterClass(&wc);
}

void CGameFramework::CreateConsole()
{
	AllocConsole(); //콘솔 할당
	FILE *acStreamOut;
	FILE *acStreamIn;
	freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
	freopen_s(&acStreamIn, "CONIN$", "r", stdin);
}

void CGameFramework::DestroyConsole()
{
	FreeConsole();   //콘솔을 해제
}

bool CGameFramework::Init()
{
	//API 초기화
	if (!InitWindow())
		return false;

	//다이렉트3D 초기화
	if (!CDirectXManager::GetInstance()->InitDirect3D(m_hWnd, m_nClientWidth, m_nClientHeight))
		return false;

	//쉐이더 만들기
	m_pShader = new CShader();
	m_pCamera = new CCamera();
	m_pCamera->InitCamera(XMFLOAT3(0.0f, 0.0f, -2.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), m_nClientWidth, m_nClientHeight);

	return true;
}

bool CGameFramework::InitWindow()
{
	CreateConsole();

	if (!InitRegisterClass())
	{
		MessageBox(0, _T("RegisterClass Failed."), 0, 0);
		return false;
	}

	m_hWnd = CreateWindow(_T("MysteryClassName"), _T("MysteryIsland"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		m_nClientWidth, m_nClientHeight, 0, 0, m_hInstance, 0);

	if (!m_hWnd)
	{
		MessageBox(0, _T("CreateWindow Failed."), 0, 0);
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}


LRESULT CALLBACK CGameFramework::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		EndPaint(hwnd, &ps);
		break;
	case WM_SIZE:
		if (CDirectXManager::GetInstance()->GetDC())
		{
			m_nClientWidth = LOWORD(lParam);
			m_nClientHeight = HIWORD(lParam);
			CDirectXManager::GetInstance()->OnResize(m_nClientWidth, m_nClientHeight);
		}
		break;
	case WM_DESTROY:
		DestroyConsole();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int CGameFramework::Run()
{
	MSG msg = { 0 };
	m_timer.Reset(); //시계 셋팅

	while (msg.message != WM_QUIT)
	{
		m_timer.Tick(); //프레임 사이 시간 계산

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (m_timer.FixedFPS()) //1초에 60프레임 고정
			{
				Update();
				Draw();
			}
		}
	}
	return (int)msg.wParam;
}

void CGameFramework::Update()
{
	m_pCamera->UpdateCamera();
	m_pShader->Update();
}

void CGameFramework::Draw()
{
	CDirectXManager* d3dMgr = CDirectXManager::GetInstance();

	d3dMgr->ClearBackBuffer();

	d3dMgr->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pShader->Draw();

	HR(d3dMgr->GetSwapChain()->Present(0, 0));
}
	
