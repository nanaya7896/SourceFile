#include "Zombie.h"


//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
CZombie::CZombie()
{
	
	
}

// コンストラクタ
// 引数		pDevice		：	デバイス
CZombie::CZombie(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	value = 0;
	scale_ = D3DXVECTOR3(1.2f, 1.2f, 1.2f);
	//scale_ = D3DXVECTOR3(0.2f,0.2f, 0.2f);
	for (int i = 0; i < NumZombies; i++)
	{
		m_pWaterSlider[i] = NULL;
	}
	m_pMathlib = nullptr;
	m_pDecorate = NULL;
}

// デストラクタ
CZombie::~CZombie()
{
	SAFE_DELETE_ARRAY(m_ZombieInfo);
	for (int i = 0; i < MOTION_ZNUM; i++)
	{
		for (int j = 0; j < SKIN_ZNUM; j++) {
			SAFE_DELETE(m_pAnimation[j][i]);
		}
	}
	SAFE_DELETE(m_pMathlib);
	for (int i = 0; i < NumZombies; i++)
	{
		SAFE_DELETE(m_pWaterSlider[i]);
	}
	SAFE_DELETE(m_pDecorate);
}

// 初期化関数
BOOL CZombie::Initialize()
{
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	// シェーダの読み込み
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// デバイス
		"Effect/Zombie.fx",		// ファイル名
		NULL,			// プリプロセッサ定義へのポインタ
		NULL,			// オプションのインターフェイスポインタ 
		0,				// コンパイルオプション
		NULL,			// エフェクトプールオブジェクトへのポインタ
		&m_pEffect,		// エフェクトオブジェクトへのポインタ
		&pErrorMsgs);	// エラーおよび警告のリストを含むバッファ

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	DWORD Time = 20;		// 20ミリ秒
	for (int i = 0; i < MOTION_ZNUM; i++)
	{
		for (int j = 0; j < SKIN_ZNUM; j++) {
			m_pAnimation[j][i] = new CAnimMesh(m_pDevice, Time, m_pEffect);

			if (!m_pAnimation[j][i]->LoadFromFile(Z_MOTIONOBJECT[j][i]))	// ゾンビのファイル名
				return FALSE;
		}
	}

	for(int i=0;i<NumZombies;i++)
	m_pWaterSlider[i] = new CWaterSlider();
	m_ZombieInfo = new ZOMBIEINFO[MaxZombie];
	m_pMathlib = new CMathLib();
	ZombieInfoInitialize();
	m_pDecorate = new CDecorate();
	m_Speed = 0.4f;
	//アニメーション情報をゾンビ１体ずつ×モーション数を保存
	for (int i = 0; i < MaxZombie; i++) {

		//プログラム上0からスタートさせるのでそのために-1を使用
		m_ZombieInfo[i].skin = Random(0, SKIN_ZNUM - 1);
		for (int j = 0; j < MOTION_ZNUM; j++) {
			m_ZombieInfo[i].AnimController[j] = m_pAnimation[m_ZombieInfo[i].skin][j]->GetAnimController();
		}
	}
	//ゾンビ毎にランダムで再生開始地点を別々にする
	for (int i = 0; i < MaxZombie; i++) {
		FLOAT startTime = Random(0.0f, m_pAnimation[0][0]->GetAnimFinishTime());
		m_ZombieInfo[i].AnimController[0]->AdvanceTime(startTime, NULL);
	}
	return TRUE;
}

