#include "stdafx.h"
#include <d2d1.h>
#include "Player.h"
#include "PlayGame.h"
#include "TextureLoader.h"
#include "GameData.h"
#include "ScoreUI.h"
#include "tama.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


#define GAMESTAGE_WIDE 1344
#define GAMESTAGE_HIGH 1080

#define TexsizeHalfX 80
#define TexsizeHalfY 90
#define PTexsizeHalfX 75
#define PTexsizeHalfY 96
#define Pressinterval 20
#define ChargeTime 30
#define ScoreLimit 3//�������E


CPlayer::CPlayer(CStage *pStage)
{
	ID2D1RenderTarget *pRenderTarget = NULL;
	m_pBitmapP = NULL;
	m_bTC = false;
	m_bTama = true;
	m_fX = 510.f;
	m_fY = 850.f;
	m_fKeyTime = 0;//�������p
	m_fThrowTime = 0;//��������p
	m_fItemGetScore = 0;//Score�p
	m_pStage = pStage;
	m_bLongPushSpace = false;
	//  CSelector �����L���Ă���ID2D1RenderTarget ���A
	//  CStage ����܂��؂肷��
	pRenderTarget = pStage->GetRenderTarget();
	if (pRenderTarget) {
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\GameSanta1.png"), &m_pBitmapP);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\GameSanta2.png"), &m_pBitmapPT);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\GameSanta3.png"), &m_pBitmapPTC);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\charge.png"), &m_pBitmapC);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\tree.png"), &m_pBitmapNT);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\tree2.png"), &m_pBitmapNT2);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\tree3.png"), &m_pBitmapNT3);
		CTextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\tree4.png"), &m_pBitmapNT4);
		pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::IndianRed), &m_pBrush);
 		SAFE_RELEASE(pRenderTarget);//�����œ�d��RELEASE���Ă��̂������ł�
	}


}


CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pBitmapP);
	SAFE_RELEASE(m_pBitmapPT);
	SAFE_RELEASE(m_pBitmapPTC);
	SAFE_RELEASE(m_pBitmapC);
	SAFE_RELEASE(m_pBitmapNT);
	SAFE_RELEASE(m_pBitmapItemNum);
	
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
	
	if (m_fX > 1230)//right
		m_fX = 1230;
	if (m_fX < -5)//left
		m_fX = -5;
	if (0 < m_fThrowTime)
		m_fThrowTime--;
	if (GetAsyncKeyState(VK_SPACE)) {
		m_fKeyTime += 1;
		if (ChargeTime < m_fKeyTime)
			m_fKeyTime = ChargeTime; 
		m_bLongPushSpace = true;
	}
	else {
		m_bLongPushSpace = false;
		m_bTama = false;
	}
	GameData::GameHoldNum = m_fItemGetScore;
	
	//-----���~�̖؃`���[�W����------
	if (!GameData::ProteinFlag) {//�v���e�C���o�[�X�g���ĂȂ��Ƃ�
		if(m_fThrowTime < m_fKeyTime){
			if (!m_bLongPushSpace) {
				if (m_fKeyTime < ChargeTime) {
					if (!m_bTama) {
						IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalfX, m_fY + TexsizeHalfY, m_fItemGetScore);
						m_pStage->AddTama(pObj);
					}
					m_fItemGetScore = 0;
					m_bTama = true;
					m_fKeyTime = 0;
					m_fThrowTime = Pressinterval;
					m_bTC = false;
					sndPlaySound(L"kakegoe.wav", SND_ASYNC);
				}
				else {
					if (!m_bTama) {
						IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalfX + PTexsizeHalfX, m_fY + TexsizeHalfY, m_fItemGetScore);
						m_pStage->AddTama(pObj);
						IGameObject *pObj2 = new CTama(m_pStage, m_fX - TexsizeHalfX + PTexsizeHalfX, m_fY + TexsizeHalfY, m_fItemGetScore);
						m_pStage->AddTama(pObj2);
					}
					m_fItemGetScore = 0;
					m_bTama = true;
					m_fKeyTime = 0;
					m_fThrowTime = Pressinterval;
					m_bTC = true;
					sndPlaySound(L"kakegoe.wav", SND_ASYNC);
				}
			}
		}
	}
	//-END----���~�̖؃`���[�W����-------------

	//----Protein-------------
	if (GameData::ProteinFlag) {
		if (!m_bTama) {
			IGameObject *pObj = new CTama(m_pStage, m_fX + TexsizeHalfX, m_fY + TexsizeHalfY,m_fItemGetScore);
			m_pStage->AddTama(pObj);
		}
	}
	// END----Pritein-------------
	return true;
}

/********************************************************
 *@method
 *  �`�揈��
 *******************************************************/
