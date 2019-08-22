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
protected:
	ID2D1Bitmap *m_pBitmap;
};