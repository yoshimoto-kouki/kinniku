#pragma once
class GameData
{
public:
	static float tamax;
	static float tamay;
	static bool shot;

	//���̃t���O�œ��_���Z�B������K�v�ȏꍇ��bool->int�ɂ��邱��
	
	static bool StartIndexEnd;
	static bool ProteinFlag;

	//�Q�[�����U���g��ʂɓn���X�R�A
	static float TotalScoreResult;
	
	//�Q�[���^�C��
	static int GameEndTime;
	static int GamePlayTime;

	static int GameHoldNum;

	static int CLEAR_LINE;
};
