#pragma once
#include <Windows.h>

//�摜�̓ǂݍ���.
bool LoadBmp(HBITMAP* phBmp, const char* fileName);

//�摜�𓧉߂��ĕ\��.
bool TransBlt(HDC hdcDest, int xDest, int yDest,
	int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc);

bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha);