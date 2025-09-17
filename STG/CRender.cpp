#include "CRender.h"
//�摜�̓ǂݍ���.
bool LoadBmp(HBITMAP* phBmp, const char* fileName)
{
	*phBmp = (HBITMAP)LoadImage(
		nullptr,			//�C���X�^���X.
		fileName,			//�t�@�C����(�p�X�܂�).
		IMAGE_BITMAP,		//�r�b�g�}�b�v.
		0, 0,				//�ǂݍ��ލ��W.
		LR_LOADFROMFILE);	//�t�@�C������ǂݍ���.

	if (*phBmp == nullptr) {
		MessageBox(nullptr,
			fileName, "�摜�ǂݍ��ݎ��s", MB_OK);
		return false;
	}
	return true;
}

//�摜�𓧉߂��ĕ\��.
bool TransBlt(HDC hdcDest, int xDest, int yDest,
	int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc)
{
	if (TransparentBlt(
		hdcDest,			//�\	����(���̓f�B�X�v���C).
		xDest, yDest,		//�\���ʒux,y���W.
		wDest, hDest,		//�\�����A����.
		hdcSrc,				//�摜���ݒ肳�ꂽ������DC.
		xSrc, ySrc,			//���摜�̎��o���J�nx,y���W.
		wDest, hDest,		//���摜�̎��o�����A����.
		RGB(0x00, 0xFF, 0x00))//���ߐF(�΂��w��).
		== FALSE)
	{
		return false;
	}
	return true;
}

bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha) {

	BLENDFUNCTION blendfunc = { AC_SRC_OVER, 0, (BYTE)Alpha, 0 };

	//�������ɂ���摜�̕\��
	if (AlphaBlend(
		hdcDest,			//�f�o�C�X�R���e�L�X�g
		xDest, yDest,						//�\���ʒu���A�����W
		wDest, hDest,					//�摜���A����
		hdcSrc,						//�������c�b
		xSrc, ySrc,						//���摜���A�����W
		wDest, hDest,					//�ǂݍ��݌��摜���A�����W
		blendfunc))				//BLENDFUNCTION
	{
		return true;
	}
	return false;
}