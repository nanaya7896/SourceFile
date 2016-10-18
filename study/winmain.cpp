
//=====IncludeFile=====//
//デバック用メモリリーク検出
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include"Lib\Application.h"
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR CmdLine, int CmdShow)
{
	//引数が関数の本体部で一度も使われていない場合に使用(WINAPIの場合)
	//自分で作った関数なんかは型だけ残し変数名をコメントアウトする(使われない意志表示)
	UNREFERENCED_PARAMETER(CmdShow); UNREFERENCED_PARAMETER(hPreInstance); UNREFERENCED_PARAMETER(CmdLine);
	//メモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

	
	//_CrtSetBreakAlloc(171);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	
	//インスタンスを取得
	CApplication* pApp = CApplication::getInstance();

	//======================================
	//アプリケーションの初期化

	//アプリケーション名
	LPCSTR WindowTitle = "InnocentGirl_SV";
	//スクリーンサイズ
	SIZE ScreenSize = {WINDOW_WIDTH,WINDOW_HEIGHT};
	//スクリーンモード設定(TRUE:ウィンドウ FALSE:フルスクリーン)
	BOOL bWindowed = TRUE;

	HWND hWnd = pApp->Initialize(hInstance, WindowTitle, ScreenSize, bWindowed);

	if (hWnd == NULL)
	{
		return 0;
	}
	
	//======================================
	return pApp->Run(hWnd);
}