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
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\seiya.png"), &m_pImageSeiya);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa1.png"), &m_pImageSanta1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa2.png"), &m_pImageSanta2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\shadow.png"), &m_pImageShadow);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\light.png"), &m_pImageLight);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
}


CTitle::~CTitle()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImageSeiya);
	SAFE_RELEASE(m_pImageShadow);
	SAFE_RELEASE(m_pImageSanta1);
	SAFE_RELEASE(m_pImageSanta2);
	SAFE_RELEASE(m_pImageLight);
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
			m_iselectstar++;
			if (13 < m_iselectstar)
				m_iselectstar = 0;
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

	D2D1_RECT_F rc,src;// , src, prc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	
	src.left = 100;
	src.top = -200;
	src.right = src.left + screenSize.width;
	src.bottom = src.top + screenSize.height;
	//”wŒi
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	D2D1_RECT_F rclight;//Œõ
	switch (m_iselectstar) {
	case 0:
		rclight.top = 160;
		rclight.left = 1200;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 1:
		rclight.top = 245;
		rclight.left = 250;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 2:
		rclight.top = 165;
		rclight.left = -5;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 3:
		rclight.top = 160;
		rclight.left = 320;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 4:
		rclight.top = 40;
		rclight.left = 410;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 5:
		rclight.top = 20;
		rclight.left = 1090;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 6:
		rclight.top = 20;
		rclight.left = 1850;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 7:
		rclight.top = 255;
		rclight.left = 1535;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 8:
		rclight.top = 295;
		rclight.left = 1225;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 9:
		rclight.top = 95;
		rclight.left = 85;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 10:
		rclight.top = 5;
		rclight.left = 150;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 11:
		rclight.top = -15;
		rclight.left = 775;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 12:
		rclight.top = 110;
		rclight.left = 785;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	case 13:
		rclight.top = 70;
		rclight.left = 645;
		rclight.right = rclight.left + 64;
		rclight.bottom = rclight.top + 64;
		pRenderTarget->DrawBitmap(m_pImageLight, rclight, 0.3f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		break;
	}
	//¹–é‚Ì•¶Žš
	pRenderTarget->DrawBitmap(m_pImageSeiya, src, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	D2D1_RECT_F rcS;
	rcS.left = 720;
	rcS.top = 850;
	rcS.right = rcS.left + 900;
	rcS.bottom = rcS.top + 284;

	//ƒTƒ“ƒ^
	if (m_bSantaFlag){
		pRenderTarget->DrawBitmap(m_pImageShadow, rcS, 0.5f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

		rcS.top = 450;
		rcS.bottom = rcS.top + 684;
		pRenderTarget->DrawBitmap(m_pImageSanta1, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
}
	else {
		pRenderTarget->DrawBitmap(m_pImageShadow, rcS, 0.8f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

		rcS.top = 450;
		rcS.bottom = rcS.top + 684;
		pRenderTarget->DrawBitmap(m_pImageSanta2, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}

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