#include "StateMachine.h"
#include"../Scene/GameScene/Main.h"

//====================================
//関数名	:event
//引数		:num:二次元配列のRowの場所を表す
//戻り値	:
//内容		:イベントを実行する
//====================================
void CStateMachine::event(int num)
{
	(this->*handler_[m_State_][num])();
}

//====================================
//関数名	:CheckPlayerPositionWaterSlider
//引数		:
//戻り値	:
//内容		:プレイヤーがウォータースライダーに入ったかどうか調べる
//====================================
BOOL CStateMachine::CheckPlayerPositionWaterSlider()
{
	if (m_pPlayer->CheckInWaterSlider())
	{
		return TRUE;
	}

	return FALSE;
}

//====================================
//関数名	:PlayerMoveing
//引数		:
//戻り値	:
//内容		:プレイヤーが動いているかどうか
//====================================
BOOL CStateMachine::PlayerMoveing()
{
	if ((m_pDInput->GetDInputJoyStick() != NULL && m_pDInput->AllControllCheck()) || m_pDInput->WASDKeyDownCheck())
	{
		stateNumber = 1;
		//event(1);
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:PlayerWait
//引数		:
//戻り値	:
//内容		:プレイヤーが止まっているかどうか
//====================================
BOOL CStateMachine::PlayerWait()
{
	if ((m_pDInput->GetDInputJoyStick() !=NULL && m_pDInput->AllControllCheck()) || (m_pDInput->WASDKeyUpCheck()))
	{
		stateNumber = 0;
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:CheckPlayerPosition
//引数		:
//戻り値	:
//内容		:プレイヤーがどの地形にいるかチェック
//====================================
int CStateMachine::CheckPlayerPosition()
{
	return m_pPlayer->CheckHitMapNum();
}

//====================================
//関数名	:CheckPlayerHaveWeapon
//引数		:
//戻り値	:
//内容		:プレイヤーがどの遊具を持っているか
//====================================
void CStateMachine::CheckPlayerHaveWeapon()
{

	//
	switch (weaponNumber)
	{
	case 0:
		if (CheckPlayerPosition() == 0)
		{
			m_State_ = STATE_Ground;
		}
		break;
	case 1:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_BBall;
		break;
	case 2:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_WGun;
		break;
	case 3:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_BGun;
		break;
	}
}

//====================================
//関数名	:ResetState
//引数		:
//戻り値	:
//内容		:リプレイ時に必要な値を初期化
//====================================
void CStateMachine::ResetState()
{
	stateNumber = 0;
	weaponNumber = 0;
	m_State_ = STATE_Ground;
	int a = 0;
	int b = 0;
}

//====================================
//関数名	:CreateResource
//引数		:
//戻り値	:
//内容		:一度だけ呼び出して作成
//====================================
BOOL CStateMachine::CreateResource(CDInput* pDInput,CPlayer* pPlayer,CSoundManager* pSManager)
{
	m_pDInput = pDInput;
	m_pPlayer = pPlayer;
	m_pSoundManager = pSManager;
	if (m_pDInput == NULL)
	{
		OutputDebugString("DirectInput is NULL");
		return FALSE;
	}
	//
	stateNumber = 0;



	if (m_pDInput->GetDInputJoyStick() == NULL)
	{
		isController = false;
	}
	else
	{
		isController = true;
	}

	return TRUE;
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CStateMachine::Update()
{
	//ウォータースライダーに入っているか
	if (CheckPlayerPositionWaterSlider())
	{
		stateNumber = 2;
	}
	////武器を持っていてボタンが押されたとき
	//if (weaponNumber != 0 && (m_pDInput->KeyDown(DIK_SPACE) || m_pDInput->IsButtonDown(CIRCLE_BUTTON)))
	//{
	//	stateNumber = 2;
	//}
	//カメラの距離を変更
	ChangeDistance();

	//if (m_State_ != STATE_Water)
	//{
	//	CheckPlayerHaveWeapon();
	//}
	//
	event(stateNumber);
}

//====================================
//関数名	:handler[COLUMN][ROW]
//引数		:
//戻り値	:
//内容		:二次元配列に格納するステートを決定する
//====================================
void (CStateMachine::*const CStateMachine::handler_[COLUMN][ROW])() = 
{
	//ニュートラルのステート
	{&CStateMachine::StateNeutralWait,&CStateMachine::StateNeutralWalk,&CStateMachine::StateWaterGSlider,&CStateMachine::StateNeutralPickUp},
	//水中のステート
	{&CStateMachine::StateInWaterWait,&CStateMachine::StateNeutralSwim,&CStateMachine::StateNeturalSwimUp},
	//ビーチボールのステート
	{&CStateMachine::StateBallWait,&CStateMachine::StateBallWalk,&CStateMachine::StateBallThrow,&CStateMachine::StateBallThrowEnd},
	//バブルガンのステート
	{&CStateMachine::StateBubbleGWait,&CStateMachine::StateBubbleGWalk,&CStateMachine::StateBubbleShot},
	//ウォーターガンのステート
	{&CStateMachine::StateWaterGWait,&CStateMachine::StateWaterGWalk,&CStateMachine::StateWaterGHave,&CStateMachine::StateWaterGStandBy}

};

//====================================
//関数名	:CheckKeyPickUpButton
//引数		:
//戻り値	:
//内容		:拾うが設定されたボタンが押されたか(Key:G JoyPad:SQUARE)
//====================================
BOOL CStateMachine::CheckKeyPickUpButton()
{
	//
	if ((m_pDInput->KeyDown(DIK_G) || m_pDInput->IsButtonDown(SQUARE_BUTTON)))
	{
		stateNumber = 3;
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:ChangeDistance
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::ChangeDistance()
{
	//水の中にいるときにカメラの距離を変更する
	if (m_State_ == STATE_Water)
	{
		CCamera::GetInstance()->SetDistance(13.0f);
	}
}
/*以下ステートの情報*/
//====================================
//関数名	:StateNeutralWait
//引数		:
//戻り値	:
//内容		:待機モーション
//====================================
void CStateMachine::StateNeutralWait()
{
	
	if (PlayerMoveing())
	{
		return;
	}
	//拾う処理
	CheckKeyPickUpButton();
	//アニメーションを切り替える
	SetAnimationNum(STATE_N_Wait);
	//MyOutputDebugString("ステートは待機です\n");
}

//====================================
//関数名	:StateNeutralWalk
//引数		:
//戻り値	:
//内容		:歩く
//====================================
void CStateMachine::StateNeutralWalk()
{
	//プレイヤーが水中状態なら
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
		SetAudio(13, FALSE);
	}
	//プレイヤーが待機中か
	if (PlayerWait())
	{
		return;
	}
	//拾う処理
	if (CheckKeyPickUpButton())
	{
		return;
	}

	SetAnimationNum(STATE_N_Walk);
	//MyOutputDebugString("ステートは歩きです\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateNeutralSwim()
{
	SetAnimationNum(STATE_N_Swim);
	
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition()==0)
	{
		
		m_State_ = STATE_Ground;
		SetAudio(6, FALSE);
		if (weaponNumber != 0)
		{
			CheckPlayerHaveWeapon();
		}
		//stateNumber = 0;
	}
	//MyOutputDebugString("ステートは泳ぎです\n");
}

//====================================
//関数名	:StateNeturalSwimUp
//引数		:
//戻り値	:
//内容		:陸に上がるです
//====================================
void CStateMachine::StateNeturalSwimUp()
{
	//MyOutputDebugString("ステートは陸に上がるです\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateBallWait()
{
	if ((m_pDInput->KeyDown(DIK_H)) || m_pDInput->IsButtonDown(X_MARK_BUTTON))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_B_Wait);
	if (PlayerMoveing())
	{
		return;
	}
	//MyOutputDebugString("ステートはビーチボール待機です\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateBallWalk()
{
	SetAnimationNum(STATE_B_Walk);
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
	}
//	MyOutputDebugString("ステートはビーチボール歩きです\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateBallThrow()
{
	SetAnimationNum(STATE_B_Throw);
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 3;
		return;
	}
	//MyOutputDebugString("ステートはビーチボール投げるです\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateBallThrowEnd()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_B_ThrowEnd);
	//MyOutputDebugString("ステートはビーチボール投げ終わりです\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateWaterGWait()
{
	SetAnimationNum(STATE_W_Wait);
	if (PlayerMoveing())
	{
		return;
	}
	if ((m_pDInput->KeyDown(DIK_H)) || m_pDInput->IsButtonDown(X_MARK_BUTTON))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
//	MyOutputDebugString("ステートは水銃待機です\n");
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateWaterGWalk()
{
	SetAnimationNum(STATE_W_Walk);
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
	}
	//MyOutputDebugString("ステートは水銃歩きです\n");
}

//====================================
//関数名	:StateWaterGHave
//引数		:
//戻り値	:
//内容		:
//====================================
void CStateMachine::StateWaterGHave()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_W_Have);
	//MyOutputDebugString("ステートは水銃を持つです\n");
}

//====================================
//関数名	:StateWaterGStandBy
//引数		:
//戻り値	:
//内容		:ウォータースライダーのモーション
//====================================
void CStateMachine::StateWaterGStandBy()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_W_StandBy);
	//MyOutputDebugString("ステートは水銃構えです\n");
}

//====================================
//関数名	:StateWaterGSlider
//引数		:
//戻り値	:
//内容		:スライダーを流れるステートです。
//====================================
void CStateMachine::StateWaterGSlider()
{
	SetAudio(12, TRUE);
	SetAnimationNum(STATE_W_Slide);
	if (CheckPlayerPosition() == 1)
	{
		stateNumber = 0;
		m_State_ = STATE_Water;
		SetAudio(13, FALSE);
		return;
	}
	//MyOutputDebugString("ステートはウォータースライダー流れです\n");
}

//====================================
//関数名	:StateBubbleGWait
//引数		:
//戻り値	:
//内容		:バブルガン待機のステートです
//====================================
void CStateMachine::StateBubbleGWait()
{
	SetAnimationNum(STATE_BG_Wait);

	if (PlayerMoveing())
	{
		return;
	}
	if ((m_pDInput->KeyDown(DIK_H) || m_pDInput->IsButtonDown(X_MARK_BUTTON)))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
	}
	//MyOutputDebugString("ステートはバブルガン待機です\n");
}

//====================================
//関数名	:StateBubbleGWalk
//引数		:
//戻り値	:
//内容		:バブルガンを所持している状態で歩いています。
//====================================
void CStateMachine::StateBubbleGWalk()
{
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
		return;
	}

	SetAnimationNum(STATE_BG_Walk);
	if (PlayerWait())
	{
		return;
	}

//	MyOutputDebugString("ステートはバブルガン歩きです\n");
}

//====================================
//関数名	:StateBubbleShot
//引数		:
//戻り値	:
//内容		:バブルガンのショットステート
//====================================
void CStateMachine::StateBubbleShot()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_BG_Shot);
//	MyOutputDebugString("ステートはバブルガンショットです\n");
}

