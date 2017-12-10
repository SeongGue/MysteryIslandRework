#include "Shader.h"


CShader::CShader()
{
	CreateVertexShaderFromFile();
	CreatePixelShaderFromFile();

	m_pObject = new CGameObject(XMFLOAT3(0.0f, 0.0f, 1.0f));
}

CShader::~CShader()
{
	SafeRelease(&m_pd3dVertexShader);
	SafeRelease(&m_pd3dVertexLayout);
	SafeRelease(&m_pd3dPixelShader);

	SafeDelete(m_pObject);
}


void CShader::CreateVertexShaderFromFile()
{
	HRESULT hResult;
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, &m_pd3dVertexShader);
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		pd3dDevice->CreateInputLayout(d3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), &m_pd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

//���̴� �ҽ� �ڵ� ���Ͽ��� �ȼ�-���̴��� �����ϴ� �Լ��̴�. 
void CShader::CreatePixelShaderFromFile()
{
	HRESULT hResult;
	ID3D11Device* pd3dDevice = CDirectXManager::GetInstance()->GetDevice();

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(_T("Effect.fx"), NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, &m_pd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::Update()
{
	m_pObject->Update();
}

void CShader::Draw()
{
	ID3D11DeviceContext* pd3dDeviceContext = CDirectXManager::GetInstance()->GetDC();

	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	m_pObject->Draw();
}  