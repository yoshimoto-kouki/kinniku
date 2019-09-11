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
	ID2D1Bitmap *m_pImageLight;
	ID2D1Bitmap *m_pImageSeiya;
	ID2D1Bitmap *m_pImageShadow;
	ID2D1Bitmap *m_pImageSanta1;
	ID2D1Bitmap *m_pImageSanta2;

	ID2D1Bitmap *m_pImagePresent1;
	ID2D1Bitmap *m_pImagePresent2;
	ID2D1Bitmap *m_pImagePresentBox;
	ID2D1Bitmap *m_pImagePresentBag;
	ID2D1Bitmap *m_pImagePresentBagO;
	ID2D1Bitmap *m_pImageSTART;

	ID2D1SolidColorBrush *m_pBlack;
	INT count = 0;
	bool	m_bFlag;
	bool	m_bSantaFlag;
	INT     m_iTimer = 0;
	INT		m_iFadeTimer;
	float		m_fPTimer;
	int    m_iselectstar = 0;
	bool   m_bPresentFlag;
	bool   m_bBound;
	bool   m_btype;
	float  m_fsize;
	float  m_fPresentmove_x;
	float  m_fPresentmove_y;
	float  m_fVX;
	float  m_ftitleX;
	float  m_fPresentNum;
	static const int ANIM_FRAME = 60;
};

