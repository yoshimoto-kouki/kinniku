/****************************************
*@file  ScoreUI.cpp
*  スコア表示用クラス CScoreUI の実装
****************************************/
#include "stdafx.h"
#include <d2d1.h>
#include <math.h>
#include "TextureLoader.h"
#include "ScoreUI.h"

//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

CScoreUI::CScoreUI(ID2D1RenderTarget *pRenderTarget)
{
	m_iScore = 0;
	m_pTexture = NULL;
	CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\digit.png"), &m_pTexture);
}


CScoreUI::~CScoreUI()
{
	SAFE_RELEASE(m_pTexture);
	m_iScore = NULL;
}

/*************************************************
 *@method
 *  スコアに値をセットする
 *@param in score   セットするスコア
 *************************************************/
void CScoreUI::SetScore(INT score) {
	m_iScore = score;
}
/*************************************************
*@method
*  スコアから値を読み出す
*@return  現在のスコア
*************************************************/
INT CScoreUI::GetScore() {
	return m_iScore;
}
/*************************************************
*@method
*  スコアに値を加算
*@param in value  加算する値
*************************************************/
void CScoreUI::AddScore(INT score) {
	if (score != 0) {
		score -= 1;
		int ScoreSum = pow(2, score);
		m_iScore += (100 + 100 * ScoreSum);
	}
}

/*************************************************
*@fn
*  スコア表示のアニメーション
*************************************************/
void CScoreUI::move() {
	//  今のところ何もせず
}

/*************************************************
 *@fn
 *  文字表示用のテクスチャ座標を算出する
 *@param in code  文字コード（ローカル）
 *@param out rc   コピー元ジオメトリ
 ************************************************/
static void prepareTextureCoords(int code, D2D1_RECT_F &rc) {
	rc.left = (code % 4) * 32;
	rc.top = (code / 4) * 32;
	rc.right = rc.left + 32;
	rc.bottom = rc.top + 32;
}

/*************************************************
 *@method
 *  スコア表示
 *@param in pRenderTarget  描画先ターゲット
 *************************************************/
void CScoreUI::draw(ID2D1RenderTarget *pRenderTarget, FLOAT x, FLOAT y, FLOAT h) {
	FLOAT tmpX = x;
	FLOAT scale = h / 64.0f;
	FLOAT strideA = 64.0f * scale;    //    width for alphabet
	FLOAT strideB = strideA;    //  width for digits
	D2D1_RECT_F dest, src;
	INT    tmp = m_iScore;
	/**/
	//  disp S C O R E : 
	for (int i = 10; i < 16; ++i) {

		prepareTextureCoords(i, src);
		dest.left = tmpX;
		dest.top = y;
		dest.bottom = dest.top + h;
		dest.right = dest.left + h;
		//pRenderTarget->DrawBitmap(m_pTexture, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

		tmpX += strideA;
	}

	//  以下プログラムは、スコアが、
	//  99,999,999を超えるとバグるので注意！
	/**/
	tmpX += strideB * 6;
	while (tmp > 0) {
		dest.left = tmpX;
		dest.right = dest.left + h;
		prepareTextureCoords(tmp % 10, src);
		pRenderTarget->DrawBitmap(m_pTexture, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		tmp /= 10;
		tmpX -= strideB;
	}
}