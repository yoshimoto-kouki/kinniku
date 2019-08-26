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
 *   ���̓G�Z�b�g�I�u�W�F�N�g���Z�b�g����\���
 *   �G�L�����I�u�W�F�N�g�p�Ƀf�o�C�X�ˑ��I�u�W�F�N�g
 *   ��ǂݍ���ł����B
 *   �������邱�Ƃŏo�����ɉ摜��ǂݍ��܂Ȃ��悤�ɂ���
 ******************************************************/
void CItemSet::Restore(ID2D1RenderTarget *pRT) {
	CStar::Restore(pRT);
	CCandy::Restore(pRT);
	COrnament::Restore(pRT);
	CBoots::Restore(pRT);
}

/******************************************************
*@method
*   ���̓G�Z�b�g�I�u�W�F�N�g���ė��@����
*   �G�L�����I�u�W�F�N�g�p�Ƀf�o�C�X�ˑ��I�u�W�F�N�g
*   ����������B
******************************************************/
void CItemSet::Finalize() {
	CStar::Finalize();
	CCandy::Finalize();
	CBoots::Finalize();
}

/******************************************************
*@method
*   �o�ꂳ����G�L������Ԃ����\�b�h
*@param in y  �G�Z�b�g�p��Y���W
******************************************************/
IGameObject *CItemSet::GetItemToSet(int timing) {
	IGameObject *pObj = NULL;
	int    sx, sy;
	if (m_iIndex < m_szSetDataSize) {
		if (timing <= m_pSetData[m_iIndex]) {//timing=�Q�[���J�n����̎���
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
 *  Item�Z�b�g�f�[�^(�������ŏo�Ă�����)�̏����ʒu�Ǝ��
 */
SHORT CItemSet::m_pSetData[] = {//���(0=���A1=��A2=�ۏ��A3=�H�H�A4=�v���e�C��)
	136,200,50,0,      //  �G�Z�b�g���s���^�C�~���O�A�Z�b�g���W(x,y)�A���
	1000,600,50,0,
	500,300,0,1,
	1300,400,20,2,
	600,200,40,3,
	500,600,100,0,
};

size_t CItemSet::m_szSetDataSize = _countof(CItemSet::m_pSetData);