//====================================
//関数名	:StateInWaterWait
//引数		:
//戻り値	:
//内容		:水待機ステート
//====================================
void CStateMachine::StateInWaterWait()
{
	SetAnimationNum(STATE_IW_Wait);
	m_pPlayer->SetPlayerPosY(5.0f);

	if (PlayerMoveing())
	{
		return;
	}
	if (CheckPlayerPosition()==0)
	{
		stateNumber = 0;
		return;
	}
//	MyOutputDebugString("ステートは水待機です\n");
}

//====================================
//関数名	:StateNeutralPickUp
//引数		:
//戻り値	:
//内容		:遊具を拾うステート
//====================================
void CStateMachine::StateNeutralPickUp()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		CheckPlayerHaveWeapon();
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_N_Pick);
	
//	MyOutputDebugString("ステートは拾うです\n");
}


void CStateMachine::SetAudio(int number, BOOL loop) {

	//現在流している音を格納(初期設定は-1として設定）
	static int nowPlayNumber = -1;

	//今流している音であれば、そのまま終了
	if (nowPlayNumber == number) {
		return;
	}

	//初期設定以外であれば、今流している音は止める
	if (nowPlayNumber != -1) {
		m_pSoundManager->Stop(nowPlayNumber);
	}

	//選択した音を再生する
	if (number > -1) {
		m_pSoundManager->Play(number, loop);
	}
	//今の番号を格納
	nowPlayNumber = number;

}