void CPlayer::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc,rcT,rcIcon;
	D2D1_SIZE_F size, Ssize;
	if (m_pBitmapP == NULL)
		return;
	size = m_pBitmapP->GetSize();
	Ssize = pRenderTarget->GetSize();
	if (m_bLongPushSpace) {
		rc.left = m_fX - 20;
		rc.top = m_fY - 20;
		rc.right = rc.left + size.width + 40;
		rc.bottom = rc.top + size.height + 40;
		pRenderTarget->DrawBitmap(m_pBitmapC, rc, (m_fKeyTime / 30), D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}

	if (!m_bTama && m_fThrowTime < 15) {//�L�����p�����O
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + size.width;
		rc.bottom = rc.top + size.height;
		pRenderTarget->DrawBitmap(m_pBitmapP, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
	if (15 <= m_fThrowTime && !m_bTC) {//�L�����p������
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + size.width;
		rc.bottom = rc.top + size.height;
		pRenderTarget->DrawBitmap(m_pBitmapPT, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
	if (15 <= m_fThrowTime && m_bTC) {//�L�����p������(�`���[�W)
		rc.left = m_fX;
		rc.top = m_fY;
		rc.right = rc.left + size.width;
		rc.bottom = rc.top + size.height;
		pRenderTarget->DrawBitmap(m_pBitmapPTC, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
	if (m_bLongPushSpace) {//�L�����������~�̖�
		if (GameData::GameHoldNum <= 0) {
			if (m_fKeyTime < 30) {
				rcT.left = m_fX + PTexsizeHalfX + 30;
		 		rcT.top = m_fY + 30;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
			else {
				rcT.left = m_fX + PTexsizeHalfX * 1.4f;
				rcT.top = m_fY + PTexsizeHalfY * 0.5f;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
				rcT.left = rc.left - PTexsizeHalfX * 0.3f;
				rcT.right = rcT.left + TexsizeHalfX;
				pRenderTarget->DrawBitmap(m_pBitmapNT, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
		}
		if (GameData::GameHoldNum>=1) {
			if (m_fKeyTime < 30) {
				rcT.left = m_fX + PTexsizeHalfX + 30;
				rcT.top = m_fY + 30;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT2, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
			else {
				rcT.left = m_fX + PTexsizeHalfX * 1.4f;
				rcT.top = m_fY + PTexsizeHalfY * 0.5f;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT2, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
				rcT.left = rc.left - PTexsizeHalfX * 0.3f;
				rcT.right = rcT.left + TexsizeHalfX;
				pRenderTarget->DrawBitmap(m_pBitmapNT2, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
		}
		if (GameData::GameHoldNum >= 2) {
			if (m_fKeyTime < 30) {
				rcT.left = m_fX + PTexsizeHalfX + 30;
				rcT.top = m_fY + 30;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT3, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
			else {
				rcT.left = m_fX + PTexsizeHalfX * 1.4f;
				rcT.top = m_fY + PTexsizeHalfY * 0.5f;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT3, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
				rcT.left = rc.left - PTexsizeHalfX * 0.3f;
				rcT.right = rcT.left + TexsizeHalfX;
				pRenderTarget->DrawBitmap(m_pBitmapNT3 , rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
		}
		if (GameData::GameHoldNum >= 3) {
			if (m_fKeyTime < 30) {
				rcT.left = m_fX + PTexsizeHalfX + 40;
				rcT.top = m_fY + 30;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT4, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
			else {
				rcT.left = m_fX + PTexsizeHalfX * 1.4f;
				rcT.top = m_fY + PTexsizeHalfY * 0.5f;
				rcT.right = rcT.left + TexsizeHalfX;
				rcT.bottom = rcT.top + TexsizeHalfY;
				pRenderTarget->DrawBitmap(m_pBitmapNT4, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
				rcT.left = rc.left - PTexsizeHalfX * 0.3f;
				rcT.right = rcT.left + TexsizeHalfX;
				pRenderTarget->DrawBitmap(m_pBitmapNT4, rcT, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
		}
	}
	if (m_bLongPushSpace) {
		if ( 0 < m_fKeyTime) {
			rcIcon.left = 1800;
			rcIcon.top = 936 - m_fKeyTime * 10;
			rcIcon.right = rcIcon.left + 64 ;
			rcIcon.bottom = 1000;
			pRenderTarget->FillRectangle(rcIcon, m_pBrush);
			if (ChargeTime <= m_fKeyTime) {
				rcIcon.top = rcIcon.top - 64;
				rcIcon.bottom = rcIcon.top + 64;
				pRenderTarget->DrawBitmap(m_pBitmapC, rcIcon, (m_fKeyTime / 30), D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
			}
		}
	}
}
bool CPlayer::collide(float x, float y, float w, float h) {
	float l = m_fX ;
	float r = m_fX + PTexsizeHalfX * 2;
	float t = m_fY ;
	float b = m_fY + PTexsizeHalfY * 2;
	if (t > (y + h))
		return false;
	if (b < y)
		return false;
	if (l > (x + w))
		return false;
	if (r < x)
		return false;
	return true;
}
/************************************************************
*@method
*  ���I�u�W�F�N�g�����蔻��
*@param in pObj�@����I�u�W�F�N�g
*@return true : ������ / false : �������ĂȂ�
************************************************************/
bool CPlayer::collide(IGameObject *pObj) {
	float l = m_fX ;
	float t = m_fY ;
	return pObj->collide(l, t, PTexsizeHalfX*2, PTexsizeHalfY*2);
}


void CPlayer::hit(float amount) {
	if (m_fItemGetScore < ScoreLimit)
		m_fItemGetScore += 1;
	GameData::Gametree += 1;
}
