#pragma once
#include "Global.h"
//�G�@�̃N���X
class CEnemy
{
public:
	
	static const int E_SPD = 7;//�G�@�̈ړ����x
	static const int E_ELITE_HP = 8;

public:
	CEnemy();
	CEnemy(CHARA enemy);
	CEnemy(CHARA enemy, bool isElite);
	~CEnemy();

	void Move();
	void SetDying();
	void DestroyAnim();
	void Shoot();

	VEC2 GetPos() const { return m_Enemy.position; };
	int GetState() const { return m_Enemy.state; };
	int GetAnimCnt() const { return m_Enemy.ExpAnimCnt; };

private :
	CHARA		m_Enemy;
};

