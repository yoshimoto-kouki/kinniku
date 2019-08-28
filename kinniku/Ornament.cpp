#include "stdafx.h"
#include <d2d1.h>
#include "PlayGame.h"
#include "Ornament.h"
#include "TextureLoader.h"
#include "Player.h"

ID2D1Bitmap *COrnament::m_pTexture = NULL;

/****************************************************
*@method
*  コンストラクタ
*@param in x  登場位置のX座標
*@param in y  登場位置のY座標
***************************************************/
COrnament::COrnament(CStage *pStage, float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVY = 1.5f;

	m_bDamage = false;
}


COrnament::~COrnament()
{
}

/****************************************************
*@method
*  move
*@return true: 生存 / false: 死亡
***************************************************/
bool COrnament::move() {
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

void COrnament::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + 64;
	rc.bottom = rc.top + 64;
	pRenderTarget->DrawBitmap(m_pTexture, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

bool COrnament::collide(float x, float y, float w, float h) {
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

bool COrnament::collide(IGameObject *pObj) {

	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, 64, 64);
}
void COrnament::hit(float amount) {
	m_bDamage = true;
}
bool COrnament::make() {
	return false;
}

/*********************************************************
*@fn
*  共有メディアファイルを読み込む
*  シーン開始時などに呼び出すようにする
*********************************************************/
void COrnament::Restore(ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\ornament.png"), &m_pTexture);
}

/*********************************************************
*@fn
*  共有メディアファイルの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void COrnament::Finalize() {
	SAFE_RELEASE(m_pTexture);
}