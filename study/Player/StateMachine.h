#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/DirectX/DInput.h"
#include"../Player/Player.h"


#define ROW			4
#define COLUMN		5

//=====クラス宣言=====//
class CStateMachine
{
	//privateメンバ変数
private:
	enum State
	{
		STATE_N_Wait=0,
		STATE_N_Walk=1,
		STATE_N_Swim=2,
		STATE_N_Agari=3,
		STATE_B_Wait=4,
		STATE_B_Walk=5,
		STATE_B_Throw=6,
		STATE_B_ThrowEnd=7,
		STATE_W_Wait=8,
		STATE_W_Walk=9,
		STATE_W_Have=10,
		STATE_W_StandBy=11,
		STATE_W_Slide=12,
		STATE_BG_Wait=13,
		STATE_BG_Walk=14,
		STATE_BG_Shot=15,
		STATE_IW_Wait=16,
		STATE_N_Pick=17
	};

	enum StateColumn
	{
		STATE_Ground,
		STATE_Water,
		STATE_BBall,
		STATE_BGun,
		STATE_WGun,
	};

	CPlayer* m_pPlayer;
	CDInput* m_pDInput;
	CSoundManager* m_pSoundManager;

	BOOL CheckKeyPickUpButton();
	void ChangeDistance();

	/*ステートの種類*/
	void StateNeutralWait();
	void StateNeutralWalk();
	void StateNeutralSwim();
	void StateNeturalSwimUp();
	void StateBallWait();
	void StateBallWalk();
	void StateBallThrow();
	void StateBallThrowEnd();
	void StateWaterGWait();
	void StateWaterGWalk();
	void StateWaterGHave();
	void StateWaterGStandBy();
	void StateWaterGSlider();
	void StateBubbleGWait();
	void StateBubbleGWalk();
	void StateBubbleShot();
	void StateInWaterWait();
	void StateNeutralPickUp();

	void SetAudio(int number,BOOL loop);
	//State m_State_;
	StateColumn m_State_;
	int stateNumber;
	int weaponNumber;

	static void (CStateMachine::*const handler_[COLUMN][ROW])();
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:

	CStateMachine() : m_State_(STATE_Ground) {}
	void ResetState();
	BOOL CreateResource(CDInput* pDInput, CPlayer*,CSoundManager* pSManager);
	void Update();
	void event(int num);
	BOOL CheckPlayerPositionWaterSlider();
	BOOL PlayerMoveing();
	BOOL PlayerWait();
	int CheckPlayerPosition();
	void CheckPlayerHaveWeapon();
	void SetState(StateColumn state) { m_State_ = state; }
	void SetStateNumber(int num) { stateNumber = num; }
	void SetAnimationNum(int num) { if(m_pPlayer->GetAnimationNum() != num)m_pPlayer->SetAnimationNum(num); }
	void SetWeaponNumber(int weapon) { weaponNumber = weapon; }
};