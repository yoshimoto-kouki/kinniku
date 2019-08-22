#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "PlayGame.h"
#include "TextureLoader.h"
#include "Selecter.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#include "ScoreUI.h"
#include "BG.h"
 
#pragma comment(lib,"winmm.lib")


CStage::CStage(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget,*pTarget2,*pRenderTarget;
	m_iFadeTimer = 0;
	m_pSystem = pSystem;
	pRenderTarget = m_pSystem->GetRenderTaget();
	m_ePhase = StagePhase::STAGE_INIT;
	m_pImage = NULL;
	m_pBlack = NULL;
	mX = 0;
	mY = 100;
	mPhase = 0;
	mTimer = 0;
	renda = 0;
	int count = 0;
	m_pBG = new CBG(pRenderTarget);//背景
	m_pScore = new CScoreUI(pRenderTarget);//UI
	if (m_pScore) {//スコア用
		m_pScore->SetScore(0);
	}

	m_bFlag = true;
	pTarget = pSystem->GetRenderTaget();
	if (pTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\smile2.tga"), &m_pImage);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
	m_bFlag = true;
	pTarget2 = pSystem->GetRenderTaget();
	if (pTarget2) {
		CTextureLoader::CreateD2D1BitmapFromFile(pTarget2, _T("res\\tama.png"), &m_pImage2);
		pTarget2->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
		pTarget2->Release();
		pTarget2 = NULL;
	}
	SAFE_RELEASE(pRenderTarget);
}
class PLAYER {
private:
	//x座標,y座標
	double x, y;

	//画像幅
	int width, height;

	//グラフィックハンドル格納用配列
	int gh[12];


	//移動係数
	float move;

	//横方向と縦方向のカウント数。
	int xcount, ycount;
	//添字用変数
	int ix, iy, result;

	//生きてるかどうかのフラグ
	bool life;

	//弾
	SHOT shot[PSHOT_NUM];

	//カウント
	int count;

private:
	void Move();
	void Draw();
	void Shot();

public:
	PLAYER();
	void All();

};



CStage::~CStage()
{
	SAFE_RELEASE(m_pImage);
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pScore);
}
/*********
アニメーション処理
***********/
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
			if (!GameData::shot) {
				
				GameData::shot = true;
				GameData::tamax = 430;
				GameData::tamay = 700;
				//**得点加算用***************
				if (m_pScore)
					m_pScore->AddScore(5);
				//**************************
			}
			break;
		}
		/*else {
			m_bFlag = false;
		}*/
		if (GameData::shot) {
			GameData::tamay -= 16;
			if (GameData::tamay = 0)m_bFlag = false;
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
		//------------------------------------------------
		if (m_pScore) //スコア用
			m_pScore->move();    //  一応呼んでおく
		//------------------------------------------------
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
/****************
レンタリング(描画)
****************/
void    CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc, src,tama;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage->GetSize();
	float tx, range = 192.0f;
	tx = 0;
	if (mFrame > ANIM_FRAME - 16) {
		tx = (mFrame - (ANIM_FRAME - 16)) >> 1;    //  のこり 16フレームを２で割る
		tx *= 64.0f;
		if (tx > range) {
			tx = (range * 2.0f) - tx;
			if (tx < 0)
				tx = 0;
		}
	}
	rc.left = 400;
	rc.top = 750;
	rc.right = rc.left + 64 ;
	rc.bottom = rc.top + 64 ;


	src.left = tx;
	src.top = 0;
	src.right = src.left + 64;
	src.bottom = src.top + 64;

	tama.left = 430;
	tama.top = 700;
	tama.right = rc.left + 64;
	tama.bottom = rc.top + 64;
	if (GameData::shot) {
		tama.left = GameData::tamax;
		tama.top = GameData::tamay;
		tama.right = rc.left + 64;
		tama.bottom = rc.top + 64;
	}
	//-------------------------------------------
	if (m_pBG)//背景用
		m_pBG->draw(pRenderTarget);
	if (m_pScore)//スコア用
		m_pScore->draw(pRenderTarget, 1100.0f, 10.0f, 32.0f);
	//----------------------------------------------
	pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	pRenderTarget->DrawBitmap(m_pImage2, tama, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);


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
