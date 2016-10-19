#include "Player.h"
#include"StateMachine.h"


BOOL CPlayer::InWater = NULL;

//====================================
//関数名	:CPlayer
//引数		:pDevice	:描画デバイス
//			 keyState	:キー入力の管理
//			 pFont		:フォント
//戻り値	:
//内容		:コンストラクタ
//====================================
CPlayer::CPlayer()
{

	/*プレイヤーの情報初期化*/
	SetPlayerPos(D3DXVECTOR3(-155.0f, 70.0f, 20.0f));
	m_pInfo.vRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	//何も持っていない
	m_pInfo.playerState = STATE_NONE;
	vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//デバック用
	//m_vPosition = D3DXVECTOR3(-150,62,30);
	m_Speed = 0.5f;
	ChangeCamera = FALSE;

	//m_pWeapon = NULL;
	m_pMathLib = nullptr;
	m_isHitMapNum = 0;
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i] = NULL;
	}
}

//====================================
//関数名	:~CPlayer
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CPlayer::~CPlayer()
{
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		SAFE_DELETE(m_pYukiAnim[i]);
	}
	SAFE_DELETE(m_pMathLib);
	SAFE_DELETE(m_pWaterSlider);
	//m_pWeapon = NULL;

};

//====================================
//関数名	:ResetPlayer
//引数		:
//戻り値	:
//内容		:ゲームを終了時またはリトライ時に情報をリセットする
//====================================
void CPlayer::ResetPlayer()
{
	SetPlayerPos(D3DXVECTOR3(-155.0f, 62.0f, 20.0f));
	m_pInfo.vRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_pInfo.playerState = STATE_NONE;
	ChangeCamera = FALSE;
	vViewAngle = D3DXVECTOR3(D3DX_PI/4.0f, 0.0f, 0.0f);
	CCamera::GetInstance()->SetViewAngle(vViewAngle);
	CCamera::GetInstance()->SetDistance(34.0f);
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i]->SetPosition(m_pInfo.vPosition);
		m_pYukiAnim[i]->set_angle(m_pInfo.vRotation);
	}
	m_pWaterSlider->SetIsSlider(FALSE);
	m_pWaterSlider->ResetValue();
	//m_pWeapon = NULL;
	state->ResetState();
	SetAnimationNum(0);
	//weaponNum = 0;
	
}

//====================================
//関数名	:CreateResource
//引数		:pDevice:描画デバイス
//戻り値	:
//内容		:リソースの作成
//====================================
BOOL CPlayer::CreateResource(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput,CSoundManager* pSManager,CPlayer*pPlayer)
{
	//描画デバイス
	m_pDevice9 = pDevice9;
	m_pInput = pInput;
	m_pSoundManager = pSManager;

	//m_pSoundManager = pSManager;
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i] = new YukiAnimation(m_pDevice9,MOTIONOBJECT[i]);
		m_pYukiAnim[i]->SetPosition(m_pInfo.vPosition);
	}
	state = new CStateMachine();
	state->CreateResource(m_pInput, pPlayer,pSManager);
	//数学計算式用関数
	m_pMathLib = new CMathLib();
	//ウォータースライダー
	m_pWaterSlider = new CWaterSlider();
	//SetAnimationNum(STATE_N_Wait);
	return TRUE;
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CPlayer::Update()
{	
	MyOutputDebugString("X:%f　Y:%f  Z:%f\n", m_pInfo.vPosition.x,m_pInfo.vPosition.y,m_pInfo.vPosition.z);

	vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	InWater = FALSE;
	//m_pInput->GetJoyStickState(&m_JoyState);
	//ウォータースライダの処理に入るかどうか
	if (CheckInWaterSlider())
	{
		InWater = TRUE;
	}
	//ウォータースライダーの中にいる場合
	if (m_pWaterSlider->GetisSlider())
	{
		InWaterSlider();
	}
	else
	{
		//地上と水中でのアップデート
		GroundAndInWaterUpdate();
	}
	state->Update();
	#ifdef _DEBUG
		CheckHitMapNum();
	#endif // DEBUG
	
	
	/*道具更新処理*/
	//PlayerAction();

	//ユキちゃんの方向・向きをアニメーションに渡す
	m_pYukiAnim[GetAnimationNum()]->set_angle(m_pInfo.vRotation);
	m_pYukiAnim[GetAnimationNum()]->SetPosition(m_pInfo.vPosition);
	//アニメーション情報更新
	m_pYukiAnim[m_AnimationNum]->Update();
}

