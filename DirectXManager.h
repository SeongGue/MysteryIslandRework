#pragma once
#include "Utility.h"

class CDirectXManager
{
private:
	D3D_DRIVER_TYPE			m_d3dDriverType;
	ID3D11Device*			m_d3dDevice;
	ID3D11DeviceContext*	m_d3dDeviceContext;
	IDXGISwapChain*			m_SwapChain;
	UINT					m_4xMsaaQuality;

	ID3D11Texture2D*		m_DepthStencilBuffer;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11DepthStencilView* m_DepthStencilView;
	D3D11_VIEWPORT			m_ScreenViewport;
	bool					m_Enable4xMsaa;

public:
	CDirectXManager();
	~CDirectXManager();

	bool InitDirect3D(HWND hwnd, int width, int height);
	void OnResize(int width, int height);

	IDXGISwapChain* GetSwapChain() { return m_SwapChain; }
};

