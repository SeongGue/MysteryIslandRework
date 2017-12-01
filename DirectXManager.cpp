#include "DirectXManager.h"


CDirectXManager::CDirectXManager()
{
	m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	m_pd3dDevice = NULL;
	m_pd3dDeviceContext = NULL;
	m_4xMsaaQuality = 0;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDepthStencilView = NULL;
	ZeroMemory(&m_d3dScreenViewport, sizeof(D3D11_VIEWPORT));
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

	//스왑체인을 위한 구조체
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = nWidth;
	dxgiSwapChainDesc.BufferDesc.Height = nHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = hwnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

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

	if (!CreateRenderTargetView())
		return false;

	D3D11_VIEWPORT d3dViewport;
	d3dViewport.TopLeftX = 0.0f;
	d3dViewport.TopLeftY = 0.0f;
	d3dViewport.Width = nWidth;
	d3dViewport.Height = nHeight;
	d3dViewport.MinDepth = 0.0f;
	d3dViewport.MaxDepth = 1.0f;
	m_pd3dDeviceContext->RSSetViewports(1, &d3dViewport);

	return true;
}

void CDirectXManager::OnResize(int width, int height)
{
	m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	SafeRelease(&m_pd3dRenderTargetView);
	m_pdxgiSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	CreateRenderTargetView();
}

void CDirectXManager::ClearBackBuffer()
{
	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
}