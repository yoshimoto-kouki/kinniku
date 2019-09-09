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
	CTama(CStage *pStage, float x, float y,int Decoration);
	~CTama();
	virtual bool move() override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;
	virtual bool collide(float x, float y, float w, float h) override;
	virtual bool collide(float x, float w) override;
	virtual bool collide(IGameObject *pObj) override;
	virtual void hit(float amount) override;
	virtual bool collidePos(float x, float y);
	virtual float TSPointBackx() { return coliPosx; }
	virtual float TSPointBacky() { return coliPosy;	}
	int TreeScoreBack() { return TreeScore; }
	float ScoreBack() { return (int)m_fTreeScore; }
	bool  StarHitFlag() { return m_bDamage; }
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();

protected:
	static ID2D1RenderTarget *pRenderTarget;
	static ID2D1Bitmap *m_pBitmap;
	CStage *m_pParent;
	BOOL m_bDamage;
	float m_fX, m_fY;
	float m_fVX, m_fVY;	
	float coliPosx, coliPosy;
	int TreeScore;
	float m_fTreeScore;
};