#include "stdafx.h"
#include <d2d1.h>
#include "Player.h"
#include "PlayGame.h"
#include "TextureLoader.h"
#include "GameData.h"
#include "ScoreUI.h"
#include "tama.h"

#define GAMESTAGE_WIDE 1344
#define GAMESTAGE_HIGH 1080

#define Texsize 60



CPlayer::CPlayer(CStage *pStage)
{
	ID2D1RenderTarget *pRenderTarget = NULL;
	m_pBitmapP = NULL;
	
	m_bTama = true;
	m_fX = 510.f;
	m_fY = 700.f;
	m_fKeyTime = 0;
	m_pStage = pStage;
	//  CSelector �����L���Ă���ID2D1RenderTarget ���A
	//  CStage ����܂��؂肷��
	pRenderTarget = pStage->GetRenderTarget();
	if (pRenderTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\smilie.tga"), &m_pBitmapP);
		pRenderTarget->Release();    //  Release ���ĉ��
	}
#ifdef _DEBUG
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::IndianRed), &m_pBrush);
	SAFE_RELEASE(pRenderTarget);
#endif // DEBUG
}


CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pBitmapP);
#ifdef _DEBUG
	SAFE_RELEASE(m_pBrush);
#endif // _DEBUG
}

/********************************************************
 *@method
 *  �P�t���[���̃A�j���[�V��������
 *@return true : ���� / false : ���S
 *******************************************************/
bool CPlayer::move() {

	if (GetAsyncKeyState(VK_LEFT))
		m_fX -= 10.0f;
	if (GetAsyncKeyState(VK_RIGHT))
		m_fX += 10.0f;
	
	if (m_fX > 1015)//right
		m_fX = 1015;
	if (m_fX < -5)//left
		m_fX = -5;
	/*********************************************
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!GameData::shot) {
			GameData::shot = true;
			GameData::tamax = 430;
			GameData::tamay = 700;
		}
	}
//	else {
//		m_bFlag = false;
//	}
	if (GameData::shot) {
		GameData::tamay -= 16;
		if (GameData::tamay = 0)m_bFlag = false;
	}
	***********************************************/ 
	if (GetAsyncKeyState(VK_SPACE)) {
		m_fKeyTime += 1;
		GameData::LongPressKey = true;
	}
	else {
		GameData::LongPressKey = false;
		m_bTama = false;
	}

	if (!GameData::LongPressKey) {
		if (0 < m_fKeyTime) {
			if (m_fKeyTime < 30) {
				if (!m_bTama) {
					IGameObject *pObj = new CTama(m_pStage, m_fX+Texsize*0.5, m_fY+Texsize*0.5, false);
					m_pStage->AddTama(pObj);
				}
				m_bTama = true;
				m_fKeyTime = 0;
			}
			else {
				if (!m_bTama) {
					IGameObject *pObj = new CTama(m_pStage, m_fX + Texsize * 0.5, m_fY + Texsize * 0.5, true);
					m_pStage->AddTama(pObj);
				}
				m_bTama = true;
				m_fKeyTime = 0;
			}
		}
	}

	
	return true;
}

/********************************************************
 *@method
 *  �`�揈��
 *******************************************************/
void CPlayer::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;
	D2D1_SIZE_F size,Ssize;
	if (m_pBitmapP == NULL)
		return;
	size = m_pBitmapP->GetSize();
	Ssize = pRenderTarget->GetSize();
	rc.left = m_fX;
	rc.top = m_fY;
	rc.right = rc.left + size.width;
	rc.bottom = rc.top + size.height;
#ifdef _DEBUG
	if (30 < m_fKeyTime)
		pRenderTarget->FillRectangle(rc, m_pBrush);
#endif // _DEBUG
	pRenderTarget->DrawBitmap(m_pBitmapP, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}
/*
D2D1_RECT_F tama;
	tama.left = 430;
	tama.top = 700;
	tama.right = rc.left + 64;
	tama.bottom = rc.top + 64;
	if (GameData::shot) {
		tama.left = GameData::tamax;
		tama.top = GameData::tamay;
		tama.right = rc.left + 64;
		tama.bottom = rc.top + 64;
	}

				//^^^^^^^^^^^^^^
			if (!GameData::shot) {

				GameData::shot = true;
				GameData::tamax = 430;
				GameData::tamay = 700;
				//^^^^^^^^^^^^^^^^^^^^^^^^^

			}		if (GameData::shot) {
				GameData::tamay -= 16;
				if (GameData::tamay = 0)m_bFlag = false;
			}



			struct SHOT {

bool flag;//�e�����˒����ǂ���

double x;//x���W

double y;//y���W

int gh;//�O���t�B�b�N�n���h��

int width, height;//�摜�̕��ƍ���
};

	void Draw(ID2D1RenderTarget *pTarget);
	void SetPos(int x, int y);
*/