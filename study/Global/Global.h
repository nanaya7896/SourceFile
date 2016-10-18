//=====================================================================
//共通読み込みの作成
//		
//=====================================================================
#pragma once
#define _CRT_SECURE_NO_WARNINGS

//=====define値を定義=====//
#define DIRECTINPUT_VERSION 0x0800			// DirectInputVersion設定(DX8.0以降)
#define	TotalAdapter		6
#define SAFE_DELETE(p)		{ if(p) { delete (p);		(p)=NULL; } }	// オブジェクト開放マクロ
#define SAFE_DELETE_ARRAY(p){ if(p) { delete [](p);		(p)=NULL; } }	// オブジェクト開放マクロ
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release();	(p)=NULL; } }	// オブジェクト開放マクロ
#define UM_SETCURSOR		WM_USER + 101	// カーソルの設定
#define VERTEX_FVF	(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE)

/*ジョイスティックのキー割り当て*/
#define		SQUARE_BUTTON		0
#define		X_MARK_BUTTON		1
#define		CIRCLE_BUTTON		2
#define		TRIANGLE_BUTTON		3
#define		L1_BUTTON			4
#define		R1_BUTTON			5

/*共通のプレイヤー・アイテムの状態*/
#define		STATE_NONE			0
#define		STATE_HAVE_WG		1
#define     STATE_HAVE_BG		2
#define		STATE_HAVE_BB		3
#define     STATE_BG_SHOT		4
#define		STATE_WG_SHOT		5
#define		STATE_BB_SHOT		6
#define     STATE_BG_PICKUP		7
#define     STATE_WG_PICKUP		8
#define     STATE_BB_PICKUP		9
#define     STATE_WATER_WAIT	10
#define     STATE_BALL_END		11

//デバッグ出力用
#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // 空実装
#endif

//ウィンドウの大きさを定義
#define WINDOW_WIDTH	1600
#define WINDOW_HEIGHT	900

//=====Libの読み込み=====//
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")

#pragma comment(lib, "dinput8.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xinput.lib ")

//=====C++=====//
#include <Windows.h>
#include <comdef.h>
#include <dsound.h>
#include <mmsystem.h>
#include <tchar.h>
#include <vector>
#include <strsafe.h>
#include<string.h>
#include<XInput.h>
#include<math.h>
#include<map>
#include<string>
#include<list>
#include<iostream>
#include<sstream>
#include <complex>
#include <XInput.h>
#include <algorithm>
#include <set>

//=====Direct3D=====//
#include <d3d9.h>
#include <d3dx9.h>
#include<d3dx9anim.h>
#include <dinput.h>





//====== 型宣言======//
enum METHOD
{
	D3DNONINDEXED,	// 頂点ブレンディング			(最速)
	D3DINDEXED,		// インデックスブレンディング	(最速)
	SOFTWARE,		// ソフトウェアブレンディング	(低速)
};

//シーン遷移用構造体
struct SCENE 
{
	enum
	{
		TITLE,
		OPERATE,
		MAIN,
		GAMEOVER,
		RESULT,
		CREDIT,
		RANKING
	};
};

enum STATE
{
	Land,
	Have,
	Throw,
	Air,
	Shot
};



//頂点用構造体
struct VTX {
	float x, y, z,u,v;
};

