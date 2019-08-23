#include "stdafx.h"
#include <d2d1.h>
#include "Player.h"
#include "PlayGame.h"
#include "TextureLoader.h"
#include "GameData.h"
#include "ScoreUI.h"
#include "tama.h"

#define GAMESTAGE_WIDE 1344
#define GAMESTAGE_HIGH 1080

CPlayer::CPlayer(CStage *pStage)
{
	ID2D1RenderTarget *pRenderTarget = NULL;
	m_pBitmapP = NULL;
	
	m_bTama = true;
	m_fX = 0.f;
	m_fY = 0.f;
	m_pStage = pStage;
	//  CSelector が所有しているID2D1RenderTarget を、
	//  CStage からまた借りする
	pRenderTarget = pStage->GetRenderTarget();
	if (pRenderTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\smilie.tga"), &m_pBitmapP);
		pRenderTarget->Release();    //  Release して解放
	}
}


CPlayer::~CPlayer()
{
	//SAFE_RELEASE(m_pRed);
	SAFE_RELEASE(m_pBitmapP);
}

/********************************************************
 *@method
 *  １フレームのアニメーション処理
 *@return true : 生存 / false : 死亡
 *******************************************************/
bool CPlayer::move() {

	if (GetAsyncKeyState(VK_LEFT))
		m_fX -= 3.0f;
	if (GetAsyncKeyState(VK_RIGHT))
		m_fX += 3.0f;
	m_fY = 0;
	/*********************************************
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!GameData::shot) {
			GameData::shot = true;
			GameData::tamax = 430;
			GameData::tamay = 700;
		}
	}
//	else {
//		m_bFlag = false;
//	}
	if (GameData::shot) {
		GameData::tamay -= 16;
		if (GameData::tamay = 0)m_bFlag = false;
	}
	***********************************************/ 
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!m_bTama) {
			IGameObject *pObj = new CTama(m_pStage, m_fX, m_fY);
			m_pStage->AddTama(pObj);
		}
		m_bTama = true;
	}
	else
		m_bTama = false;

	GameData::TreeConplete = true;//このフラグで得点加算
	
	return true;
}

/********************************************************
 *@method
 *  描画処理
 *******************************************************/
void CPlayer::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	D2D1_SIZE_F size,Ssize;
	if (m_pBitmapP == NULL)
		return;
	size = m_pBitmapP->GetSize();
	Ssize = pRenderTarget->GetSize();
	rc.left = Ssize.width * 0.35 - size.width + m_fX;
	rc.top = Ssize.height - size.height - 30;
	rc.right = rc.left + size.width;
	rc.bottom = rc.top + size.height;
	pRenderTarget->DrawBitmap(m_pBitmapP, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

}
/*
D2D1_RECT_F tama;
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

				//^^^^^^^^^^^^^^
			if (!GameData::shot) {

				GameData::shot = true;
				GameData::tamax = 430;
				GameData::tamay = 700;
				//^^^^^^^^^^^^^^^^^^^^^^^^^

			}		if (GameData::shot) {
				GameData::tamay -= 16;
				if (GameData::tamay = 0)m_bFlag = false;
			}



			struct SHOT {

bool flag;//弾が発射中かどうか

double x;//x座標

double y;//y座標

int gh;//グラフィックハンドル

int width, height;//画像の幅と高さ
};

	void Draw(ID2D1RenderTarget *pTarget);
	void SetPos(int x, int y);
*/