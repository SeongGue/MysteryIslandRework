#pragma once
#include "Utility.h"
#include "DirectXManager.h"

struct CVertex
{
	XMFLOAT3 Pos;
};

class CMesh
{
private:
	ID3D11Buffer* m_BoxVB;
	ID3D11Buffer* m_BoxIB;

public:
	CMesh();
	~CMesh();

	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	void Draw();
};

