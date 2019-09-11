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
#include <windows.h>
#include <mmsystem.h>

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

	m_fPresentmove_x = 0;
	m_fPresentmove_y = 0;
	m_bPresentFlag = false;
	m_fPresentNum = 0;
	m_fVX = 1;
	m_fPTimer = 0;
	m_bBound = false;
	m_fsize = 1;
	m_ftitleX = 0;
	m_btype = true;

	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\taitoru.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\seiya.png"), &m_pImageSeiya);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa1.png"), &m_pImageSanta1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\santa2.png"), &m_pImageSanta2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\shadow.png"), &m_pImageShadow);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\light.png"), &m_pImageLight);

		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present1.png"), &m_pImagePresent1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present2.png"), &m_pImagePresent2);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\presentbox.png"), &m_pImagePresentBox);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\presentbag.png"), &m_pImagePresentBag);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\fukuro.png"), &m_pImagePresentBagO);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\start.png"), &m_pImageSTART);


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

	SAFE_RELEASE(m_pImagePresent1);
	SAFE_RELEASE(m_pImagePresent2);
	SAFE_RELEASE(m_pImagePresentBox);
	SAFE_RELEASE(m_pImagePresentBag);
	SAFE_RELEASE(m_pImagePresentBagO);
	SAFE_RELEASE(m_pImageSTART);
}


GameSceneResultCode    CTitle::move() {
	switch (m_ePhase) {
	case TITLE_INIT:
		mciSendString(L"open TitleBGM.mp3 type mpegvideo alias BGM01", NULL, 0, NULL);
		mciSendString(L"play BGM01 from 0 repeat", NULL, 0, NULL);
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
		if (m_iTimer == 1 && !m_bSantaFlag)
			m_bPresentFlag = false;


		if (!m_bPresentFlag) {
			if (!m_bBound) {
				m_fPTimer += 0.2;
				float V0 = -100;
				float g = 4.9f;
				m_fPresentmove_y = V0 * m_fPTimer + g * m_fPTimer*m_fPTimer;
				m_fPresentmove_x += m_fVX * 4;
				if (m_iTimer == 20 && 20 < m_fPresentmove_y) {
					m_bBound = !m_bBound;
					m_fPTimer = 0;
				}
			}
			else {
				m_fsize -= 0.004;
				m_fPTimer += 0.2;
				float V0 = -100;
				float g = 4.9f;
				m_fPresentmove_y = V0 * m_fPTimer + g * m_fPTimer*m_fPTimer;
				m_fPresentmove_x += m_fVX * 8;
				if (1312 < m_fPresentmove_x) {
					m_fPresentNum += 0.03f;
					m_bBound = !m_bBound;
					m_fPTimer = 0;
					m_fPresentmove_x = 0;
					m_fsize = 1;
					m_bPresentFlag = !m_bPresentFlag;
					m_btype = !m_btype;
				}
			}
		}
		if (0.7 < m_fPresentNum) {
			m_ftitleX = -1000;
		}
		if (0.9 < m_fPresentNum) {
			return GameSceneResultCode::GAMESCENE_END_TIMEOUT;
		}

		if (GetAsyncKeyState(VK_SPACE)) {
			if (!m_bFlag) {
				
				mciSendString(L"stop all", NULL, 0, NULL);
				sndPlaySound(L"op.wav", SND_ASYNC);
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
	//”wŒi
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	src.left = 1300 + m_ftitleX;
	src.top = -20;
	src.right = src.left + 2268*0.4;
	src.bottom = src.top + 756*0.4;
	//¯–é‚Ì•¶Žš
	pRenderTarget->DrawBitmap(m_pImageSeiya, src, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	//¯‚ÌŒõ
	D2D1_RECT_F rclight;
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
	
	//ƒvƒŒƒ[ƒ“ƒgŠÖŒW
	D2D1_RECT_F rcPresent, rcPresentBox, rcPresentBag;
	rcPresent.left = 350 + m_fPresentmove_x;
	rcPresent.top = 450 + m_fPresentmove_y;
	rcPresent.right = rcPresent.left + (378 * 0.5f) * m_fsize;
	rcPresent.bottom = rcPresent.top + (378 * 0.5f) * m_fsize;

	if (m_btype) {		//ŒÂ•ÊƒvƒŒƒ[ƒ“ƒgEÂ
		pRenderTarget->DrawBitmap(m_pImagePresent2, rcPresent, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	else {		//ŒÂ•ÊƒvƒŒƒ[ƒ“ƒgEÔ
		pRenderTarget->DrawBitmap(m_pImagePresent1, rcPresent, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	//ƒvƒŒƒ[ƒ“ƒg‚ÌŽR
	rcPresentBox.left = -5;
	rcPresentBox.top = 50;
	rcPresentBox.right = rcPresentBox.left +  827 * 0.8f;
	rcPresentBox.bottom = rcPresentBox.top + 1181 * 0.8f;
	pRenderTarget->DrawBitmap(m_pImagePresentBox, rcPresentBox, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	//ƒvƒŒƒ[ƒ“ƒg‘Ü
	rcPresentBag.right = 1880 +(100*m_fPresentNum);
	rcPresentBag.bottom = 1000+(100*m_fPresentNum);
	rcPresentBag.left = rcPresentBag.right - 709 * (0.4f + m_fPresentNum);
	rcPresentBag.top = rcPresentBag.bottom - 945 * (0.4f + m_fPresentNum);

	if(!m_bFlag) {
		pRenderTarget->DrawBitmap(m_pImagePresentBagO, rcPresentBag, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}else	{
		pRenderTarget->DrawBitmap(m_pImagePresentBag, rcPresentBag, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}

	//startƒƒS
	D2D1_RECT_F rcstart;
	rcstart.left = 1420;
	rcstart.top = 850;
	rcstart.right = rcstart.left + 500;
	rcstart.bottom = rcstart.top + 100;
	pRenderTarget->DrawBitmap(m_pImageSTART, rcstart, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);


	//ƒTƒ“ƒ^‚Æ‚»‚Ì‰e
	D2D1_RECT_F rcS;
	rcS.left = 520;
	rcS.top = 850;
	rcS.right = rcS.left + 900;
	rcS.bottom = rcS.top + 284;
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