//     fxc /E VS /T vs_5_0 /Od /Zi /Fo CompiledShader.fxo Effect.fx

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
}

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

VS_OUTPUT VS(float4 position : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(position, gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);
	return output;
}

//float4 VS(float4 Pos : POSITION) : SV_POSITION
//{
//	//Pos = mul(Pos, gmtxWorld);
//	return Pos;
//}

float4 PS(VS_OUTPUT Pos : SV_POSITION) : SV_Target
{
	/*�ȼ�-���̴��� �׻� ������ ��Ÿ���� 4���� �Ǽ�(float4)�� ��ȯ�ؾ� �Ѵ�. �Է� �Ķ���Ϳ� ������� ����� ������ ��ȯ�Ѵ�. �׷��Ƿ� �������� ����� �Ǵ� ��� �ȼ��� ������ ������� �ȴ�(����� = ������ + ���).*/
	return float4(1.0f, 1.0f, 0.0f, 1.0f);    // R=1.0f, G=1.0f, B=0.0f, A=1.0f, 
}
