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
*  ��`�Ƃ̓����蔻��
*@param in x �����X���W
*@param in y �����Y���W
*@param in w ��`�̕�
*@param in h ��`�̍�
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CTama::collide(float x, float y, float w, float h) {
	return false;
}
/************************************************************
*@method
*  ���I�u�W�F�N�g�����蔻��
*@param in pObj�@����I�u�W�F�N�g
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CTama::collide(IGameObject *pObj) {
	return false;
}


//void CTama::damage(float amount) {}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̐���
*  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CTama::Restore(ID2D1RenderTarget *pRT){
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot.png"), &m_pBitmap);
//	m_pParent = pStage;
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
}