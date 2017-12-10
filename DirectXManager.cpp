#include "DirectXManager.h"

CDirectXManager* CDirectXManager::m_d3dMgr = NULL;

CDirectXManager* CDirectXManager::GetInstance()
{
	if(m_d3dMgr == NULL)
		m_d3dMgr = new CDirectXManager();
	return m_d3dMgr;
}

void CDirectXManager::DetroyInstance()
{
	delete m_d3dMgr;
}

CDirectXManager::CDirectXManager()
{
	m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	m_pd3dDevice = NULL;
	m_pd3dDeviceContext = NULL;
	m_4xMsaaQuality = 0;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDepthStencilView = NULL;

	m_d3dViewport.TopLeftX = 0.0f;
	m_d3dViewport.TopLeftY = 0.0f;
	m_d3dViewport.Width = 0;
	m_d3dViewport.Height = 0;
	m_d3dViewport.MinDepth = 0.0f;
	m_d3dViewport.MaxDepth = 1.0f;

	m_Enable4xMsaa = false;
}


CDirectXManager::~CDirectXManager()
{
	SafeRelease(&m_pd3dRenderTargetView);
	SafeRelease(&m_pd3dDepthStencilView);
	SafeRelease(&m_pdxgiSwapChain);
	SafeRelease(&m_pd3dDepthStencilBuffer);

	// Restore all default settings.
	if (m_pd3dDeviceContext)
		m_pd3dDeviceContext->ClearState();

	SafeRelease(&m_pd3dDeviceContext);
	SafeRelease(&m_pd3dDevice);

	CDirectXManager::DetroyInstance();
}

bool CDirectXManager::InitDirect3D(HWND hwnd, int width, int height)
{
	if (!CreateDirect3DDisplay(hwnd))
		return false;

	return true;
}

bool CDirectXManager::CreateRenderTargetView()
{
	HRESULT hResult = S_OK;

	//스왑체인의 후면버퍼를 위한 렌더타겟뷰를 생성
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer)))
		return false;
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView)))
		return false;
	SafeRelease(&pd3dBackBuffer);
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);
	
	return true;
}

bool CDirectXManager::CreateDirect3DDisplay(HWND hwnd)
{
	RECT rcClient;
	::GetClientRect(hwnd, &rcClient);
	UINT nWidth = rcClient.right - rcClient.left;
	UINT nHeight = rcClient.bottom - rcClient.top;

	UINT dxCreateDeviceFlags = 0;
#ifdef _DEBUG
	dxCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//UINT m4xMsaaQuality = 0;
	//HR(m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality))

	//스왑체인을 위한 구조체
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1; //후면버퍼의 갯수
	dxgiSwapChainDesc.BufferDesc.Width = nWidth;
	dxgiSwapChainDesc.BufferDesc.Height = nHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;  //60번 화면 갱신
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; //1초에 
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //후면버퍼의 사용 방식과 CPU접근 방법
	dxgiSwapChainDesc.OutputWindow = hwnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;  
	dxgiSwapChainDesc.SampleDesc.Quality = 0; //다중샘플링 
	dxgiSwapChainDesc.Windowed = TRUE;	//FALSE면 전체화면

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;

	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(
			NULL,
			nd3dDriverType,
			NULL,
			dxCreateDeviceFlags,
			d3dFeatureLevels,
			nFeatureLevels,
			D3D11_SDK_VERSION,
			&dxgiSwapChainDesc,
			&m_pdxgiSwapChain,
			&m_pd3dDevice,
			&nd3dFeatureLevel,
			&m_pd3dDeviceContext)))
			break;
	}
	if (!m_pdxgiSwapChain || !m_pd3dDevice || !m_pd3dDeviceContext)
		return(false);

	OnResize(nWidth, nHeight);

	return true;
}

void CDirectXManager::OnResize(int width, int height)
{
	m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	SafeRelease(&m_pd3dRenderTargetView);
	m_pdxgiSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	CreateRenderTargetView();

	m_d3dViewport.Width = width;
	m_d3dViewport.Height = height;
	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CDirectXManager::ClearBackBuffer()
{
	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
}