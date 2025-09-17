#include "CSV.h"
#include <stdio.h> //ファイル入出力で使用する
#include <crtdbg.h> //ASSERT関数で使用する
#include <string.h>	//strtok_s関数で使用する
#include <stdlib.h>

//定数宣言.C++での書き方
const int DATA_MAX = 10;	//読み込むデータの行数
const int STR_MAX = 256;	//1行の最大文字数(byte数)

//CSVファイルの読み込み
bool LoadCSV()
{

	char fileName[] = "Data\\CSV\\enemy_list.csv";	//ファイル名（パス含む）

	FILE* pf;	//ファイルポインタ
	errno_t error;	//エラー番号

	error = fopen_s(&pf, fileName, "r");	//r:読み出し専用
	if (error != 0) {
		_ASSERT_EXPR(false, L"CSVファイル読み込み失敗");
		return false;
	}

	//データ
	ENEMY_CSV Data[DATA_MAX];

	char line[STR_MAX] = "";	//1行単位で読み込む文字列格納用
	char* pNext;				//区切られた文字列
	char delim[] = ",";			//デリミタ：区切り文字
	char* ctx = nullptr;					//内部使用

	//データの最大数分処理する
	for (int line_no = 0; line_no < DATA_MAX; line_no++)
	{
		//ファイルから1行を取り出す。関数を呼び出すたびに次ぎの行に進む
		if (fgets(line, STR_MAX, pf) == nullptr) {
			//データがなくなれば強制終了
			break;
		}
		//文字列からしていされた区切り文字までの文字列を取得
		//関数を呼び出すたびに次の区切りまで進む
		pNext = strtok_s(line, delim, &ctx);
		int no = 0; //○番目

		while (pNext) {
			//〇番目ごとに取得したデータの格納先を変える
			switch (no)
			{
			case 0: Data[line_no].appearance	= atoi(pNext);	break;
			case 1: Data[line_no].x				= atoi(pNext);	break;
			case 2: Data[line_no].y				= atoi(pNext);	break;
			case 3: Data[line_no].moveType		= atoi(pNext);	break;
			case 4: Data[line_no].speed			= atoi(pNext);	break;
			}
			//つぎの区切りまでの文字列を取得する
			//行に対して2回目以降の関数を呼び出しなので第一引数をnullptrにする
			//第一引数にlineを入れると意図した動きにならないので注意
			pNext = strtok_s(nullptr, delim, &ctx);
	
			no++;
		}
	}

	//ファイルを閉じる
	fclose(pf);

	return true;
}

//CSVファイルの読み込み(汎用性向上版)
bool LoadCSV(
	const char* fileName,		//ファイル名
	ENEMY_CSV** pOutData,		//(out)	
	int DataMax					//データ最大
)
{

	FILE* pf;	//ファイルポインタ
	errno_t error;	//エラー番号

	error = fopen_s(&pf, fileName, "r");	//r:読み出し専用
	if (error != 0) {
		_ASSERT_EXPR(false, L"CSVファイル読み込み失敗");
		return false;
	}

	//データ
	//DataMaxの数だけENEMY_CSV構造体を用意する
	ENEMY_CSV* pData = new ENEMY_CSV[DataMax]();

	char line[STR_MAX] = "";	//1行単位で読み込む文字列格納用
	char* pNext;				//区切られた文字列
	char delim[] = ",";			//デリミタ：区切り文字
	char* ctx = nullptr;					//内部使用

	//データの最大数分処理する
	for (int line_no = 0; line_no < DATA_MAX; line_no++)
	{
		//ファイルから1行を取り出す。関数を呼び出すたびに次ぎの行に進む
		if (fgets(line, STR_MAX, pf) == nullptr) {
			//データがなくなれば強制終了
			break;
		}
		//文字列からしていされた区切り文字までの文字列を取得
		//関数を呼び出すたびに次の区切りまで進む
		pNext = strtok_s(line, delim, &ctx);
		int no = 0; //○番目

		while (pNext) {
			//〇番目ごとに取得したデータの格納先を変える
			switch (no)
			{
			case 0: pData[line_no].appearance		= atoi(pNext);	break;
			case 1: pData[line_no].x				= atoi(pNext);	break;
			case 2: pData[line_no].y				= atoi(pNext);	break;
			case 3: pData[line_no].moveType			= atoi(pNext);	break;
			case 4: pData[line_no].speed			= atoi(pNext);	break;
			}
			//つぎの区切りまでの文字列を取得する
			//行に対して2回目以降の関数を呼び出しなので第一引数をnullptrにする
			//第一引数にlineを入れると意図した動きにならないので注意
			pNext = strtok_s(nullptr, delim, &ctx);
	
			no++;
		}
	}

	
	*pOutData = pData;

	//ファイルを閉じる
	fclose(pf);

	return true;
}


void DeleteCSV(ENEMY_CSV** pInData)
{
	if (*pInData != nullptr) {
		delete[] * pInData;
		*pInData = nullptr;
	}
}
