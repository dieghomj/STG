#include "CEnemy.h"

CEnemy::CEnemy()
	:CEnemy(CHARA{})
{
}

CEnemy::CEnemy(CHARA enemy)
	: m_Enemy(enemy)
{
}

CEnemy::~CEnemy()
{
}

//敵機の動作
void CEnemy::Move()
{
	
	if (m_Enemy.state != enCharaState::Living) 
		return;

	m_Enemy.position.y += E_SPD;	//下へ移動

	//画面の外へ出た
	if (m_Enemy.position.y > WND_H) {
		m_Enemy.position.y = -C_SIZE;
	}
}

void CEnemy::SetState(enCharaState state)
{
	//敵機が生存中のときに呼び出される
	if (state == enCharaState::Dying)
	{
		m_Enemy.state = state;			//状態を死亡中に設定
		m_Enemy.ExpAnimCnt = 0;						//爆発アニメーションカウンタを０に設定
	}
	else m_Enemy.state = state;

}

void CEnemy::DestroyAnim()
{
	m_Enemy.ExpAnimCnt++;
	if (m_Enemy.ExpAnimCnt > 15)	//爆発画像が0~15の16コマ.
	{
		m_Enemy.state = enCharaState::Living;	//状態生存中.

		ResetEnemy();
	}
}

void CEnemy::ResetEnemy()
{
	//敵機の出現位置を変更
	m_Enemy.state = enCharaState::Living;	//状態生存中.
	m_Enemy.position.x = rand() % (WND_W - C_SIZE); //ランダムで出現位置を設定 ( 0 ~ 416);
	m_Enemy.position.y = -C_SIZE;
}
