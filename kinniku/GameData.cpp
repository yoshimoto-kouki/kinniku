#include "stdafx.h"
#include "GameData.h"


float GameData::tamax = 0;
float GameData::tamay = 0;

bool GameData::shot = false;

bool GameData::StartIndexEnd = false;//初期固定生成用
bool GameData::ProteinFlag = false;//プロテインバースト中かどうか

float GameData::TotalScoreResult = 0;//ゲーム中のトータルスコア

int  GameData::GameEndTime = 2000;//本編プレイ制限時間
int  GameData::GamePlayTime = 0;//本編プレイ時間計測
int  GameData::GameHoldNum = 0;//サンタの現在所持アイテム数
