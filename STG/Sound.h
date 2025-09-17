#pragma once

#include <Windows.h>

//���C�u�����ǂݍ���
#pragma comment( lib, "winmm.lib")	//���Đ��ŕK�v.

//���t�@�C�����J��
bool sound_Open(const char* sFileName, const char* sAlia, HWND hWnd);

//���t�@�C�������
bool sound_Close(const char* sAlias, HWND hWnd);

//�����Đ�����(��Ԓʒm�̗L���t���O����)
bool sound_Play(const char* sAlias, HWND hWnd, bool bNotify = false);

//�����Đ�����(SE�Ŏg��)
bool sound_PlaySE(const char* sAlias, HWND hWnd);

bool sound_PlayLoop(const char* sAlias, HWND hWnd);

//�����~����.
bool sound_Stop(const char* sAlias, HWND hWnd);

//���̏�Ԃ��擾����.
bool sound_GetStatus(char* sStatus, const char* sAlias, HWND hWnd);

//���̒�~���m�F����.
bool sound_IsStopped(const char* sAlias, HWND hWnd);

//���̍Đ��ʒu���ŏ��ɂ���.
bool sound_SeekToStart(const char* sAlias, HWND hWnd);
