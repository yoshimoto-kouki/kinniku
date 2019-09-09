#include "stdafx.h"
#include "ItemSet.h"
#include "Star.h"
#include "Candycane.h"
#include "Ornament.h"
#include "Boots.h"
#include "Protein.h"
#include "ScoreRemnant.h"
#include "GameData.h"

CItemSet::CItemSet(CStage *pStage)
{
	m_iIndex = 0;
	m_pParent = pStage;
	m_iTiming = 0;
}


CItemSet::~CItemSet()
{
	if (m_pParent)
		m_pParent = NULL;
}


/******************************************************
 *@method
 *   この敵セットオブジェクトがセットする予定の
 *   敵キャラオブジェクト用にデバイス依存オブジェクト
 *   を読み込んでおく。
 *   こうすることで出現毎に画像を読み込まないようにする
 ******************************************************/
void CItemSet::Restore(ID2D1RenderTarget *pRT) {
	CStar::Restore(m_pParent,pRT);
	CCandy::Restore(m_pParent, pRT);
	COrnament::Restore(m_pParent, pRT);
	CBoots::Restore(m_pParent, pRT);
	CProtein::Restore(m_pParent, pRT);
	CScoreRemnant::Restore(m_pParent, pRT);
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
	COrnament::Finalize();
	CBoots::Finalize();
	CProtein::Finalize();
	CScoreRemnant::Finalize();
}

/******************************************************
*@method
*   登場させる敵キャラを返すメソッド
*@param in y  敵セット用のY座標
******************************************************/
IGameObject *CItemSet::GetItemToSet(int timing) {
	IGameObject *pObj = NULL;
	int    sx, sy;
	int SetTimingMAX;
	for (int i = 0; i < m_szSetDataSize - 4; i += 4) {
		if (m_pSetData[i] > m_pSetData[i + 4])
			SetTimingMAX = m_pSetData[i];
	}
	if (timing >= SetTimingMAX)
		GameData::StartIndexEnd = !GameData::StartIndexEnd;
	if (m_iIndex < m_szSetDataSize) {
		if (timing >= m_pSetData[m_iIndex]) {//timing=ゲーム開始からの時間
			sx = m_pSetData[m_iIndex + 1];
			sy = m_pSetData[m_iIndex + 2];
			switch (m_pSetData[m_iIndex + 3]) {
			case 0://star
				pObj = new CStar(sx, sy);
				break;
			case 1://candy
				pObj = new CCandy(sx, sy);
				break;
			case 2://Ornament balls
				pObj = new COrnament(sx, sy);
				break;
			case 3://X'mas bootu
				pObj = new CBoots(sx, sy);
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

IGameObject *CItemSet::ItemAdd(int rand ,int type) {
	IGameObject *pObj = NULL;
	int Setx, Sety, id;
	Setx = rand % 26 * 50;
	Sety = -60;
	id = rand % type;
	switch (id) {
	case 0://star
		pObj = new CStar(Setx, Sety);
		break;
	case 1://candy
		pObj = new CCandy(Setx, Sety);
		break;
	case 2://Ornament balls
		pObj = new COrnament(Setx, Sety);
		break;
	case 3://X'mas bootu
		pObj = new CBoots(Setx, Sety);
		break;
	}
	return pObj;
}
IGameObject *CItemSet::ProteinAdd(int rand) {
	IGameObject *pObj = NULL;
	int Setx, Sety;
	Setx = rand % 26 * 50;
	Sety = -60;
	pObj = new CProtein(Setx, Sety);
	return pObj;
}
IGameObject *CItemSet::ScoreRemnantAdd(float x, float y, int tscore,int score)
{
	IGameObject *pObj = NULL;
	pObj = new CScoreRemnant(x, y,tscore,score);
	return pObj;
}
/*
 *  Itemセットデータ(無条件で出てくるやつ)の初期位置と種類
 */
SHORT CItemSet::m_pSetData[] = {//種類(0=星、1=杖、2=丸飾、3=靴)
	136,200,-50,0,      //  星セットを行うタイミング、セット座標(x,y)、種類
	50 ,600,30,0,
	400,300,0,0,
	130,400,-20,0,
	620,200,40,0,
	500,600,-10,0,
	230,400,20,0,
};

size_t CItemSet::m_szSetDataSize = _countof(CItemSet::m_pSetData);