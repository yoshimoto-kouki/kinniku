#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif
#define PSHOT_NUM  20

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_FADE = 2,
	STAGE_DONE = 3
};
class CSelector;
class CScoreUI;
class CBG;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;
 
struct SHOT {
	
bool flag;//弾が発射中かどうか
	
double x;//x座標
	
double y;//y座標
	
int gh;//グラフィックハンドル
	
int width, height;//画像の幅と高さ
};

class CStage : public IGameScene
{
public:
	CStage(CSelector *pSystem);
	virtual ~CStage();
	
	virtual GameSceneResultCode    move() override;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) override;
	void Draw(ID2D1RenderTarget *pTarget);
	void SetPos(int x, int y);
protected:
	CSelector *m_pSystem;
	CScoreUI *m_pScore;//UI
	CBG			*m_pBG;//BG
	StagePhase m_ePhase;
	ID2D1Bitmap *m_pImage2;
	ID2D1Bitmap *m_pImage;
	ID2D1SolidColorBrush *m_pBlack;
	INT     m_iTimer = 0;
	INT		m_iFadeTimer;
	
	INT renda;
	INT	m_bFlag = true;
	float mX, mY;
	INT mPhase;//状態変数
	INT mTimer;//タイマー
	INT mFrame;//アニメーションフレーム
	static const int ANIM_FRAME = 60;

};

