#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif


struct ID2D1Bitmap;
class CStage;

class CScoreRemnant : public IGameObject
{
public:
	CScoreRemnant(float x,float y,int score);
	virtual ~CScoreRemnant();

	virtual bool move()override;
	virtual void draw(ID2D1RenderTarget *pRenderTarget)override;
	static void Restore(CStage *pStage, ID2D1RenderTarget *pRT);
	static void Finalize();
protected:
	static ID2D1Bitmap *m_pBitmap;
	float m_fX;
	float m_fY;
	float m_fVY;
	int  m_iRemScore;
	bool  m_bEND;
	static CStage *m_pParent;
};

