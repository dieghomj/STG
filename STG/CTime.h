#pragma once
#include "time.h"
#include "Windows.h"

class CTime
{
public:
	CTime();
	~CTime();

	void Tick();
	void Start();
	void Stop();
	void Reset();
	void StartTimer();
	float StopTimer();
	float ProgressTime();
	
	float TotalTime() const;
	float GameTime() const;
	float DeltaTime() const { return m_deltaTime; };

private:
	double m_secondsPassed;
	double m_deltaTime;
	__int64 m_baseTime;
	__int64 m_pausedTime;
	__int64 m_stopTime;
	__int64 m_beforeTime;
	__int64 m_currentTime;
	bool  m_isStopped;
	
	float m_totalTime;
	float m_progressTime;
	float m_idealTime;
};

