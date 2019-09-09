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
			return  false;    //    ��ʊO�ɏo����I��
	}
	else {
		if (m_fY > 1080)
			return  false;    //    ��ʊO�ɏo����I��
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
*  ��`�Ƃ̓����蔻��
*@param in x �����X���W
*@param in y �����Y���W
*@param in w ��`�̕�
*@param in h ��`�̍�
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CProtein::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float top = m_fY;
	float right = m_fX +  texX; //����{�摜��
	float bottom = m_fY + texY;//����{�摜��
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
*  ���I�u�W�F�N�g�����蔻��
*@param in pObj�@����I�u�W�F�N�g
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CProtein::collide(IGameObject *pObj) {//���Tama�Ƃ̂����蔻��Ɏg�p���Ă��邽�߁Afasle�Ŏ��s���Ȃ��悤�ɂ���
/*
	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, texX, texY);//���l�͉摜����
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
*  ���L���f�B�A�f�[�^�̐���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CProtein::Restore(CStage *pStage, ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pTexture);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\protein.png"), &m_pTexture);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CProtein::Finalize() {
	SAFE_RELEASE(m_pTexture);
	m_pParent = NULL;
}