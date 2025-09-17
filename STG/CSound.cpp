#include "CSound.h"
#include <stdio.h>	//sprintf_s関数を使用するのに必要

CSound::CSound()
	//	初期化リスト
	: m_hWnd	( nullptr)
	, m_sAlias	()
{
}

CSound::~CSound()
{
}

//===============================================================
//	音ファイルを開く
//===============================================================
bool CSound:: Open(const char* sFileName, const char* sAlias, HWND hWnd) {

	//初期化設定
	SetInitParam(sAlias, hWnd);

	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "open %s alias %s", sFileName, m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//===============================================================
//	音ファイルを閉じる
//===============================================================
bool CSound::Close() {
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "close %s", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//音を再生する
//===============================================================
bool CSound::Play(bool bNotifiy)
{
	char cmd[STR_BUFFER_MAX] = "";

	if (bNotifiy == true) {
		sprintf_s(cmd, "play %s notify", m_sAlias);	//notify:　音の状態取得で必要　
	}
	else
	{
		sprintf_s(cmd, "play %s", m_sAlias);
	}


	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

bool CSound::PlaySE()
{
	SeekToStart();
	if (Play() == true) {
		return true;
	}
	return false;
}

bool CSound::PlayLoop()
{
	if (IsStopped() == true)
	{
		SeekToStart();
		if (Play(true) == true) {
			return true;
		}
	}
	return false;
}

//===============================================================
//音を停止する.
//===============================================================
bool CSound::Stop()
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "stop %s", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//音の状態を取得する.
//	sStatus の配列数は256以下にすること
//	※ただし、状態をする場合は、再生時に「notifiy」を設定し、
//	　ウィンドウハンドルにメッセージを送っておく必要がある.
//===============================================================
bool CSound::GetStatus(char* sStatus)
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "status %s mode", m_sAlias);

	if (mciSendString(cmd, sStatus, STR_BUFFER_MAX, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//	音の停止を確認する.
//===============================================================
bool CSound:: IsStopped()
{
	char sSTatus[STR_BUFFER_MAX] = "";

	//状態の取得
	if (GetStatus(sSTatus) == true) {
		//文字列比較.
		if (strcmp(sSTatus, "stopped") == 0) {
			return true;
		}
	}
	return false;
}
//===============================================================
//	音の再生位置を最初にする.
//===============================================================
bool CSound::SeekToStart()
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "seek %s to start", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//初期化値を設定する関数
void CSound::SetInitParam(const char* sAlias, HWND hWnd)
{
	m_hWnd = hWnd;

	strcpy_s(m_sAlias, sizeof(m_sAlias), sAlias);
}
