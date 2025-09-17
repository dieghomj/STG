#pragma once

#include <Windows.h>

//ライブラリ読み込み
#pragma comment( lib, "winmm.lib")	//音再生で必要.


//***********************************************//
//	サウンドクラス
//		midi,mp3,wav形式ファイルの再生・停止などを行う
//
//***********************************************//
class CSound
{

public:

	//定数宣言
	static const int STR_BUFFER_MAX = 256;	

	CSound();
	
	~CSound();

	//音ファイルを開く
	bool Open(const char* sFileName, const char* sAlias, HWND hWnd);

	//音ファイルを閉じる
	bool Close();

	//音を再生する(状態通知の有無フラグあり)
	bool Play(bool bNotify = false);

	//音を再生する(SEで使う)
	bool PlaySE();

	bool PlayLoop();

	//音を停止する.
	bool Stop();

	//音の状態を取得する.
	bool GetStatus(char* sStatus );

	//音の停止を確認する.
	bool IsStopped();

	//音の再生位置を最初にする.
	bool SeekToStart();

private:

	//初期化値を設定する関数
	void SetInitParam(const char* sAlias, HWND hWnd);

private:

	HWND	m_hWnd;
	char	m_sAlias[STR_BUFFER_MAX];

};

