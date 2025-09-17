#include "CGame.h"
#include <time.h>
#include <gdiplus.h>
#include "Sound.h"
#include "CSV.h"

CGame::CGame()
	: CGame( nullptr )
{
}

CGame::CGame(GameWindow* pGameWnd)
	: m_pGameWnd(pGameWnd)
	, m_pXInput(nullptr)
	, m_hMemDC(nullptr)
	, m_hFont(nullptr)
	, m_hBack(nullptr)
	, m_hChara(nullptr)
	, m_hExplosion(nullptr)
	, m_hTitle(nullptr)
	, m_hGameOver(nullptr)
	, m_hEnding(nullptr)
	, m_Alpha(0)
	, m_Scene(enScene::Title)
	, m_Score()
	, m_pPlayer(nullptr)
	, m_pPBullets(nullptr)
	, m_pPBulletsFlags(nullptr)
	, m_Enemy()
	, m_back_y()
	, m_BGMNo()
	, m_pBGM1(nullptr)
	, m_IsMuted( true )
{
}

CGame::~CGame()
{
}

//�������֐�.
void CGame::InitializeGame()
{

	//���@�����z�u
	CHARA player;
	player.position.x = (WND_W / 2) - (C_SIZE / 2);
	player.position.y = WND_H - C_SIZE - 16;	//�␳�F��ʂ̉�����L�����P�̕��{16��̈ʒu.
	player.state = enCharaState::Living;
	player.ExpAnimCnt = 0;
	
	m_pPlayer = new CPlayer(player);

	//���@�̒e�̏����z�u
	m_pPlayer->PlayerBulletsInit();

	m_pPBullets = m_pPlayer->m_ps;				//*************CHANGE THIS LATER*********************
	m_pPBulletsFlags = m_pPlayer->m_ps_shotFlag;//*************CHANGE THIS LATER*********************

	//�G�@�̏����z�u
	for (int i = 0; i < E_MAX; i++)
	{
		CHARA enemy;
		enemy.position.x = rand() % (WND_W - C_SIZE);	//�����_���o���ʒu��ݒ�
		enemy.position.y = -C_SIZE;
		enemy.state = enCharaState::Living; //������
		enemy.ExpAnimCnt = 0;
		m_Enemy[i] = new CEnemy(enemy);

	}

	//�w�i�X�N���[�������z�u
	m_back_y = WND_H;

	m_Score = 0;

}
//�\�z�֐�
bool CGame::Create()
{

	//�����̏�����
	srand((unsigned int)time(nullptr));

	m_pXInput = new CXInput(0);	//�R���g���[���[�N���X�̃C���X�^���X�𐶐�

	//������DC�̍쐬.
	m_hMemDC = CreateCompatibleDC(nullptr);

	AddFontResourceEx("Data\\Font\\BoldPixels.ttf", FR_PRIVATE, 0);

	//�t�H���g�̍쐬.
	m_hFont = CreateFont(
		60,	//��������.
		20,	//������.
		0,	//�p�x.
		0,	//�x�[�X���C���p�x.
		FW_REGULAR,	//����(����:FW_BOLD,�ʏ�:FW_REGULAR).
		FALSE,		//�Α�(TRUE:�L��).
		FALSE,		//����(TRUE:�L��).
		FALSE,		//�ł�������(TRUE:�L��).
		DEFAULT_CHARSET,	//�����Z�b�g.
		OUT_DEFAULT_PRECIS,	//�o�͐��x.
		CLIP_DEFAULT_PRECIS,//�N���b�s���O���x.
		DEFAULT_QUALITY,	//�o�͕i��.
		VARIABLE_PITCH | FF_ROMAN,	//�σs�b�`�ƃt�H���g�t�@�~��.
		"BoldPixels");		//����(nullptr:���ݎg���Ă��鏑��).


	//�w�i�̓ǂݍ���.
	if (LoadBmp(&m_hBack,		"Data\\BMP\\background.bmp")	== false)	return false;
	//�L�����N�^�[�̓ǂݍ���.
	if (LoadBmp(&m_hChara,		"Data\\BMP\\chara.bmp")			== false)	return false;
	//�����̓ǂݍ���
	if (LoadBmp(&m_hExplosion,	"Data\\BMP\\explosion.bmp")		== false)	return false;
	//�^�C�g���摜�ǂݍ���
	if (LoadBmp(&m_hTitle,		"Data\\BMP\\title.bmp")			== false)	return false;
	//	�Q�[���I�[�o�[�摜�ǂݍ���
	if (LoadBmp(&m_hGameOver,	"Data\\BMP\\gameover.bmp")		== false)	return false;

	if (LoadBmp(&m_hEnding,		"Data\\BMP\\ending.bmp")		== false)	return false;

	//�T�E���h�̃C���X�^���X����
	m_pBGM1 = nullptr;	//nullpter�ŏ�����
	m_pBGM1 = new CSound;	//�C���X�^���X����

	//MIDI�܂���mp3�t�@�C���̃I�[�v��.
	if(m_pBGM1->Open("Data\\Sound\\BGM\\title.mp3", "BGM1", m_pGameWnd->hWnd) == false ) return false;
	//if (sound_Open("Data\\Sound\\BGM\\title.mp3", "BGM1",		pGS->gameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\gamemain.mp3", "BGM2",	m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\gameover.mp3", "BGM3",	m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\BGM\\ending.mp3", "BGM4",		m_pGameWnd->hWnd) == false) return false;
	if (sound_Open("Data\\Sound\\SE\\explosion.mp3", "SE1",		m_pGameWnd->hWnd) == false) return false;


	//BGM�̍Đ�.
	m_pBGM1->PlayLoop();	//���[�v�Đ�.
	//sound_Play("BGM1", m_gameWnd->hWnd);

	// BGM�t���O��ݒ�.
	m_BGMNo = 1;

	//CSV�f�[�^�󂯎��p
	ENEMY_CSV* pEnemyCSV = nullptr;
	//�f�[�^�̍ő吔
	int EnemyCSV_Max = 10;


	if (LoadCSV(
		"Data\\CSV\\enemy_list.csv",
		&pEnemyCSV,
		EnemyCSV_Max ) == false)
	{
		return false;
	}
	//CSV�f�[�^�̍폜
	DeleteCSV(&pEnemyCSV);


	return true;
}
//�j���֐�
void CGame::Destroy()
{
	//mp3�̃N���[�Y.
	sound_Close("SE1",	m_pGameWnd->hWnd);
	sound_Close("BGM4", m_pGameWnd->hWnd);
	sound_Close("BGM3", m_pGameWnd->hWnd);
	sound_Close("BGM2", m_pGameWnd->hWnd);
	m_pBGM1->Close();	//BGM1�̃N���[�Y.
	//sound_Close("BGM1", m_gameWnd->hWnd);

	//BGM1�̉��.
	if( m_pBGM1 != nullptr) 	{
		delete m_pBGM1;
		m_pBGM1 = nullptr;
	}

	//BITMAP�̉��.
	DeleteObject(m_hEnding);
	DeleteObject(m_hGameOver);
	DeleteObject(m_hTitle);
	DeleteObject(m_hExplosion);
	DeleteObject(m_hChara);
	DeleteObject(m_hBack);


	//�t�H���g�̉��.
	DeleteObject(m_hFont);
	RemoveFontResourceEx(("Data\\Font\\BoldPixels.ttf"), FR_PRIVATE, 0);

	//������DC�̉��.
	DeleteDC(m_hMemDC);

	//�R���g���[���[�N���X�̔j��
	if (m_pXInput != nullptr)
	{
		delete m_pXInput;
		m_pXInput = nullptr;
	}
}
//�X�V�֐�
void CGame::Update()
{
	m_pXInput->Update();	//�R���g���[���̓��͏����X�V
	HandleInput();

	//BGM��������
	if (IsMuted()) {
		if (m_BGMNo != -1) {
			char buffer[5] = "BGM";
			buffer[3] = m_BGMNo + '0';
			buffer[4] = '\0';
			sound_Stop(buffer, m_pGameWnd->hWnd);
			sound_Close(buffer,m_pGameWnd->hWnd);
		}
		m_BGMNo = -1;
	}

	switch (m_BGMNo) {
	case -1:
		break;
	case 1:
		//�Ȃ̏�Ԃ��擾����.
		m_pBGM1->PlayLoop();
		//sound_PlayLoop("BGM1", m_pGameWnd->hWnd);
		break;
	case 2:
		//�Ȃ̏�Ԃ��擾����.
		sound_PlayLoop("BGM2", m_pGameWnd->hWnd);
		break;
	case 3:
		//�Ȃ̏�Ԃ��擾����.
		sound_PlayLoop("BGM3", m_pGameWnd->hWnd);
		break;
	case 4:
		//�Ȃ̏�Ԃ��擾����.
		sound_PlayLoop("BGM4", m_pGameWnd->hWnd);
		break;
	}

	//�V�[�����Ƃ̏���
	switch (m_Scene) {

	case enScene::Title:

		m_Alpha++;
		if (m_Alpha >= 255) {
			m_Alpha = 0;
		}

		if (IsKeyDown("Enter") ||
			m_pXInput->IsDown(CXInput::START))
		{
			m_Alpha = 0;

			m_Scene = enScene::GameMain;	//�Q�[�����C��

			//BGM�̕ύX.
			m_pBGM1->Stop();	//BGM1�̒�~.
			//sound_Stop("BGM1", m_pGameWnd->hWnd);	//BGM1�̒�~
			sound_PlayLoop("BGM2", m_pGameWnd->hWnd);	//BGM1�̒�~
			m_BGMNo = 2;

			//����������
			InitializeGame();
		}

		if (GetAsyncKeyState('P') & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
		{
			Mute();
		}

		break;
	
	case enScene::GameMain:
	{
		//Get cursor position
		if (GetCursorPos(&m_CursorPos))
		{
			ScreenToClient(m_pGameWnd->hWnd, &m_CursorPos);
			m_pPlayer->UpdatePosToCursor(m_CursorPos);
		}
		else
		{
			m_CursorPos.x = 0;
			m_CursorPos.y = 0;
		}

		//F1�L�[.
		if (IsKeyDown("F1")) {
			//�E�B���h�E�����ʒm�𑗂�.
			PostMessage(m_pGameWnd->hWnd, WM_CLOSE, 0, 0);
		}

		//��.
		if (IsKeyDown("Up"))
		{
			m_pPlayer->MoveUp();
		}
	
		//��.
		if (IsKeyDown("Down"))
		{
			m_pPlayer->MoveDown();

		}
		//��.
		if (IsKeyDown("Left"))
		{
			m_pPlayer->MoveLeft();
		}

		//��.
		if (IsKeyDown("Right"))
		{
			m_pPlayer->MoveRight();
		}

		//Z�L�[.
		if (IsKeyDown("Z"))
		{
			m_pPlayer->Shoot();
		}

		//Enter�L�[.
		if (IsKeyDown("Enter"))
		{
		}

		m_pPlayer->PlayerBulletsMove();

		//�G�@�̓���
		for (int i = 0; i < E_MAX; i++) {
			m_Enemy[i]->Move();
		}

		//���@�̐����m�F.
		if (m_pPlayer->GetPlayerState() == enCharaState::Living)
		{
			for (int i = 0; i < E_MAX; i++)
			{
				VEC2 playerPos = m_pPlayer->GetPlayerPos();
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				//�G�@���������Ă���
				if (m_Enemy[i]->GetState() == enCharaState::Living) {
					if (CollsionDetection(
						playerPos.x, playerPos.y, C_SIZE, C_SIZE,
						enemyPos.x, enemyPos.y, C_SIZE, C_SIZE))
					{
						//���@
						m_pPlayer->SetPlayerState(enCharaState::Dying);	//��Ԃ����S���ɐݒ�.
						m_pPlayer->SetPlayerAnimCnt(0);					//�����A�j���[�V�����J�E���^���O�ɐݒ�

						m_Enemy[i]->SetState(enCharaState::Dying);

						break;		//�G�@�Ɠ������for���𔲂���
					}
					else if (m_Enemy[i]->GetPos().y >= 600) {
						//�G�@����ʊO�ɏo����
						m_Enemy[i]->ResetEnemy();
						m_pPlayer->UpdateLife(-1);	//���C�t��1���炷
					}
				}

			}

		}
		//���@�̔�������
		else if (m_pPlayer->GetPlayerState() == enCharaState::Dying)
		{
			m_pPlayer->Destroy();

			if (m_pPlayer->GetPlayerAnimCnt() > 15) {
				//���S���Ă���̂ŃQ�[���I�[�o�[
				m_Scene = enScene::GameOver;

				//BGM�̕ύX.
				sound_Stop("BGM2", m_pGameWnd->hWnd);	//BGM1�̒�~
				m_pBGM1->PlayLoop();	//BGM1�̒�~
				m_BGMNo = 1;
			}
		}

		for (int eNo = 0; eNo < E_MAX; eNo++) {
			//�G�@�̐����m�F
			if (m_Enemy[eNo]->GetState() == enCharaState::Living) {
				//���@�̒e�����˂���Ă���E
				VEC2 enemyPos = m_Enemy[eNo]->GetPos();
				for (int psNo = 0; psNo < PS_MAX; psNo++) {
					if (m_pPBulletsFlags[psNo]) {
						//���@�̒e�ƓG�@�̓����蔻��
						if (CollsionDetection(
							m_pPBullets[psNo].x, m_pPBullets[psNo].y, C_SIZE, C_SIZE,
							enemyPos.x, enemyPos.y, C_SIZE, C_SIZE))
						{
							//����������
							m_Enemy[eNo]->SetState(enCharaState::Dying);

							//���@�̒e�̒��e��̏���
							m_pPBulletsFlags[psNo] = false;
							m_pPBullets[psNo].x = WND_W;
							m_pPBullets[psNo].y = WND_H;

							//�X�R�A���Z
							m_Score += 1;

							//��������炷
							//sound_PlaySE("SE1", m_pGameWnd->hWnd); *****************SET TO MUTE*********************

							break;
						}
					}
				}
			}

			//�G�@�̔�������
			else if (m_Enemy[eNo]->GetState() == enCharaState::Dying) {
				m_Enemy[eNo]->DestroyAnim();
			}
			else if (m_Enemy[eNo] != nullptr)
			{
				delete m_Enemy[eNo];
				m_Enemy[eNo] = nullptr;
			}
				

		}

		//�w�i�X�N���[������.
		if (m_back_y > -WND_H) {

			m_back_y--;	//�X�N���[�������炷.
		}
		else {
			m_back_y = WND_H;	//�����l�ɖ߂�.
		}

		if (m_Score >= 300)
		{
			m_Scene = enScene::Ending;
			//BGM�̕ύX.
			sound_Stop("BGM2", m_pGameWnd->hWnd);	//BGM1�̒�~
			sound_PlayLoop("BGM1", m_pGameWnd->hWnd);	//BGM1�̒�~
			m_BGMNo = 1;
		}
	}
	break;

	case enScene::GameOver:

		if (IsKeyDown("Enter"))
		{
			m_Scene = enScene::Title;	//�Q�[�����C��
		}

		break;
	case enScene::Ending:
		if (IsKeyDown("Enter"))
		{
			m_Scene = enScene::Title;	//�Q�[�����C��
		}
		break;

	default:		//��L�ȊO
		break;
	}
}

void CGame::Draw()
{
	
	//�V�[�����Ƃ̏���
	switch (m_Scene) {

	case enScene::Title:
		//�^�C�g���摜��\��.
		SelectObject(m_hMemDC, m_hTitle);
		BitBlt(m_pGameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
			0, 0,							//�\���ʒux,y���W.
			WND_W, WND_H,					//�摜���A����.
			m_hMemDC,						//������DC.
			0, 0,							//���摜x,y���W.
			SRCCOPY);						//�R�s�[����.

		//�^�C�g���摜��PressEnterKey�𔼓����ŕ\�������邽�߂�
		//�^�C�g���摜�̃s���N�����𔼓����ɂ��ĕ\��.
		AlBlend(
			m_pGameWnd->hScreenDC,			//�f�o�C�X�R���e�L�X�g
			0, 430,							//�\���ʒu���A�����W
			WND_W, 32,						//�摜���A����
			m_hMemDC,						//�������c�b
			0, 0,							//���摜���A�����W
			m_Alpha);						//�A���t�@�l

		break;
	case enScene::GameMain:
	{

		//�w�i�̉摜��������DC�փR�s�[.
		SelectObject(m_hMemDC, m_hBack);
		BitBlt(m_pGameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
			0, 0,							//�\���ʒux,y���W.
			WND_W, WND_H,					//�摜���A����.
			m_hMemDC,						//������DC.
			0, m_back_y,					//���摜x,y���W.
			SRCCOPY);						//�R�s�[����.

		

		//�w�i�̕\��(�T�u�X�N���[��)
		if (m_back_y < 0) {
			BitBlt(m_pGameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
				0, 0,							//�\���ʒux,y���W.
				WND_W, -(m_back_y),				//�摜���A����.
				m_hMemDC,						//������DC.
				0,								//���摜x���W.
				(WND_H * 2 + m_back_y),			//���摜y���W.
				SRCCOPY);						//�R�s�[����.
		}
		
		for (int i = 0; i < PS_MAX; i++)
		{
			if (m_pPBulletsFlags[i] == true)
			{
				//�L�����N�^�[�̉摜��������DC�փR�s�[
				SelectObject(m_hMemDC, m_hChara);
				//���@�̒e�̕\��
				TransBlt(m_pGameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g
					m_pPBullets[i].x, m_pPBullets[i].y,		//�\���ʒu���A�����W
					C_SIZE, C_SIZE,				//�摜���A����
					m_hMemDC,					//�������c�b
					0,							//���摜�����W
					C_SIZE * 1);				//���摜�����W
			}
		}
		
		m_pPlayer->Draw(m_pGameWnd->hScreenDC, m_hMemDC, m_hChara, m_hExplosion);

		for (int i = 0; i < E_MAX; i++) {
			if (m_Enemy[i] == nullptr)
				continue;
			//�G�@�������Ă���
			if (m_Enemy[i]->GetState() == enCharaState::Living) {
				//�L�����N�^�[�̉摜��������DC�փR�s�[
				SelectObject(m_hMemDC, m_hChara);
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				//�G�@�̕\��
				TransBlt(m_pGameWnd->hScreenDC,			//�f�o�C�X�R���e�L�X�g
					enemyPos.x, enemyPos.y,			//�\���ʒu���A�����W
					C_SIZE, C_SIZE,						//�摜���A����
					m_hMemDC,							//�������c�b
					C_SIZE * 1,
					0);									//���摜���A�����W
			}

			//�G�@�����S��
			else if (m_Enemy[i]->GetState() == enCharaState::Dying) {
				//�����̉摜��������DC�փR�s�[
				SelectObject(m_hMemDC, m_hExplosion);
				VEC2 enemyPos = m_Enemy[i]->GetPos();
				int enemyAnimCnt = m_Enemy[i]->GetAnimCnt();
				//�����̕\��
				TransBlt(m_pGameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g
					enemyPos.x, enemyPos.y,						//�\���ʒu���A�����W
					C_SIZE, C_SIZE,				//�摜���A����
					m_hMemDC,						//�������c�b
					(enemyAnimCnt % 8) * C_SIZE,		//���摜���A�����W:8�Ŋ������]����o��
					(enemyAnimCnt / 8) * C_SIZE);		//���摜���A�����W:8�Ŋ������l������
			}
			
		}
	
	//*************************************
	// Screen UI
	//*************************************
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = WND_W/2;
		rect.bottom = WND_H/2;

		//SCORE TEXT
		char scoreText[128] = "";
		wsprintf(scoreText, "SCORE:%d", m_Score);
		HFONT hOldFont = (HFONT)SelectObject(m_pGameWnd->hScreenDC, m_hFont);
		SetBkMode(m_pGameWnd->hScreenDC, TRANSPARENT); // No background fill
		DrawTextA(m_pGameWnd->hScreenDC, scoreText, -1, &rect, DT_CENTER | DT_SINGLELINE);

		//HP TEXT
		rect.right = WND_W;
		rect.right = WND_H;
		char lifeText[128] = "";
		wsprintf(lifeText, "HP:%d", m_pPlayer->UpdateLife(0));
		DrawTextA(m_pGameWnd->hScreenDC, lifeText, -1, &rect, DT_CENTER | DT_SINGLELINE);

		SelectObject(m_pGameWnd->hScreenDC, hOldFont);


	}

	break;
	case enScene::GameOver:
		//�Q�[���I�[�o�[�摜��\��.
		SelectObject(m_hMemDC, m_hGameOver);
		BitBlt(m_pGameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
			0, 0,			//�\���ʒux,y���W.
			WND_W, WND_H,	//�摜���A����.
			m_hMemDC,			//������DC.
			0, 0,			//���摜x,y���W.
			SRCCOPY);		//�R�s�[����.
	break;
	case enScene::Ending:
		//�G���f�B���O�摜��\��.
		SelectObject(m_hMemDC, m_hEnding);
		BitBlt(m_pGameWnd->hScreenDC,		//�f�o�C�X�R���e�L�X�g.
			0, 0,			//�\���ʒux,y���W.
			WND_W, WND_H,	//�摜���A����.
			m_hMemDC,			//������DC.
			0, 0,			//���摜x,y���W.
			SRCCOPY);		//�R�s�[����.
		break;

	default:		//��L�ȊO
		break;
	}
}

void CGame::HandleInput()
{
	//F1�L�[.
	if (GetAsyncKeyState(VK_F1) & 0x0001) {
		//�E�B���h�E�����ʒm�𑗂�.
		m_keyBoardInput["F1"] = true;
		PostMessage(m_pGameWnd->hWnd, WM_CLOSE, 0, 0);
	}
	else
		m_keyBoardInput["F1"] = false;

	//��.
	if (GetAsyncKeyState(VK_UP) & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Up"] = true;
		m_pPlayer->MoveUp();
	}
	else
		m_keyBoardInput["Up"] = false;
	//��.
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Down"] = true;
		m_pPlayer->MoveDown();

	}
	else
		m_keyBoardInput["Down"] = false;
	//��.
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Left"] = true;
		m_pPlayer->MoveLeft();
	}
	else
		m_keyBoardInput["Left"] = false;

	//��.
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Right"] = true;
		m_pPlayer->MoveRight();

	}
	else
		m_keyBoardInput["Right"] = false;

	//Z�L�[.
	if (GetAsyncKeyState('Z') & 0x8000)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Z"] = true;
		m_pPlayer->Shoot();
	}
	else
		m_keyBoardInput["Z"] = false;

	//Enter�L�[.
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)//0x0001[�x���A��], 0x8000[���A��].
	{
		m_keyBoardInput["Enter"] = true;
	}
	else
		m_keyBoardInput["Enter"] = false;
}

bool CGame::IsKeyDown(std::string id)
{
	if (m_keyBoardInput[id] == true) return true;

	return false;
}

//��`���m
bool CGame::CollsionDetection(
	int Ax, int Ay, int Aw, int Ah,	//��`A�̂��A�����W�ƕ�����.
	int Bx, int By, int Bw, int Bh)	//��`�a�̂��A�����W�ƕ�����.
{
	//���@�̒e�ƓG�@�̓����蔻��.
	if ((Ax <= Bx + C_SIZE) && (Bx <= Ax + C_SIZE) &&
		(Ay <= By + C_SIZE) && (By <= Ay + C_SIZE))
	{
		//���������Ƃ�.
		return true;
	}
	//�O�ꂽ��.
	return false;
}

