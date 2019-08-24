#include "stdafx.h"
#include <d2d1.h>
#include <list>
#include <dwrite.h>
#include "PlayGame.h"//Stage
#include "TextureLoader.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#include "ScoreUI.h"
#include "BG.h"
#include "Player.h"
#include "tama.h"
 
#pragma comment(lib,"winmm.lib")


CStage::CStage(CSelector *pSystem)
{
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_pBlack = NULL;
	m_pSystem = pSystem;
	ID2D1RenderTarget *pRenderTarget;
	pRenderTarget = m_pSystem->GetRenderTaget();
	if (pRenderTarget) {
		m_pBG = new CBG(pRenderTarget);//�w�i
		m_pScore = new CScoreUI(pRenderTarget);//UI
		m_pPlayer = new CPlayer(this);
		pTarget = pSystem->GetRenderTaget();
		
		
		if (pTarget) { //�����̕������Ȃ��ƃt�F�[�h�A�E�g���Ȃ�
			pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
			pTarget->Release();
			pTarget = NULL;
		}
		if (m_pScore) {//�X�R�A�p
			m_pScore->SetScore(0);
		}
		CTama::Restore(pRenderTarget);//tama�p
		m_pTamas = new std::list<IGameObject*>();
	}
	SAFE_RELEASE(pRenderTarget);
}



CStage::~CStage()
{
	//  �S�V���b�g�̋����폜
	if (m_pTamas) {
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
			SAFE_DELETE(*it);
			it = m_pTamas->erase(it);
		}
		delete m_pTamas;
		m_pTamas = NULL;
	}
	CTama::Finalize();
	SAFE_DELETE(m_pBG);//BG
	SAFE_DELETE(m_pScore);//UI
	SAFE_DELETE(m_pPlayer);//player
	CTextureLoader::Destroy();//TextureLoder���
}
/*********
���Z�b�g
**********/
void CStage::reset() {}
/*********
�A�j���[�V��������
***********/
GameSceneResultCode    CStage::move() {
	switch (m_ePhase) {
	case STAGE_INIT:
		m_iTimer = 0;
		m_ePhase = STAGE_RUN;
		
	case STAGE_RUN:
	{
		bool bDone = false;
		++m_iTimer;
		if (m_iTimer > 300)
			bDone = true;
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = STAGE_FADE;
		}
		//------------------------------------------------
		if (m_pScore) //�X�R�A�p
			m_pScore->move();    //  �ꉞ�Ă�ł���

		if (m_pTamas) {//  �V���b�g�̏���
			std::list<IGameObject*>::iterator it = m_pTamas->begin();
			while (it != m_pTamas->end()) {
				if ((*it)->move()) {
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pTamas->erase(it);
				}
			}
		}
		if (m_pPlayer)//Player
			m_pPlayer->move();
		//**���_���Z�p***************
		if (GameData::TreeConplete) {
			GameData::TreeConplete = false;
			if (m_pScore)
				m_pScore->AddScore(5);
		}
		//**************************
		//------------------------------------------------
		break;
	}
	case STAGE_FADE:
		m_iFadeTimer++;
		if (m_iFadeTimer < 30)
			break;
		m_ePhase = STAGE_DONE;
	case STAGE_DONE:
		return GameSceneResultCode::GAMESCENE_END_OK;
	}
	mFrame = (mFrame + 1) % ANIM_FRAME;
	return GameSceneResultCode::GAMESCENE_DEFAULT;
}
/****************
�����^�����O(�`��)
****************/
void    CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	//-------------------------------------------
	if (m_pBG)//�w�i�p
		m_pBG->draw(pRenderTarget);
	if (m_pScore)//�X�R�A�p
		m_pScore->draw(pRenderTarget, 1100.0f, 10.0f, 32.0f);
	if (m_pPlayer)//player
		m_pPlayer->draw(pRenderTarget);
	if (m_pTamas) {//  �V���b�g�̏���
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	//----------------------------------------------
	
	D2D1_RECT_F rc, src;
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	
	rc.left = 0;
	rc.top = 0;
	rc.right = rc.left + screenSize.width;
	rc.bottom = rc.top + screenSize.height;
	
	switch (m_ePhase) {
	case STAGE_FADE:
	case STAGE_DONE:
		rc.left = 0;
		rc.top = 0;
		rc.right = screenSize.width;
		rc.bottom = screenSize.height;

		src.left = 0;
		src.top = 0;
		src.right = src.left + 64;
		src.bottom = src.top + 64;

		//�����ǂ��Ȃ��Ă�̂����� �@���������܂����BpTarget���Ȃ������̂�m_pBlock�ɂȂɂ������ĂȂ������̂������ł�
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);
	}

}
/***********************************************
*@method
*   ID2D1RenderTarget ���擾���ĕԂ��A
*    ���̃��\�b�h��RenderTarget ���󂯎������
*   �g�p�I���ォ�Ȃ炸Release ���邱��
*@return ID2D1RenderTarget �I�u�W�F�N�g
**********************************************/
ID2D1RenderTarget *CStage::GetRenderTarget() {
	ID2D1RenderTarget *result = NULL;
	if (m_pSystem) {
		result = m_pSystem->GetRenderTaget();
	}
	return result;
}
/***********************************************
*@method
*    �������ꂽ�V���b�g�������N���X�g�ɓo�^����
*@param in pObj  �V�����e�̃I�u�W�F�N�g
************************************************/
void CStage::AddTama(IGameObject *pObj) {
	if (m_pTamas) {
		m_pTamas->push_back(pObj);
	}
}
/***********************************************
*@method
*    �v���C���[�̃C���X�^���X��Ԃ�
*@return  CPlayer �I�u�W�F�N�g
************************************************/
CPlayer *CStage::GetPlayer() {
	return m_pPlayer;
}