#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include "GameOver.h"
#include "TextureLoader.h"
#include "PlayGame.h"
#include  <windows.h>
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")

CGameOver::CGameOver(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = GameOverPhase::GAMEOVER_INIT;
	m_pImage = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;
	m_fTime = 0;
	m_iAdbise = 0;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\playgamen_sukoa.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\fontprotein.png"), &m_pImageBonus);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\power.png"), &m_pImagePower);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\sasi.png"), &m_pImageSasi);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\again.png"), &m_pImageAgain);

		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}

}


CGameOver::~CGameOver()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImageBonus);
	SAFE_RELEASE(m_pImagePower);
	SAFE_RELEASE(m_pImageSasi);
	SAFE_RELEASE(m_pImageAgain);
}

GameSceneResultCode    CGameOver::move() {
	switch (m_ePhase) {
	case GAMEOVER_INIT:
		m_iTimer = 0;
		m_bFlag = true;
		m_ePhase = GAMEOVER_RUN;
	case GAMEOVER_RUN:
	{
		sndPlaySound(L"buront.wav", SND_ASYNC);

		bool bDone = false;
		++m_iTimer;
<<<<<<< HEAD
		m_fTime++;
		if (100 < m_fTime) {
			m_fTime = 0;
			m_iAdbise++;
		}


		CSelector::a;
=======
		
>>>>>>> origin/master

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
			m_ePhase = GAMEOVER_FADE;
		}
		break;
	}
	case GAMEOVER_FADE:
		m_iFadeTimer++;
		if (m_iFadeTimer < 40)
			break;
		m_ePhase = GAMEOVER_DONE;
	case GAMEOVER_DONE:
		return GameSceneResultCode::GAMESCENE_END_OK;
	}
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
void    CGameOver::draw(ID2D1RenderTarget *pRenderTarget) {
	int frc = 0;
	D2D1_RECT_F rc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = (screenSize.width - textureSize.width) / 2;
	rc.top = (screenSize.height - textureSize.height) / 2;
	rc.right = rc.left + textureSize.width;
	rc.bottom = rc.top + textureSize.height;
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + 921*0.5;
	rc.bottom = rc.top + 460*0.5;
	pRenderTarget->DrawBitmap(m_pImageAgain, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	switch(m_iAdbise) {
	case 0:
		rc.left = 200;
		rc.top = 250;
		rc.right = rc.left + 1924 * 0.8;
		rc.bottom = rc.top + 645 * 0.8;
		pRenderTarget->DrawBitmap(m_pImageSasi, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 1:
		rc.left = 200;
		rc.top = 250;
		rc.right =  rc.left + 802*2;
		rc.bottom = rc.top + 280*2;
		pRenderTarget->DrawBitmap(m_pImageBonus, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 2:
		rc.left = 200;
		rc.top = 250;
		rc.right = rc.left + 1924 * 0.8;
		rc.bottom = rc.top + 629 * 0.8;
		pRenderTarget->DrawBitmap(m_pImagePower, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break; 
	case 3:
		break;
	case 4:
		break;
	}




	switch (m_ePhase) {
	case GAMEOVER_FADE:
	case GAMEOVER_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);

	}

}

