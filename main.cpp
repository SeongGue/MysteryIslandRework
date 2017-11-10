// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include "GameFramework.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	CGameFramework framework(hInstance);

	//프레임워크 초기화
	if (!framework.Init())
		return 0;

	return framework.Run();
}