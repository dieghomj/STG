#include "CPlayer.h"
#include "CRender.h"
#include "math.h"

CPlayer::CPlayer()
	:CPlayer( CHARA{} )
{
}

CPlayer::CPlayer(CHARA player)
	: m_Player(player)
	, m_ps()
	, m_ps_shotFlag()
	, m_Sight({0,0})
	, m_playerSprite ( {0,0} )
	, m_rotationAngle (0.0f)
	, m_Life	(10)
{ 

}

CPlayer::~CPlayer()
{
}

void CPlayer::Shoot()
{
	for (int i = 0; i < PS_MAX; i++)
	{
		//�e�����˂���Ă��Ȃ�
		if (m_ps_shotFlag[i] == false)
		{
			//���@�̈ʒu�ɒe�𑵂���
			m_ps[i].x = m_Player.position.x;
			m_ps[i].y = m_Player.position.y;
			m_ps_shotFlag[i] = true;		//���˃t���O�𗧂Ă�.

			break;
		}
	}
}

void CPlayer::MoveUp()
{
	m_Player.position.y -= P_SPD;

}

void CPlayer::MoveLeft()
{
	m_Player.position.x -= P_SPD;
}

void CPlayer::MoveRight()
{
	m_Player.position.x += P_SPD;
}

void CPlayer::MoveDown()
{
	m_Player.position.y += P_SPD;
}

void CPlayer::PlayerBulletsInit()
{
	//���@�̒e�̏����z�u
	for (int i = 0; i < PS_MAX; i++)	//���@�̒e�̐�������������
	{
		m_ps[i].x = WND_W;
		m_ps[i].y = WND_H;
		m_ps_shotFlag[i] = false;
	}
}

void CPlayer::PlayerBulletsMove()
{
	//���@�̒e�̓���.
	for (int i = 0; i < PS_MAX; i++)
	{
		//���@�̒e�����˒��Ȃ�.
		if (m_ps_shotFlag[i] == true)
		{
			m_ps[i].y -= PS_SPD;	//�e����ֈړ�

			//��ʂ̊O�֏o��
			if (m_ps[i].y < -C_SIZE) {
				m_ps[i].x = WND_W;
				m_ps[i].y = WND_H;
				m_ps_shotFlag[i] = false;		//���˃t���O�~�낷
			}
		}
	}
}

void CPlayer::Destroy()
{
	m_Player.ExpAnimCnt++;

	if (m_Player.ExpAnimCnt > 15)//�����摜��0~15�̂P�U�R�}.
	{
		m_Player.state = enCharaState::Living;		//��Ԃ𐶑���
		//�����ʒu�ɖ߂�
		m_Player.position.x = (WND_W / 2) - (C_SIZE / 2);
		m_Player.position.y = WND_H - (C_SIZE + 16);
	}
}

void CPlayer::Draw(HDC hdc, HDC hMemDc, HBITMAP hChara, HBITMAP hExplosion) {
	
	VEC2 playerPos = GetPlayerPos();
	VEC2 playerSprite = GetPlayerSpritePos();
	int playerAnimCnt = GetPlayerAnimCnt();

	//���@��������
	if (GetPlayerState() == enCharaState::Living) {
		//�L�����N�^�[�̉摜��������DC�փR�s�[
		SelectObject(hMemDc, hChara);
		//���@�̕\��
		TransBlt(hdc,	//�f�o�C�X�R���e�L�X�g
			playerPos.x, playerPos.y,						//�\���ʒu���A�����W
			C_SIZE, C_SIZE,									//�摜���A����
			hMemDc,											//�������c�b
			playerSprite.x, playerSprite.y);				//���摜���A�����W
		

	// *******************************
	//Drawing Aimsight
	// *******************************
		//int radius = 5;
		//int left	= m_Sight.x - radius;
		//int top		= m_Sight.y - radius;
		//int right	= m_Sight.x + radius;
		//int bottom	= m_Sight.y + radius;

		//HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		//HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		//// Draw the circle
		//Ellipse(hdc, left, top, right, bottom);
		//// Clean up
		//SelectObject(hdc, hOldBrush);
		//DeleteObject(hBrush);

	}

	//���@�����S��
	else if (GetPlayerState() == enCharaState::Dying) {
		//�����̉摜��������DC�փR�s�[
		SelectObject(hMemDc, hExplosion);
		//�����̕\��
		TransBlt(hdc,										//�f�o�C�X�R���e�L�X�g
			playerPos.x, playerPos.y,						//�\���ʒu���A�����W
			C_SIZE, C_SIZE,									//�摜���A����
			hMemDc,											//�������c�b
			(playerAnimCnt % 8) * C_SIZE,					//���摜���A�����W:8�Ŋ������]����o��
			(playerAnimCnt / 8) * C_SIZE);					//���摜���A�����W:8�Ŋ������l������
	}


}

