#pragma once
#ifndef __IGAMEOBJECT_H__
#include "IGameObject.h"
#endif
class CStage;

class CItemSet
{
public:
	CItemSet(CStage *pStage);
	virtual ~CItemSet();
	virtual void Restore(ID2D1RenderTarget *pRT);
	virtual void Finalize();
	virtual IGameObject *GetItemToSet(int timing);
	virtual void Reset();
	virtual IGameObject *ItemAdd(int rand,int type);
	virtual IGameObject *ProteinAdd(int rand);
	virtual IGameObject *ScoreRemnantAdd(float x,float y,int score);
protected:
	CStage * m_pParent;
	int m_iIndex;
	int m_iTiming;
	static SHORT m_pSetData[];
	static size_t m_szSetDataSize;
};