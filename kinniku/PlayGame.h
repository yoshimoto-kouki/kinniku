#pragma once
#ifndef __IGAMESCENE_H__
#include "IGameScene.h"
#endif
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif

#include "Selecter.h"
#define PSHOT_NUM  20

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_FADE = 2,
	STAGE_DONE = 3
};
class CSelector;
class CScoreUI;
class CBG;
class CPlayer;
class CItemSet;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;
 
namespace std {
	template<class _Ty> class allocator;
	template <class _Ty, class _Alloc = allocator<_Ty>>class list;
}


class CStage : public IGameScene
{
public:
	CStage(CSelector *pv);
	virtual ~CStage(void);
	virtual GameSceneResultCode  move() override; //  �V�[���̃A�j���[�g
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override; //  �V�[���̕`��
	virtual void reset();
	virtual void AddTama(IGameObject *pObj);//tama
	virtual CPlayer *GetPlayer();
	ID2D1RenderTarget *GetRenderTarget();
protected:
	CSelector * m_pSystem;
	//BOOL        m_bGameOver;
	CScoreUI *m_pScore;//UI
	CBG			*m_pBG;//BG
	CPlayer     *m_pPlayer;//player
	CItemSet	*m_pItemSet;//���Ƃ��̍~��A�C�e���p

	StagePhase m_ePhase;//�V�[���p
	ID2D1SolidColorBrush *m_pBlack;//�t�F�[�h�A�E�g�p?
	ID2D1SolidColorBrush *m_pBlack2;//�t�F�[�h�A�E�g�p?
	INT     m_iTimer = 0;
	INT		m_iFadeTimer;
	
	INT	m_bFlag = true;
	INT mTimer;//�^�C�}�[
	INT mFrame;//�A�j���[�V�����t���[��
	static const int ANIM_FRAME = 60;

	std::list<IGameObject*> *m_pItems;
	std::list<IGameObject*> *m_pTamas;//tama
};


