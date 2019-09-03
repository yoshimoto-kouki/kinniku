#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

struct ID2D1RenderTarget;
struct ID2D1Bitmap;
class CStage;

class CTama : public IGameObject
{
public:
	CTama(CStage *pStage, float x, float y,bool Keystate);
	~CTama();
	virtual bool move() override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual void hit(float amount) override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();

protected:
	static ID2D1RenderTarget *pRenderTarget;
	static ID2D1Bitmap *m_pBitmap;
	static ID2D1Bitmap *m_pBitmap2;
	CStage *m_pParent;
	BOOL m_bDamage;
	bool m_bLong;
	float m_fX, m_fY;
	float m_fVX, m_fVY;	
};