//====================================
//関数名	:InputKeyUpdate
//引数		:viewAngle:カメラが向いている方向
//戻り値	:
//内容		:入力されたキーに応じて移動する
//====================================
void CPlayer::InputKeyUpdate(D3DXVECTOR3 viewAngle)
{
	//左右上
	if (m_pInput->LAnalogState(LEFTRIGHT_UP))
	{
		MoveDirection(LEFTRIGHT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	else if (m_pInput->LAnalogState(RIGHTLEFT_UP))
	{
		MoveDirection(RIGHTLEFT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);
	}
	else if (m_pInput->LAnalogState(LEFTLEFT_UP))
	{
		MoveDirection(LEFTLEFT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	else if (m_pInput->LAnalogState(RIGHTRIGHT_UP))
	{
		MoveDirection(RIGHTRIGHT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);
	}
	else if (m_pInput->LAnalogState(LEFTLEFT_DOWN))
	{
		MoveDirection(LEFTLEFT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	else if (m_pInput->LAnalogState(RIGHTRIGHT_DOWN))
	{
		MoveDirection(RIGHTRIGHT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);

	}
	else if (m_pInput->LAnalogState(LEFTRIGHT_DOWN))
	{
		MoveDirection(LEFTRIGHT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	else if (m_pInput->LAnalogState(RIGHTLEFT_DOWN))
	{
		MoveDirection(RIGHTLEFT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);

	}
	//左上
	else if ((m_pInput->KeyState(DIK_Q)) || (m_pInput->LAnalogState(UP_LEFT)))
	{
		MoveDirection(UP_LEFT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	//右上
	else if ((m_pInput->KeyState(DIK_E)) || (m_pInput->LAnalogState(UP_RIGHT)))
	{
		MoveDirection(UP_RIGHT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);

	}
	//左下
	else if ((m_pInput->KeyState(DIK_Z)) || (m_pInput->LAnalogState(DOWN_LEFT)))
	{
		MoveDirection(DOWN_LEFT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	//右下
	else if ((m_pInput->KeyState(DIK_C)) || (m_pInput->LAnalogState(DOWN_RIGHT)))
	{
		MoveDirection(DOWN_RIGHT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);
	}
	//上
	else if ((m_pInput->KeyState(DIK_W)) || (m_pInput->LAnalogState(UP_MOVE)))
	{
		MoveDirection(UP_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI, 0.0f);
	}
	//下
	else if ((m_pInput->KeyState(DIK_S)) || (m_pInput->LAnalogState(DOWN_MOVE)))
	{
		MoveDirection(DOWN_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + 0.0f, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + 0.0f, 0.0f);
	}
	//左
	else if ((m_pInput->KeyState(DIK_D)) || (m_pInput->LAnalogState(RIGHT_MOVE)))
	{
		MoveDirection(RIGHT_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + (-D3DX_PI / 2), 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + (-D3DX_PI / 2), 0.0f);
	}
	//右
	else if ((m_pInput->KeyState(DIK_A)) || (m_pInput->LAnalogState(LEFT_MOVE)))
	{
		MoveDirection(LEFT_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 2, 0.0f);
	}
}

//====================================
//関数名	:MoveDirection
//引数		:num:方向
//戻り値	:
//内容		:方向に応じて移動値を決定する
//====================================
void CPlayer::MoveDirection(char num)
{
	float moveSpeed = 1.0f;
	if (num == UP_MOVE || num == DOWN_MOVE || num == RIGHT_MOVE || num == LEFT_MOVE)
	{
		//斜め移動以外
		moveSpeed = 1.0f;
	}
	//斜め移動の場合
	else
	{
		//移動係数0.71をかける
		moveSpeed = 0.71f;
	}


	switch (num)
	{
		/*四方向*/
	case UP_MOVE:
		m_vDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case DOWN_MOVE:
		m_vDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f)*moveSpeed;
		break;
	case RIGHT_MOVE:
		m_vDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f)*moveSpeed;
		break;
	case LEFT_MOVE:
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, 0.0f)*moveSpeed;
		break;
		/*八方向*/
	case UP_LEFT:
	//	m_vDirection = D3DXVECTOR3(-0.5f, 0.0f, 0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case UP_RIGHT:
		//m_vDirection = D3DXVECTOR3(0.5f, 0.0f, 0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(1.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case DOWN_LEFT:
		//m_vDirection = D3DXVECTOR3(-0.5f, 0.0f, -0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, -1.0f)*moveSpeed;
		break;
	case DOWN_RIGHT:
		//m_vDirection = D3DXVECTOR3(0.5f, 0.0f, -0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(1.0, 0.0f, -1.0)*moveSpeed;
		break;
		/*十六方向*/
	case LEFTRIGHT_UP:
		//m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, 0.75f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, 0.75f)*moveSpeed;
		break;
	case LEFTRIGHT_DOWN:
		m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, -0.75f)*moveSpeed;
		break;

	case LEFTLEFT_UP:
		m_vDirection = D3DXVECTOR3(-0.75f, 0.0f, 0.25f)*moveSpeed;
		break;
	case LEFTLEFT_DOWN:
		m_vDirection = D3DXVECTOR3(-0.75f, 0.0f, -0.25f)*moveSpeed;
		break;
	
	case RIGHTLEFT_UP:
		m_vDirection = D3DXVECTOR3(0.25f, 0.0f, 0.75f)*moveSpeed;
		break;
	case RIGHTLEFT_DOWN:
		m_vDirection = D3DXVECTOR3(0.25f, 0.0f, -0.75f)*moveSpeed;
		break;
	
	case RIGHTRIGHT_UP:
		m_vDirection = D3DXVECTOR3(0.75f, 0.0f, 0.25f)*moveSpeed;
		break;
	case RIGHTRIGHT_DOWN:
		m_vDirection = D3DXVECTOR3(0.75f, 0.0f, -0.25f)*moveSpeed;
		break;

	}
	//Playerの移動
	PlayerMove();

}

//====================================
//関数名	:PlayerMove
//引数		:
//戻り値	:
//内容		:プレイヤーの移動処理の関数
//====================================
void CPlayer::PlayerMove()
{
	D3DXMATRIX matRotation;
	D3DXVECTOR3 newPostion;

	D3DXMatrixRotationY(&matRotation, CCamera::GetInstance()->GetViewAngle().y);
	D3DXVec3TransformCoord(&m_vDirection, &m_vDirection, &matRotation);
	m_vDirection *= m_Speed;
	m_pInfo.vPosition = m_pYukiAnim[GetAnimationNum()]->GetPosition();
	newPostion = m_pInfo.vPosition + m_vDirection;


	///密城変更分(2016/07/15)
	///*当たり判定*/
	float meshDistance;
	m_beforeIsHitMapNum = m_isHitMapNum;

	Collider* pColManager = Collider::GetInstance();

	D3DXVECTOR3 checkrota(-sin(m_pInfo.vRotation.y), 0.0f, -cos(m_pInfo.vRotation.y));

	if (!pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &meshDistance)) {

		if (meshDistance < 4.0f) {

			if (pColManager->RayCast(newPostion, m_pInfo.vRotation) ||
				pColManager->RayCast(newPostion, m_pInfo.vRotation + D3DXVECTOR3(0, 0.5, 0)) ||
				pColManager->RayCast(newPostion, m_pInfo.vRotation - D3DXVECTOR3(0, 0.5, 0))
				) {
				if (pColManager->RayCast(newPostion, D3DXVECTOR3(0, D3DX_PI / 2, 0)) ||
					pColManager->RayCast(newPostion, D3DXVECTOR3(0, -D3DX_PI / 2, 0))) {
					newPostion.x = newPostion.x - m_vDirection.x;
				}
				if (pColManager->RayCast(newPostion, D3DXVECTOR3(0, 0, 0)) ||
					pColManager->RayCast(newPostion, D3DXVECTOR3(0, D3DX_PI, 0))) {
					newPostion.z = newPostion.z - m_vDirection.z;
				}
			}
		}
		m_pInfo.vPosition = newPostion;
		m_pInfo.vPosition.y += meshDistance;
	}
	///ここまで

	//Collider* pColManager = Collider::GetInstance();
	////pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num);
	//if (pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num))
	//{
	//	//	/*	m_vPosition = newPostion;
	//	//	m_vPosition.y += num;*/
	//}
	//else
	//{
	//	if (num < 4.0f)
	//	{
	//		float no;
	//		if (pColManager->RayCast(newPostion, m_pInfo.vRotation, &no))
	//		{
	//			return;
	//		}
	//	}
	//	m_pInfo.vPosition = newPostion;
	//	m_pInfo.vPosition.y += num;
	//}
	//m_pInfo.vPosition = newPostion;
	//m_pInfo.vPosition.y += num;
	m_pYukiAnim[GetAnimationNum()]->SetPosition(newPostion);
	//SEの開始
	/*if (m_beforeIsHitMapNum != 1 && m_isHitMapNum == 1)
	{
		m_pSoundManager->Play(11, TRUE);
	}
	if (m_beforeIsHitMapNum == 1 && m_isHitMapNum != 1)
	{
		m_pSoundManager->Stop(6);
		m_pSoundManager->Stop(11);
		m_pSoundManager->Play(6, FALSE);

	}*/
}


//====================================
//関数名	:GroundAndInWaterUpdate
//引数		:
//戻り値	:
//内容		:地上と水中内でのアップデート
//====================================
void CPlayer::GroundAndInWaterUpdate()
{
	
	if (InWater)
	{
		InWaterUpdate();
	}
	vViewAngle = CCamera::GetInstance()->GetViewAngle();

	if (m_isHitMapNum != 1)
	{
		//水の中以外なら
		vViewAngle.x = D3DX_PI / 4;
	}
	else
	{
		vViewAngle.x = D3DX_PI / 10.0f;
	}
	Collider *pColManager = Collider::GetInstance();

	//ユキちゃんのアングルを変更するところ
	if ((m_pInput->IsButtonDown(L1_BUTTON)) || (m_pInput->KeyState(DIK_UP)) || (m_pInput->RAnalogState(1)))
	{
		vViewAngle.y -= 0.05f;
	}
	if ((m_pInput->IsButtonDown(R1_BUTTON)) || (m_pInput->KeyState(DIK_DOWN)) || (m_pInput->RAnalogState(0)))
	{
		vViewAngle.y += 0.05f;
	}
	///密城変更分
	//カメラ自身の当たり判定
	float getdis;
	//壁のみの当たり判定を行う
	pColManager->DirectRayCast(m_pInfo.vPosition, vViewAngle, 4, &getdis);

	//カメラの壁にめり込まないように調節更新
	if (getdis >  QUARERVIEW_WALL_DIS)
	{
		getdis = QUARERVIEW_WALL_DIS;
	}
	CCamera::GetInstance()->SetDistance(getdis / (QUARERVIEW_WALL_DIS / QUATERVIEW_DIS));
	CCamera::GetInstance()->SetViewAngle(vViewAngle);

	//キー入力の判定
	InputKeyUpdate(vViewAngle);

	if (m_isHitMapNum == 1)
	{
		PlayerMoveInWater();
	}

}

//====================================
//関数名	:InWaterUpdate
//引数		:
//戻り値	:
//内容		:水の中にいる場合のアップデート
//====================================
void CPlayer::InWaterUpdate()
{
	/*m_pSoundManager->Stop(12);
	m_pSoundManager->Stop(13);
	m_pSoundManager->Play(13, FALSE);*/
	InWater = FALSE;
}

int CPlayer::CheckHitMapNum()
{
	float distance;
	int hitMap;
	Collider::GetInstance()->GroundCol(m_pInfo.vPosition, D3DXVECTOR3(0, 0, 0), &hitMap,&distance);
	//MyOutputDebugString("%d", hitMap);
	return hitMap;
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CPlayer::Draw()
{
	m_pYukiAnim[m_AnimationNum]->Draw();
}



//====================================
//関数名	:PlayerAction
//引数		:
//戻り値	:
//内容		:プレイヤーが行う動作
//====================================
void CPlayer::PlayerAction()
{
	if ((m_pInput->KeyState(DIK_SPACE)) || m_pInput->IsButtonDown(CIRCLE_BUTTON))
	{
		if (m_pWeapon != NULL)
		{
			if (m_pWeapon->GetState() == Have)
			{
				m_pWeapon->SetPos(m_pInfo.vPosition);
				m_pWeapon->SetRot(m_pInfo.vRotation);
			}
			//武器で攻撃する
			m_pWeapon->Attack();
		}
	}
}

//====================================
//関数名	:PlayerMoveInWater
//引数		:
//戻り値	:
//内容		:流れるプール(中央の処理)
//====================================
void CPlayer::PlayerMoveInWater()
{
	D3DXVECTOR2 tmp;
	tmp = m_pMathLib->MyVecAngle(m_pInfo.vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DX_PI / 1000);
	float num;
	Collider* pColManager = Collider::GetInstance();
	if (pColManager->GroundCol(D3DXVECTOR3(tmp.x, m_pInfo.vPosition.y, tmp.y), D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num))
	{
		/*	m_vPosition = newPostion;
		m_vPosition.y += num;*/
	}
	else
	{
		//if (GetAnimationNum() == STATE_IW_Wait)
		//{
		//	m_pInfo.vPosition.y += (num - 4.0f);
		//}
		//else if (GetAnimationNum() == STATE_N_Swim)
		//{
		//	/*	m_vPosition = newPostion;*/
		//	m_pInfo.vPosition.y += num;
		//}
	}
	SetPlayerPos(D3DXVECTOR3(tmp.x, m_pInfo.vPosition.y, tmp.y));
}

//====================================
//関数名	:CheckInWaterSlider
//引数		:
//戻り値	:
//内容		:ウォータースライダーの中にいるかどうか判定する
//====================================
BOOL CPlayer::CheckInWaterSlider()
{
	if (m_pInfo.vPosition.y > 62 && m_pInfo.vPosition.z > 25 && m_pInfo.vPosition.z < 27)
	{
	/*	m_pSoundManager->Stop(12);
		m_pSoundManager->Play(12, TRUE);*/
		m_pWaterSlider->SetIsSlider(TRUE);
		//SetAnimationNum(STATE_W_Slide);
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:InWaterSlider
//引数		:
//戻り値	:
//内容		:ウォータースライダーにプレイヤーがいる場合にプレイヤーの状態を更新する
//====================================
void CPlayer::InWaterSlider()
{
	m_pWaterSlider->SetPlayerPos(m_pInfo.vPosition);
	m_pWaterSlider->SetPlayerRot(m_pInfo.vRotation);
	m_pInfo.vPosition = m_pWaterSlider->WaterSliderAction(FALSE);
	m_pInfo.vRotation.y = m_pWaterSlider->GetRot();
	//if (m_pInfo.vPosition.y <= 10.0f)
	//{
	//	m_isHitMapNum = 1;
	//}
}

//====================================
//関数名	:SetWeapon
//引数		:pWeapon:武器管理クラス
//戻り値	:
//内容		:武器管理クラスへ各武器のアドレスをセットする
//====================================
CAbstractWeapon * CPlayer::SetWeapon(CAbstractWeapon* pWeapon)
{
	m_pWeapon = pWeapon;
	return m_pWeapon;
}

void CPlayer::SetHaveWeaponInfo(int weapon)
{
	weaponNum = weapon;
}
