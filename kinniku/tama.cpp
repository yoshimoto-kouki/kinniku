#include "stdafx.h"
#include <d2d1.h>
#include "tama.h"
#include "TextureLoader.h"
#include <math.h>

ID2D1Bitmap *CTama::m_pBitmap = NULL;
#define Bitmap01sizeX 100//25
#define Bitmap01sizeY 180//34


CTama::CTama(CStage *pStage, float x, float y,int Decoration)
{
	m_pParent = pStage;
	TreeScore = Decoration;
 	m_fX = x - Bitmap01sizeX * 0.5;
	m_fY = y - Bitmap01sizeY * 0.5;
	m_fVX = 0;
	m_fVY = -3.0f;
	m_bDamage = false;
	coliPosx = 0;
	coliPosy = 0;
	m_fTreeScore = 100;
}

CTama::~CTama()
{
}


bool CTama::move() {
	if (m_bDamage)
		return false;//Tamaが消える理由。
	m_fY += m_fVY;
	if (m_fY < -110) //画面上部へ向かって-yして飛んでいくため
		return    false;
	return    true;
}

void CTama::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + Bitmap01sizeX;
	rc.bottom = rc.top + Bitmap01sizeY;
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
	float center = x + w * 0.5;//弾に当たる画像の中心
	float left = m_fX;
	float top = m_fY;
	float right = m_fX + Bitmap01sizeX; //左上＋画像幅
	float bottom = m_fY + Bitmap01sizeY*0.6;//左上＋画像高
	if (top > (y + h))
		return false;
	if (bottom < y)
		return false;
	if (left > center)
		return false;
	if (right < center)
		return false;
	return true;
}
bool CTama::collide(float x,float w)
{
	float senter_x = m_fX + (Bitmap01sizeX * 0.5f);
	float StarSenterx = x + (w * 0.5f);
	m_fTreeScore *= (32.f - fabsf(senter_x - StarSenterx)) / 32.f;
	
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
	return pObj->collide(l, t, Bitmap01sizeX,Bitmap01sizeY );//数値は画像幅高
}


void CTama::hit(float amount) {
	if (1.0f != amount)//特殊処理Starであるとき
		m_bDamage = true;//Tamaが消える理由。
}

bool CTama::collidePos(float x, float y)
{
	coliPosx = x;
	coliPosy = y;
	return false;
}

/*********************************************************
*@fn
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする->したかった・・・
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot2.png"), &m_pBitmap);
}

/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
}