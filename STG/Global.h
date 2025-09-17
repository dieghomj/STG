#pragma once
#include <Windows.h>

//---------------------------------------------------------------
//�萔�錾.
//---------------------------------------------------------------
const int PI		= 3.14159265358979f;	//�~����.
const int FPS		= 60;		//FPS(�).
const int WND_W		= 480;		//�E�B���h�E�̕�.
const int WND_H		= 640;		//�E�B���h�E�̍���.

const int C_SIZE = 64;//�L�����N�^�[�T�C�Y


static const int E_MAX = 5;//�G�@�̍ő吔
static const int PS_MAX = 32;	//Linked to CPlayer PS_MAX

//-----------------------------------------
//�ዓ�^.
//-----------------------------------------
//�L�����N�^�[��ԗዓ�^.
enum enCharaState {
	Ready = 0,
	Living,
	Dying,
	Dead,
};

//�V�[���ዓ�^
enum enScene
{
	Title,
	GameMain,
	GameOver,
	Ending,
};

//---------------------------------------------------------------
//�\����.
//---------------------------------------------------------------
typedef struct _GameWindow
{
	HWND	hWnd;		//�E�B���h�E�n���h��.
	HDC		hScreenDC;	//�o�b�N�o�b�t�@.
	SIZE	size;		//�T�C�Y.
	DWORD	dwFPS;	//FPS( Frame Per Second: �t���[�� / �b ).
	BOOL	isWndClose;//�E�B���h�E���Ă邩�m�F�p.
} GameWindow;

struct VEC2
{
	int x;
	int y;
};


//�L�����N�^�[�\����.
struct CHARA
{
	VEC2 position;	//���@�̍��W
	int state;		//���@�̏��.
	int ExpAnimCnt;	//���@�̔����A�j���[�V�����J�E���^
};
