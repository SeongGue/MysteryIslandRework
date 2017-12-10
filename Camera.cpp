#include "Camera.h"



CCamera::CCamera()
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mView, I);
	XMStoreFloat4x4(&m_mProj, I);
}


CCamera::~CCamera()
{
}

void CCamera::InitCamera(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up, int width, int height)
{
	//카메라 행렬 생성
	XMVECTOR p = XMLoadFloat3(&pos);
	XMVECTOR t = XMLoadFloat3(&target);
	XMVECTOR u = XMLoadFloat3(&up);

	XMMATRIX V = XMMatrixLookAtLH(p, t, u);
	XMStoreFloat4x4(&m_mView, V);

	//투영 행렬 생성
	XMMATRIX P = XMMatrixPerspectiveFovLH(3.1415926535f * 0.25f, static_cast<float>(width / height), 1.0f, 500.0f);
	XMStoreFloat4x4(&m_mProj, P);

	HR(CreateConstantBuffer());
}

HRESULT CCamera::CreateConstantBuffer()
{
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	return pd3dDevice->CreateBuffer(&bd, NULL, &m_cbCamera);
}

void CCamera::UpdateCamera()
{
	ID3D11DeviceContext* pd3dDeviceContext = CDirectXManager::GetInstance()->GetDC();

	XMMATRIX V = XMLoadFloat4x4(&m_mView);
	XMMATRIX P = XMLoadFloat4x4(&m_mProj);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_cbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA* cbCamera = (VS_CB_CAMERA*)d3dMappedResource.pData;
	XMStoreFloat4x4(&cbCamera->m_mView, XMMatrixTranspose(V));
	XMStoreFloat4x4(&cbCamera->m_mProj, XMMatrixTranspose(P));
	pd3dDeviceContext->Unmap(m_cbCamera, 0);

	pd3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cbCamera);
}