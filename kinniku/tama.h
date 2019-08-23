#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
//struct ID2D1SolidColorBrush;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
class CStage;

class CTama : public IGameObject
{
public:
	CTama(CStage *pStage, float x, float y);
	//CTama(float x, float y);
	~CTama();

	virtual bool move() override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(IGameObject *pObj) override;
//	virtual void damage(float amount) override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();

protected:
	static ID2D1RenderTarget *pRenderTarget;
	static ID2D1Bitmap *m_pBitmap;
	//CStage *m_pParent;
	static CStage *m_pParent;
	float m_fX, m_fY;
	float m_fVX, m_fVY;	
};