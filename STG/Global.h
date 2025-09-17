#pragma once
#include <Windows.h>

//---------------------------------------------------------------
//定数宣言.
//---------------------------------------------------------------
const int PI		= 3.14159265358979f;	//円周率.
const int FPS		= 60;		//FPS(基準).
const int WND_W		= 480;		//ウィンドウの幅.
const int WND_H		= 640;		//ウィンドウの高さ.

const int C_SIZE = 64;//キャラクターサイズ


static const int E_MAX = 5;//敵機の最大数
static const int PS_MAX = 32;	//Linked to CPlayer PS_MAX

//-----------------------------------------
//例挙型.
//-----------------------------------------
//キャラクター状態例挙型.
enum enCharaState {
	Ready = 0,
	Living,
	Dying,
	Dead,
};

//シーン例挙型
enum enScene
{
	Title,
	GameMain,
	GameOver,
	Ending,
};

//---------------------------------------------------------------
//構造体.
//---------------------------------------------------------------
typedef struct _GameWindow
{
	HWND	hWnd;		//ウィンドウハンドル.
	HDC		hScreenDC;	//バックバッファ.
	SIZE	size;		//サイズ.
	DWORD	dwFPS;	//FPS( Frame Per Second: フレーム / 秒 ).
	BOOL	isWndClose;//ウィンドウ閉じてるか確認用.
} GameWindow;

struct VEC2
{
	int x;
	int y;
};


//キャラクター構造体.
struct CHARA
{
	VEC2 position;	//自機の座標
	int state;		//自機の状態.
	int ExpAnimCnt;	//自機の爆発アニメーションカウンタ
};
