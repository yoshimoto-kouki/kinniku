#include "stdafx.h"
#include <d2d1.h>
#include "tama.h"
#include "TextureLoader.h"

CStage *CTama::m_pParent = NULL;
ID2D1Bitmap *CTama::m_pBitmap = NULL;

CTama::CTama(CStage *pStage, float x, float y)
{
	m_pParent = pStage;
 	m_fX = x;
	m_fY = y;
	m_fVX = 0;
	m_fVY = -3.0f;
}

CTama::~CTama()
{
	SAFE_RELEASE(m_pBitmap);
}


bool CTama::move() {
	m_fY += m_fVY;
	if (m_fY < -1000)
		return    false;
	return    true;
}

void CTama::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	D2D1_SIZE_F size, Ssize;
	if (m_pBitmap == NULL)
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\shot.png"), &m_pBitmap);
	size = m_pBitmap->GetSize();
	Ssize = pRenderTarget->GetSize();
	rc.left = Ssize.width * 0.35 - size.width + m_fX;
	rc.top = Ssize.height - size.height - 30 + m_fY;
	rc.right = rc.left + size.width;
	rc.bottom = rc.top + size.height;
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
	return false;
}
/************************************************************
*@method
*  他オブジェクト当たり判定
*@param in pObj　相手オブジェクト
*@return true : 当たり / false : あたってない
************************************************************/
bool CTama::collide(IGameObject *pObj) {
	return false;
}


//void CTama::damage(float amount) {}

/*********************************************************
*@fn
*  共有メディアデータの生成
*  シーン開始時などに呼び出すようにする
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot.png"), &m_pBitmap);
//	m_pParent = pStage;
}

/*********************************************************
*@fn
*  共有メディアデータの消去
*  シーン削除時などに呼び出すようにする
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
}