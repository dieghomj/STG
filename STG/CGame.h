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

#pragma comment( lib, "msimg32.lib" )	//�w��F�𓧉߂��ĕ`��Ŏg�p����.
										//�������̕`��ł��g�p����.

class CGame
{
public:
	//---------------------------------------
	//	�萔�錾.
	//---------------------------------------
	

	//-----------------------------------------
	// �ዓ�^�錾
	//-----------------------------------------
	
	//�V�[���񋓌^.


	//---------------------------------------
	//	�\���̐錾.
	//---------------------------------------
	

public:
	CGame();	// Constructor
	CGame(GameWindow* pGameWnd);
	~CGame();	// Destructor
	//�������֐�.
	void InitializeGame();
	//�\�z�֐�
	bool Create();
	//�j���֐�
	void Destroy();
	//�X�V�֐�
	void Update();
	//�`��֐�
	void Draw();
	//�E�B���h�E����Ă��邩�m�F.
	BOOL IsWindowClosed() const { return m_pGameWnd->isWndClose; }
	//FPS���擾.
	DWORD GetFPS() const { return m_pGameWnd->dwFPS; }
	//�o�b�N�o�b�t�@���擾.
	HDC GetScreenDC() const { return m_pGameWnd->hScreenDC; }
	//�E�B���h�E�n���h�����擾.
	HWND GetWnd() const { return m_pGameWnd->hWnd; }
	//BGM�������m�F
	bool IsMuted() const { return m_IsMuted; }
	//BGM��������
	void Mute() { m_IsMuted = true; }


private:
	void HandleInput();
	bool IsKeyDown(std::string id);

private:
	GameWindow* m_pGameWnd;		//�Q�[���E�B���h�E�\����.
	CXInput*	m_pXInput;		//�R���g���[���[�N���X���|�C���^�ŗp��.
	HDC			m_hMemDC;		//�������f�o�C�X�R���e�L�X�g.
	HFONT		m_hFont;		//�t�H���g�n���h��.
	HBITMAP		m_hBack;		//�w�i.
	HBITMAP		m_hChara;		//�L�����N�^�[.
	HBITMAP		m_hExplosion;	//����
	HBITMAP		m_hTitle;		//�^�C�g���摜
	HBITMAP		m_hGameOver;	//�Q�[���I�[�o�[�摜
	HBITMAP		m_hEnding;		//�G���f�B���O�摜
	//�A���t�@�l.
	int			m_Alpha;		//�^�C�g����PressEnterKey�Ŏg��
	//�V�[��.
	enScene		m_Scene;
	//�X�R�A
	int			m_Score;
	
	//---------���@------------
	CPlayer*	m_pPlayer;
	VEC2*		m_pPBullets;
	bool*		m_pPBulletsFlags;
	//---------�G�@------------
	CEnemy*		m_Enemy[E_MAX];
	//�w�i�X�N���[��.
	int			m_back_y = 0;	//y���W
	//---------�T�E���h------------
	int			m_BGMNo;		//BGM�ԍ�.
	CSound*		m_pBGM1;		//BGM�N���X.
	bool		m_IsMuted;		//BGM����
	//---------����------------
	std::map<std::string, bool> m_keyBoardInput;
	POINT 		m_CursorPos;

	bool CollsionDetection(
		int Ax, int Ay, int Aw, int Ah,	//��`A�̂��A�����W�ƕ�����.
		int Bx, int By, int Bw, int Bh);	//��`�a�̂��A�����W�ƕ�����


};

