#include "CRender.h"
//画像の読み込み.
bool LoadBmp(HBITMAP* phBmp, const char* fileName)
{
	*phBmp = (HBITMAP)LoadImage(
		nullptr,			//インスタンス.
		fileName,			//ファイル名(パス含む).
		IMAGE_BITMAP,		//ビットマップ.
		0, 0,				//読み込む座標.
		LR_LOADFROMFILE);	//ファイルから読み込む.

	if (*phBmp == nullptr) {
		MessageBox(nullptr,
			fileName, "画像読み込み失敗", MB_OK);
		return false;
	}
	return true;
}

//画像を透過して表示.
bool TransBlt(HDC hdcDest, int xDest, int yDest,
	int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc)
{
	if (TransparentBlt(
		hdcDest,			//表	示先(実はディスプレイ).
		xDest, yDest,		//表示位置x,y座標.
		wDest, hDest,		//表示幅、高さ.
		hdcSrc,				//画像が設定されたメモリDC.
		xSrc, ySrc,			//元画像の取り出し開始x,y座標.
		wDest, hDest,		//元画像の取り出し幅、高さ.
		RGB(0x00, 0xFF, 0x00))//透過色(緑を指定).
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

	//半透明にする画像の表示
	if (AlphaBlend(
		hdcDest,			//デバイスコンテキスト
		xDest, yDest,						//表示位置ｘ、ｙ座標
		wDest, hDest,					//画像幅、高さ
		hdcSrc,						//メモリＤＣ
		xSrc, ySrc,						//元画像ｘ、ｙ座標
		wDest, hDest,					//読み込み元画像ｘ、ｙ座標
		blendfunc))				//BLENDFUNCTION
	{
		return true;
	}
	return false;
}