//====================================
//関数名	:PositionCheck
//引数		:num:エネミーの番号
//戻り値	:
//内容		:ランダムでエネミーの座標を設定する
//====================================
void CZombie::PositionCheck(int num)
{
	float check;
	int hitNum;
	int value;
	//int value;
	//value = (int)Random(0, 10000);
	/*端の座標値が203前後なのでそれに合わせて作成*/
	//2体だけ例外でゆきちゃんのそばに設定
	if (0 == num) {
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(-155.5f, 54.0f, -15.5f);
		//m_ZombieInfo[num].vAngle = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	}
	else if (1 == num) {
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(-155.5f, 49.0f, -25.5f);
	}
	//まだ仮の状態後ほどなおします。。。
	else {
		do {
			value = (int)Random(0, 10000);
			if (value % 2 == 0)
			{
				/*端の座標値が203前後なのでそれに合わせて作成*/
				m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-200.0f, 200.0f), 14.0f, Random(150.0f, 200.0f));
			}
			else
			{
				m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-200.0f, 200.0f), 14.0f, Random(-150.0f, -200.0f));
			}
			Collider::GetInstance()->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &hitNum, &check);
		} while (hitNum!=0);
	}

	if (Collider::GetInstance()->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &hitNum, &check))
	{
		m_ZombieInfo[num].vPosition.y += check;
	}

}

//====================================
//関数名	:ZombieInfoInitialize
//引数		:
//戻り値	:
//内容		:ゾンビの初期値設定。リセット時も仕様する
//====================================
void CZombie::ZombieInfoInitialize()
{
	/*ゾンビにステータス情報を与える*/
	for (DWORD i = 0; i < NumZombies; i++)
	{
		PositionCheck(i);
		m_ZombieInfo[i].isAlive = TRUE;		
		/*場所・向き*/
		m_ZombieInfo[i].vAngle = D3DXVECTOR3(0.0f, 0.0f,1.0f);

		/*追跡モード*/
		m_ZombieInfo[i].state = CHASER;
		/*/死亡時の生存時間*/
		m_ZombieInfo[i].LifeTime = 120.0f;
		m_ZombieInfo[i].animNum = 0;
		m_ZombieInfo[i].ZombieStaNum = 0;
		m_ZombieInfo[i].z_num = 0;
		/*階段にのぼるかどうかの判定*/
		m_ZombieInfo[i].isStairs = FALSE;
		/*階段を上る試行回数*/
		m_ZombieInfo[i].val = 0;
		/*大きさ*/
		m_ZombieInfo[i].vScale = { 1.2f,1.2f,1.2f };


		
		//m_ZombieInfo[i].AnimController= m_pAnimation[m_ZombieInfo[i].animNum]->Anim
	}
	for (DWORD i = NumZombies; i < MaxZombie; i++)
	{
		if (i == NumZombies)
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(100.0f, 7.5f, 80.0f);
		//	m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, 110.0f);
		}
		else if (i == NumZombies + 1)
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, 100.0f);
		}
		else
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, -60.0f);
		}
		m_ZombieInfo[i].isAlive = TRUE;

		/*場所・向き*/
		m_ZombieInfo[i].vAngle = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		/*追跡モード*/
		m_ZombieInfo[i].state = CHASER;
		/*/死亡時の生存時間*/
		m_ZombieInfo[i].LifeTime = 120.0f;
		m_ZombieInfo[i].animNum = 4;

		/*for (int j = 0; j < MOTION_ZNUM; j++) 
		{
			m_ZombieInfo[i].AnimController[j] = m_pAnimation[j]->GetAnimController();
		}*/
	}
	
}

