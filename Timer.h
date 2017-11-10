#pragma once

class CTimer
{
private:
	__int64 m_nPrevCnt;				//���� ������ ����
	double m_dSecondPerCount;		//���� �� ��(�ʴ� ������ ��)
	double m_dDeltaTime;			//�� ������ �� �ð�
	double m_dTotalDeltaTime;		//���� ������Ʈ ������ ��ŸŸ�� �ջ�
	double m_dPeriod;				//1�ʿ� ��� ������ ����

public:
	CTimer();
	~CTimer();
	void Reset();
	void Tick();
	bool FixedFPS();
	float GetDeltaTime();
};

