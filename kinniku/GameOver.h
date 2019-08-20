#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

enum GameOverPhase {
	GAMEOVER_INIT = 0,
	GAMEOVER_RUN = 1,
	GAMEOVER_FADE = 2,
	GAMEOVER_DONE = 3
};
class IGameObject;
extern IGameObject *CreateMyscore;
class CSelector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;
class CGameOver : public IGameScene
{
public:
	CGameOver(CSelector *pSystem);
	virtual ~CGameOver();
	virtual GameSceneResultCode    move() override;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) override;
protected:
	CSelector *m_pSystem;
	GameOverPhase m_ePhase;
	ID2D1Bitmap *m_pImage;
	ID2D1Bitmap *m_pImage2;
	ID2D1SolidColorBrush *m_pBlack;
	bool	m_bFlag;
	INT     m_iTimer = 0;
	INT		m_iFadeTimer;
};

