#include "stdafx.h"
#include <d2d1.h>
#include "PlayGame.h"
#include "Candycane.h"
#include "TextureLoader.h"
#include "Player.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


ID2D1Bitmap *CCandy::m_pTexture = NULL;
CStage *CCandy::m_pParent = NULL;

/****************************************************
*@method
*  コンストラクタ
*@param in x  登場位置のX座標
*@param in y  登場位置のY座標
***************************************************/
CCandy::CCandy(float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVY = 1.5f;
	m_bDamage = false;
}


CCandy::~CCandy()
{
}

/****************************************************
*@method
*  move
*@return true: 生存 / false: 死亡
***************************************************/
bool CCandy::move() {
	m_fY += m_fVY;
	if (m_fVY < 0) {
		if (m_fY < -64)
			return  false;    //    画面外に出たら終了
	}
	else {
		if (m_fY > 1080)
			return  false;    //    画面外に出たら終了
	}
	if (m_bDamage)
		return false;
	return    true;

}

void CCandy::draw(ID2D1RenderTarget *pRenderTarget) {
	int    ix, iy;
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + 64;
	rc.bottom = rc.top + 64;
	pRenderTarget->DrawBitmap(m_pTexture, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

bool CCandy::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float right = m_fX + 64;
	float top = m_fY;
	float bottom = m_fY + 64;
	if (top > (y + h))
		return false;
	if (bottom < y)
		return false;
	if (left > (x + w))
		return false;
	if (right < x)
		return false;
	return true;
}

bool CCandy::collide(IGameObject *pObj) {

	float l = m_fX ;
	float t = m_fY ;
	return pObj->collide(l, t, 64, 64);
}
void CCandy::hit(float amount) {
	if (amount != 1.0f) {
		sndPlaySound(L"item.wav", SND_ASYNC);
		m_bDamage = true;
	}
}

/*********************************************************
*@fn
*  共有メディアファイルを読み込む
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CCandy::Restore(CStage *pStage, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\candy.png"), &m_pTexture);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  共有メディアファイルの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CCandy::Finalize() {
	SAFE_RELEASE(m_pTexture);
	m_pParent = NULL;
}