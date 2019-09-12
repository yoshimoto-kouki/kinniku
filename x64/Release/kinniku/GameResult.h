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
	ID2D1Bitmap *m_pImageBG;
	ID2D1Bitmap *m_pImageRESULT;
	ID2D1Bitmap *m_pImageCLEAR;
	ID2D1Bitmap *m_pImagePUSH;
	ID2D1Bitmap *m_pImagedigit;
	ID2D1Bitmap *m_pImageBag;
	ID2D1Bitmap *m_pImagePresent1;
	ID2D1Bitmap *m_pImagePresent2;
	ID2D1SolidColorBrush *m_pBlack;
	bool m_bFlag;
	float m_itimer;
	float m_fY;
	float m_fpY;
	float m_fpX;
	int m_iFTimer;
	INT m_iTimer = 0;
	INT m_iFadeTimer;

};

