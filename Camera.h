#pragma once
#include "Utility.h"
#include "DirectXManager.h"

struct VS_CB_CAMERA
{
	XMFLOAT4X4 m_mView;
	XMFLOAT4X4 m_mProj;
};

class CCamera
{
private:
	XMFLOAT4X4 m_mView;
	XMFLOAT4X4 m_mProj;

	ID3D11Buffer *m_cbCamera;

public:
	CCamera();
	~CCamera();

	void InitCamera(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up, int width, int height);
	HRESULT CreateConstantBuffer();
	void UpdateCamera();
	const XMFLOAT4X4* GetViewMatrix() const { return &m_mView; }
	const XMFLOAT4X4* GetProjMatrix() const { return &m_mProj; }
};