#include "stdafx.h"
#include "ItemSet.h"
#include "Star.h"
#include "Candycane.h"
#include "Ornament.h"
#include "Boots.h"

CItemSet::CItemSet(CStage *pStage)
{
	m_iIndex = 0;
	m_pParent = pStage;
}


CItemSet::~CItemSet()
{
}


/******************************************************
 *@method
 *   この敵セットオブジェクトがセットする予定の
 *   敵キャラオブジェクト用にデバイス依存オブジェクト
 *   を読み込んでおく。
 *   こうすることで出現毎に画像を読み込まないようにする
 ******************************************************/
void CItemSet::Restore(ID2D1RenderTarget *pRT) {
	CStar::Restore(pRT);
	CCandy::Restore(pRT);
	COrnament::Restore(pRT);
	CBoots::Restore(pRT);
}

/******************************************************
*@method
*   この敵セットオブジェクトが監理　する
*   敵キャラオブジェクト用にデバイス依存オブジェクト
*   を消去する。
******************************************************/
void CItemSet::Finalize() {
	CStar::Finalize();
	CCandy::Finalize();
	CBoots::Finalize();
}

/******************************************************
*@method
*   登場させる敵キャラを返すメソッド
*@param in y  敵セット用のY座標
******************************************************/
IGameObject *CItemSet::GetItemToSet(int timing) {
	IGameObject *pObj = NULL;
	int    sx, sy;
	if (m_iIndex < m_szSetDataSize) {
		if (timing <= m_pSetData[m_iIndex]) {//timing=ゲーム開始からの時間
			sx = m_pSetData[m_iIndex + 1];
			sy = m_pSetData[m_iIndex + 2];
			switch (m_pSetData[m_iIndex + 3]) {
			case 0://star
				pObj = new CStar(m_pParent, sx, sy);
				break;
			case 1://candy
				pObj = new CCandy(m_pParent, sx, sy);
				break;
			case 2://Ornament balls
				pObj = new COrnament(m_pParent, sx, sy);
				break;
			case 3://X'mas bootu
				pObj = new CBoots(m_pParent, sx, sy);
				break;
			}
			m_iIndex += 4;
		}
	}
	return    pObj;
}
void CItemSet::Reset() {
	m_iIndex = 0;
}

/*
 *  Itemセットデータ(無条件で出てくるやつ)の初期位置と種類
 */
SHORT CItemSet::m_pSetData[] = {//種類(0=星、1=杖、2=丸飾、3=？？、4=プロテイン)
	136,200,50,0,      //  敵セットを行うタイミング、セット座標(x,y)、種類
	1000,600,50,0,
	500,300,0,1,
	1300,400,20,2,
	600,200,40,3,
	500,600,100,0,
};

size_t CItemSet::m_szSetDataSize = _countof(CItemSet::m_pSetData);