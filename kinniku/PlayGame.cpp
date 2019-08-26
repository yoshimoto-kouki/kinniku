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
	CTama::Finalize();
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
	switch (m_ePhase) {
	case STAGE_INIT:
		m_iTimer = 0;
		m_ePhase = STAGE_RUN;
		
	case STAGE_RUN:
	{
		bool bDone = false;
		++m_iTimer;
		if (m_iTimer > 500)//ゲーム終了条件
			bDone = true;
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = STAGE_FADE;
		}
		//------------------------------------------------
		int timing = m_iTimer;//STAGE_RUN開始からのカウントを渡している。(Item出現タイミング用)
		//~~~~~~~Item処理~~~~~~~~~~~~~~~~~~~~~~
		if (m_pItems && m_pItemSet) {
			IGameObject *pObj;
			//if () {//    条件を満たしたらしたらItemセットをリセットするとき用
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
		if (m_pScore) //スコア用
			m_pScore->move();  

		if (m_pTamas) {//  tamaの処理
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
		//  tamaとItemの当たり判定
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
		//**得点加算用***************
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
レンタリング(描画)
****************/
void    CStage::draw(ID2D1RenderTarget *pRenderTarget) {
	//-------------------------------------------
	if (m_pBG)//背景用
		m_pBG->draw(pRenderTarget);
	if (m_pScore)//スコア用
		m_pScore->draw(pRenderTarget, 1100.0f, 10.0f, 32.0f);
	if (m_pPlayer)//player
		m_pPlayer->draw(pRenderTarget);
	if (m_pTamas) {//  ショットの処理
		std::list<IGameObject*>::iterator it = m_pTamas->begin();
		while (it != m_pTamas->end()) {
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
/***********************************************
*@method
*    プレイヤーのインスタンスを返す
*@return  CPlayer オブジェクト
************************************************/
CPlayer *CStage::GetPlayer() {
	return m_pPlayer;
}