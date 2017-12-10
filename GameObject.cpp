#include "GameObject.h"



CGameObject::CGameObject()
{
	m_pMesh = NULL;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mWorld, I);
}

CGameObject::CGameObject(XMFLOAT3 pos)
{
	m_pMesh = new CMesh();
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mWorld, I);

	HR(CreateConstantBuffer());
	m_mWorld._41 = pos.x;
	m_mWorld._42 = pos.y;
	m_mWorld._43 = pos.z;
}

CGameObject::~CGameObject()
{
	SafeDelete(m_pMesh);
}

HRESULT CGameObject::CreateConstantBuffer()
{
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	return pd3dDevice->CreateBuffer(&bd, NULL, &m_cbWorld);
}

void CGameObject::Update()
{
	ID3D11DeviceContext* pd3dDeviceContext = CDirectXManager::GetInstance()->GetDC();

	XMMATRIX W = XMLoadFloat4x4(&m_mWorld);
	//XMMATRIX S = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	//XMMATRIX R = XMMatrixRotationY()

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_cbWorld, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD* cbWorld = (VS_CB_WORLD*)d3dMappedResource.pData;
	XMStoreFloat4x4(&cbWorld->m_mWorld, XMMatrixTranspose(W));
	pd3dDeviceContext->Unmap(m_cbWorld, 0);

	pd3dDeviceContext->VSSetConstantBuffers(1, 1, &m_cbWorld);
}
#define VS_CAMERA 0x00
#define VS_CAMERA 0x01

void CGameObject::Draw()
{
	ID3D11DeviceContext* pd3dDeviceContext = CDirectXManager::GetInstance()->GetDC();

	m_pMesh->Draw();
}