// 描画関数
// 引数		Number		：	ゾンビの配列番号
void CZombie::Draw(DWORD Number)
{

	//MyOutputDebugString("ZOmbie[50]:%f\n", m_ZombieInfo[50].vPosition.x);
	D3DXMATRIX matView, matProjection, matScaling;
	CCamera* pCamera = CCamera::GetInstance();
	matView = pCamera->getView();
	matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);
	D3DXMATRIX matWorld, matPosition, matRotation;
	//
	if (m_ZombieInfo[Number].isAlive)
	{
		if (m_pEffect)
		{
			// テクニックの選択
			m_pEffect->SetTechnique("ShaderTechnique");
			// パスを指定
			m_pEffect->Begin(0, 0);
			//スケール変換
			D3DXMatrixScaling(&matScaling, m_ZombieInfo[Number].vScale.x, m_ZombieInfo[Number].vScale.y, m_ZombieInfo[Number].vScale.z);

			// 平行移動行列を作成
			D3DXMatrixTranslation(
				&matPosition,		// 演算結果の行列へのアドレス
				m_ZombieInfo[Number].vPosition.x,		// X座標のオフセット
				m_ZombieInfo[Number].vPosition.y,		// X座標のオフセット
				m_ZombieInfo[Number].vPosition.z);		// X座標のオフセット
									// ヨー、ピッチ、ロールを指定して行列を作成
			D3DXMatrixRotationY(
				&m_ZombieInfo[Number].matRotation,	// 演算結果の行列へのアドレス
				m_ZombieInfo[Number].DirectionRadian);
			matWorld =matScaling *  m_ZombieInfo[Number].matRotation * matPosition;
			// ワールド行列を指定
			m_pEffect->SetMatrix("matWorld", &matWorld);
			// ビュー行列を指定
			m_pEffect->SetMatrix("matView", &matView);
			// プロジェクション行列を指定
			m_pEffect->SetMatrix("matProjection", &matProjection);
			// カメラの位置を指定
			m_pEffect->SetVector("ViewPosition", &vViewPosition);
			// ライト方向を指定
			m_pEffect->SetVector("DirectionalLight", &vDirectionalLight);
			// シェーダ設定をグラボに更新
			m_pEffect->CommitChanges();
			//
			m_pAnimation[m_ZombieInfo[Number].skin][m_ZombieInfo[Number].animNum]->Draw();
			//
			m_pEffect->EndPass();
		}
	}
}
// アニメーション再生関数
// 引数		Number		：	ゾンビの配列番号
void CZombie::AnimationPlay(DWORD Number)
{
	/*スコア加算時光らせる処理*/
	m_pDecorate->UpdateeFlashScore();
	m_pDecorate->FlashScore();
	FLOAT Time = 0.0005f * 50;
	m_ZombieInfo[Number].AnimController[m_ZombieInfo[Number].animNum]->AdvanceTime(Time, NULL);
	//m_ZombieInfo[Number].AnimController->AdvanceTime(Time, NULL);
}

