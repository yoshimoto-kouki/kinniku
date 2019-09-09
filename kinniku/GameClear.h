#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

enum GameClearPhase {
	GAMECLEAR_INIT = 0,
	GAMECLEAR_RUN = 1,
	GAMECLEAR_FADE = 2,
	GAMECLEAR_DONE = 3
};

class IGameObject;
extern IGameObject *CreateMyscore;

class CSelector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class CGameClear :public IGameScene
{
public:
	CGameClear(CSelector *pSystem);
	virtual ~CGameClear();
	virtual GameSceneResultCode move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
protected:
	CSelector *m_pSystem;
	GameClearPhase m_ePhase;
	ID2D1Bitmap *m_pImage;
	ID2D1SolidColorBrush *m_pBlack;
	bool m_bFlag;
	INT m_iTimer = 0;
	INT m_iFadeTimer;

};

