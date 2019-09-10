#include "stdafx.h"
#include <d2d1.h>
#include "PlayGame.h"
#include "Star.h"
#include "TextureLoader.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

ID2D1Bitmap *CStar::m_pBitmap = NULL;
CStage *CStar::m_pParent = NULL;

/****************************************************
 *@method
 *  �R���X�g���N�^
 *@param in x  �o��ʒu��X���W
 *@param in y  �o��ʒu��Y���W
 ***************************************************/
CStar::CStar(float x, float y)
{
	m_fX = x;
	m_fY = y;
	m_fVY = 1;
	m_bDamage = false;
}


CStar::~CStar()
{
}

/****************************************************
 *@method
 *  move
 *@return true: ���� / false: ���S
 ***************************************************/
bool CStar::move() {
	if (m_bDamage)//�������������
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

void CStar::draw(ID2D1RenderTarget *pRenderTarget) {
		D2D1_RECT_F rc;
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + 64;
		rc.bottom = rc.top + 64;
		pRenderTarget->DrawBitmap(m_pBitmap, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

bool CStar::collide(float x, float y, float w, float h) {
	float left = m_fX;
	float right = m_fX + 64;//�摜��
	float top = m_fY;
	float bottom = m_fY + 64;//�摜��
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

bool CStar::collide(IGameObject *pObj) {
	float l = m_fX;
	float t = m_fY;
	return pObj->collide(l, t, 64, 32);
}

bool CStar::collide(IGameObject * pObj, int x)
{
	//���̃|�C���g����p*********************
	float l = m_fX;
	return pObj->collide(l,64);
	//***************************************
}

void CStar::hit(float amount) {
	if (amount == 1.0f)
		sndPlaySound(L"atari.wav", SND_ASYNC);
		m_bDamage = true;
}
bool CStar::hitType(){
	return true;
}
bool CStar::make() {
	return true;
}

/*********************************************************
 *@fn
 *  ���L���f�B�A�t�@�C����ǂݍ���
 *  �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
 *********************************************************/
void CStar::Restore(CStage *pStage, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pBitmap);
	CTextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\hosi.png"), &m_pBitmap);
	m_pParent = pStage;
}

/*********************************************************
*@fn
*  ���L���f�B�A�t�@�C���̏���
*  �V�[���폜���ȂǂɌĂяo���悤�ɂ���
*********************************************************/
void CStar::Finalize() {
	SAFE_RELEASE(m_pBitmap);
	m_pParent = NULL;
}