void CPlayer::UpdatePosToCursor(POINT cursor)
{
	m_Player.position.x = cursor.x - (C_SIZE / 2);
	m_Player.position.y = cursor.y - (C_SIZE / 2);

	VEC2 center = Normalize(m_Player.position);
	VEC2 target = Normalize({ cursor.x,cursor.y });

	/*m_Sight.x = m_Player.position.x + C_SIZE * 0.5;
	m_Sight.y = m_Player.position.y + C_SIZE * 0.7;*/

	//int radius = sqrt((m_Sight.x - m_Player.position.x) * (m_Sight.x - m_Player.position.x) + (m_Sight.y - m_Player.position.y) * (m_Sight.y - m_Player.position.y));
	//
	//m_Sight.x = cx + v.x * radius;
	//m_Sight.y = cy + v.y * radius;
}

int CPlayer::UpdateLife(int dmg)
{
	m_Life += dmg;
	return m_Life;
}

VEC2 Normalize(VEC2 v) {
	int lnt = sqrt(v.x * v.x + v.y * v.y);
	v.x = v.x / lnt;
	v.y = v.y / lnt;

	return v;
}






















////���@��������
//if (m_pPlayer->GetPlayerState() == enCharaState::Living) {
//	VEC2 playerPos = m_pPlayer->GetPlayerPos();
//	VEC2 playerSprite = m_pPlayer->GetPlayerSpritePos();
//	//�L�����N�^�[�̉摜��������DC�փR�s�[
//	SelectObject(m_hMemDC, m_hChara);
//	//���@�̕\��
//	TransBlt(m_pGameWnd->hScreenDC,	//�f�o�C�X�R���e�L�X�g
//		playerPos.x, playerPos.y,		//�\���ʒu���A�����W
//		C_SIZE, C_SIZE,				//�摜���A����
//		m_hMemDC,					//�������c�b
//		playerSprite.x, playerSprite.y);						//���摜���A�����W
//}
//
////���@�����S��
//else if (m_pPlayer->GetPlayerState() == enCharaState::Dying) {
//	VEC2 playerPos = m_pPlayer->GetPlayerPos();
//	int playerAnimCnt = m_pPlayer->GetPlayerAnimCnt();
//	//�����̉摜��������DC�փR�s�[
//	SelectObject(m_hMemDC, m_hExplosion);
//	//�����̕\��
//	TransBlt(m_pGameWnd->hScreenDC,					//�f�o�C�X�R���e�L�X�g
//		playerPos.x, playerPos.y,						//�\���ʒu���A�����W
//		C_SIZE, C_SIZE,								//�摜���A����
//		m_hMemDC,									//�������c�b
//		(playerAnimCnt % 8) * C_SIZE,			//���摜���A�����W:8�Ŋ������]����o��
//		(playerAnimCnt / 8) * C_SIZE);		//���摜���A�����W:8�Ŋ������l������
//}