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
	if (m_fY < -1000)//��ʏ㕔�֌�������-y���Ĕ��ł�������
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
*  ��`�Ƃ̓����蔻��
*@param in x �����X���W
*@param in y �����Y���W
*@param in w ��`�̕�
*@param in h ��`�̍�
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CTama::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float top = m_fY;
	float right = m_fX + 25; //����{�摜��
	float bottom = m_fY + 34;//����{�摜��
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
bool CTama::collide(IGameObject *pObj) {
	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, 25,34 );//���l�͉摜����
}


void CTama::hit(float amount) {
	m_bDamage = true;
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̐���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���->�����������E�E�E
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot.png"), &m_pBitmap);
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
}