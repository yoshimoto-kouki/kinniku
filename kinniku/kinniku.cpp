// kinniku.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "kinniku.h"
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include <d2d1.h>
#include <dwrite.h>    //  文字表示に使用
#include "TextureLoader.h"
#include "Selecter.h"

#pragma comment(lib,"d2d1.lib")    //  direct2d に必要
#pragma comment(lib,"dwrite.lib")  //  文字表示に使用

#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ID2D1Factory             *g_pD2DFactory = NULL;      //  D2D1Factoryオブジェクト
ID2D1HwndRenderTarget    *g_pRenderTarget = NULL;    //  描画ターゲット
IDWriteTextFormat        *g_pDefaultTextFormat = NULL;  //  テキストフォーマット

double      g_dblDenominator;
double      g_dblFrame;
__int64     g_i64OldTimer;

#define FPS   30
#define INTERVAL   (1.0/FPS)

DWORD    g_dwCount = 0;   //  カウント値

CSelector *g_pSystem = NULL;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex;
	CoInitialize(NULL);
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("Windows05");
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	HWND hWnd;
	hWnd =
		CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wcex.lpszClassName, _T("Windows05"),
			WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			CW_USEDEFAULT, 0, 852, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, _T("Couldn't create a window.\n"), _T("initialization failure"), MB_OK);
		goto eExit;
	}

	RECT    bounds, client;

	GetWindowRect(hWnd, &bounds);
	GetClientRect(hWnd, &client);
	MoveWindow(hWnd, bounds.left, bounds.top,
		852 * 2 - client.right,
		480 * 2 - client.bottom,
		false);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//  Direct2D 初期化
	{
		HRESULT hr;
		//  Direct2D Factory の生成
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
		GetClientRect(hWnd, &client);
		D2D1_SIZE_U size = D2D1::SizeU(
			client.right - client.left,
			client.bottom - client.top
		);

		//  Direct2D 描画ターゲットの生成
		hr = g_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&g_pRenderTarget
		);
	}

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
			&g_pDefaultTextFormat
		);

		SAFE_RELEASE(pFactory);
	}

	//    画像ファイル(smilie.gif)読み込み
	g_pSystem = new CSelector(g_pRenderTarget);

	//  タイマーの初期化
	::QueryPerformanceCounter((LARGE_INTEGER*)&g_i64OldTimer);
	__int64   i64Tmp;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&i64Tmp);
	g_dblDenominator = 1.0 / (double)i64Tmp;
	g_dblFrame = 0.0f;

	InvalidateRect(hWnd, NULL, true);

	MSG        msg;
	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			++g_dwCount;
			double   t;
			::QueryPerformanceCounter((LARGE_INTEGER*)&i64Tmp);
			t = (i64Tmp - g_i64OldTimer) * g_dblDenominator;
			g_i64OldTimer = i64Tmp;
			g_dblFrame += t;

			if (g_dblFrame >= INTERVAL) {
				int   c = (int)(g_dblFrame / INTERVAL);
				g_dblFrame -= INTERVAL * c;
				if (g_pSystem)
					g_pSystem->doAnim();

				++g_dwCount;
				InvalidateRect(hWnd, NULL, false);  //  書き換えの実行
			}
		}
	}
eExit:
	SAFE_DELETE(g_pSystem);
	SAFE_RELEASE(g_pRenderTarget);
	SAFE_RELEASE(g_pDefaultTextFormat);
	SAFE_RELEASE(g_pD2DFactory);

	CTextureLoader::Destroy();  //  画像読み込み処理の後片付け
	CoUninitialize();
	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_PAINT:
		if (g_pRenderTarget) {
			static D2D1::ColorF bg = D2D1::ColorF(36 / 255.0f, 120 / 255.0f, 120 / 255.0f);

			g_pRenderTarget->BeginDraw();  //  描画開始

			g_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); //  Transform の設定

			g_pRenderTarget->Clear(bg);   //  画面のクリア

			if (g_pSystem) {
				g_pSystem->doDraw(g_pRenderTarget);

			}

			g_pRenderTarget->EndDraw();  //  描画終了
		}
		ValidateRect(hWnd, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return    0;
}