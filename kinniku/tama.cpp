#include "stdafx.h"
#include <d2d1.h>
#include "tama.h"
#include "TextureLoader.h"

ID2D1Bitmap *CTama::m_pBitmap = NULL;
ID2D1Bitmap *CTama::m_pBitmap2 = NULL;
#define Bitmap01sizeX 25
#define Bitmap01sizeY 34
#define Bitmap02sizeX 136
#define Bitmap02sizeY 148

CTama::CTama(CStage *pStage, float x, float y,bool Keystate)
{
	m_bLong = Keystate;
	m_pParent = pStage;
	float texsizeX = 0, texsizeY = 0;
	if (!m_bLong) {//�`���[�W���ĂȂ�
		texsizeX = Bitmap01sizeX;
		texsizeY = Bitmap01sizeY;
	}
	else {//�`���[�W����
		texsizeX = Bitmap02sizeX;
		texsizeY = Bitmap02sizeY;
	}

 	m_fX = x - texsizeX * 0.5;
	m_fY = y - texsizeY * 0.5;
	m_fVX = 0;
	m_fVY = -3.0f;
	m_bDamage = false;
}

CTama::~CTama()
{
}


bool CTama::move() {
	if (m_bDamage)
		return false;
	m_fY += m_fVY;
	if (m_fY < -1000)//��ʏ㕔�֌�������-y���Ĕ��ł�������
		return    false;
	return    true;
}

void CTama::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	rc.left = m_fX;
	rc.top = m_fY;
	if (!m_bLong) {
		rc.right = rc.left + Bitmap01sizeX;
		rc.bottom = rc.top + Bitmap01sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
	if (m_bLong) {
		rc.right = rc.left + Bitmap02sizeX;
		rc.bottom = rc.top + Bitmap02sizeY;
		pRenderTarget->DrawBitmap(m_pBitmap2, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
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
	float right, bottom;
	if(!m_bLong){
	right = m_fX + Bitmap01sizeX; //����{�摜��
	bottom = m_fY + Bitmap01sizeY;//����{�摜��
	}
	else{
		right = m_fX + Bitmap02sizeX; //����{�摜��
		bottom = m_fY + Bitmap02sizeY;//����{�摜��
	}
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
	if(m_bLong)
		return pObj->collide(l, t, Bitmap02sizeX, Bitmap02sizeY);//���l�͉摜����
	return pObj->collide(l, t, Bitmap01sizeX,Bitmap01sizeY );//���l�͉摜����
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
	SAFE_RELEASE(m_pBitmap2);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot.png"), &m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\tama.png"), &m_pBitmap2);
}

/*********************************************************
*@fn
*  ���L���f�B�A�f�[�^�̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CTama::Finalize() {
	SAFE_RELEASE(m_pBitmap);
	SAFE_RELEASE(m_pBitmap2);
}