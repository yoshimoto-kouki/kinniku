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
	virtual float TSPointBackx() override;
	virtual float TSPointBacky() override;
	int TreeScoreBack() { return TreeScore; }
	float ScoreBack() override;
	virtual bool  StarHitFlag()override;
	static void Restore(ID2D1RenderTarget *pRT);
	static void Finalize();

protected:
	static ID2D1RenderTarget *pRenderTarget;
	static ID2D1Bitmap *m_pBitmap;
	static ID2D1Bitmap *m_pBitmap2;
	static ID2D1Bitmap *m_pBitmap3;
	static ID2D1Bitmap *m_pBitmap4;
	CStage *m_pParent;
	BOOL m_bDamage; //本来、このフラグで処理しようとしたが、エラーを吐く.boolを操作するとエラー何故。

	float m_fX, m_fY;
	float m_fVX, m_fVY;	
	int TreeScore;
	float m_fTreeScore;
};