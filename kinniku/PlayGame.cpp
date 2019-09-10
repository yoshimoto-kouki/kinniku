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
	m_fProteinTimer = 0;//プロテインバースト時間計測用
	ID2D1RenderTarget *pTarget;
	m_iFadeTimer = 0;
	m_pBlack = NULL;
	m_pSystem = pSystem;
	ID2D1RenderTarget *pRenderTarget;
	pRenderTarget = m_pSystem->GetRenderTaget();
	if (pRenderTarget) {
		m_pBG = new CBG(pRenderTarget);//背景
		m_pScore = new CScoreUI(pRenderTarget);//UI
		m_pPlayer = new CPlayer(this);
		pTarget = pSystem->GetRenderTaget();
		
		if (pTarget) { //ここの部分がないとフェードアウトしない
			pTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &m_pBlack);
			pTarget->Release();
			pTarget = NULL;
		}
		if (m_pScore) {//スコア用
			m_pScore->SetScore(0);
		}
		CTama::Restore(pRenderTarget);//tama用
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
	//  全流星の強制削除
	if (m_pItems) {
		std::list<IGameObject*>::iterator it = m_pItems->begin();
		while (it != m_pItems->end()) {
			SAFE_DELETE(*it);
			it = m_pItems->erase(it);
		}
		delete m_pItems;
	}
	m_pItemSet->Finalize();
	

	//  全ショットの強制削除
	if (m_pTamas) {
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
			SAFE_DELETE(*it);
			it = m_pTamas->erase(it);
		}
		delete m_pTamas;
		m_pTamas = NULL;
	}
	//  全プロテインの強制削除
	if (m_pProtein) {
		std::list<IGameObject*>::iterator it = m_pProtein->begin();
		while (it != m_pProtein->end()) {
			SAFE_DELETE(*it);
			it = m_pProtein->erase(it);
		}
		delete m_pProtein;
		m_pProtein = NULL;
	}
	//  全一時スコアの強制削除
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
	SAFE_DELETE(m_pItemSet);//流星とかの削除
	CTextureLoader::Destroy();//TextureLoder解放
}
/*********
リセット
**********/
void CStage::reset() {}
/*********
アニメーション処理
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
		if (m_iTimer > GameData::GameEndTime)//ゲーム終了条件1分30秒になってるはず
			bDone = true;

#ifndef rtamura
	//	if (m_iTimer > 3000)
//			bDone = true;
#endif
		if (bDone) {
			mciSendString(L"stop all", NULL, 0, NULL);
			m_iFadeTimer = 0;
			m_ePhase = STAGE_FADE;
		}
		//*****ゲーム本編**************************************
		int timing = m_iTimer;//STAGE_RUN開始からのカウントを渡している。(Item出現タイミング用)
		//~~~~~~~Item生成~~~~~~~~~~~~~~~~~~~~~~
		if (m_pItems && m_pItemSet) {
			//初期生成
			if (!GameData::StartIndexEnd) {//初期生成配列の最大値が生成されたら切り替わる
				IGameObject *pObj;
				do {
					pObj = m_pItemSet->GetItemToSet(timing);
					if (pObj != NULL)
						m_pItems->push_back(pObj);
				} while (pObj);
			}
			//END-初期生成---

			//***星ランダム位置に生成******
			if (timing % 50 == 0) {
				IGameObject *pObj;
				pObj = m_pItemSet->ItemAdd(rand(),1);//()の数値は星だけ生成するために使用
				if (pObj != NULL)
					m_pItems->push_back(pObj);
			}
			//END***星ランダム位置に生成******
			//Protein生成=========
			if (timing % 900 == 0) {
				IGameObject *pObj;
				pObj = m_pItemSet->ProteinAdd(rand());
				if (pObj != NULL)
					m_pItems->push_back(pObj);
			}
			//END Protein生成=====

			//~~~~~~~~Item処理~~~~~~~~~~~~
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
			//~~END~~Item処理~~~~~~~~~~~~
		}
		//END ITEM生成----------------


		if (m_pPlayer)//Player処理
			m_pPlayer->move();
		if (m_pBG)
			m_pBG->move();
		
		if (GameData::ProteinFlag) {//プロテインバースト中カウント
			m_fProteinTimer++;
			if (m_fProteinTimer > 150){//30FPSで更新されるだろうから＊5して出した。5秒用
				GameData::ProteinFlag = !GameData::ProteinFlag;
				m_fProteinTimer = 0;
				}
		}
		
		//---------------tamaとItemの当たり判定-------------
		
		if (m_pTamas) {
			std::list<IGameObject*>::iterator it1 = m_pTamas->begin();
			std::list<IGameObject*>::iterator it2;
			while (it1 != m_pTamas->end()) {
				if (m_pItems) {
					
					it2 = m_pItems->begin();
					while (it2 != m_pItems->end()) {
   						if ((*it2)->collide(*it1)) {//当たり判定true/false
							if ((*it2)->hitType()) {//星か否か判定
								(*it2)->collide(*it1,1);
								(*it1)->hit(1.0f);
							}
							(*it2)->hit(1.0f);
							if ((*it2)->make()) {//星破壊時の判定。	;
								IGameObject *pObj;
								pObj = m_pItemSet->ItemAdd(rand(),4);//()の数値で4種のItem生成・星を含む。
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
					if (m_pScore)//スコア用
						if ((*it1)->StarHitFlag()) {
							m_pScore->AddScore(
								(*it1)->TreeScoreBack()//Tamaが失われるとき得点を取得して総得点に加算
								, (*it1)->ScoreBack()
							);
							m_pScore->move();
							//**星刺し時得点用***************
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
		//-----END-tamaとItemの当たり判定--------------------------

		
		// ----ItemとPlayerの当たり判定-----------------
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
レンタリング(描画)
****************/
void    CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	//-------------------------------------------
	if (m_pBG)//背景用
		m_pBG->draw(pRenderTarget);
	if (m_pScore)//スコア用
		m_pScore->Draw(pRenderTarget, 1400.0f, 80.0f, 32.0f);
	if (m_pPlayer)//player
		m_pPlayer->draw(pRenderTarget);
	if (m_pTamas) {//  ショットの処理
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	if (m_pProtein) {//  プロテインの処理
		std::list<IGameObject*>::iterator it = m_pProtein->begin();
		while (it != m_pProtein->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	
	if (m_pScoreRemnant) {//  一時Scoreの処理
		std::list<IGameObject*>::iterator it = m_pScoreRemnant->begin();
		while (it != m_pScoreRemnant->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
	if (m_pItems) {//流星とかの処理
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

		//ここどうなってるのか聞く 　←解決しました。pTargetがなかったのでm_pBlockになにも入ってなかったのが原因です
		m_pBlack->SetOpacity(m_iFadeTimer / 30.0f);
		pRenderTarget->FillRectangle(rc, m_pBlack);
	}

}
/***********************************************
*@method
*   ID2D1RenderTarget を取得して返す、
*    このメソッドでRenderTarget を受け取ったら
*   使用終了後かならずRelease すること
*@return ID2D1RenderTarget オブジェクト
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
*    生成されたtamaをリンクリストに登録する
*@param in pObj  新しい弾のオブジェクト
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
*    プレイヤーのインスタンスを返す
*@return  CPlayer オブジェクト
************************************************/
CPlayer *CStage::GetPlayer() {
	return m_pPlayer;
}