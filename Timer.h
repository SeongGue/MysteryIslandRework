#pragma once

class CTimer
{
private:
	__int64 m_nPrevCnt;				//이전 프레임 개수
	double m_dSecondPerCount;		//개수 당 초(초당 개수의 역)
	double m_dDeltaTime;			//한 프레임 당 시간
	double m_dTotalDeltaTime;		//다음 업데이트 전까지 델타타임 합산
	double m_dPeriod;				//1초에 몇번 돌릴지 결정

public:
	CTimer();
	~CTimer();
	void Reset();
	void Tick();
	bool FixedFPS();
	float GetDeltaTime();
};

