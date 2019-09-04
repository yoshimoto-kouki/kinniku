#include "stdafx.h"
#include <d2d1.h>
#include "PlayGame.h"
#include "Boots.h"
#include "TextureLoader.h"
#include "Player.h"

ID2D1Bitmap *CBoots::m_pBitmap = NULL;
CStage *CBoots::m_pParent = NULL;

/****************************************************
*@method
*  �R���X�g���N�^
*@param in x  �o��ʒu��X���W
*@param in y  �o��ʒu��Y���W
***************************************************/
CBoots::CBoots(float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVY = 1.5f;

	m_bDamage = false;
}


CBoots::~CBoots()
{
}

/****************************************************
*@method
*  move
*@return true: ���� / false: ���S
***************************************************/
bool CBoots::move() {
	m_fY += m_fVY;
	if (m_fVY < 0) {
		if (m_fY < -64)
			return  false;    //    ��ʊO�ɏo����I��
	}
	else {
		if (m_fY > 1080)
			return  false;    //    ��ʊO�ɏo����I��
	}
	if (m_bDamage)
		return false;
	return    true;
}

void CBoots::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + 64;
	rc.bottom = rc.top + 64;
	pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

bool CBoots::collide(float x, float y, float w, float h) {
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

bool CBoots::collide(IGameObject *pObj) {

	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, 64, 64);
}
void CBoots::hit(float amount) {
	if (amount == 1.0f)
		m_bDamage = true;
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C����ǂݍ���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CBoots::Restore(CStage *pStage,ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\boots.png"), &m_pBitmap);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C���̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CBoots::Finalize() {
	SAFE_RELEASE(m_pBitmap);
	m_pParent = NULL;
}