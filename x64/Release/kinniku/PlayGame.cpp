#include "stdafx.h"
#include <d2d1.h>
#include <list>
#include <dwrite.h>
#include <stdlib.h>
#include <time.h>
#include "PlayGame.h"//Stage
#include "TextureLoader.h"
#include "GameData.h"
#include  <windows.h>
#include  <mmsystem.h>
#include "BG.h"
#include "ScoreUI.h"
#include "ScoreRemnant.h"
#include "Player.h"
#include "tama.h"
#include "ItemSet.h"
#include "Protein.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


CStage::CStage(CSelector *pSystem)
{
	m_fProteinTimer = 0;//�v���e�C���o�[�X�g���Ԍv���p
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
	//  �S�v���e�C���̋����폜
	if (m_pProtein) {
		std::list<IGameObject*>::iterator it = m_pProtein->begin();
		while (it != m_pProtein->end()) {
			SAFE_DELETE(*it);
			it = m_pProtein->erase(it);
		}
		delete m_pProtein;
		m_pProtein = NULL;
	}
	//  �S�ꎞ�X�R�A�̋����폜
	if (m_pScoreRemnant) {
		std::list<IGameObject*>::iterator it = m_pScoreRemnant->begin();
		while (it != m_pScoreRemnant->end()) {
			SAFE_DELETE(*it);
			it = m_pScoreRemnant->erase(it);
		}
		delete m_pScoreRemnant;
		m_pScoreRemnant = NULL;
	}

	CTama::Finalize();
	CProtein::Finalize();
	CScoreRemnant::Finalize();
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
	srand(time(NULL));
	switch (m_ePhase) {
		 
	case STAGE_INIT:
		mciSendString(L"open GameBGM.mp3 type mpegvideo alias GameBGM", NULL, 0, NULL);
		mciSendString(L"play GameBGM from 0 repeat", NULL, 0, NULL);
		m_iTimer = 0;
		m_ePhase = STAGE_RUN;
		
	case STAGE_RUN:
	{
		bool bDone = false;
		++m_iTimer;
		GameData::GamePlayTime = m_iTimer;
		if (m_iTimer > GameData::GameEndTime)//�Q�[���I������1��30�b�ɂȂ��Ă�͂�
			bDone = true;

#ifndef rtamura
	//	if (m_iTimer > 3000)
//			bDone = true;
#endif
		if (bDone) {
			mciSendString(L"stop all", NULL, 0, NULL);
			mciSendString(L"open score.mp3 type mpegvideo alias score", NULL, 0, NULL);
			mciSendString(L"play score from 0 repeat", NULL, 0, NULL);
			m_iFadeTimer = 0;
			m_ePhase = STAGE_FADE;
		}
		//*****�Q�[���{��**************************************
		int timing = m_iTimer;//STAGE_RUN�J�n����̃J�E���g��n���Ă���B(Item�o���^�C�~���O�p)
		//~~~~~~~Item����~~~~~~~~~~~~~~~~~~~~~~
		if (m_pItems && m_pItemSet) {
			//��������
			if (!GameData::StartIndexEnd) {//���������z��̍ő�l���������ꂽ��؂�ւ��
				IGameObject *pObj;
				do {
					pObj = m_pItemSet->GetItemToSet(timing);
					if (pObj != NULL)
						m_pItems->push_back(pObj);
				} while (pObj);
			}
			//END-��������---

			//***�������_���ʒu�ɐ���******
			if (timing % 50 == 0) {
				IGameObject *pObj;
				pObj = m_pItemSet->ItemAdd(rand(),1);//()�̐��l�͐������������邽�߂Ɏg�p
				if (pObj != NULL)
					m_pItems->push_back(pObj);
			}
			//END***�������_���ʒu�ɐ���******
			//Protein����=========
			if (timing % 900 == 0) {
				IGameObject *pObj;
				pObj = m_pItemSet->ProteinAdd(rand());
				if (pObj != NULL)
					m_pItems->push_back(pObj);
			}
			//END Protein����=====

			//~~~~~~~~Item����~~~~~~~~~~~~
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
			//~~END~~Item����~~~~~~~~~~~~
		}
		//END ITEM����----------------


		if (m_pPlayer)//Player����
			m_pPlayer->move();
		if (m_pBG)
			m_pBG->move();
		
			
		
		

		if (GameData::ProteinFlag) {//�v���e�C���o�[�X�g���J�E���g
			//mciSendString(L"open protein BGM.mp3 type mpegvideo alias protein BGM", NULL, 0, NULL);
			//mciSendString(L"play protein BGM from 0 repeat", NULL, 0, NULL);
			m_fProteinTimer++;
			if (m_fProteinTimer > 150){//30FPS�ōX�V����邾�낤���灖5���ďo�����B5�b�p
				GameData::ProteinFlag = !GameData::ProteinFlag;
				//mciSendString(L"stop all", NULL, 0, NULL);
				///mciSendString(L"open GameBGM.mp3 type mpegvideo alias GameBGM", NULL, 0, NULL);
				//mciSendString(L"play GameBGM from 0 repeat", NULL, 0, NULL);

				m_fProteinTimer = 0;
				}
		}
		
		//---------------tama��Item�̓����蔻��-------------
		
		if (m_pTamas) {
			std::list<IGameObject*>::iterator it1 = m_pTamas->begin();
			std::list<IGameObject*>::iterator it2;
			while (it1 != m_pTamas->end()) {
				if (m_pItems) {
					
					it2 = m_pItems->begin();
					while (it2 != m_pItems->end()) {
   						if ((*it2)->collide(*it1)) {//�����蔻��true/false
							if ((*it2)->hitType()) {//�����ۂ�����
								(*it2)->collide(*it1,1);
								(*it1)->hit(1.0f);
							}
							(*it2)->hit(1.0f);
							if ((*it2)->make()) {//���j�󎞂̔���B	;
								IGameObject *pObj;
								pObj = m_pItemSet->ItemAdd(rand(),4);//()�̐��l��4���Item�����E�����܂ށB
								if (pObj != NULL)
									m_pItems->push_back(pObj);
							}
						}
						++it2;
					}
				}
				if ((*it1)->move()) {
					++it1;
				}
				else {
					if (m_pScore)//�X�R�A�p
						if ((*it1)->StarHitFlag()) {
							m_pScore->AddScore(
								(*it1)->TreeScoreBack()//Tama��������Ƃ����_���擾���đ����_�ɉ��Z
								, (*it1)->ScoreBack()
							);
							m_pScore->move();
							//**���h�������_�p***************
							IGameObject *pObj;
							pObj = m_pItemSet->ScoreRemnantAdd((*it1)->TSPointBackx(), (*it1)->TSPointBacky(), (*it1)->TreeScoreBack(),(*it1)->ScoreBack());
							if (pObj != NULL)
								m_pItems->push_back(pObj);
							//**************************
						}
					
					SAFE_DELETE(*it1);
					it1 = m_pTamas->erase(it1);
				}
			}
		}
		//-----END-tama��Item�̓����蔻��--------------------------

		
		// ----Item��Player�̓����蔻��-----------------
		if (m_pItems && m_pPlayer) {
			std::list<IGameObject*>::iterator it1 = m_pItems->begin();
			while (it1 != m_pItems->end()) {
				if (m_pItems) {
   					if ((m_pPlayer)->collide(*it1)) {
						if (!(*it1)->hitType()) {
							(*it1)->hit(2.0f);
							(m_pPlayer)->hit(1.0f);
						}
					}
				}
				else {
					SAFE_DELETE(*it1);
					it1 = m_pItems->erase(it1);
				}
				++it1;
			}
		}
		
		//-------------------------------
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
		m_pScore->Draw(pRenderTarget, 1400.0f, 80.0f, 32.0f);
	if (m_pPlayer)//player
		m_pPlayer->draw(pRenderTarget);
	if (m_pTamas) {//  �V���b�g�̏���
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	if (m_pProtein) {//  �v���e�C���̏���
		std::list<IGameObject*>::iterator it = m_pProtein->begin();
		while (it != m_pProtein->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	
	if (m_pScoreRemnant) {//  �ꎞScore�̏���
		std::list<IGameObject*>::iterator it = m_pScoreRemnant->begin();
		while (it != m_pScoreRemnant->end()) {
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
	switch (m_ePhase) {
	case STAGE_FADE:
	case STAGE_DONE:
		D2D1_RECT_F rc, src;
		D2D1_SIZE_F screenSize, textureSize;
		screenSize = pRenderTarget->GetSize();

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
void CStage::AddProtein(IGameObject *pObj) {
	if (m_pProtein) {
		m_pProtein->push_back(pObj);
	}
}

void CStage::AddScoreRemnant(IGameObject *pObj) {
	if (m_pScoreRemnant) {
		m_pScoreRemnant->push_back(pObj);
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