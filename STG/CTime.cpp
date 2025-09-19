#include "CTime.h"

CTime::CTime()
	:	m_totalTime (0)
		,m_secondsPassed(0.0f)
		,m_currentTime (0)
		,m_beforeTime(0)
		,m_idealTime (0)
		,m_deltaTime (-1.0f)
		,m_progressTime (0)
		,m_baseTime(0)
		,m_pausedTime(0)
		,m_isStopped(false)
{	 
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_secondsPassed = 1.0 / (double)countsPerSec;
}	 
CTime::~CTime()
{
}

void CTime::Tick()
{
	//If game paused
	if (m_isStopped)
	{
		m_deltaTime = 0.0f;
		return;
	}

	// Get the time this frame.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_currentTime = currTime;
	// Time difference between this frame and the previous.
	m_deltaTime = (m_currentTime - m_beforeTime) * m_secondsPassed;
	// Prepare for next frame.
	m_beforeTime = m_currentTime;
	// Force nonnegative. The DXSDK's CDXUTTimer mentions that if the
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0;
	}

}

void CTime::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	// Accumulate the time elapsed between stop and start pairs.
	//
	// |<-------d------->|
	// ---------------*-----------------*------------> time
	// mStopTime startTime
	// If we are resuming the timer from a stopped state...
	if (m_isStopped)
	{
		// then accumulate the paused time.
		m_pausedTime += (startTime - m_stopTime);
		// since we are starting the timer back up, the current
		// previous time is not valid, as it occurred while paused.
		// So reset it to the current time.
		m_beforeTime = startTime;
		// no longer stopped...
		m_stopTime = 0;
		m_isStopped = false;
	}
}

void CTime::Stop()
{
	// If we are already stopped, then don't do anything.
	if (!m_isStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		// Otherwise, save the time we stopped at, and set
		// the Boolean flag indicating the timer is stopped.
		m_stopTime = currTime;
		m_isStopped = true;
	}
}

void CTime::Reset() {
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_baseTime = currTime;
	m_beforeTime = currTime;
	m_stopTime = 0;
	m_isStopped = false;
}

void CTime::StartTimer()
{
}

float CTime::StopTimer()
{
	return 0;
}

float CTime::TotalTime() const
{
	// If we are stopped, do not count the time that has passed
	// since we stopped. Moreover, if we previously already had
	// a pause, the distance mStopTime - mBaseTime includes paused
	// time,which we do not want to count. To correct this, we can
	// subtract the paused time from mStopTime:
	//
	// previous paused time
	// |<----------->|
	// ---*------------*-------------*-------*-----------*------> time
	// mBaseTime mStopTime mCurrTime
	if (m_isStopped)
	{
		return (float)(((m_stopTime - m_pausedTime) -
			m_baseTime) * m_secondsPassed);
	}
	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count. To correct this, we can subtract
	// the paused time from mCurrTime:
	//
	// (mCurrTime - mPausedTime) - mBaseTime
	//
	// |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	// mBaseTime mStopTime startTime mCurrTime
	else
	{
		return (float)(((m_currentTime - m_pausedTime) -
			m_baseTime) * m_secondsPassed);
	}
}

float CTime::GameTime() const 
{
	return 0;
}

float CTime::ProgressTime()
{
	m_progressTime = m_currentTime - m_beforeTime;	//Œo‰ßŽžŠÔ.
	return m_progressTime;
}
