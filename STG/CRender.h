#pragma once
#include <Windows.h>

//‰æ‘œ‚Ì“Ç‚İ‚İ.
bool LoadBmp(HBITMAP* phBmp, const char* fileName);

//‰æ‘œ‚ğ“§‰ß‚µ‚Ä•\¦.
bool TransBlt(HDC hdcDest, int xDest, int yDest,
	int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc);

bool AlBlend(
	HDC hdcDest, int xDest, int yDest, int wDest, int hDest,
	HDC hdcSrc, int xSrc, int ySrc, int Alpha);