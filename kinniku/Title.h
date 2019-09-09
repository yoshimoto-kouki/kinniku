#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif

enum TitlePhase {
	TITLE_INIT = 0,
	TITLE_RUN = 1,
	TITLE_FADE = 2,
	TITLE_DONE = 3
};
class CSelector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;
class CTitle : public IGameScene
{
public:
	CTitle(CSelector *pSystem);
	virtual ~CTitle();
	virtual GameSceneResultCode    move() override;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) override;
protected:
	static const int BLINK_INTERVAL1 = 60;
	static const int BLINK_INTERVAL2 = 10;
	CSelector *m_pSystem;
	TitlePhase m_ePhase;
	ID2D1Bitmap *m_pImage;
	ID2D1Bitmap *m_pImageSanta1;
	ID2D1Bitmap *m_pImageSanta2;
	ID2D1SolidColorBrush *m_pBlack;
	INT count = 0;
	bool	m_bFlag;
	bool	m_bSantaFlag;
	INT     m_iTimer = 0;
	INT		m_iFadeTimer;

	static const int ANIM_FRAME = 60;
};

