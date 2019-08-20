#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_FADE = 2,
	STAGE_DONE = 3
};
class CSelector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;
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
	StagePhase m_ePhase;

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

