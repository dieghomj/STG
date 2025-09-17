#include "CSound.h"
#include <stdio.h>	//sprintf_s�֐����g�p����̂ɕK�v

CSound::CSound()
	//	���������X�g
	: m_hWnd	( nullptr)
	, m_sAlias	()
{
}

CSound::~CSound()
{
}

//===============================================================
//	���t�@�C�����J��
//===============================================================
bool CSound:: Open(const char* sFileName, const char* sAlias, HWND hWnd) {

	//�������ݒ�
	SetInitParam(sAlias, hWnd);

	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "open %s alias %s", sFileName, m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//===============================================================
//	���t�@�C�������
//===============================================================
bool CSound::Close() {
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "close %s", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//�����Đ�����
//===============================================================
bool CSound::Play(bool bNotifiy)
{
	char cmd[STR_BUFFER_MAX] = "";

	if (bNotifiy == true) {
		sprintf_s(cmd, "play %s notify", m_sAlias);	//notify:�@���̏�Ԏ擾�ŕK�v�@
	}
	else
	{
		sprintf_s(cmd, "play %s", m_sAlias);
	}


	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

bool CSound::PlaySE()
{
	SeekToStart();
	if (Play() == true) {
		return true;
	}
	return false;
}

bool CSound::PlayLoop()
{
	if (IsStopped() == true)
	{
		SeekToStart();
		if (Play(true) == true) {
			return true;
		}
	}
	return false;
}

//===============================================================
//�����~����.
//===============================================================
bool CSound::Stop()
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "stop %s", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//���̏�Ԃ��擾����.
//	sStatus �̔z�񐔂�256�ȉ��ɂ��邱��
//	���������A��Ԃ�����ꍇ�́A�Đ����Ɂunotifiy�v��ݒ肵�A
//	�@�E�B���h�E�n���h���Ƀ��b�Z�[�W�𑗂��Ă����K�v������.
//===============================================================
bool CSound::GetStatus(char* sStatus)
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "status %s mode", m_sAlias);

	if (mciSendString(cmd, sStatus, STR_BUFFER_MAX, m_hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//	���̒�~���m�F����.
//===============================================================
bool CSound:: IsStopped()
{
	char sSTatus[STR_BUFFER_MAX] = "";

	//��Ԃ̎擾
	if (GetStatus(sSTatus) == true) {
		//�������r.
		if (strcmp(sSTatus, "stopped") == 0) {
			return true;
		}
	}
	return false;
}
//===============================================================
//	���̍Đ��ʒu���ŏ��ɂ���.
//===============================================================
bool CSound::SeekToStart()
{
	char cmd[STR_BUFFER_MAX] = "";

	sprintf_s(cmd, "seek %s to start", m_sAlias);

	if (mciSendString(cmd, nullptr, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//�������l��ݒ肷��֐�
void CSound::SetInitParam(const char* sAlias, HWND hWnd)
{
	m_hWnd = hWnd;

	strcpy_s(m_sAlias, sizeof(m_sAlias), sAlias);
}
