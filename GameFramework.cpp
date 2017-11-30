#include "GameFramework.h"
#include "Utility.h"

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

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
}

CGameFramework::~CGameFramework()
{
	SafeRelease(&mBoxVB);
	SafeRelease(&mBoxIB);

	SafeRelease(&m_pd3dVertexShader);
	SafeRelease(&m_pd3dVertexLayout);
	SafeRelease(&m_pd3dPixelShader);
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
	if (!m_d3dMgr.InitDirect3D(m_hWnd, m_nClientWidth, m_nClientHeight))
		return false;

	// Create vertex buffer
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.5f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dMgr.GetDevice()->CreateBuffer(&vbd, &vinitData, &mBoxVB));


	// Create the index buffer

	UINT indices[] = {
		0, 1, 2,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dMgr.GetDevice()->CreateBuffer(&ibd, &iinitData, &mBoxIB));

	//입력 조립기
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	//버텍스 쉐이더
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dVertexShaderBlob = NULL, *pd3dErrorBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pd3dVertexShaderBlob, &pd3dErrorBlob, NULL)))
	{
		m_d3dMgr.GetDevice()->CreateVertexShader(pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), NULL, &m_pd3dVertexShader);
		m_d3dMgr.GetDevice()->CreateInputLayout(d3dInputLayout, nElements, pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize(), &m_pd3dVertexLayout);
		pd3dVertexShaderBlob->Release();
	}
	//픽셀 쉐이더
	ID3DBlob *pd3dPixelShaderBlob = NULL;
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pd3dPixelShaderBlob, &pd3dErrorBlob, NULL)))
	{
		m_d3dMgr.GetDevice()->CreatePixelShader(pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize(), NULL, &m_pd3dPixelShader);
		pd3dPixelShaderBlob->Release();
	}

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

}

void CGameFramework::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_d3dMgr.GetDC()->IASetInputLayout(m_pd3dVertexLayout);
	m_d3dMgr.GetDC()->VSSetShader(m_pd3dVertexShader, NULL, 0);
	m_d3dMgr.GetDC()->PSSetShader(m_pd3dPixelShader, NULL, 0);

	m_d3dMgr.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_d3dMgr.GetDC()->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	m_d3dMgr.GetDC()->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	m_d3dMgr.GetDC()->DrawIndexed(1, 0, 0);
	//m_d3dMgr.GetDC()->Draw(3, 0);
	HR(m_d3dMgr.GetSwapChain()->Present(0, 0));
}


	
