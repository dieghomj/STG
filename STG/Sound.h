#pragma once

#include <Windows.h>

//ライブラリ読み込み
#pragma comment( lib, "winmm.lib")	//音再生で必要.

//音ファイルを開く
bool sound_Open(const char* sFileName, const char* sAlia, HWND hWnd);

//音ファイルを閉じる
bool sound_Close(const char* sAlias, HWND hWnd);

//音を再生する(状態通知の有無フラグあり)
bool sound_Play(const char* sAlias, HWND hWnd, bool bNotify = false);

//音を再生する(SEで使う)
bool sound_PlaySE(const char* sAlias, HWND hWnd);

bool sound_PlayLoop(const char* sAlias, HWND hWnd);

//音を停止する.
bool sound_Stop(const char* sAlias, HWND hWnd);

//音の状態を取得する.
bool sound_GetStatus(char* sStatus, const char* sAlias, HWND hWnd);

//音の停止を確認する.
bool sound_IsStopped(const char* sAlias, HWND hWnd);

//音の再生位置を最初にする.
bool sound_SeekToStart(const char* sAlias, HWND hWnd);
