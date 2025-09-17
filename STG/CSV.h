#pragma once

//CSVデータ構造体
struct ENEMY_CSV
{
	int appearance;		//出現位置
	int x;				//ｘ座標
	int y;				//ｙ座標
	int moveType;		//種類
	int speed;			//速度
};

//CSVファイルの読み込み
bool LoadCSV();

//CSVファイルの読み込み(汎用性向上版)
bool LoadCSV(
	const char* fileName,
	ENEMY_CSV** pOutData,
	int DataMax
);

void DeleteCSV(ENEMY_CSV** pInData);