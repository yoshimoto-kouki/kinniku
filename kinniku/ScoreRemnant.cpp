#include "stdafx.h"
#include <d2d1.h>
#include <math.h>
#include "PlayGame.h"
#include "ScoreRemnant.h"
#include "TextureLoader.h"

#define ScoreLimitY 30
ID2D1Bitmap *CScoreRemnant::m_pBitmap = NULL;
CStage *CScoreRemnant::m_pParent = NULL;

CScoreRemnant::CScoreRemnant(float x, float y, int score)
{
	m_fX = x;
	m_fY = y;
	m_fVY = -0.3f;
	m_bEND = false;
	m_iRemScore = (1 + pow(2, score));
	m_fLimitY = y - ScoreLimitY;
}

CScoreRemnant::~CScoreRemnant(){}

bool CScoreRemnant::move()
{
	if (m_bEND)
		return false;
	m_fY += m_fVY;
	if (m_fY < m_fLimitY)
		m_bEND = true;
	return true;
}

void CScoreRemnant::draw(ID2D1RenderTarget * pRenderTarget)
{
	int RemScore = m_iRemScore;
	D2D1_RECT_F rc,src;
	while (0 < RemScore) {
		rc.left = m_fX - 16 * 3;
		rc.top = m_fY;
		rc.right = rc.left + 16;
		rc.bottom = rc.top + 16;

		src.left   = RemScore % 4 * 32;
		src.top    = RemScore / 4 * 32;
		src.right  = src.left + 32;
		src.bottom = src.top  + 32;
		pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,src);
		RemScore /= 10;
	}
	int i = 2;
	while (0 < i) {
		rc.left = m_fX - 16 * i;
		rc.top = m_fY;
		rc.right = rc.left + 16;
		rc.bottom = rc.top + 16;

		src.left = 0;
		src.top  = 0;
		src.right = 32;
		src.bottom = 32;
		pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
		i--;
	}

}

void CScoreRemnant::Restore(CStage * pStage, ID2D1RenderTarget * pRT)
{
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\digit.png"), &m_pBitmap);
	m_pParent = pStage;
}

void CScoreRemnant::Finalize()
{
	SAFE_RELEASE(m_pBitmap);
	m_pParent = NULL;
}
