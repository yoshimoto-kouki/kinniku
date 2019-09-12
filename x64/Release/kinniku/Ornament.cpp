#include "stdafx.h"
#include <d2d1.h>
#include "PlayGame.h"
#include "Ornament.h"
#include "TextureLoader.h"
#include "Player.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


ID2D1Bitmap *COrnament::m_pTexOrnamentR = NULL;
ID2D1Bitmap *COrnament::m_pTexOrnamentRD = NULL;
ID2D1Bitmap *COrnament::m_pTexOrnamentB = NULL;
ID2D1Bitmap *COrnament::m_pTexOrnamentBD = NULL;
ID2D1Bitmap *COrnament::m_pTexOrnamentG = NULL;
ID2D1Bitmap *COrnament::m_pTexOrnamentGD = NULL;
CStage *COrnament::m_pParent = NULL;

/****************************************************
*@method
*  コンストラクタ
*@param in x  登場位置のX座標
*@param in y  登場位置のY座標
***************************************************/
COrnament::COrnament(float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVY = 1.5f;
	m_fOType = (int)rand() % 5;
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
	switch (m_fOType) {
	case 0:
		pRenderTarget->DrawBitmap(m_pTexOrnamentR, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 1:
		pRenderTarget->DrawBitmap(m_pTexOrnamentRD, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 2:
		pRenderTarget->DrawBitmap(m_pTexOrnamentB, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 3:
		pRenderTarget->DrawBitmap(m_pTexOrnamentBD, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 4:
		pRenderTarget->DrawBitmap(m_pTexOrnamentG, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	case 5:
		pRenderTarget->DrawBitmap(m_pTexOrnamentGD, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
		break;
	}
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
void COrnament::Restore(CStage *pStage, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pTexOrnamentR);
	SAFE_RELEASE(m_pTexOrnamentRD);
	SAFE_RELEASE(m_pTexOrnamentB);
	SAFE_RELEASE(m_pTexOrnamentBD);
	SAFE_RELEASE(m_pTexOrnamentG);
	SAFE_RELEASE(m_pTexOrnamentGD);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\akairo.png"), &m_pTexOrnamentR);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\akairo2.png"), &m_pTexOrnamentRD);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\aoiro.png"), &m_pTexOrnamentB);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\aoiro2.png"), &m_pTexOrnamentBD);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\midoriiro.png"), &m_pTexOrnamentG);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\midoriiro2.png"), &m_pTexOrnamentGD);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  共有メディアファイルの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void COrnament::Finalize() {
	SAFE_RELEASE(m_pTexOrnamentR);
	SAFE_RELEASE(m_pTexOrnamentRD);
	SAFE_RELEASE(m_pTexOrnamentB);
	SAFE_RELEASE(m_pTexOrnamentBD);
	SAFE_RELEASE(m_pTexOrnamentG);
	SAFE_RELEASE(m_pTexOrnamentGD);
	m_pParent = NULL;
}