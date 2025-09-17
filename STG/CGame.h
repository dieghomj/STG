#pragma once
#include <Windows.h>
#include <map>
#include <string>

#include "Global.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CXInput.h"
#include "CSound.h"
#include "CRender.h"

#pragma comment( lib, "msimg32.lib" )	//指定色を透過して描画で使用する.
										//半透明の描画でも使用する.

class CGame
{
public:
	//---------------------------------------
	//	定数宣言.
	//---------------------------------------
	

	//-----------------------------------------
	// 例挙型宣言
	//-----------------------------------------
	
	//シーン列挙型.


	//---------------------------------------
	//	構造体宣言.
	//---------------------------------------
	

public:
	CGame();	// Constructor
	CGame(GameWindow* pGameWnd);
	~CGame();	// Destructor
	//初期化関数.
	void InitializeGame();
	//構築関数
	bool Create();
	//破棄関数
	void Destroy();
	//更新関数
	void Update();
	//描画関数
	void Draw();
	//ウィンドウを閉じているか確認.
	BOOL IsWindowClosed() const { return m_pGameWnd->isWndClose; }
	//FPSを取得.
	DWORD GetFPS() const { return m_pGameWnd->dwFPS; }
	//バックバッファを取得.
	HDC GetScreenDC() const { return m_pGameWnd->hScreenDC; }
	//ウィンドウハンドルを取得.
	HWND GetWnd() const { return m_pGameWnd->hWnd; }
	//BGM無音か確認
	bool IsMuted() const { return m_IsMuted; }
	//BGM無音する
	void Mute() { m_IsMuted = true; }


private:
	void HandleInput();
	bool IsKeyDown(std::string id);

private:
	GameWindow* m_pGameWnd;		//ゲームウィンドウ構造体.
	CXInput*	m_pXInput;		//コントローラークラスをポインタで用意.
	HDC			m_hMemDC;		//メモリデバイスコンテキスト.
	HFONT		m_hFont;		//フォントハンドル.
	HBITMAP		m_hBack;		//背景.
	HBITMAP		m_hChara;		//キャラクター.
	HBITMAP		m_hExplosion;	//爆発
	HBITMAP		m_hTitle;		//タイトル画像
	HBITMAP		m_hGameOver;	//ゲームオーバー画像
	HBITMAP		m_hEnding;		//エンディング画像
	//アルファ値.
	int			m_Alpha;		//タイトルのPressEnterKeyで使う
	//シーン.
	enScene		m_Scene;
	//スコア
	int			m_Score;
	
	//---------自機------------
	CPlayer*	m_pPlayer;
	VEC2*		m_pPBullets;
	bool*		m_pPBulletsFlags;
	//---------敵機------------
	CEnemy*		m_Enemy[E_MAX];
	//背景スクロール.
	int			m_back_y = 0;	//y座標
	//---------サウンド------------
	int			m_BGMNo;		//BGM番号.
	CSound*		m_pBGM1;		//BGMクラス.
	bool		m_IsMuted;		//BGM無音
	//---------入力------------
	std::map<std::string, bool> m_keyBoardInput;
	POINT 		m_CursorPos;

	bool CollsionDetection(
		int Ax, int Ay, int Aw, int Ah,	//矩形Aのｘ、ｙ座標と幅高さ.
		int Bx, int By, int Bw, int Bh);	//矩形Ｂのｘ、ｙ座標と幅高さ


};

