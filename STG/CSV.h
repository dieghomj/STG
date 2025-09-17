#pragma once

//CSV�f�[�^�\����
struct ENEMY_CSV
{
	int appearance;		//�o���ʒu
	int x;				//�����W
	int y;				//�����W
	int moveType;		//���
	int speed;			//���x
};

//CSV�t�@�C���̓ǂݍ���
bool LoadCSV();

//CSV�t�@�C���̓ǂݍ���(�ėp�������)
bool LoadCSV(
	const char* fileName,
	ENEMY_CSV** pOutData,
	int DataMax
);

void DeleteCSV(ENEMY_CSV** pInData);