// ゾンビの歩く処理(現在平面移動)
// 引数		PlayerPos	：	プレイヤーの座標
//			Number		：	ゾンビの配列番号
void CZombie::ZombieWalk(D3DXVECTOR3 PlayerPos, DWORD Number)
{

	D3DXVECTOR3 Position;
	D3DXVECTOR3 vDirection = PlayerPos - m_ZombieInfo[Number].vPosition;
	m_ZombieInfo[Number].vAngle = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ベクトルの単位化
	D3DXVec3Normalize(&vDirection, &vDirection);
	D3DXVec3Normalize(&m_ZombieInfo[Number].vAngle, &m_ZombieInfo[Number].vAngle);

	// 2つの3Dベクトルの内積を計算
	FLOAT Dot = D3DXVec3Dot(&vDirection, &m_ZombieInfo[Number].vAngle);

	m_ZombieInfo[Number].DirectionRadian = acosf(Dot);		// アークコサインで角度へ変換

	//----- 外積 -----//
	D3DXVECTOR3 vNormal;
	// 2つの3Dベクトルの外積を計算
	D3DXVec3Cross(&vNormal, &vDirection, &m_ZombieInfo[Number].vAngle);
	// 単位化
	D3DXVec3Normalize(&vNormal, &vNormal);
	// 0～πまでの弧度を0～2πまでに変更
	if (vNormal.y >= 0.0f)
	{
		m_ZombieInfo[Number].DirectionRadian = (D3DX_PI - m_ZombieInfo[Number].DirectionRadian) + D3DX_PI;
	}
	D3DXVec3TransformCoord(&vDirection, &vDirection, &m_ZombieInfo[Number].matRotation);
	vDirection.x = PlayerPos.x - m_ZombieInfo[Number].vPosition.x;
	vDirection.y = 0.0f;
	vDirection.z = PlayerPos.z - m_ZombieInfo[Number].vPosition.z;
	// 座標の更新
	D3DXVec3Normalize(&vDirection, &vDirection);
	vDirection *= m_Speed;
	Position = m_ZombieInfo[Number].vPosition + vDirection;
	//m_ZombieInfo[Number].vPosition += vDirection;
	/**/
	/*当たり判定*/
	float num;
	int hitNum;

	Collider *pColManager = Collider::GetInstance();

	for (DWORD i = 0; i < NumZombies; i++)
	{
		if (Number == i)
		{
			continue;
		}
		//ぞんび同士の当たり判定とその調整
		pColManager->Distance(Position, GetZombiePosition(i), 5.0f, &num, &Position);
		//
		pColManager->Distance(Position, D3DXVECTOR3(-174.0f, 12.0f, -12.5), 15.0f, &num, &Position);
	}

	if (!pColManager->GroundCol(Position, D3DXVECTOR3(0, 0, 0), &hitNum, &num))
	{
		if (Number < NumZombies)
		{
			if (hitNum != 1)
			{
				float no;
				if (pColManager->RayCast(Position, m_ZombieInfo[Number].vAngle, &no))
				{
					
				}
				else {
					m_ZombieInfo[Number].vPosition = Position;
					m_ZombieInfo[Number].vPosition.y += num;
				}
			}
			//海の地形だったら
			else if (hitNum == 1)
			{
				//移動分の値を戻す
				//	m_ZombieInfo[Number].vPosition = Position - vDirection*2;
			}
		}
		else if (Number >= NumZombies)
		{
			if (hitNum != 0)
			{
				m_ZombieInfo[Number].vPosition = Position;
				m_ZombieInfo[Number].vPosition.y += num;
				//m_ZombieInfo[Number].hitMesh = hitNum;
				D3DXVECTOR2 tmp;
				tmp = m_pMathlib->MyVecAngle(m_ZombieInfo[Number].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DX_PI / 1000);
				m_ZombieInfo[Number].vPosition = D3DXVECTOR3(tmp.x, m_ZombieInfo[Number].vPosition.y, tmp.y);
			}
			else if (hitNum == 0)
			{
				//移動分の値を戻す
				m_ZombieInfo[Number].vPosition = Position - vDirection;
			}

			if (pColManager->Distance(Position, D3DXVECTOR3(108.0f, 7.5f, -5.0f), 10)) 
			{
				//移動分の値を戻す
				Position.z = Position.z - vDirection.z;
			}
		}
	}
}

// 最大と最小の間の値をランダムに返すヘルパー関数
float CZombie::Random(float min, float max)
{
	return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

// 最大と最小の間の値をランダムに返すヘルパー関数
int CZombie::Random(int min, int max)
{
	return lroundf(((float)rand() / (float)RAND_MAX) * (max - min) + min);
}

//====================================
//関数名	:Update
//引数		:playerPos:プレイヤーの座標
//			 num:エネミーの番号
//戻り値	:
//内容		:更新処理
//====================================
void CZombie::Update(D3DXVECTOR3 playerPos, DWORD num)
{

	//double time = m_pAnimation[m_ZombieInfo[num].animNum]->GetTime();
	//更新前にステートをチェック
	//StateCheck(playerPos, m_ZombieInfo[num].vPosition, num);
	Collider* pColManager = Collider::GetInstance();
	float nums;
	int hitNum;


	pColManager->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0, 0, 0), &hitNum, &nums);

	if ((m_ZombieInfo[num].vPosition.y<=11.0f && m_ZombieInfo[num].state !=DEATH ) && num<NumZombies)
	{
		m_ZombieInfo[num].animNum = 1;
		m_ZombieInfo[num].state = WALLOW;
	}
	else if (m_ZombieInfo[num].vPosition.y > 62 && m_ZombieInfo[num].vPosition.z > 25 && m_ZombieInfo[num].vPosition.z < 27 )
	{
		m_pWaterSlider[num]->SetIsSlider(TRUE);
		m_ZombieInfo[num].state = FLOW;
		m_ZombieInfo[num].animNum = 3;	
	}
	else if (pColManager->EasyCheck(m_ZombieInfo[num].vPosition, D3DXVECTOR3(-174.5f, 12.72f, 13.5f), 10.0f) && m_ZombieInfo[num].state != STAIRS)
	{
		//階段を上る処理
		m_ZombieInfo[num].state = STAIRS;
		//m_ZombieInfo[num].vPosition = STAIRSPOSITION[0];
	}
	if (m_ZombieInfo[num].isAlive)
	{
		ExcuteState(num,playerPos);
	}
}

