#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

//struct ID2D1RenderTarget;
struct ID2D1Bitmap;
class CStage;

class CProtein : public IGameObject
{
public:
	CProtein(float x, float y);
	virtual ~CProtein();

	virtual bool move() override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual void hit(float amount) override;
	static void Restore(CStage *pStage, ID2D1RenderTarget *pRT);
	static void Finalize();


protected:
	//static ID2D1RenderTarget *pRenderTarget;
	static ID2D1Bitmap *m_pTexture;
	static CStage * m_pParent;
	BOOL  m_bDamage;
	float m_fX, m_fY;
	float m_fVX, m_fVY;
};