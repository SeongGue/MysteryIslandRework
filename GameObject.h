#pragma once
#include "Mesh.h"

struct VS_CB_WORLD
{
	XMFLOAT4X4 m_mWorld;
};

class CGameObject
{
private:
	CMesh* m_pMesh;
	XMFLOAT4X4 m_mWorld;
	ID3D11Buffer* m_cbWorld;

public:
	CGameObject();
	CGameObject(XMFLOAT3 pos);
	~CGameObject();

	HRESULT CreateConstantBuffer();
	void Update();
	void Draw();
};

