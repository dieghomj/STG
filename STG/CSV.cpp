#include "CSV.h"
#include <stdio.h> //�t�@�C�����o�͂Ŏg�p����
#include <crtdbg.h> //ASSERT�֐��Ŏg�p����
#include <string.h>	//strtok_s�֐��Ŏg�p����
#include <stdlib.h>

//�萔�錾.C++�ł̏�����
const int DATA_MAX = 10;	//�ǂݍ��ރf�[�^�̍s��
const int STR_MAX = 256;	//1�s�̍ő啶����(byte��)

//CSV�t�@�C���̓ǂݍ���
bool LoadCSV()
{

	char fileName[] = "Data\\CSV\\enemy_list.csv";	//�t�@�C�����i�p�X�܂ށj

	FILE* pf;	//�t�@�C���|�C���^
	errno_t error;	//�G���[�ԍ�

	error = fopen_s(&pf, fileName, "r");	//r:�ǂݏo����p
	if (error != 0) {
		_ASSERT_EXPR(false, L"CSV�t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	//�f�[�^
	ENEMY_CSV Data[DATA_MAX];

	char line[STR_MAX] = "";	//1�s�P�ʂœǂݍ��ޕ�����i�[�p
	char* pNext;				//��؂�ꂽ������
	char delim[] = ",";			//�f���~�^�F��؂蕶��
	char* ctx = nullptr;					//�����g�p

	//�f�[�^�̍ő吔����������
	for (int line_no = 0; line_no < DATA_MAX; line_no++)
	{
		//�t�@�C������1�s�����o���B�֐����Ăяo�����тɎ����̍s�ɐi��
		if (fgets(line, STR_MAX, pf) == nullptr) {
			//�f�[�^���Ȃ��Ȃ�΋����I��
			break;
		}
		//�����񂩂炵�Ă����ꂽ��؂蕶���܂ł̕�������擾
		//�֐����Ăяo�����тɎ��̋�؂�܂Ői��
		pNext = strtok_s(line, delim, &ctx);
		int no = 0; //���Ԗ�

		while (pNext) {
			//�Z�Ԗڂ��ƂɎ擾�����f�[�^�̊i�[���ς���
			switch (no)
			{
			case 0: Data[line_no].appearance	= atoi(pNext);	break;
			case 1: Data[line_no].x				= atoi(pNext);	break;
			case 2: Data[line_no].y				= atoi(pNext);	break;
			case 3: Data[line_no].moveType		= atoi(pNext);	break;
			case 4: Data[line_no].speed			= atoi(pNext);	break;
			}
			//���̋�؂�܂ł̕�������擾����
			//�s�ɑ΂���2��ڈȍ~�̊֐����Ăяo���Ȃ̂ő�������nullptr�ɂ���
			//��������line������ƈӐ}���������ɂȂ�Ȃ��̂Œ���
			pNext = strtok_s(nullptr, delim, &ctx);
	
			no++;
		}
	}

	//�t�@�C�������
	fclose(pf);

	return true;
}

//CSV�t�@�C���̓ǂݍ���(�ėp�������)
bool LoadCSV(
	const char* fileName,		//�t�@�C����
	ENEMY_CSV** pOutData,		//(out)	
	int DataMax					//�f�[�^�ő�
)
{

	FILE* pf;	//�t�@�C���|�C���^
	errno_t error;	//�G���[�ԍ�

	error = fopen_s(&pf, fileName, "r");	//r:�ǂݏo����p
	if (error != 0) {
		_ASSERT_EXPR(false, L"CSV�t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	//�f�[�^
	//DataMax�̐�����ENEMY_CSV�\���̂�p�ӂ���
	ENEMY_CSV* pData = new ENEMY_CSV[DataMax]();

	char line[STR_MAX] = "";	//1�s�P�ʂœǂݍ��ޕ�����i�[�p
	char* pNext;				//��؂�ꂽ������
	char delim[] = ",";			//�f���~�^�F��؂蕶��
	char* ctx = nullptr;					//�����g�p

	//�f�[�^�̍ő吔����������
	for (int line_no = 0; line_no < DATA_MAX; line_no++)
	{
		//�t�@�C������1�s�����o���B�֐����Ăяo�����тɎ����̍s�ɐi��
		if (fgets(line, STR_MAX, pf) == nullptr) {
			//�f�[�^���Ȃ��Ȃ�΋����I��
			break;
		}
		//�����񂩂炵�Ă����ꂽ��؂蕶���܂ł̕�������擾
		//�֐����Ăяo�����тɎ��̋�؂�܂Ői��
		pNext = strtok_s(line, delim, &ctx);
		int no = 0; //���Ԗ�

		while (pNext) {
			//�Z�Ԗڂ��ƂɎ擾�����f�[�^�̊i�[���ς���
			switch (no)
			{
			case 0: pData[line_no].appearance		= atoi(pNext);	break;
			case 1: pData[line_no].x				= atoi(pNext);	break;
			case 2: pData[line_no].y				= atoi(pNext);	break;
			case 3: pData[line_no].moveType			= atoi(pNext);	break;
			case 4: pData[line_no].speed			= atoi(pNext);	break;
			}
			//���̋�؂�܂ł̕�������擾����
			//�s�ɑ΂���2��ڈȍ~�̊֐����Ăяo���Ȃ̂ő�������nullptr�ɂ���
			//��������line������ƈӐ}���������ɂȂ�Ȃ��̂Œ���
			pNext = strtok_s(nullptr, delim, &ctx);
	
			no++;
		}
	}

	
	*pOutData = pData;

	//�t�@�C�������
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
