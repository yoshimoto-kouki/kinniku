#include "stdafx.h"
#include <d2d1.h>
#include "BG.h"
#include "TextureLoader.h"
#include "GameData.h"


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

#define Five_point_starsPosx 0 
#define Five_point_starsPosy 150 

CBG::CBG(ID2D1RenderTarget *pRenderTarget)
{
	m_pBitmap = NULL;
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\playgamen.jpg"), &m_pBitmap);

	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\sei.png"), &m_pCHARSeiya1);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\ya.png"), &m_pCHARSeiya2);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\atension.png"), &m_pCHARSeiya3);

	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\Protein.png"), &m_pBGIconProtein);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\boots.png"), &m_pBGIconBoots);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\Ornament.png"), &m_pBGIconOrnament);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\candy.png"), &m_pBGIconCandy);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\hosi.png"), &m_pBGIconStar);

	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\star.png"), &m_pBGStar);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\digit.png"), &m_pBGtime);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\setumei.png"), &m_pBGDescription);

	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\digit_another.png"), &m_pBitmapItemNum);
	m_fY = 0;
	m_fVY = 9.8f * 5;
	m_fGY = -4.9f;
	time = 0;
	movetype = 0;
	//Icon�p
	startime = 0;
	type = 0;
	m_fVI0Y = 1;
	m_fVI0X = -0.325f;
	m_fVI1Y = -0.615f;
	m_fVI1X = 0.825f;
	m_fVI2Y = 0;
	m_fVI2X = -1;
	m_fVI3Y = 0.615f;
	m_fVI3X = 0.825f;
	m_fVI4Y = -1;
	m_fVI4X = -0.325f;




	m_fISX = Five_point_starsPosx + 0;
	m_fISY = Five_point_starsPosy + 0;
	m_fIOX = Five_point_starsPosx + -65;
	m_fIOY = Five_point_starsPosy + 200;
	m_fICX = Five_point_starsPosx + 100;
	m_fICY = Five_point_starsPosy + 77;
	m_fIPX = Five_point_starsPosx + -100;
	m_fIPY = Five_point_starsPosy + 77;
	m_fIBX = Five_point_starsPosx + 65;
	m_fIBY = Five_point_starsPosy + 200;
}

CBG::~CBG()
{
	SAFE_RELEASE(m_pBitmap);

	SAFE_RELEASE(m_pCHARSeiya1);
	SAFE_RELEASE(m_pCHARSeiya2);
	SAFE_RELEASE(m_pCHARSeiya3);

	SAFE_RELEASE(m_pBGIconProtein);
	SAFE_RELEASE(m_pBGIconBoots);
	SAFE_RELEASE(m_pBGIconOrnament);
	SAFE_RELEASE(m_pBGIconCandy);
	SAFE_RELEASE(m_pBGIconStar);

	SAFE_RELEASE(m_pBGStar);
	SAFE_RELEASE(m_pBGtime);
	SAFE_RELEASE(m_pBGDescription);
}

void CBG::move(){

	if (GameData::ProteinFlag) {
		if (m_fY < 0) {
			m_fY = 0;
			time = 0;
			movetype++;
			if (3 < movetype)
				movetype = 0;
		}
		time++;
		m_fY = m_fGY * time * time + m_fVY * time;
	}
	else{
		m_fY = 0;
		time = 0;
		movetype = 0;
	}

	startime++;
	if (200 < startime) {
		startime = 0;
		type++;
		if (4 < type)
			type = 0;
	}
	switch (type) {
	case 0:
		m_fISX += m_fVI0X;
		m_fISY += m_fVI0Y;
		m_fIOX += m_fVI1X;
		m_fIOY += m_fVI1Y;
		m_fICX += m_fVI2X;
		m_fICY += m_fVI2Y;
		m_fIPX += m_fVI3X;
		m_fIPY += m_fVI3Y;
		m_fIBX += m_fVI4X;
		m_fIBY += m_fVI4Y;
		break;
	case 1:

		m_fIBX += m_fVI0X;
		m_fIBY += m_fVI0Y;
		m_fISX += m_fVI1X;
		m_fISY += m_fVI1Y;
		m_fIOX += m_fVI2X;
		m_fIOY += m_fVI2Y;
		m_fICX += m_fVI3X;
		m_fICY += m_fVI3Y;
		m_fIPX += m_fVI4X;
		m_fIPY += m_fVI4Y;
		break;
	case 2:
		m_fIPX += m_fVI0X;
		m_fIPY += m_fVI0Y;
		m_fIBX += m_fVI1X;
		m_fIBY += m_fVI1Y;
		m_fISX += m_fVI2X;
		m_fISY += m_fVI2Y;
		m_fIOX += m_fVI3X;
		m_fIOY += m_fVI3Y;
		m_fICX += m_fVI4X;
		m_fICY += m_fVI4Y;
		break;
	case 3:
		m_fICX += m_fVI0X;
		m_fICY += m_fVI0Y;
		m_fIPX += m_fVI1X;
		m_fIPY += m_fVI1Y;
		m_fIBX += m_fVI2X;
		m_fIBY += m_fVI2Y;
		m_fISX += m_fVI3X;
		m_fISY += m_fVI3Y;
		m_fIOX += m_fVI4X;
		m_fIOY += m_fVI4Y;
		break;
	case 4:
		m_fIOX += m_fVI0X;
		m_fIOY += m_fVI0Y;
		m_fICX += m_fVI1X;
		m_fICY += m_fVI1Y;
		m_fIPX += m_fVI2X;
		m_fIPY += m_fVI2Y;
		m_fIBX += m_fVI3X;
		m_fIBY += m_fVI3Y;
		m_fISX += m_fVI4X;
		m_fISY += m_fVI4Y;
		break;
	}
}
/****************************************************
 *@method
 *  �`��v���O����
 ***************************************************/
