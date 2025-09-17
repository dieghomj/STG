#include "Sound.h"
#include <stdio.h>	//sprintf_s関数を使用するのに必要.

//===============================================================
//	音ファイルを開く
//===============================================================
bool sound_Open(const char* sFileName, const char* sAlias, HWND hWnd) {

	char cmd[256] = "";

	sprintf_s(cmd, "open %s alias %s", sFileName, sAlias);

	if ( mciSendString(cmd, nullptr, 0, hWnd) == 0 ) {
		return true;
	}
	return false;
}

//===============================================================
//	音ファイルを閉じる
//===============================================================
bool sound_Close(const char* sAlias, HWND hWnd) {
	char cmd[256] = "";

	sprintf_s(cmd, "close %s", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//音を再生する
//===============================================================
bool sound_Play(const char* sAlias, HWND hWnd, bool bNotifiy)
{
	char cmd[256] = "";

	if (bNotifiy == true) {
		sprintf_s(cmd, "play %s notify", sAlias);	//notify:　音の状態取得で必要　
	}
	else
	{
		sprintf_s(cmd, "play %s", sAlias);
	}


	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}

bool sound_PlaySE(const char* sAlias, HWND hWnd)
{
	sound_SeekToStart(sAlias, hWnd);
	if (sound_Play(sAlias, hWnd) == true) {
		return true;
	}
	return false;
}

bool sound_PlayLoop(const char* sAlias, HWND hWnd)
{
	if (sound_IsStopped(sAlias, hWnd) == true)
	{
		sound_SeekToStart(sAlias, hWnd);
		if (sound_Play(sAlias, hWnd, true) == true) {
			return true;
		}
	}
	return false;
}

//===============================================================
//音を停止する.
//===============================================================
bool sound_Stop(const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "stop %s", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
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
bool sound_GetStatus(char* sStatus, const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "status %s mode", sAlias);

	if (mciSendString(cmd, sStatus, 256, hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//	音の停止を確認する.
//===============================================================
bool sound_IsStopped(const char* sAlias, HWND hWnd)
{
	char sSTatus[256] = "";

	//状態の取得
	if (sound_GetStatus(sSTatus, sAlias, hWnd) == true) {
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
bool sound_SeekToStart(const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "seek %s to start", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}
