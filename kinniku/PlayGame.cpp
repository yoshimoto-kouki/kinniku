#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "PlayGame.h"
#include "TextureLoader.h"
#include "Selecter.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")


CStage::CStage(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_ePhase = StagePhase::STAGE_INIT;
	m_pImage = NULL;
	m_pSystem = pSystem;
	m_pBlack = NULL;
	mX = 0;
	mY = 100;
	mPhase = 0;
	mTimer = 0;
	renda = 0;
	int count = 0;


	m_bFlag = true;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\smile2.tga"), &m_pImage);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
}


CStage::~CStage()
{
	SAFE_RELEASE(m_pImage);
}

GameSceneResultCode    CStage::move() {
	switch (m_ePhase) {
	case STAGE_INIT:
		m_iTimer = 0;
		m_ePhase = STAGE_RUN;
	case STAGE_RUN:
	{
		bool bDone = false;
		++m_iTimer;

		if (GetAsyncKeyState(VK_SPACE)) {
			if (!m_bFlag) {
				mX += 2;
				sndPlaySound(L"test.wav", SND_ASYNC);

				renda++;
				m_bFlag = true;


			}
		}
		else {
			m_bFlag = false;
		}

		if (m_iTimer > 300) {

			bDone = true;
		}

		if (bDone) {
			m_iFadeTimer = 0;
			renda = CSelector::a;
			;
			m_ePhase = STAGE_FADE;
		}
		break;
	}
	case STAGE_FADE:
		m_iFadeTimer++;
		if (m_iFadeTimer < 30)
			break;
		m_ePhase = STAGE_DONE;
	case STAGE_DONE:
		return GameSceneResultCode::GAMESCENE_END_OK;
	}
	mFrame = (mFrame + 1) % ANIM_FRAME;
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
void    CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc, src;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();

	float tx, range = 192.0f;



	tx = 0;
	if (mFrame > ANIM_FRAME - 16) {
		tx = (mFrame - (ANIM_FRAME - 16)) >> 1;    //  ‚Ì‚±‚è 16ƒtƒŒ[ƒ€‚ð‚Q‚ÅŠ„‚é
		tx *= 64.0f;
		if (tx > range) {
			tx = (range * 2.0f) - tx;
			if (tx < 0)
				tx = 0;
		}

	}


	rc.left = 0 + mX;
	rc.top = 0 + mX;
	rc.right = rc.left + 64 + mX;
	rc.bottom = rc.top + 64 + mX;




	src.left = tx;
	src.top = 0;
	src.right = src.left + 64;
	src.bottom = src.top + 64;


	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	switch (m_ePhase) {
	case STAGE_FADE:
	case STAGE_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;

		src.left = 0;
		src.top = 0;
		src.right = src.left + 64;
		src.bottom = src.top + 64;

		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);
	}

}
