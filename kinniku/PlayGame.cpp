#include "stdafx.h"
#include <d2d1.h>
#include <list>
#include <dwrite.h>
#include "PlayGame.h"//Stage
#include "TextureLoader.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#include "BG.h"
#include "ScoreUI.h"
#include "Player.h"
#include "tama.h"
#include "ItemSet.h"
 
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

		m_pItemSet = new CItemSet(this);
		m_pItemSet->Restore(pRenderTarget);
		m_pItems = new std::list<IGameObject*>();
		SAFE_RELEASE(pRenderTarget);
	}
	SAFE_RELEASE(pRenderTarget);
}



CStage::~CStage()
{
	//  �S�����̋����폜
	if (m_pItems) {
		std::list<IGameObject*>::iterator it = m_pItems->begin();
		while (it != m_pItems->end()) {
			SAFE_DELETE(*it);
			it = m_pItems->erase(it);
		}
		delete m_pItems;
	}
	m_pItemSet->Finalize();
	

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
	SAFE_DELETE(m_pItemSet);//�����Ƃ��̍폜
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
		if (m_iTimer > 500)//�Q�[���I������
			bDone = true;
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = STAGE_FADE;
		}
		//------------------------------------------------
		int timing = m_iTimer;//STAGE_RUN�J�n����̃J�E���g��n���Ă���B(Item�o���^�C�~���O�p)
		//~~~~~~~Item����~~~~~~~~~~~~~~~~~~~~~~
		if (m_pItems && m_pItemSet) {
			IGameObject *pObj;
			//if () {//    �����𖞂������炵����Item�Z�b�g�����Z�b�g����Ƃ��p
			//	m_pItemSet->Reset();    
			//}
			do {
				pObj = m_pItemSet->GetItemToSet(timing);
				if (pObj != NULL)
					m_pItems->push_back(pObj);
			} while (pObj);
			std::list<IGameObject*>::iterator it = m_pItems->begin();
			while (it != m_pItems->end()) {
				if ((*it)->move()) {
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pItems->erase(it);
				}
			}
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (m_pScore) //�X�R�A�p
			m_pScore->move();  

		if (m_pTamas) {//  tama�̏���
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
		//  tama��Item�̓����蔻��
		if (m_pTamas && m_pItems) {
			std::list<IGameObject*>::iterator it1 = m_pTamas->begin();
			std::list<IGameObject*>::iterator it2;
			while (it1 != m_pTamas->end()) {
				it2 = m_pItems->begin();
				while (it2 != m_pItems->end()) {
					if ((*it2)->collide(*it1)) {
						(*it1)->hit(1.0f);
						(*it2)->hit(1.0f);
					}
					++it2;
				}
				++it1;
			}
		}
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
	if (m_pItems) {//�����Ƃ��̏���
		std::list<IGameObject*>::iterator it = m_pItems->begin();
		while (it != m_pItems->end()) {
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
*    �������ꂽtama�������N���X�g�ɓo�^����
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