//====================================
//関数名	:ExcuteState
//引数		:num:エネミーの番号
//			 playerPos:プレイヤーの座標
//戻り値	:
//内容		:エネミーのステートを実効する箇所
//====================================
void CZombie::ExcuteState(int num,D3DXVECTOR3 playerPos)
{

	//ステートによって動作を変更
	switch (m_ZombieInfo[num].state)
	{
	case CHASER:
		ZombieWalk(playerPos, num);
		break;
	case DEATH:
		ZombieDeathState(num);
		break;
		//流れる処理
	case FLOW:
		ZombieFlowState(num);
		break;
		//溺れる処理
	case WALLOW:
		ZombieWalowState(num);
		break;
		//階段を上る処理
	case STAIRS:
		ZombieWalkUp(num);
		break;
		//遊具でぶつかったときの処理
	case HITSTOP:
		HitStop(num);
		break;
	case STAIRS_Walk:
		
		break;
	default:
	#ifdef _DEBUG
		assert(!"存在しないステートです");
	#endif // DEBUG	
		break;
	}
}

//====================================
//関数名	:ZombieDeathState
//引数		:num:エネミーの番号
//戻り値	:
//内容		:エネミーの死亡時の処理
//====================================
void CZombie::ZombieDeathState(int num)
{
	m_ZombieInfo[num].animNum = 2;
	//if (m_ZombieInfo[num].)
	if (m_pAnimation[m_ZombieInfo[num].skin][m_ZombieInfo[num].animNum]->GetAnimFinishTime() <= m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->GetTime())
	{
		m_ZombieInfo[num].isAlive = FALSE;
		value++;
		CScore::GetInstnce()->SetScoreNum(value);
		m_pDecorate->SetisFlash(TRUE);
		Zombierevival(num);
		m_pWaterSlider[num]->ResetValue();
	}
}

//====================================
//関数名	:ZombieFlowState
//引数		:num:エネミーの番号
//戻り値	:
//内容		:エネミーが流れる時に入る処理
//====================================
void CZombie::ZombieFlowState(int num)
{
	m_pWaterSlider[num]->SetPlayerPos(m_ZombieInfo[num].vPosition);
	m_pWaterSlider[num]->SetPlayerRot(m_ZombieInfo[num].vAngle);
	m_ZombieInfo[num].vPosition = m_pWaterSlider[num]->WaterSliderAction(TRUE);
	m_ZombieInfo[num].DirectionRadian = m_pWaterSlider[num]->GetDirectionRadian();
	m_ZombieInfo[num].ZombieStaNum = 0;
}

//====================================
//関数名	:ZombieWalowState
//引数		:num:エネミーの番号
//戻り値	:
//内容		:エネミーが溺れる処理
//====================================
void CZombie::ZombieWalowState(int num)
{
	//
	int v = 0;

	if (m_ZombieInfo[num].LifeTime < 0.0f)
	{
		if (m_ZombieInfo[num].vPosition.y < -10.0f)
		{
			m_ZombieInfo[num].state = DEATH;
		}
		else
		{
			m_ZombieInfo[num].vPosition.y -= 0.1f;
			m_ZombieInfo[num].vPosition.x += 0.2f;
		}
	}
	else
	{
		m_ZombieInfo[num].LifeTime--;
		m_ZombieInfo[num].vPosition.y = 1.0f;

		v = Random(0.0f, 1000.0f);
		if (v % 2 == 0)
		{
			m_ZombieInfo[num].vPosition.z += 0.2f;
		}
		else
		{
			m_ZombieInfo[num].vPosition.x += 0.2f;
		}
	}
	m_ZombieInfo[num].ZombieStaNum = 0;
}

