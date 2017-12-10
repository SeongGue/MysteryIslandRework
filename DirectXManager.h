#pragma once
#include "Utility.h"

class CDirectXManager
{
private:
	static CDirectXManager* m_d3dMgr;

	D3D_DRIVER_TYPE			m_d3dDriverType;
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pd3dDeviceContext;
	IDXGISwapChain*			m_pdxgiSwapChain;
	UINT					m_4xMsaaQuality;

	ID3D11Texture2D*		m_pd3dDepthStencilBuffer;
	ID3D11RenderTargetView* m_pd3dRenderTargetView;
	ID3D11DepthStencilView* m_pd3dDepthStencilView;
	D3D11_VIEWPORT			m_d3dViewport;
	bool					m_Enable4xMsaa;
	
private:
	static void DetroyInstance();

public:
	CDirectXManager();
	~CDirectXManager();

	static CDirectXManager* GetInstance();

	bool InitDirect3D(HWND hwnd, int width, int height);
	bool CreateRenderTargetView();
	bool CreateDirect3DDisplay(HWND hwnd);
	void OnResize(int width, int height);
	void ClearBackBuffer();

	ID3D11Device*		 GetDevice()	{ return m_pd3dDevice; }
	ID3D11DeviceContext* GetDC()		{ return m_pd3dDeviceContext; }		
	IDXGISwapChain*		 GetSwapChain() { return m_pdxgiSwapChain; }
};
	