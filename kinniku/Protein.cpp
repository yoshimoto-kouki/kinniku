#include "stdafx.h"
#include <d2d1.h>
#include "Protein.h"
#include "TextureLoader.h"
#include "GameData.h"

ID2D1Bitmap *CProtein::m_pTexture = NULL;
CStage *CProtein::m_pParent = NULL;
#define texX 64
#define texY 64
CProtein::CProtein(float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVX = 0;
	m_fVY = 3.0f;
	m_bDamage = false;
}


CProtein::~CProtein(){}


bool CProtein::move() {
	if (m_bDamage)
		return false;
	m_fY += m_fVY;
	if (m_fVY < 0) {
		if (m_fY < -64)
			return  false;    //    画面外に出たら終了
	}
	else {
		if (m_fY > 1080)
			return  false;    //    画面外に出たら終了
	}
	return    true;
}

void CProtein::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_SIZE_F size;
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + texX;
	rc.bottom = rc.top + texY;
	pRenderTarget->DrawBitmap(m_pTexture, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
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
bool CProtein::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float top = m_fY;
	float right = m_fX +  texX; //左上＋画像幅
	float bottom = m_fY + texY;//左上＋画像高
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
bool CProtein::collide(IGameObject *pObj) {//主にTamaとのあたり判定に使用しているため、fasleで実行しないようにする
/*
	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, texX, texY);//数値は画像幅高
*/
	return false;
}


void CProtein::hit(float amount) {
	if (amount != 1.0f) {
		m_bDamage = true;
		GameData::ProteinFlag = true;
	}
}

/*********************************************************
*@fn
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CProtein::Restore(CStage *pStage, ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\protein.png"), &m_pTexture);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CProtein::Finalize() {
	SAFE_RELEASE(m_pTexture);
	m_pParent = NULL;
}