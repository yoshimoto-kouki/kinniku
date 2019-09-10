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
	
	float m_fIPY, m_fIPX;
	float m_fIOY, m_fIOX;
	float m_fIBY, m_fIBX;
	float m_fICY, m_fICX;
	float m_fISY, m_fISX;
	float m_fVI0Y, m_fVI0X;
	float m_fVI1Y, m_fVI1X;
	float m_fVI2Y, m_fVI2X;
	float m_fVI3Y, m_fVI3X;
	float m_fVI4Y, m_fVI4X;
	int type;
	int startime;
	int time;
	int movetype;
	ID2D1Bitmap *m_pBitmap;
	ID2D1Bitmap *m_pCHARSeiya1;
	ID2D1Bitmap *m_pCHARSeiya2;
	ID2D1Bitmap *m_pCHARSeiya3;
	ID2D1Bitmap *m_pBGIconProtein;
	ID2D1Bitmap *m_pBGIconOrnament;
	ID2D1Bitmap *m_pBGIconBoots;
	ID2D1Bitmap *m_pBGIconCandy;
	ID2D1Bitmap *m_pBGIconStar;

	ID2D1Bitmap *m_pBGStar;
	ID2D1Bitmap *m_pBGtime;
	ID2D1Bitmap *m_pBGDescription;
	ID2D1Bitmap *m_pBitmapItemNum;
};