//====================================
//関数名	:AllZombiCheckCollider
//引数		:
//戻り値	:
//内容		:エネミーのヒットチェック
//====================================
BOOL CZombie::AllZombiCheckCollider(D3DXVECTOR3 pos,int num)
{
	Collider *pColManager = Collider::GetInstance();
	for (int i = 0; i < MaxZombie; i++) 
	{
		if (num == i)continue;

		if (pColManager->Distance(pos, m_ZombieInfo[i].vPosition, 5.0f)) 
		{
			return TRUE;
		}	
	}
	return FALSE;
}

//====================================
//関数名	:IsDeadCheck
//引数		:
//戻り値	:
//内容		:エネミーの死亡処理
//====================================
BOOL CZombie::IsDeadCheck()
{
	for (int i = 0; i < MaxZombie; i++)
	{
		if (m_ZombieInfo[i].isAlive)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//====================================
//関数名	:ZombieWalkUp
//引数		:num:エネミーの番号
//戻り値	:
//内容		:ゾンビが階段を上るときの処理
//====================================
void CZombie::ZombieWalkUp(int num)
{
	//一時保存用変数
	D3DXVECTOR3 zPos,prevPos,vDirection,direction;
	//現在の座標を保存
	prevPos = m_ZombieInfo[num].vPosition;


	if (m_ZombieInfo[num].isStairs)
	{
		m_ZombieInfo[num].vPosition += ((STAIRSPOSITION[m_ZombieInfo[num].ZombieStaNum + 1] - STAIRSPOSITION[m_ZombieInfo[num].ZombieStaNum]) / dividValue[m_ZombieInfo[num].ZombieStaNum]);

		zPos = m_ZombieInfo[num].vPosition;
		//
		vDirection = zPos - prevPos;

		// ベクトルの単位化
		D3DXVec3Normalize(&vDirection, &vDirection);
		D3DXVec3Normalize(&m_ZombieInfo[num].vAngle, &m_ZombieInfo[num].vAngle);


		// 2つの3Dベクトルの内積を計算
		FLOAT Dot = D3DXVec3Dot(&vDirection, &m_ZombieInfo[num].vAngle);

		m_ZombieInfo[num].DirectionRadian = acosf(Dot);
		//----- 外積 -----//
		D3DXVECTOR3 vNormal;
		// 2つの3Dベクトルの外積を計算
		D3DXVec3Cross(&vNormal, &vDirection, &m_ZombieInfo[num].vAngle);
		// 単位化
		D3DXVec3Normalize(&vNormal, &vNormal);
		// 0～πまでの弧度を0～2πまでに変更
		if (vNormal.y >= 0.0f)
		{
			m_ZombieInfo[num].DirectionRadian = (D3DX_PI - m_ZombieInfo[num].DirectionRadian) + D3DX_PI;
		}
		m_ZombieInfo[num].z_num++;
		if (m_ZombieInfo[num].z_num >= dividValue[m_ZombieInfo[num].ZombieStaNum])
		{
			m_ZombieInfo[num].ZombieStaNum++;
			m_ZombieInfo[num].z_num = 0;
			if (m_ZombieInfo[num].ZombieStaNum >= 3)
			{
				m_ZombieInfo[num].z_num = 0;
				m_ZombieInfo[num].state = FLOW;
			}
		}
	}
	else
	{

		m_ZombieInfo[num].vPosition+=(STAIRSPOSITION[0] - m_ZombieInfo[num].vPosition) / 10.0f;
		m_ZombieInfo[num].val++;
		if (m_ZombieInfo[num].val==10)
		{
			m_ZombieInfo[num].isStairs = TRUE;
			m_ZombieInfo[num].val = 0;
		}



	}
}

//====================================
//関数名	:Zombierevival
//引数		:num:エネミーの番号
//戻り値	:
//内容		:ゾンビの復活処理
//====================================
void CZombie::Zombierevival(int num)
{
	//ゾンビ再復活
	m_ZombieInfo[num].isAlive = TRUE;
	m_ZombieInfo[num].state = CHASER;
	int value = (int)Random(0.0f, 10000.0f);
	if (value % 3 == 0)
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(200.0f,13.0f,Random(-42,43));
	}
	else if (value % 3 == 1)
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-45.0f, 45.0f), 13.0f, 195.45f);
	}
	else
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-45.0f, 45.0f), 13.0f, -200.155f);
	}
	/*/死亡時の生存時間*/
	m_ZombieInfo[num].LifeTime = 120.0f;
	m_ZombieInfo[num].animNum = 0;
	m_ZombieInfo[num].ZombieStaNum = 0;
	m_ZombieInfo[num].z_num = 0;
	m_ZombieInfo[num].DirectionRadian = 0.0f;
	m_ZombieInfo[num].ZombieStaNum = 0;
	m_ZombieInfo[num].isStairs = FALSE;
	m_ZombieInfo[num].val = 0;
}

