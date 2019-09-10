#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include "GameClear.h"
#include "TextureLoader.h"
#include "PlayGame.h"
#include  <windows.h>
#include  <mmsystem.h>


CGameClear::CGameClear(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = GameClearPhase::GAMECLEAR_INIT;
	m_pImage = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;
	m_fY = 0;
	m_fVY = 1;//otitekuru protein

	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\playgamen_sukoa.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa1.png"), &m_pImageSanta1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa2.png"), &m_pImageSanta2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\Protein.png"), &m_pImageProtein);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
}

CGameClear::~CGameClear()
{
	SAFE_RELEASE(m_pImage);
}

GameSceneResultCode CGameClear::move() {
	switch (m_ePhase) {
	case GAMECLEAR_INIT:
		m_iTimer = 0;
		m_bFlag = true;
		m_ePhase = GAMECLEAR_RUN;
	case GAMECLEAR_RUN:
	{
		bool bDone = false;
		++m_iTimer;
		if (20 < m_iTimer) {
			m_iTimer = 0;
			m_bSantaFlag = !m_bSantaFlag;
		}

		m_fY = -20*m_iTimer + m_fVY * m_iTimer*m_iTimer;
		
		if (GetAsyncKeyState(VK_SPACE)) {
			if (!m_bFlag) {

				bDone = true;
				m_bFlag = true;
			}
		}
		else {
			m_bFlag = false;
		}
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = GAMECLEAR_FADE;
		}
		break;
	}
	case GAMECLEAR_FADE:
		m_iFadeTimer++;
		if (m_iFadeTimer < 40)
			break;
		m_ePhase = GAMECLEAR_DONE;
	case GAMECLEAR_DONE:
		return GameSceneResultCode::GAMESCENE_END_OK;
	}
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
void    CGameClear::draw(ID2D1RenderTarget *pRenderTarget) {
	int frc = 0;
	D2D1_RECT_F rc, prc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	
	prc.left = 830;
	prc.top = 800 + m_fY;
	prc.right =  prc.left + 64;
	prc.bottom = prc.top + 64;
	pRenderTarget->DrawBitmap(m_pImageProtein, prc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);


	D2D1_RECT_F rcS;
	rcS.left = 720;
	rcS.top = 450;
	rcS.right = rcS.left + 900;
	rcS.bottom = rcS.top + 684;
	if (m_bSantaFlag) {
		pRenderTarget->DrawBitmap(m_pImageSanta1, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	else {
		pRenderTarget->DrawBitmap(m_pImageSanta2, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}


	switch (m_ePhase) {
	case GAMECLEAR_FADE:
	case GAMECLEAR_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);

	}

}

