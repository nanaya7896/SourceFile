#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/DirectX/DXFont.h"
#include"../Lib/DirectX/DInput.h"
#include"YukiAnimation.h"
#include"../Lib/Collider.h"
#include"../Math/MyMathLib.h"
#include"WaterSlider.h"
#include "../Lib/Sound/SoundManager.h"
#include"../Weapon/AbstractWeapon.h"
#include"../Actor/Actor.h"
//#include"StateMachine.h"

#define MAXMOTION_NUM		18
#define ColNum				 4
#define QUARERVIEW_WALL_DIS		100
#define QUATERVIEW_DIS			30
#define SIDEVIEW_WALL_DIS		60
#define SIDEVIEW_DIS			10

/*コントローラー*/
#define UP_MOVE				0
#define DOWN_MOVE			1
#define LEFT_MOVE			2
#define RIGHT_MOVE			3
#define UP_LEFT				4
#define UP_RIGHT			5
#define DOWN_LEFT			6
#define DOWN_RIGHT			7
#define LEFTRIGHT_UP		8
#define RIGHTLEFT_UP		9
#define LEFTLEFT_UP			10
#define RIGHTRIGHT_UP		11
#define LEFTLEFT_DOWN		12
#define RIGHTRIGHT_DOWN		13
#define LEFTRIGHT_DOWN		14
#define RIGHTLEFT_DOWN		15

class CStateMachine;
//プレイヤー
//アニメーション
//enum PAnimNum
//{
//	STATE_N_Wait,
//	STATE_N_Walk,
//	STATE_N_Swim,
//	STATE_N_Agari,
//	STATE_B_Wait,
//	STATE_B_Walk,
//	STATE_B_Throw,
//	STATE_B_ThrowEnd,
//	STATE_W_Wait,
//	STATE_W_Walk,
//	STATE_W_Have,
//	STATE_W_StandBy,
//	STATE_W_Slide,
//	STATE_BG_Wait,
//	STATE_BG_Walk,
//	STATE_BG_Shot,
//	STATE_IW_Wait,
//	STATE_N_Pick
//};

struct PlayerInfo
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vRotation;
	int playerState;
};

class CActor;

//=====クラス宣言=====//
class CPlayer : public CActor
{
//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9   m_pDevice9;
	//フォント
	CDXFont				*m_pFont;
	//キー
	CDInput*			m_pInput;
	//サウンドマネージャー
	CSoundManager*		m_pSoundManager;
	//アニメーション
	YukiAnimation*		m_pYukiAnim[MAXMOTION_NUM];
	//自作の数学関数
	CMathLib*				m_pMathLib;
	//武器クラス
	CAbstractWeapon*		m_pWeapon;
	//ウォータースライダーの処理
	CWaterSlider*			m_pWaterSlider;

	D3DXVECTOR3			m_vFlowPos[101] = {};
	DIJOYSTATE2			m_JoyState;
	D3DXVECTOR3			m_vDirection;
	FLOAT				m_Speed;
	BOOL    ChangeCamera;
	D3DXVECTOR3 vViewAngle;
	//アニメーション番号
	INT					m_AnimationNum;
	//プレイヤー情報の構造体
	PlayerInfo			m_pInfo;
	int					m_isHitMapNum;
	int					m_beforeIsHitMapNum;
	BOOL m_setSlider;
	BOOL m_isValueSet = TRUE;
	D3DXVECTOR3			m_bbpos;

	static BOOL InWater;
	int weaponNum;
	CStateMachine* state;

	//アニメーション読み込み
	TCHAR MOTIONOBJECT[MAXMOTION_NUM][MAX_PATH] =
	{
		//何も持っていない状態
		"Model/yuki_taiki.X",					// 0
		"Model/yuki_hokou.X",					// 1
		"Model/yuki_swim.X",					// 2
		"Model/yuki_agari.X",					// 3
		//ボールを持っているとき				    
		"Model/yuki_bo_taiki.X",				// 4
		"Model/yuki_bo_hokou.X",				// 5
		"Model/yuki_bool.X",					// 6
		"Model/yuki_boolend.X",					// 7
		//ウォーターガンを持っているとき		    
		"Model/yuki_wa_taiki.X",				// 8
		"Model/yuki_wa_hokou.X",				// 9
		"Model/yuki_watergan.X",				// 10
		"Model/yuki_w_standby.X",				// 11
		"Model/yuki_w_suberi.X",				// 12
		//バブルガンをもっているとき				  
		"Model/yuki_ba_taiki.X",				// 13
		"Model/yuki_ba_hokou.X",				// 14
		"Model/yuki_baburugan.X",				// 15
		//水の中での待機
		"Model/yuk_pooltaiki.X",				// 16
		//拾うモーション
		"Model/yuki_hiroi.X"					// 17
	};
	//publicメンバ変数
public:
	//privateメンバ関数
private:
	//publicメンバ関数
public:
	//コンストラクタ
	CPlayer();
	//デストラクタ
	~CPlayer();
	//リソースの作成
	BOOL CreateResource(LPDIRECT3DDEVICE9, CDInput*, CSoundManager*,CPlayer*);
	void Update();
	void InputKeyUpdate(D3DXVECTOR3 viewAngle);
	void MoveDirection(char num);
	void PlayerMove();
	void Draw();
	void ResetPlayer();
	void PlayerAction();
	void PlayerMoveInWater();

	BOOL CheckInWaterSlider();
	
	void InWaterSlider();

	//void ChangePlayerState();
	//void ChangeMovePlayerState();
	//void ChangeAnimation();
	void GroundAndInWaterUpdate();
	void InWaterUpdate();

	int CheckHitMapNum();
	BOOL isAnimationFinishTime()
	{
		if (m_pYukiAnim[GetAnimationNum()]->getFinishTime() <= m_pYukiAnim[GetAnimationNum()]->getTime())
		{
			m_pYukiAnim[GetAnimationNum()]->ResetTime();
			return TRUE;
		}
		return FALSE;
	}

	//======ゲット関数=====//
	D3DXVECTOR3 GetPlayerPos()
	{
		return m_pInfo.vPosition;
	}
	INT GetAnimationNum()
	{
		return m_AnimationNum;
	}
	D3DXVECTOR3 GetPlayerAngle()
	{
		return  m_pYukiAnim[GetAnimationNum()]->GetAngle();
	}
	BOOL GetSlider()
	{
		return m_pWaterSlider->GetisSlider();
	}
	int GetNum()
	{
		return m_pWaterSlider->GetNum();
	}
	int GetHitMapNum()
	{
		return m_isHitMapNum;
	}
	CAbstractWeapon*	GetWeapon()
	{
		return m_pWeapon;
	}

	BOOL GetEnterMesh(int mesh) {

		if (m_isHitMapNum != mesh) {
			return FALSE;
		}

		if (m_beforeIsHitMapNum != m_isHitMapNum) {
			return TRUE;
		}

		return FALSE;
	}
	
	//======セット関数=====//
	void SetAnimationNum(INT animNumber)
	{
		m_AnimationNum = animNumber;
	}
	void SetPlayerPos(D3DXVECTOR3 pos)
	{
		m_pInfo.vPosition = pos;
	}
	void SetPlayerPosY(FLOAT pos)
	{
		m_pInfo.vPosition.y =  pos;
	}
	
	void SetPlayerRot(D3DXVECTOR3 rot)
	{
		m_pInfo.vRotation = rot;
	}
	void SetPlayerState(int state){	m_pInfo.playerState = state;}

	CAbstractWeapon*	SetWeapon(CAbstractWeapon* pWeapon);
	void SetHaveWeaponInfo(int weapon);
	
	
};
