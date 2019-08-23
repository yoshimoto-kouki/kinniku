#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
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
	CStage * m_pStage;
	ID2D1Bitmap *m_pBitmapP;
	FLOAT    m_fX, m_fY;
	BOOL m_bTama;
};
