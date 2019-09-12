#include "stdafx.h"
#include <d2d1.h>
#include "tama.h"
#include "TextureLoader.h"
#include <math.h>
#include "GameData.h"

ID2D1Bitmap *CTama::m_pBitmap = NULL;
ID2D1Bitmap *CTama::m_pBitmap2 = NULL;
ID2D1Bitmap *CTama::m_pBitmap3 = NULL;
ID2D1Bitmap *CTama::m_pBitmap4 = NULL;
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
	
	if (TreeScore <=0) {
		D2D1_RECT_F rc;
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + Bitmap01sizeX;
		rc.bottom = rc.top + Bitmap01sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	}
	if (TreeScore >= 1) {
		D2D1_RECT_F rc;
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + Bitmap01sizeX;
		rc.bottom = rc.top + Bitmap01sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap2, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	}
	if (TreeScore >= 2) {
		D2D1_RECT_F rc;
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + Bitmap01sizeX;
		rc.bottom = rc.top + Bitmap01sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap3, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	}
	if (TreeScore >= 3) {
		D2D1_RECT_F rc;
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + Bitmap01sizeX;
		rc.bottom = rc.top + Bitmap01sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap4, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	}

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
	float bottom = m_fY + Bitmap01sizeY;//左上＋画像高
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

	if (x < senter_x && senter_x < x + w) {
		m_fTreeScore *= (32.f - fabsf(senter_x - StarSenterx)) / 32.f;
		int val = m_fTreeScore;
		m_fTreeScore = val;	
	}
	else {
		if(!GameData::ProteinFlag)
			m_fTreeScore = 0;
	}
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
	m_bDamage = true;
}

float CTama::TSPointBackx()
{
	return m_fX+ Bitmap01sizeX*0.5f;
}

float CTama::TSPointBacky()
{
	return m_fY;
}


float CTama::ScoreBack()
{
	return m_fTreeScore;
}

bool CTama::StarHitFlag()
{
	if(m_bDamage)
		return true;
	return false;
}

/*********************************************************
*@fn
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする->したかった・・・
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pBitmap2);
	SAFE_RELEASE(m_pBitmap3);
	SAFE_RELEASE(m_pBitmap4);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\tree.png"), &m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\tree2.png"), &m_pBitmap2);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\tree3.png"), &m_pBitmap3);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\tree4.png"), &m_pBitmap4);
}

/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pBitmap2);
	SAFE_RELEASE(m_pBitmap3);
	SAFE_RELEASE(m_pBitmap4);
}