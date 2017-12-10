#pragma once
#include "GameObject.h"

class CShader
{
private:
	ID3D11VertexShader				*m_pd3dVertexShader;
	ID3D11InputLayout				*m_pd3dVertexLayout;
	ID3D11PixelShader				*m_pd3dPixelShader;

	CGameObject* m_pObject;

public:
	CShader();
	~CShader();

	void CreateVertexShaderFromFile();
	void CreatePixelShaderFromFile();
	void Update();
	void Draw();
};

