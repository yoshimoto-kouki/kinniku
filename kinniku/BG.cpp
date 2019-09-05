#include "stdafx.h"
#include <d2d1.h>
#include "BG.h"
#include "TextureLoader.h"
#include "GameData.h"


//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

CBG::CBG(ID2D1RenderTarget *pRenderTarget)
{
	m_pBitmap = NULL;
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\snowshow.png"), &m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\sei.png"), &m_pCHARSeiya1);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\ya.png"), &m_pCHARSeiya2);
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\atension.png"), &m_pCHARSeiya3);
	m_fY = 0;
	m_fVY = 9.8f*5;
	m_fGY = -4.9f;
	time = 0;
	movetype = 0;
}

CBG::~CBG()
{
	SAFE_RELEASE(m_pBitmap);
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
}
/****************************************************
 *@method
 *  描画プログラム
 ***************************************************/
void CBG::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc, rcw;
	D2D1_SIZE_F size;
	if (m_pBitmap == NULL)
		return;
	size = m_pBitmap->GetSize();

	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + size.width;
	rc.bottom = rc.top + size.height;
	pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	
	float moveY = m_fY;
	if (movetype != 0) {
		moveY = 0;
	}
	rcw.left = size.width+50;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;
	pRenderTarget->DrawBitmap(m_pCHARSeiya1, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	moveY = m_fY;
	if (movetype != 1) {
		moveY = 0;
	}
	rcw.left = size.width + 136 + 50;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;

	pRenderTarget->DrawBitmap(m_pCHARSeiya2, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	moveY = m_fY;
	if (movetype != 2) {
		moveY = 0;
	}
	rcw.left = size.width + 272 + 50;
	rcw.top = 900 - moveY;
	rcw.right = rcw.left + 126;
	rcw.bottom = rcw.top + 126;

	pRenderTarget->DrawBitmap(m_pCHARSeiya3, rcw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

}