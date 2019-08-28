#include "stdafx.h"
#include <d2d1.h>
#include "tama.h"
#include "TextureLoader.h"

ID2D1Bitmap *CTama::m_pBitmap = NULL;

CTama::CTama(CStage *pStage, float x, float y)
{
	m_pParent = pStage;
 	m_fX = x;
	m_fY = y;
	m_fVX = 0;
	m_fVY = -3.0f;
	m_bDamage = false;
}

CTama::~CTama()
{
}


bool CTama::move() {
	m_fY += m_fVY;
	if (m_fY < -1000)//画面上部へ向かって-yして飛んでいくため
		return    false;
	if (m_bDamage)
		return false;
	return    true;
}

void CTama::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + 25;
	rc.bottom = rc.top + 34;
	pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

/************************************************************
*@method
*  矩形との当たり判定
*@param in x 左上のX座標
*@param in y 左上のY座標
*@param in w 矩形の幅
*@param in h 矩形の高
*@return true : 当たり / false : あたってない
************************************************************/
bool CTama::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float top = m_fY;
	float right = m_fX + 25; //左上＋画像幅
	float bottom = m_fY + 34;//左上＋画像高
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
/************************************************************
*@method
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
************************************************************/
bool CTama::collide(IGameObject *pObj) {
	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, 25,34 );//数値は画像幅高
}


void CTama::hit(float amount) {
	m_bDamage = true;
}

/*********************************************************
*@fn
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする->したかった・・・
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot.png"), &m_pBitmap);
}

/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
}