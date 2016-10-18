#pragma once

//=====IncludeFile=====//
#include"../../Global/Global.h"
#include"../../Scene/Scene.h"
//=====クラス宣言=====//
class CDXDevice
{
	//privateメンバ変数
private:
	//Direct3Dシステム
	LPDIRECT3D9				m_pDirect3D;
	
	//描画デバイス
	LPDIRECT3DDEVICE9		m_pDevice9;

	//プレゼントパラメーター情報
	D3DPRESENT_PARAMETERS	m_d3dpp;

	//ディスプレイ情報
	D3DDISPLAYMODE			m_d3ddm;

	//DX環境
	D3DCAPS9				m_d3dCaps;

	//デバイス消失の有無
	BOOL					m_bDeviceLost;

	//ObjectCleanup実行有無
	BOOL					m_bCleanupObject;

	//ウィンドウ/フルスクリーン
	BOOL					m_bWindowed;

	//ディスプレイモード変更実行の有無
	BOOL					m_bChangeDisplayMode;

	//ウィンドウハンドル
	HWND					m_hWnd;

	//モード情報
	D3DDISPLAYMODE			m_Mode;

	//シーンの情報
	CScene*					m_pScene;

	//フォント
	CDXFont*				m_pFont;

	//DirectInput
	CDInput*				m_pDInput;

	//キー入力格納用配列
	BYTE					m_byKeyState[BUFFER_SIZE];

	//ジョイスティック
	DIJOYSTATE2				m_JoyState;
	// アニメーション環境
	BYTE					m_byAnimMethod;			

	//publicメンバ変数
public:

	//privateメンバ関数
private:
	void		InitPresentParameters(HWND,BOOL);
	D3DDISPLAYMODE	CheckFullScreenAdapter(D3DDISPLAYMODE);
	HRESULT		CreateDevice(HWND,D3DPRESENT_PARAMETERS);
	BOOL		CreateResource();
	BOOL		CreateObject();
	void		CleanupObject();
	//publicメンバ関数
public:
	BOOL		BeginScene();
	HRESULT		EndScene();
	HRESULT		Initialize(HWND, BOOL);
	BOOL		RestoreDevice();
	//コンストラクタ
	CDXDevice();
	//デストラクタ
	virtual		~CDXDevice();
	void		CleanupResource();
	void		ChangeDisplaymode();
	HRESULT		Render();

	//=====セット関数=====//
	void		setDeviceLost(BOOL);
	void		setWindowed(BOOL);
	void		setChangeDisplayMode(BOOL);

	//=====ゲット関数=====//
	BOOL		getDeviceLost();
	LPDIRECT3D9	getDirect3D();
	LPDIRECT3DDEVICE9	getDevice9();
	CDInput*			getDInput();
	BOOL		getWindowed();
	BOOL		getChangeDisplayMode();
	LPBYTE		getKeyStateArray();
	HWND		GetWindowHandle()
	{
		return m_hWnd;
	}
};