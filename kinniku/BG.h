/**********************************************
 *@file BG.h
 *  ”wŒi•`‰æƒNƒ‰ƒX
 *@date 17.Nov.2018
 *********************************************/
#pragma once

struct ID2D1Bitmap;
struct ID2D1RenderTarget;
class CBG
{
public:
	CBG(ID2D1RenderTarget *pRenderTarget);
	virtual ~CBG();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);
	virtual void move();
protected:
	float m_fY;
	float m_fVY, m_fGY;
	int time;
	int movetype;
	ID2D1Bitmap *m_pBitmap;
	ID2D1Bitmap *m_pCHARSeiya1;
	ID2D1Bitmap *m_pCHARSeiya2;
	ID2D1Bitmap *m_pCHARSeiya3;
};