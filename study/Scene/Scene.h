#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/Fade.h"
#include"../Lib/DirectX/DInput.h"
#include"../Lib/DirectX/DXFont.h"
#include"GameScene/Title.h"
#include"GameScene/Operating.h"
#include"GameScene/Main.h"
#include"GameScene/Result.h"
#include"GameScene/GameOver.h"
#include"GameScene/Credit.h"
#include"GameScene/Ranking.h"

//=====クラス宣言=====//
class CScene
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9		m_pDevice9;
	LPDIRECT3DTEXTURE9      m_ShadowMapTexture;
	////フォント
	CDXFont*				m_pFont;
	//キー入力格納用配列
	BYTE*					m_byKeyState;
	//ジョイスティックの情報
	DIJOYSTATE2*			m_pJoyState;
	//テクスチャ・サウンドのポインタ
	CDSystem*				m_pDSystem;
	//キー情報
	CDInput*				m_pDInput;
	//サウンドマネージャー
	CSoundManager*			m_pSoundManager;
	//現在のゲームシーン
	DWORD					m_NowGameScene;

	//マップ
	//=====各シーンの読み込み=====//
	CTitle*					m_pTitle;
	COperating*				m_pOperat;
	CMain*					m_pMain;
	CResult*				m_pResult;
	CGameOver*				m_pGameOver;
	CCredit*				m_pCredit;
	CRanking*				m_pRanking;


	//サウンドを一回鳴らす用
	BOOL					m_isOnce;
	////publicメンバ変数
public:
	D3DXVECTOR3 CameraPos;
	D3DXVECTOR3 CameraRot;
	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CScene();
	//デストラクタ
	~CScene();

	//=====各処理の関数=====//
	void CleanupObject();
	BOOL CreateObject();
	BOOL CreateResource(LPDIRECT3DDEVICE9, HWND, BYTE*, DIJOYSTATE2*, CDXFont*, CDInput*);


	//ゲーム状態の初期化
	void AllGameInitialize();
	//メインゲームの実行処理
	void MainGame();
	//ゲーム処理の統合
	void SceneChange();
	//

	void OnLostDevice();
	void OnResetDevice();
};