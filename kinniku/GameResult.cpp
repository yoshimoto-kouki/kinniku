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


#define GAME_CLEAR_LINE 50000

CGameResult::CGameResult(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = GameResultPhase::GAMERESULT_INIT;
	m_pImage = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;

	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\playgamen_sukoa.jpg"), &m_pImage);
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\digit.png"), &m_pImagedigit);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;

	}
}


CGameResult::~CGameResult()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImagedigit);
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

		if (GetAsyncKeyState(VK_SPACE)) {
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
		if (m_iFadeTimer < 40)
			break;
		m_ePhase = GAMERESULT_DONE;
	case GAMERESULT_DONE:
		//****************************
		/*ここの条件式でゲームクリアとゲームオーバー変更*/
		if (GameData::TotalScoreResult > GAME_CLEAR_LINE) {
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

	int val = GameData::TotalScoreResult;

	D2D1_RECT_F rcd,drc;
	rcd.left = 1400;
	rcd.top = 500;
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

