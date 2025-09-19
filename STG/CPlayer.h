#pragma once
#include "Global.h"


class CPlayer {

public:

	CPlayer();
	CPlayer(CHARA player);
	~CPlayer();

	int GetPlayerState() const { return m_Player.state; }
	VEC2<int> GetPlayerPos() const { return m_Player.position; }
	int GetPlayerAnimCnt() const { return m_Player.ExpAnimCnt; }
	int GetPlayerPSMax() const { return PS_MAX; }
	VEC2<int> GetPlayerSpritePos() const { return m_playerSprite; }
	float GetRotationAngle() const { return m_rotationAngle; }
	int GetPlayerLife() const { return m_Life; }

	void SetPlayerState(enCharaState state) { m_Player.state = state; }
	void SetPlayerAnimCnt(int cnt) { m_Player.ExpAnimCnt = cnt; }

	void UpdatePosToCursor(POINT cursor);
	int UpdateLife(int dmg);

	void Shoot();
	void MoveUp();
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void Destroy();
	void Draw(HDC hdc, HDC hMemDc, HBITMAP hChara, HBITMAP hExplosion);

	//�e�̊֐�
	void PlayerBulletsInit();
	void PlayerBulletsMove();

private:

	static const int P_SPD = 3;//�v���C���[�ړ����x
	static const int PS_SPD = 10;//���@�̒e�̑��x
	static const int PS_MAX = 32;//���@�̒e�̍ő吔

private:

	//---------���@------------
	CHARA		m_Player;
	VEC2<int>		m_playerSprite;
	VEC2<int>		m_playerAnimSprite;
	float 		m_rotationAngle;
	int			m_Life;

	VEC2<int>		m_Sight;

public:
	//---------���@�e------------
	VEC2<int>		m_ps[PS_MAX];
	bool		m_ps_shotFlag[PS_MAX];	//���˃t���O.
};

VEC2<int> Normalize(VEC2<int> v);