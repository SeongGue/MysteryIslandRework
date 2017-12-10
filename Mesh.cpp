#include "Mesh.h"


CMesh::CMesh()
{
	HR(CreateVertexBuffer());
	HR(CreateIndexBuffer());
}


CMesh::~CMesh()
{
	SafeRelease(&m_BoxVB);
	SafeRelease(&m_BoxIB);
}


HRESULT CMesh::CreateVertexBuffer()
{
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	CVertex pVertices[3] = {
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f)
	};

	//정점 버퍼를 생성한다. 
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CVertex) * 3;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;

	return pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_BoxVB);
}

HRESULT CMesh::CreateIndexBuffer()
{
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	UINT indices[] = {
		0, 1, 2,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	return pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_BoxIB);
}

void CMesh::Draw()
{
	ID3D11DeviceContext* pd3dDeviceContext = CDirectXManager::GetInstance()->GetDC();

	UINT stride = sizeof(CVertex);
	UINT offset = 0;

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_BoxVB, &stride, &offset);
	pd3dDeviceContext->IASetIndexBuffer(m_BoxIB, DXGI_FORMAT_R32_UINT, 0);
	pd3dDeviceContext->DrawIndexed(3, 0, 0);
}