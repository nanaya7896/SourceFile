#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Player/Player.h"
#include"../Score.h"
#include"../../Camera/Camera.h"
#include"../../Lib/DirectX/DInput.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
#include"../Field.h"
#include"../../Graphic/DirectXAnimation/AllocateHierarchy.h"
#include"../../Zombie/Zombie.h"
#include"Result.h"
#include"../../Design/Decorate.h"
#include"../../Weapon/AbstractWeapon.h"
#include"../../Weapon/BB.h"
#include"../../Weapon/BG.h"
#include"../../Weapon/WG.h"
#include"../../Actor/ActorFactory.h"

//=====define値=====//
#define MAX_TIME		120
#define MAX_DIGIT		2
#define ZOMBIE_NUM		20
//=====クラス宣言=====//
class CMain
{
	//privateメンバ変数
private:
	LPDIRECT3DDEVICE9		m_pDevice9;
	//プレイヤーの読み込み
	CPlayer*				m_pPlayer;
	//キー入力用
	CDInput*				m_pInput;
		//フィールド描画用
	CFiled*					m_pField;
	//リザルト
	CResult*				m_pResult;
	//ゾンビ
	CZombie*				m_pZombie;
	//サウンドマネージャー
	CSoundManager*			m_pSoundManager;
	//デコレーター
	CDecorate*				m_pDecorate;
	//武器管理クラス
	CAbstractWeapon	m_pWeapon;
	//ビーチボール
	Cbb		m_BB;
	//水鉄砲
	Cwg     m_WG;
	//シャボン玉
	Cbg		m_BG;

	CActorFactory			 m_ActorFactory;

	//時間進行用
	int TimeFrame;

	//時間管理
	int m_Time;

	BOOL m_Ready;
	BOOL m_Go;
	int time;
	//
	D3DXVECTOR2 m_Alpha;
	D3DXVECTOR2 m_Value;
	float       m_Height;
	//publicメンバ変数
public:

	//privateメンバ関数
private:
	enum HavePlayer
	{
		None,
		BBall,
		WGun,
		BGun,


	};

	//publicメンバ関数
public:
	//コンストラクタ
	CMain(/*LPDIRECT3DDEVICE9, CDInput**/);
	//デストラクタ
	~CMain();
	//リソース作成
	void CreateResource(LPDIRECT3DDEVICE9,CDInput*,CSoundManager*);
	//更新
	void Update();
	//描画
	void Draw();
	//ゲームのリセット
	void AllGameReset();
	//時間更新
	void TimeUpdate(int digit, BOOL isPull);
	//楽死んだゾンビの数を描画
	void ZombieValueDraw();
	//Ready処理
	void ReadyDraw();

	BOOL PlayerAndEnemyHitCheck();
	BOOL isDeadCheck();

	void AllGetWeapon();
	void AllPutWeapon();
	BOOL AllHitCheck(int num);
	BOOL BBHitCheck(int num);

	void SetResult(CResult* pResult)
	{
		m_pResult = pResult;
	}
	//ゲット関数
	BOOL GetLimitTime();
	CPlayer* GetpPlayer() 
	{
		return m_pPlayer; 
	}

	BOOL GetisSlider()
	{
		return m_pPlayer->GetSlider();
	}
	int GetisNum()
	{
		return m_pPlayer->GetNum();
	}

	int GetWeaponNumber()
	{
		if (m_pPlayer->GetWeapon() == NULL){return None;}//何ももっていない
		if (m_pPlayer->GetWeapon() == &m_BB){return BBall;}
		if (m_pPlayer->GetWeapon() == &m_WG){return WGun;}
		if (m_pPlayer->GetWeapon() == &m_BG){return BGun;}
	}

};
