#include "stdafx.h"
#include "GameData.h"


float GameData::tamax = 0;
float GameData::tamay = 0;

bool GameData::shot = false;

bool GameData::StartIndexEnd = false;//�����Œ萶���p
bool GameData::ProteinFlag = false;//�v���e�C���o�[�X�g�����ǂ���

float GameData::TotalScoreResult = 100000;//�Q�[�����̃g�[�^���X�R�A0

int  GameData::GameEndTime = 20;//�{�҃v���C��������2700
int  GameData::GamePlayTime = 0;//�{�҃v���C���Ԍv��
int  GameData::GameHoldNum = 0;//�T���^�̌��ݏ����A�C�e����

int GameData::CLEAR_LINE = 50000;//�Q�[���N���A���C��
