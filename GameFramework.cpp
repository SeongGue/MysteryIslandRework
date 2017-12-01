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
	//XMStoreFloat4x4(&mView, I);
	//XMStoreFloat4x4(&mProj, I);

	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
}

CGameFramework::~CGameFramework()
{
	SafeRelease(&m_BoxVB);
	SafeRelease(&m_BoxIB);

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
	AllocConsole(); //�ܼ� �Ҵ�
	FILE *acStreamOut;
	FILE *acStreamIn;
	freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
	freopen_s(&acStreamIn, "CONIN$", "r", stdin);
}

void CGameFramework::DestroyConsole()
{
	FreeConsole();   //�ܼ��� ����
}

bool CGameFramework::Init()
{
	//API �ʱ�ȭ
	if (!InitWindow())
		return false;

	//���̷�Ʈ3D �ʱ�ȭ
	if (!m_d3dMgr.InitDirect3D(m_hWnd, m_nClientWidth, m_nClientHeight))
		return false;

	CVertex pVertices[3] = {
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f)
	};

	//���� ���۸� �����Ѵ�. 
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CVertex) * 3;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	m_d3dMgr.GetDevice()->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_BoxVB);

	CreateVertexShaderFromFile();
	CreatePixelShaderFromFile();

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
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hwnd, &ps);
		break;
	case WM_SIZE:
		if (m_d3dMgr.GetDC())
		{
			m_nClientWidth = LOWORD(lParam);
			m_nClientHeight = HIWORD(lParam);
			m_d3dMgr.OnResize(m_nClientWidth, m_nClientHeight);
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
	m_timer.Reset(); //�ð� ����

	while (msg.message != WM_QUIT)
	{
		m_timer.Tick(); //������ ���� �ð� ���

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (m_timer.FixedFPS()) //1�ʿ� 60������ ����
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
	UINT stride = sizeof(CVertex);
	UINT offset = 0;
	m_d3dMgr.ClearBackBuffer();

	m_d3dMgr.GetDC()->IASetInputLayout(m_pd3dVertexLayout);
	m_d3dMgr.GetDC()->VSSetShader(m_pd3dVertexShader, NULL, 0);
	m_d3dMgr.GetDC()->PSSetShader(m_pd3dPixelShader, NULL, 0);


	m_d3dMgr.GetDC()->IASetVertexBuffers(0, 1, &m_BoxVB, &stride, &offset);
	m_d3dMgr.GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dMgr.GetDC()->Draw(3, 0);

	HR(m_d3dMgr.GetSwapChain()->Present(0, 0));
}

void CGameFramework::CreateVertexShaderFromFile()
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);

	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		m_d3dMgr.GetDevice()->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, &m_pd3dVertexShader);
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		m_d3dMgr.GetDevice()->CreateInputLayout(d3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), &m_pd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

//���̴� �ҽ� �ڵ� ���Ͽ��� �ȼ�-���̴��� �����ϴ� �Լ��̴�. 
void CGameFramework::CreatePixelShaderFromFile()
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		m_d3dMgr.GetDevice()->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, &m_pd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}
	
