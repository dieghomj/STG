#include "Sound.h"
#include <stdio.h>	//sprintf_s�֐����g�p����̂ɕK�v.

//===============================================================
//	���t�@�C�����J��
//===============================================================
bool sound_Open(const char* sFileName, const char* sAlias, HWND hWnd) {

	char cmd[256] = "";

	sprintf_s(cmd, "open %s alias %s", sFileName, sAlias);

	if ( mciSendString(cmd, nullptr, 0, hWnd) == 0 ) {
		return true;
	}
	return false;
}

//===============================================================
//	���t�@�C�������
//===============================================================
bool sound_Close(const char* sAlias, HWND hWnd) {
	char cmd[256] = "";

	sprintf_s(cmd, "close %s", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//�����Đ�����
//===============================================================
bool sound_Play(const char* sAlias, HWND hWnd, bool bNotifiy)
{
	char cmd[256] = "";

	if (bNotifiy == true) {
		sprintf_s(cmd, "play %s notify", sAlias);	//notify:�@���̏�Ԏ擾�ŕK�v�@
	}
	else
	{
		sprintf_s(cmd, "play %s", sAlias);
	}


	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}

bool sound_PlaySE(const char* sAlias, HWND hWnd)
{
	sound_SeekToStart(sAlias, hWnd);
	if (sound_Play(sAlias, hWnd) == true) {
		return true;
	}
	return false;
}

bool sound_PlayLoop(const char* sAlias, HWND hWnd)
{
	if (sound_IsStopped(sAlias, hWnd) == true)
	{
		sound_SeekToStart(sAlias, hWnd);
		if (sound_Play(sAlias, hWnd, true) == true) {
			return true;
		}
	}
	return false;
}

//===============================================================
//�����~����.
//===============================================================
bool sound_Stop(const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "stop %s", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
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
bool sound_GetStatus(char* sStatus, const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "status %s mode", sAlias);

	if (mciSendString(cmd, sStatus, 256, hWnd) == 0) {
		return true;
	}
	return false;
}
//===============================================================
//	���̒�~���m�F����.
//===============================================================
bool sound_IsStopped(const char* sAlias, HWND hWnd)
{
	char sSTatus[256] = "";

	//��Ԃ̎擾
	if (sound_GetStatus(sSTatus, sAlias, hWnd) == true) {
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
bool sound_SeekToStart(const char* sAlias, HWND hWnd)
{
	char cmd[256] = "";

	sprintf_s(cmd, "seek %s to start", sAlias);

	if (mciSendString(cmd, nullptr, 0, hWnd) == 0) {
		return true;
	}
	return false;
}
