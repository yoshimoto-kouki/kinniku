#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include "GameClear.h"
#include "TextureLoader.h"
#include "PlayGame.h"
#include  <windows.h>
#include  <mmsystem.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


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
	m_fthank = 0;
	m_fbord = 0;
	m_bpush = false;
	m_itimer = 0;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\playgamen_sukoa.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\push.png"), &m_pImagepush);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\shadow.png"), &m_pImageShadow);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa1.png"), &m_pImageSanta1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa2.png"), &m_pImageSanta2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\Protein.png"), &m_pImageProtein);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\seiya_english.png"), &m_pImageSeiya);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\thankyou.png"), &m_pImageThank);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\movielight.png"), &m_pImagemovielight);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present2.png"), &m_pImagepresent);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present1.png"), &m_pImagepresent2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\hosi.png"), &m_pImagehosi);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
}

CGameClear::~CGameClear()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImageShadow);
	SAFE_RELEASE(m_pImageSanta1);
	SAFE_RELEASE(m_pImageSanta2);
	SAFE_RELEASE(m_pImageProtein);
	SAFE_RELEASE(m_pImageSeiya);
	SAFE_RELEASE(m_pImageThank);
	SAFE_RELEASE(m_pImagemovielight);
	SAFE_RELEASE(m_pImagepresent);
	SAFE_RELEASE(m_pImagepresent2);
	SAFE_RELEASE(m_pImagehosi);
}

GameSceneResultCode CGameClear::move() {
	switch (m_ePhase) {
		
	case GAMECLEAR_INIT:
		mciSendString(L"open ED.mp3 type mpegvideo alias ED", NULL, 0, NULL);
		mciSendString(L"play ED from 0 repeat", NULL, 0, NULL);
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
			if (m_fthank < 2.7)
				m_fthank += 0.3f;
		}
		m_fY = -20*m_iTimer + m_fVY * m_iTimer*m_iTimer;
		
		if (m_fbord <= 1)
			m_fbord += 0.005f;
		m_itimer++;
		if (m_itimer == 360)
			m_itimer = 0;

		if (GetAsyncKeyState(VK_SPACE)) {
			m_bpush = true;
			if (!m_bFlag) {
				mciSendString(L"stop all", NULL, 0, NULL);
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
	D2D1_RECT_F rc,rcp, prc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	rcp.left = 10;
	rcp.top = 950;
	rcp.right = rcp.left + 502;
	rcp.bottom = rcp.top + 64;
	pRenderTarget->DrawBitmap(m_pImagepush, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	prc.left = 730;
	prc.top = 960 + m_fY;
	prc.right =  prc.left + 64;
	prc.bottom = prc.top + 64;
	pRenderTarget->DrawBitmap(m_pImageProtein, prc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);


	//•¶Žš
	D2D1_RECT_F rcT;
	rcT.top = 200;
	rcT.bottom = rcT.top + 512;
	rcT.left = 300;
	rcT.right = rcT.left + 674;

	pRenderTarget->DrawBitmap(m_pImageThank, rcT, m_fbord, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	
	D2D1_RECT_F rcS,rcSw,rc_seiya, rcs_seiya,rclight, rcstar;

	//light
	rclight.left = 237;
	rclight.top = 198;
	rclight.right = rclight.left + 739;
	rclight.bottom = rclight.top + 718;
	pRenderTarget->DrawBitmap(m_pImagemovielight, rclight, m_fbord, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	//present
	rclight.left = 200;
	rclight.top = 830;
	rclight.right = rclight.left + 170;
	rclight.bottom = rclight.top + 100;
	if(!m_bpush)
		pRenderTarget->DrawBitmap(m_pImagepresent, rclight, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	else
		pRenderTarget->DrawBitmap(m_pImagepresent2, rclight, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	//santa&shadow
	rcS.left = 720;
	rcS.top = 450;
	rcS.right = rcS.left + 900;
	rcS.bottom = rcS.top + 684;
	rcSw.left = 720;
	rcSw.top = 850;
	rcSw.right = rcSw.left + 900;
	rcSw.bottom = rcSw.top + 284;
	if (m_bSantaFlag) {
		pRenderTarget->DrawBitmap(m_pImageShadow, rcSw, 0.5f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		pRenderTarget->DrawBitmap(m_pImageSanta1, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	else {
		pRenderTarget->DrawBitmap(m_pImageShadow, rcSw, 0.8f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
		pRenderTarget->DrawBitmap(m_pImageSanta2, rcS, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	

	//seiya
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(-45.f, D2D1::Point2F(945,189)));
	if (m_bSantaFlag) {
		rc_seiya.left = 250;
		rc_seiya.top = 450;
		rc_seiya.right = rc_seiya.left + 160;
		rc_seiya.bottom = rc_seiya.top + 80;

		rcs_seiya.top = 0;
		rcs_seiya.bottom = rcs_seiya.top + 378;
		rcs_seiya.left = 516;
		rcs_seiya.right = rcs_seiya.left + 482;
		pRenderTarget->DrawBitmap(m_pImageSeiya, rc_seiya, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rcs_seiya);
	}
	else {
		rc_seiya.left = 250;
		rc_seiya.top = 450;
		rc_seiya.right = rc_seiya.left + 128;
		rc_seiya.bottom = rc_seiya.top + 80;

		rcs_seiya.top = 0;
		rcs_seiya.bottom = rcs_seiya.top + 378;
		rcs_seiya.left = 1000;		
		rcs_seiya.right = rcs_seiya.left + 400;
		pRenderTarget->DrawBitmap(m_pImageSeiya, rc_seiya, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rcs_seiya);
	}
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


	rcstar.top = 200;
	rcstar.bottom = rcstar.top + 356;
	rcstar.left = 1200;
	rcstar.right = rcstar.left + 356;
	
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(m_itimer, D2D1::Point2F(rcstar.left + 356 * 0.5, rcstar.top + 356 * 0.5)));
	pRenderTarget->DrawBitmap(m_pImagehosi, rcstar, m_fbord, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


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

