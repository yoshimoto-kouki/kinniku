/****************************************
*@file  ScoreUI.cpp
*  �X�R�A�\���p�N���X CScoreUI �̎���
****************************************/
#include "stdafx.h"
#include <d2d1.h>
#include <math.h>
#include "TextureLoader.h"
#include "ScoreUI.h"

//  �}�N����`
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
 *  �X�R�A�ɒl���Z�b�g����
 *@param in score   �Z�b�g����X�R�A
 *************************************************/
void CScoreUI::SetScore(INT score) {
	m_iScore = score;
}
/*************************************************
*@method
*  �X�R�A����l��ǂݏo��
*@return  ���݂̃X�R�A
*************************************************/
INT CScoreUI::GetScore() {
	return m_iScore;
}
/*************************************************
*@method
*  �X�R�A�ɒl�����Z
*@param in value  ���Z����l
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
*  �X�R�A�\���̃A�j���[�V����
*************************************************/
void CScoreUI::move() {
	//  ���̂Ƃ��뉽������
}

/*************************************************
 *@fn
 *  �����\���p�̃e�N�X�`�����W���Z�o����
 *@param in code  �����R�[�h�i���[�J���j
 *@param out rc   �R�s�[���W�I���g��
 ************************************************/
static void prepareTextureCoords(int code, D2D1_RECT_F &rc) {
	rc.left = (code % 4) * 32;
	rc.top = (code / 4) * 32;
	rc.right = rc.left + 32;
	rc.bottom = rc.top + 32;
}

/*************************************************
 *@method
 *  �X�R�A�\��
 *@param in pRenderTarget  �`���^�[�Q�b�g
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

	//  �ȉ��v���O�����́A�X�R�A���A
	//  99,999,999�𒴂���ƃo�O��̂Œ��ӁI
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