//====================================
//関数名	:ResetZombieInfo
//引数		:
//戻り値	:
//内容		:ゾンビの情報を初期化する(ゲームリセット時使用)
//====================================
void CZombie::ResetZombieInfo()
{
	ZombieInfoInitialize();
	for (int i = 0; i < NumZombies; i++)
	{
		m_pWaterSlider[i]->SetIsSlider(FALSE);
		m_pWaterSlider[i]->ResetValue();
	}
	value = 0;
}

//====================================
//関数名	:HitStop
//引数		:num:エネミーの番号
//戻り値	:
//内容		:ゾンビが遊具に当たった際に止まる処理
//====================================
void CZombie::HitStop(int num)
{
	if (m_pAnimation[m_ZombieInfo[num].skin][m_ZombieInfo[num].animNum]->GetAnimFinishTime() <= m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->GetTime())
	{
		m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->ResetTime();
		//m_pAnimation[m_ZombieInfo[num].animNum]->SetTime(0.15f);
		m_ZombieInfo[num].animNum = 0;
		m_ZombieInfo[num].state = CHASER;
	}
}

void CZombie::STAIRS_WalkState(int num)
{

}


//void CZombie::StateCheck(D3DXVECTOR3 playerPos, D3DXVECTOR3 enemyPos,DWORD num)
//{
//	//一時保存用変数
//	double temp,temp2,temp3,finaltemp, vTemp,angle;
//
//	//最少角度
//	temp2 = atan2(100.0f,100.0f+enemyPos.x);
//	temp2 = D3DXToDegree(temp2);
//	//最大角度
//	temp3 = atan2(100.0f , enemyPos.x-100.0f);
//	temp3 = D3DXToDegree(temp3);
//	//finaltemp = temp3 - temp2;
//
//	//現在のプレイヤーの位置と敵の位置の角度をチェック
//	temp=atan2((playerPos.z - enemyPos.z), (playerPos.x - enemyPos.x));
//	temp=D3DXToDegree(temp);
//	//エネミーとプレイヤーとの距離の差を検出
//	vTemp = sqrt(pow((playerPos.z - enemyPos.z), 2) + pow((playerPos.x - enemyPos.x), 2));
//	//絶対値
//	vTemp = abs(vTemp);
//	//直線の長さを検出
//	if ((temp>temp2 && temp<temp3) && (vTemp>0 && vTemp<100))
//	{
//		//追跡モードに変更
//		m_ZombieInfo[num].state = CHASER;
//	}
//	else
//	{
//		//モードに変更
//		m_ZombieInfo[num].state = CHASER;
//	}
//}