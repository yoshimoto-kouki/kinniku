#include "stdafx.h"
#include <d2d1.h>
#include <dwrite.h>
#include "Selecter.h"
#include<stdio.h>
#include "PlayGame.h"
#include "GameOver.h"
#include "Title.h"
#include <directxmath.h>

#include "GameData.h"
#pragma comment(lib,"dwrite.lib")  //!< 文字表示に使用

CSelector::CSelector(ID2D1RenderTarget *pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
	m_pRenderTarget->AddRef();
	m_iCount;
	m_eGamePhase = GamePhase::GAMEPHASE_INIT;
	m_pScene = NULL;
	m_pWhiteBrush = NULL;
	pBrush = NULL;
	//  Direct Write 初期化
	{
		HRESULT hr;
		IDWriteFactory *pFactory;
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pFactory),
			reinterpret_cast<IUnknown **>(&pFactory)
		);

		hr = pFactory->CreateTextFormat(
			_T("consolas"),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"", //locale
			&m_pTextFormat
		);


		SAFE_RELEASE(pFactory);
	}
	//  塗りつぶし用ブラシの生成
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	))) {
		SAFE_RELEASE(m_pWhiteBrush);
	}
	//  画面分割用ブラシの生成-----------
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Snow),
		&pBrush
	))) {
		SAFE_RELEASE(pBrush);
	}
	//-----------------------------------
}

CSelector::~CSelector(void)
{
	SAFE_DELETE(m_pScene);
	SAFE_RELEASE(m_pWhiteBrush);
	SAFE_RELEASE(pBrush);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTextFormat);
}

void CSelector::doAnim() {
	GameSceneResultCode rc = GAMESCENE_DEFAULT;
	switch (m_eGamePhase) {
	case    GAMEPHASE_INIT:     //  立ち上げ
		m_eGamePhase = GAMEPHASE_RESET;
	case    GAMEPHASE_RESET:    //  ソフトリセット
		SAFE_DELETE(m_pScene);
		m_pScene = new CTitle(this);
		m_eGamePhase = GAMEPHASE_TITLE;
	case	GAMEPHASE_TITLE:
		if (m_pScene != NULL) {
			rc = m_pScene->move();

		}
		if (rc == GAMESCENE_DEFAULT)
			break;
		SAFE_DELETE(m_pScene);
		m_pScene = new CStage(this);
		m_eGamePhase = GAMEPHASE_GAME;
	case    GAMEPHASE_GAME:
		if (m_pScene != NULL) {
			rc = m_pScene->move();
		}
		if (rc == GAMESCENE_DEFAULT)
			break;
		SAFE_DELETE(m_pScene);
		m_pScene = new CGameOver(this);
		m_eGamePhase = GAMEPHASE_GAMEOVER;
		break;
	case    GAMEPHASE_GAMEOVER:
		if (m_pScene != NULL) {
			rc = m_pScene->move();

		}
		if (rc == GAMESCENE_DEFAULT)
			break;
		m_eGamePhase = GAMEPHASE_RESET;
	}
	
}


void CSelector::doDraw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_SIZE_F Ssize;
	Ssize = pRenderTarget->GetSize();
		//画面分割7割-----------------------
	if (m_eGamePhase == GAMEPHASE_GAME) {
		D2D1_RECT_F rec;
		rec.left = 0;
		rec.right = Ssize.width*0.7;
		rec.top = Ssize.height;
		rec.bottom = 0;
		pRenderTarget->FillRectangle(&rec, pBrush);
	}
	//----------------------------------
	TCHAR    str[256];
	if (m_pScene != NULL) {
		m_pScene->draw(pRenderTarget);
	}

	_stprintf_s(str, _countof(str), _T("KINNIKU"));
	D2D1_RECT_F     src;
	src.left = Ssize.width;
	src.right = 0;
	src.top = Ssize.height;
	src.bottom = 0;
	if (m_pWhiteBrush) {
		pRenderTarget->DrawText(str, (DWORD)_tcslen(str), m_pTextFormat, &src, m_pWhiteBrush);//名前
		//pRenderTarget->DrawText(mozi, (DWORD)_tcslen(mozi), m_pTextFormat, &rc, m_pWhiteBrush);
	}

}

//
//!> Method Name : GetRenderTarget
//  @Desc : ゲーム画面用のRenderTarget を返す
//  @Note : このメソッドが返した ID2D1RenderTarget は必ずRelease すること
//
ID2D1RenderTarget *CSelector::GetRenderTaget() {
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}

//
//!> Method Name : GetSystemTextFormat
//  @Desc : デバッグ用のTextFormat を返す
//  @Note : このメソッドが返した IDWriteTextFormat は必ずRelease すること
//
IDWriteTextFormat *CSelector::GetSystemTextFormat() {
	m_pTextFormat->AddRef();
	return m_pTextFormat;
}

