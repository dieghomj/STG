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

//�G�@�̓���
void CEnemy::Move()
{
	
	if (m_Enemy.state != enCharaState::Living) 
		return;

	m_Enemy.position.y += E_SPD;	//���ֈړ�

	//��ʂ̊O�֏o��
	if (m_Enemy.position.y > WND_H) {
		m_Enemy.position.y = -C_SIZE;
	}
}

void CEnemy::SetState(enCharaState state)
{
	//�G�@���������̂Ƃ��ɌĂяo�����
	if (state == enCharaState::Dying)
	{
		m_Enemy.state = state;			//��Ԃ����S���ɐݒ�
		m_Enemy.ExpAnimCnt = 0;						//�����A�j���[�V�����J�E���^���O�ɐݒ�
	}
	else m_Enemy.state = state;

}

void CEnemy::DestroyAnim()
{
	m_Enemy.ExpAnimCnt++;
	if (m_Enemy.ExpAnimCnt > 15)	//�����摜��0~15��16�R�}.
	{
		m_Enemy.state = enCharaState::Living;	//��Ԑ�����.

		ResetEnemy();
	}
}

void CEnemy::ResetEnemy()
{
	//�G�@�̏o���ʒu��ύX
	m_Enemy.state = enCharaState::Living;	//��Ԑ�����.
	m_Enemy.position.x = rand() % (WND_W - C_SIZE); //�����_���ŏo���ʒu��ݒ� ( 0 ~ 416);
	m_Enemy.position.y = -C_SIZE;
}
