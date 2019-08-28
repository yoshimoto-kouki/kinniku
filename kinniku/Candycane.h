#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

struct ID2D1Bitmap;
class CStage;

class CCandy :
	public IGameObject
{
public:
	CCandy(CStage *pStage, float x, float y);
	virtual ~CCandy();
	virtual bool move();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual void hit(float amount) override;
	virtual bool make()override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();
protected:
	CStage * m_pParent;
	static ID2D1Bitmap *m_pTexture;
	float m_fX;
	float m_fY;
	float m_fVY;
	BOOL  m_bDamage;
};