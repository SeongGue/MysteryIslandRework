// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include "GameFramework.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	CGameFramework framework(hInstance);

	//�����ӿ�ũ �ʱ�ȭ
	if (!framework.Init())
		return 0;

	return framework.Run();
}