#pragma once

#include <Windows.h>

//���C�u�����ǂݍ���
#pragma comment( lib, "winmm.lib")	//���Đ��ŕK�v.


//***********************************************//
//	�T�E���h�N���X
//		midi,mp3,wav�`���t�@�C���̍Đ��E��~�Ȃǂ��s��
//
//***********************************************//
class CSound
{

public:

	//�萔�錾
	static const int STR_BUFFER_MAX = 256;	

	CSound();
	
	~CSound();

	//���t�@�C�����J��
	bool Open(const char* sFileName, const char* sAlias, HWND hWnd);

	//���t�@�C�������
	bool Close();

	//�����Đ�����(��Ԓʒm�̗L���t���O����)
	bool Play(bool bNotify = false);

	//�����Đ�����(SE�Ŏg��)
	bool PlaySE();

	bool PlayLoop();

	//�����~����.
	bool Stop();

	//���̏�Ԃ��擾����.
	bool GetStatus(char* sStatus );

	//���̒�~���m�F����.
	bool IsStopped();

	//���̍Đ��ʒu���ŏ��ɂ���.
	bool SeekToStart();

private:

	//�������l��ݒ肷��֐�
	void SetInitParam(const char* sAlias, HWND hWnd);

private:

	HWND	m_hWnd;
	char	m_sAlias[STR_BUFFER_MAX];

};