void CBG::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc, rcw;
	D2D1_SIZE_F sizescreen;
	if (m_pBitmap == NULL)
		return;

	sizescreen = pRenderTarget->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + sizescreen.width*0.7;
	rc.bottom = rc.top + sizescreen.height;
	pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	float moveY = m_fY;
	if (movetype != 0) {
		moveY = 0;
	}
	rcw.left = sizescreen.width*0.7 + 50;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;
	pRenderTarget->DrawBitmap(m_pCHARSeiya1, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	moveY = m_fY;
	if (movetype != 1) {
		moveY = 0;
	}
	rcw.left = sizescreen.width*0.7 + 136 + 50;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;

	pRenderTarget->DrawBitmap(m_pCHARSeiya2, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	moveY = m_fY;
	if (movetype != 2) {
		moveY = 0;
	}
	rcw.left = sizescreen.width*0.7 + 322;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;

	pRenderTarget->DrawBitmap(m_pCHARSeiya3, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	D2D1_RECT_F rcBS, rcs, rco, rcb, rcc, rcp;
	rcBS.left = sizescreen.width*0.7 + 185+ Five_point_starsPosx;
	rcBS.top = Five_point_starsPosy + 45;
	rcBS.right = rcBS.left + 200;
	rcBS.bottom = rcBS.top + 200;
	pRenderTarget->DrawBitmap(m_pBGStar, rcBS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	rcs.left = sizescreen.width*0.7 + 250 + m_fISX+ Five_point_starsPosx;
	rcs.top = 10 + m_fISY;
	rcs.right = rcs.left + 64;
	rcs.bottom = rcs.top + 64;

	rco.left = sizescreen.width*0.7 + 250 + m_fIOX+ Five_point_starsPosx;
	rco.top = 10 + m_fIOY;
	rco.right = rco.left + 64;
	rco.bottom = rco.top + 64;

	rcb.left = sizescreen.width*0.7 + 250 + m_fIBX+ Five_point_starsPosx;
	rcb.top = 10 + m_fIBY;
	rcb.right = rcb.left + 64;
	rcb.bottom = rcb.top + 64;

	rcc.left = sizescreen.width*0.7 + 250 + m_fICX+ Five_point_starsPosx;
	rcc.top = 10 + m_fICY;
	rcc.right = rcc.left + 64;
	rcc.bottom = rcc.top + 64;

	rcp.left = sizescreen.width*0.7 + 250 + m_fIPX+ Five_point_starsPosx;
	rcp.top = 10 + m_fIPY;
	rcp.right = rcp.left + 64;
	rcp.bottom = rcp.top + 64;
	switch (type) {
	case 0:
		pRenderTarget->DrawBitmap(m_pBGIconStar, rcs, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconOrnament, rco, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconBoots, rcb, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconCandy, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconProtein, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 1:
		pRenderTarget->DrawBitmap(m_pBGIconProtein, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconStar, rcs, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconOrnament, rco, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconBoots, rcb, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconCandy, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 2:
		pRenderTarget->DrawBitmap(m_pBGIconCandy, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconProtein, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconStar, rcs, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconOrnament, rco, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconBoots, rcb, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 3:
		pRenderTarget->DrawBitmap(m_pBGIconBoots, rcb, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconCandy, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconProtein, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconStar, rcs, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconOrnament, rco, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 4:
		pRenderTarget->DrawBitmap(m_pBGIconOrnament, rco, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconBoots, rcb, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconCandy, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconProtein, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		pRenderTarget->DrawBitmap(m_pBGIconStar, rcs, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	}
	D2D1_RECT_F rccount, rcpos;
	rccount.top = Five_point_starsPosy + 140;
	rccount.left = sizescreen.width*0.7f + 270+ Five_point_starsPosx;
	rccount.bottom = rccount.top + 32;
	rccount.right = rccount.left + 32;
	int Itemnum = GameData::GameHoldNum;

	rcpos.left = (Itemnum % 10) % 4 * 32;
	rcpos.top = (Itemnum % 10) / 4 * 32;
	rcpos.right = rcpos.left + 32;
	rcpos.bottom = rcpos.top + 32;
	pRenderTarget->DrawBitmap(m_pBitmapItemNum, rccount, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rcpos);

	
	int val = GameData::GameEndTime - GameData::GamePlayTime;
	val /= 30;
	D2D1_RECT_F rctime, rcpix;
	rctime.top = 5;
	rctime.bottom = rctime.top + 64;
	rctime.left = sizescreen.width - 128;
	while (0 < val) {
		
		rctime.right = rctime.left + 64;

		rcpix.left = (val % 10) % 4 * 32;
		rcpix.top = (val % 10) / 4 * 32;
		rcpix.right = rcpix.left + 32;
		rcpix.bottom = rcpix.top + 32;
		pRenderTarget->DrawBitmap(m_pBGtime, rctime, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rcpix);
		val /= 10;
		rctime.left -= 64;
	}
	
	D2D1_RECT_F rcDis;
	rcDis.left =sizescreen.width*0.7;
	rcDis.top = 450;
	rcDis.right = rcDis.left + 400;
	rcDis.bottom = rcDis.top + 400;
	pRenderTarget->DrawBitmap(m_pBGDescription, rcDis, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}