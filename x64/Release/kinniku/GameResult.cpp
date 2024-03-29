#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include "GameResult.h"
#include "TextureLoader.h"
#include "PlayGame.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


CGameResult::CGameResult(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = GameResultPhase::GAMERESULT_INIT;
	m_pImageBG = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;
	m_itimer = 0;
	m_fpY = 1;
	m_fY = 0;
	m_fpX = 0;
	m_iFTimer = 0;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\playgamen_sukoa.jpg"), &m_pImageBG);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\result.png"), &m_pImageRESULT);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\clearline.png"), &m_pImageCLEAR);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\push.png"), &m_pImagePUSH);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\digit.png"), &m_pImagedigit);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\presentbag.png"), &m_pImageBag);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present1.png"), &m_pImagePresent1);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\present2.png"), &m_pImagePresent2);

		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;

	}
}


CGameResult::~CGameResult()
{
	SAFE_RELEASE(m_pImageBG); 
	SAFE_RELEASE(m_pImageRESULT);
	SAFE_RELEASE(m_pImageCLEAR);
	SAFE_RELEASE(m_pImagePUSH);
	SAFE_RELEASE(m_pImagedigit);
	SAFE_RELEASE(m_pImageBag);
	SAFE_RELEASE(m_pImagePresent1);
	SAFE_RELEASE(m_pImagePresent2);
}

GameSceneResultCode CGameResult::move() {
	switch (m_ePhase) {

	case GAMERESULT_INIT:
	
		m_iTimer = 0;
		m_bFlag = true;

		m_ePhase = GAMERESULT_RUN;

	case GAMERESULT_RUN:
	{
		bool bDone = false;
		++m_iTimer;
		if (m_fpY < 1000) {
			m_fpY += 3;
		}
		m_itimer++;
		if (20 <= m_itimer)
			m_itimer = 0;
		m_fY = -14 * m_itimer + 0.7*m_itimer * m_itimer;



		if (GetAsyncKeyState(VK_SPACE) || 930 < m_iTimer) {
			m_fY = 0;
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
			m_ePhase = GAMERESULT_FADE;
		}
		break;
	}
	case GAMERESULT_FADE:
		m_iFadeTimer++;
		if (1300 < m_fpX) {
			m_iFTimer++;
		}
		if (m_iFadeTimer < 40)
			break;
		m_ePhase = GAMERESULT_DONE;
	case GAMERESULT_DONE:
		//****************************
		/*ここの条件式でゲームクリアとゲームオーバー変更*/
		if (GameData::TotalScoreResult > GameData::CLEAR_LINE) {
			return GameSceneResultCode::GAMESCENE_END_OK;
		}
		else {
			return GameSceneResultCode::GAMESCENE_END_OK2;
		}
		//***************************
	}
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
void    CGameResult::draw(ID2D1RenderTarget *pRenderTarget) {

	//背景
	int frc = 0;
	D2D1_RECT_F rc,rcr, prc;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImageBG->GetSize();
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	pRenderTarget->DrawBitmap(m_pImageBG, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	//result文字
	rcr.left = 1300 - m_fpY;
	rcr.top = 200;
	rcr.right = rcr.left + 312;
	rcr.bottom = rcr.top + 64;
	pRenderTarget->DrawBitmap(m_pImageRESULT, rcr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	//clear line　文字
	rcr.left = 1300 - m_fpY;
	rcr.top = 700;
	rcr.right = rcr.left + 491;
	rcr.bottom = rcr.top + 64;
	pRenderTarget->DrawBitmap(m_pImageCLEAR, rcr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	rcr.left = 1100;
	rcr.top = -800 + m_fpY;
	rcr.bottom = rcr.top + 64;
	rcr.right = rcr.left + 234;
	if (1000 <= m_fpY)
		rcr.right = rcr.left + 502;

	prc.top = 0;
	prc.bottom = prc.top + 64;
	prc.left = 0;
	prc.right = prc.left + 234;
	if (1000 <= m_fpY)
		prc.right = prc.left + 502;
	pRenderTarget->DrawBitmap(m_pImagePUSH, rcr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, prc);
	//スコア
	int val = GameData::TotalScoreResult;

	D2D1_RECT_F rcd,drc;
	rcd.left = 2250 - m_fpY;
	rcd.top = 350;
	rcd.bottom = rcd.top + 128;
	while (0 < val) {
		rcd.right = rcd.left + 128;

		drc.left = (val % 10) % 4 * 32;
		drc.top = (val % 10) / 4 * 32;
		drc.right = drc.left + 32;
		drc.bottom = drc.top + 32;
		pRenderTarget->DrawBitmap(m_pImagedigit, rcd, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, drc);
		val /= 10;
		rcd.left -= 128;
	}

	int clear = GameData::CLEAR_LINE;
	D2D1_RECT_F rcc;
	rcc.left = 2100 - m_fpY;
	rcc.top = 750;
	rcc.bottom = rcc.top + 128;
	while (0 < clear) {
		rcc.right = rcc.left + 128;

		drc.left = (clear % 10) % 4 * 32;
		drc.top = (clear % 10) / 4 * 32;
		drc.right = drc.left + 32;
		drc.bottom = drc.top + 32;
		pRenderTarget->DrawBitmap(m_pImagedigit, rcc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, drc);
		clear /= 10;
		rcc.left -= 128;
	}

	//Present & bag
	
	D2D1_RECT_F rct, rcp, rcp2;
	rct.left = 1300;
	rct.top = 400;
	if (m_iFTimer != 0) {
		rct.left = 1300 + ((1500 - m_fpY) / 10) * m_iFTimer;
		rct.top = 400 + m_fY - m_iFTimer * 7;
	}
	rct.right = rct.left + 600;
	rct.bottom = rct.top + 600;
	pRenderTarget->DrawBitmap(m_pImageBag, rct, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	rcp.left = 1600;
	rcp.top = 900;
	if (m_iFTimer != 0) {
		rcp.left = 1600 + ((1500 - m_fpY) / 10) * m_iFTimer;
		rcp.top = 900 + m_fY - m_iFTimer * 7;
	}
	rcp.right = rcp.left + 100;
	rcp.bottom = rcp.top + 100;
	pRenderTarget->DrawBitmap(m_pImagePresent1, rcp, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	
	if (GameData::TotalScoreResult > GameData::CLEAR_LINE) {
		rcp2.left = 1700 - m_fpY * 1.5;
		rcp2.top = 900 - m_iFadeTimer * 2 + m_fY;
		if (m_iTimer < 930 && m_iFadeTimer != 0) {
			rcp2.left = 1700 - m_fpY * 1.5 - ((1500 - m_fpY) / 30)*m_iFadeTimer;
			if (rcp2.left < 200)
				rcp2.left = 200;
		}
	}
	else {
		rcp2.left = 1700 - m_fpY * 1.5;
		rcp2.top = 900 + m_fY;
		if (m_iTimer < 930 && m_iFadeTimer != 0) {
			rcp2.left = 1700 - m_fpY * 1.5 + ((1500 - m_fpY) / 10) * m_iFadeTimer;
			rcp2.top = 900 + m_fY - m_iFadeTimer * 7;
			m_fpX = rcp2.left;
		}
	}	
	rcp2.right = rcp2.left + 170;
	rcp2.bottom = rcp2.top + 100;

	pRenderTarget->DrawBitmap(m_pImagePresent2, rcp2, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);



	switch (m_ePhase) {
	case GAMERESULT_FADE:
	case GAMERESULT_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);
	}
}

