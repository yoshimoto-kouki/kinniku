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

#define TexsizeHalf 30
#define PTexsizeHalf 32


CPlayer::CPlayer(CStage *pStage)
{
	ID2D1RenderTarget *pRenderTarget = NULL;
	m_pBitmapP = NULL;
	
	m_bTama = true;
	m_fX = 510.f;
	m_fY = 700.f;
	m_fKeyTime = 0;
	m_pStage = pStage;
	m_bLongPushSpace = false;
	//  CSelector が所有しているID2D1RenderTarget を、
	//  CStage からまた借りする
	pRenderTarget = pStage->GetRenderTarget();
	if (pRenderTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\smilie.tga"), &m_pBitmapP);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\charge.png"), &m_pBitmapC);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\shotS.png"), &m_pBitmapNT);
		pRenderTarget->Release();    //  Release して解放
	}

#ifdef _DEBUG
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::IndianRed), &m_pBrush);
	SAFE_RELEASE(pRenderTarget);
#endif // DEBUG
}


CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pBitmapP);
	SAFE_RELEASE(m_pBitmapC);
	SAFE_RELEASE(m_pBitmapNT);
	SAFE_RELEASE(m_pBitmapCT);
#ifdef _DEBUG
	SAFE_RELEASE(m_pBrush);
#endif // _DEBUG
}

/********************************************************
 *@method
 *  １フレームのアニメーション処理
 *@return true : 生存 / false : 死亡
 *******************************************************/
bool CPlayer::move() {

	if (GetAsyncKeyState(VK_LEFT))
		m_fX -= 10.0f;
	if (GetAsyncKeyState(VK_RIGHT))
		m_fX += 10.0f;
	
	if (m_fX > 1015)//right
		m_fX = 1015;
	if (m_fX < -5)//left
		m_fX = -5;
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
		m_fKeyTime += 1;
		m_bLongPushSpace = true;
	}
	else {
		m_bLongPushSpace = false;
		m_bTama = false;
	}
	//-----モミの木チャージ判定------
	if (!GameData::ProteinFlag) {//プロテインバーストしてないとき
		if (!m_bLongPushSpace) {
			if (0 < m_fKeyTime) {
				if (m_fKeyTime < 30) {
					if (!m_bTama) {
						IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalf, m_fY + TexsizeHalf);
						m_pStage->AddTama(pObj);
					}
					m_bTama = true;
					m_fKeyTime = 0;
				}
				else {
					if (!m_bTama) {
						IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalf+ PTexsizeHalf, m_fY + TexsizeHalf);
						m_pStage->AddTama(pObj);
						IGameObject *pObj2 = new CTama(m_pStage, m_fX - TexsizeHalf + PTexsizeHalf, m_fY + TexsizeHalf);
						m_pStage->AddTama(pObj2);
					}
					m_bTama = true;
					m_fKeyTime = 0;
				}
			}
		}
	}
	//-END----モミの木チャージ判定-------------

	//----Protein-------------
	if (GameData::ProteinFlag) {

		if (!m_bTama) {
			IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalf, m_fY + TexsizeHalf);
			m_pStage->AddTama(pObj);
		}

	}
	// END----Pritein-------------
	return true;
}

/********************************************************
 *@method
 *  描画処理
 *******************************************************/
void CPlayer::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc,rcT;
	D2D1_SIZE_F size, Ssize;
	if (m_pBitmapP == NULL)
		return;
	size = m_pBitmapP->GetSize();
	Ssize = pRenderTarget->GetSize();
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + size.width;
	rc.bottom = rc.top + size.height;
	pRenderTarget->DrawBitmap(m_pBitmapP, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	if (m_bLongPushSpace) {
		rc.left = m_fX - 20;
		rc.top = m_fY - 20;
		rc.right = rc.left + size.width + 40;
		rc.bottom = rc.top + size.height + 40;
		pRenderTarget->DrawBitmap(m_pBitmapC, rc, (m_fKeyTime / 30), D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		
		if (m_fKeyTime < 30) {
			rcT.left = m_fX + 40;
			rcT.top = m_fY + 30;
			rcT.right = rcT.left + 11;
			rcT.bottom = rcT.top + 17;
			pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		}
		else {
			rcT.left = m_fX + PTexsizeHalf * 2 - 5;
			rcT.top = m_fY + PTexsizeHalf * 2 - 10;
			rcT.right = rcT.left + 11;			
			rcT.bottom = rcT.top + 17;
			pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			rcT.left = m_fX - 5;
			rcT.right = rcT.left + 11;
			pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		}
	}

}
bool CPlayer::collide(float x, float y, float w, float h) {
	float l = m_fX ;
	float r = m_fX + PTexsizeHalf * 2;
	float t = m_fY ;
	float b = m_fY + PTexsizeHalf * 2;
	if (t > (y + h))
		return false;
	if (b < y)
		return false;
	if (l > (x + w))
		return false;
	if (r < x)
		return false;
	return true;
}
/************************************************************
*@method
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
************************************************************/
bool CPlayer::collide(IGameObject *pObj) {
	float l = m_fX ;
	float t = m_fY ;
	return pObj->collide(l, t, PTexsizeHalf*2, PTexsizeHalf*2);
}


void CPlayer::hit(float amount) {
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