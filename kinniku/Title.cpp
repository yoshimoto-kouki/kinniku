#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include "Title.h"
#include "TextureLoader.h"
#include <mmsyscom.h>
#include <mmeapi.h>
#include<mmsystem.h>
#include <dsound.h>
#include "GameData.h"
#include <mmiscapi.h>
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")



CTitle::CTitle(CSelector *pSystem)
{

	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = TitlePhase::TITLE_INIT;
	m_pImage = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;
	count = 0;
	m_bSantaFlag = false;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\taitoru.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa1.png"), &m_pImageSanta1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa2.png"), &m_pImageSanta2);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
}


CTitle::~CTitle()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImageSanta1);
	SAFE_RELEASE(m_pImageSanta2);
}


GameSceneResultCode    CTitle::move() {
	switch (m_ePhase) {
	case TITLE_INIT:

		m_iTimer = 0;
		m_bFlag = true;
		m_ePhase = TITLE_RUN;
		//mciSendString(L"open honoo.mp3 type mpegvideo alias honoo", NULL, 0, NULL);
		//mciSendString(L"stop all", NULL, 0, NULL);
		//mciSendString(L"play honoo from 0 repeat", NULL, 0, NULL);
	case TITLE_RUN:
	{

		bool bDone = false;
		++m_iTimer;
		if (20 < m_iTimer) {
			m_iTimer = 0;
			m_bSantaFlag = !m_bSantaFlag;
		}
		if (GetAsyncKeyState(VK_SPACE)) {
			if (!m_bFlag) {
				//mciSendString(L"open kinniku.mp3 type mpegvideo alias kinniku", NULL, 0, NULL);
				//mciSendString(L"stop all", NULL, 0, NULL);
				//mciSendString(L"play kinniku from 0 repeat", NULL, 0, NULL);

				bDone = true;

				
				m_bFlag = true;

			}
		}
		else {
			m_bFlag = false;
		}
		if (bDone) {
			m_iFadeTimer = 0;

			m_ePhase = TITLE_FADE;
		}
		break;
	}
	case TITLE_FADE:

		m_iFadeTimer++;
		if (m_iFadeTimer < 30)
			break;
		m_ePhase = TITLE_DONE;
	case TITLE_DONE:
		return GameSceneResultCode::GAMESCENE_END_OK;
	}
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
void    CTitle::draw(ID2D1RenderTarget *pRenderTarget) {
	int a = 60;

	float FLAME = 60, krc;

	D2D1_RECT_F rc;// , src, prc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	/*
	src.left = 0;
	src.top = 0;
	src.right = rc.left + textureSize.width;
	src.bottom = 256 / 2;
	*/
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	D2D1_RECT_F rcS;
	rcS.left = 720;
	rcS.top = 450;
	rcS.right = rcS.left + 900;
	rcS.bottom = rcS.top + 684;
	if (m_bSantaFlag){
		pRenderTarget->DrawBitmap(m_pImageSanta1, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
}
	else {
		pRenderTarget->DrawBitmap(m_pImageSanta2, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}

	/*
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = 70+(screenSize.width - textureSize.width) / 2;
	rc.top = 190+(screenSize.height - textureSize.height) / 2;
	rc.right = rc.left + textureSize.width;
	rc.bottom = rc.top + textureSize.height;

	prc.left = 100;
	prc.top = 256 / 2;
	prc.right = prc.left + textureSize.width;
	prc.bottom = prc.top + textureSize.height;
	++count;
	//フラッシュ機構
	if (count % 10 == 1) {
		krc = 0.0f;
	}
	else if (count % 5 == 2) {
		krc = 0.5f;
	}
	else
		krc = 1.0f;
	pRenderTarget->DrawBitmap(m_pImage, rc, krc, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, prc);
	*/

	switch (m_ePhase) {
	case TITLE_FADE:
	case TITLE_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);
	}

}