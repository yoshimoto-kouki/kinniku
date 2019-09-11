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
	ID2D1Bitmap *m_pImagepush;
	ID2D1Bitmap *m_pImageShadow;
	ID2D1Bitmap *m_pImageSanta1;
	ID2D1Bitmap *m_pImageSanta2;
	ID2D1Bitmap *m_pImageProtein;
	ID2D1Bitmap *m_pImageSeiya;
	ID2D1Bitmap *m_pImageThank;
	ID2D1SolidColorBrush *m_pBlack;
	bool m_bFlag;
	bool m_bSantaFlag;
	INT m_iTimer = 0;
	INT m_iFadeTimer;
	float m_fbord;
	float m_fthank;
	int m_iTimerP = 0;
	float m_fY ,m_fVY;

};

