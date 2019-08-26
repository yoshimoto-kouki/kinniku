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
protected:
	CStage * m_pParent;
	int m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDataSize;
};