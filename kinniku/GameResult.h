#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

enum GameResultPhase {
	GAMERESULT_INIT = 0,
	GAMERESULT_RUN = 1,
	GAMERESULT_FADE = 2,
	GAMERESULT_DONE = 3
};

class IGameObject;
extern IGameObject *CreateMyscore;

class CSelector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class CGameResult :public IGameScene
{
public:
	CGameResult(CSelector *pSystem);
	virtual ~CGameResult();
	virtual GameSceneResultCode move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
protected:
	CSelector *m_pSystem;
	GameResultPhase m_ePhase;
	ID2D1Bitmap *m_pImage;
	ID2D1SolidColorBrush *m_pBlack;
	bool m_bFlag;
	INT m_iTimer = 0;
	INT m_iFadeTimer;

};

