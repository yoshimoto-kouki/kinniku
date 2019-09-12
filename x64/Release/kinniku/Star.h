#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

struct ID2D1Bitmap;
class CStage;

class CStar : public IGameObject
{
public:
	CStar(float x, float y);
	virtual ~CStar();
	virtual bool move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget)override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual bool collide(IGameObject *pObj,int x) override;
	virtual void hit(float amount) override;
	virtual bool hitType() override;
	virtual bool make()override;
	
	static void Restore(CStage *pStage, ID2D1RenderTarget *pRT);
	static void Finalize();
protected:
	static ID2D1Bitmap *m_pBitmap;
	float m_fX;
	float m_fY;
	float m_fVX;
	float m_fVY;
	BOOL  m_bDamage;
	static CStage		*m_pParent;
};