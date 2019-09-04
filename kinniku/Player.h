#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class CStage;

class CPlayer : public IGameObject
{
public:
	CPlayer(CStage *pStage);
	virtual ~CPlayer();
	virtual bool    move() override;
	virtual void    draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual float    getX() {
		return m_fX;
	}
	virtual float    getY() {
		return m_fY;
	}
protected:
	CStage		* m_pStage;
	ID2D1Bitmap *m_pBitmapP;
	ID2D1Bitmap *m_pBitmapC;
	FLOAT		m_fX, m_fY;
	FLOAT		m_fKeyTime;
	BOOL		m_bTama;
	BOOL		m_bLongPushSpace;
#ifdef _DEBUG
	ID2D1SolidColorBrush		*m_pBrush;
#endif // _DEBUG
};
