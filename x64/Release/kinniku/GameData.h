#pragma once
class GameData
{
public:
	static float tamax;
	static float tamay;
	static bool shot;

	//このフラグで得点加算。複数種必要な場合はbool->intにすること
	
	static bool StartIndexEnd;
	static bool ProteinFlag;

	//ゲームリザルト画面に渡すスコア
	static float TotalScoreResult;
	
	//ゲームタイム
	static int GameEndTime;
	static int GamePlayTime;

	static int GameHoldNum;

	static int CLEAR_LINE;
};

