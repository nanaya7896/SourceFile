#include "Main.h"

//====================================
//関数名	:CMain
//引数		:pDevice9	:描画デバイス
//			 pField		:フィールド
//			 byKeyState :キーの格納情報
//戻り値	:
//内容		:コンストラクタ
//====================================
CMain::CMain(/*LPDIRECT3DDEVICE9 pDevice9,CDInput* pInput*/)
{
	m_pPlayer = nullptr;
	m_pField = nullptr;
	m_Ready = TRUE;
	m_Go = FALSE;
	m_pDecorate = NULL;
	//m_Value = D3DXVECTOR2(0.05f,0.05f);
	m_Alpha = D3DXVECTOR2(0.0f, 0.0f);
	CScore::Create();
	m_Height = 250.0f;
	//時間用のテクスチャ番号(初期設定60秒)
	m_Time = 90;
	
}

//====================================
//関数名	:~CMain
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CMain::~CMain()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pField);
	SAFE_DELETE(m_pZombie);
	SAFE_DELETE(m_pDecorate);
	CScore::GetInstnce()->Destroy();
}

//====================================
//関数名	:CreateResource
//引数		:pDevice9:描画デバイス
//戻り値	:
//内容		:作成時一度だけ呼ばれる
//====================================
void CMain::CreateResource(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput, CSoundManager* pSManager)
{
	//描画デバイス
	m_pDevice9 = pDevice9;
	m_pInput = pInput;
	m_pSoundManager = pSManager;
	//Playerのインスタンスの生成
	m_pPlayer = (CPlayer*)m_ActorFactory.CreateActor("Player");
	m_pPlayer->CreateResource(m_pDevice9, m_pInput, m_pSoundManager,m_pPlayer);
	/*池原追加*/
	m_pPlayer->SetWeapon(&m_pWeapon);
	//マップテスト用
	m_pField = new CFiled(m_pDevice9);
	m_pField->CreateResource(m_pDevice9);
	//ゾンビ
	m_pZombie = new CZombie(m_pDevice9);
	m_pZombie->Initialize();
	m_pDecorate = new CDecorate();
	
}

//====================================
//関数名	:AllGameReset
//引数		:
//戻り値	:
//内容		:初期化処理
//====================================
void CMain::AllGameReset()
{
	m_Time = 90;
	m_Ready = TRUE;
	m_Go = FALSE;
	m_Height = 250.0f;
	m_Value = D3DXVECTOR2(0.02f, 0.02f);
	m_Alpha = D3DXVECTOR2(0.0f, 0.0f);
	/*プレイヤーの初期化*/
	m_pPlayer->ResetPlayer();
	m_pZombie->ResetZombieInfo();
	//CCamera::GetInstance()->SetViewAngle(D3DXVECTOR3(, 0.0f, 0.0f));
	//m_pZombie->ResetZombieInfo();
	CScore::GetInstnce()->ResetScore();
	m_pDecorate->ResetDeco();
}

//====================================
//関数名	:Upadate
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CMain::Update()
{
	if (!m_Ready && !m_Go)
	{
		//Player更新
		m_pPlayer->Update();
		//
		for (int i = 0; i < MaxZombie; i++)
		{
			if (AllHitCheck(i))
			{
				//数秒止まるアニメーション
				m_pZombie->SetScale(i, { 0.1f,0.1f,0.1f });
				m_pZombie->SetAniamtionNum(i, 6);
				m_pZombie->SetState(i, HITSTOP);
			}
			else if (BBHitCheck(i) && m_BB.GetState() == Air)
			{
				//死ぬ処理
				m_pZombie->SetAniamtionNum(i, 2);
				m_pZombie->SetState(i, DEATH);
			}
			else if (m_pZombie->GetState(i) != HITSTOP)
			{
				m_pZombie->SetScale(i, { 1.2f,1.2f,1.2f });
			}
			//
			m_pZombie->Update(m_pPlayer->GetPlayerPos(), i);
		}

		m_pField->Update();
		CScore::GetInstnce()->Update();
		/*時間の更新*/
		if (!m_pPlayer->GetSlider())
		{
			TimeFrame++;
		}
		//GetWeapon();
		//if (m_pPlayer->GetWeapon() == NULL)
		//{
		//	//MyOutputDebugString("拾えます\n");
		//	AllGetWeapon();
		//}
		//else
		//{
		//	//MyOutputDebugString("置けます\n");
		//	AllPutWeapon();
		//}
		//m_BB.BUpdate({ m_pPlayer->GetPlayerPos().x,m_pPlayer->GetPlayerPos().y + 10.0f ,m_pPlayer->GetPlayerPos().z }, m_pPlayer->GetPlayerAngle());
		//if ((m_pInput->KeyState(DIK_SPACE)) || m_pInput->IsButtonDown(CIRCLE_BUTTON))
		//{
		//	//if(m_pPlayer->GetWeapon() == &m_BB)
		//	//{
		//	//	//m_pPlayer->SetPlayerState(STATE_BB_SHOT);
		//	//}
		//	//else if(m_pPlayer->GetWeapon() == &m_BG)
		//	//{
		//	//	//m_pPlayer->SetPlayerState(STATE_BG_SHOT);
		//	//}
		//	//else if(m_pPlayer->GetWeapon() == &m_WG)
		//	//{
		//	////	m_pPlayer->SetPlayerState(STATE_WG_SHOT);
		//	//}
		//	m_BG.SetWPos({ m_pPlayer->GetPlayerPos().x,m_pPlayer->GetPlayerPos().y + 5.0f,m_pPlayer->GetPlayerPos().z });
		//	m_BG.SetRot(m_pPlayer->GetPlayerAngle());
		//}
		for (int i = 0; i < 53; i++)
		{
			m_pDecorate->Update(m_pPlayer->GetPlayerPos(), m_pZombie->GetZombiePosition(i));
		}
		//デバック用チートボタン
		//if (m_pInput->KeyState(DIK_L))
		//{
		//	TimeFrame = 60;
		//}
		
		/*更新処理終了*/
	}
	else
	{
		if (m_Ready == TRUE)
		{
			m_Alpha.x += m_Value.x;
		}
		if (m_Alpha.x >= 1.0f)
		{
			m_Ready = FALSE;
			m_Go = TRUE;
		}
		if (m_Alpha.x >= 1.0f)
		{
			m_Value.x = -1.0f;

		}
		if (m_Go == TRUE)
		{
			m_Alpha.y += m_Value.y;
		}
		if (m_Alpha.y >= 1.0f)
		{
			m_Value.y = -1.0f;
		}
	}
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CMain::Draw()
{
	m_pField->Draw();
	m_pPlayer->Draw();
	m_pDecorate->FalshScreen();
	for (int i = 0; i < MaxZombie; i++)
	{
		m_pZombie->Draw(i);
		m_pZombie->AnimationPlay(i);
	}
	//m_BB.Draw();
	//m_BG.Draw();
	//m_WG.Draw();
	ZombieValueDraw();
	TimeUpdate(MAX_DIGIT, TRUE);

	if (!m_Ready && !m_Go)
	{

	}
	else
	{
		ReadyDraw();
	}

}

//====================================
//関数名	:TimeUpdate
//引数		:digit: 桁数
//			:isPull:減算か加算か
//戻り値	:
//内容		:時間更新
//====================================
void CMain::TimeUpdate(int digit, BOOL isPull)
{
	float posValue = 9.5f;
	//一時時間保存用
	int tmp;
	if (TimeFrame >= 60)
	{
		/*条件式 ? 真 : 偽*/
		(isPull) ? m_Time-- : m_Time++;
		TimeFrame = 0;
	}

	for (int i = 0; i < digit; i++)
	{
		tmp = m_Time / (int)pow(10, i);
		tmp %= 10;
		//テクスチャのインスタンス生成
		CTextureManager* pTextureManager = CTextureManager::GetInstance();
		float pos = (WINDOW_WIDTH / 20.0f)*posValue;
		pTextureManager->Draw(tmp, D3DXVECTOR3(pos, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		posValue -= 1.2f;
	}
}

//====================================
//関数名	:ZombieValueDraw
//引数		:
//戻り値	:
//内容		:ゾンビの倒された数を数える
//====================================
void CMain::ZombieValueDraw()
{

	//テクスチャのインスタンス生成
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(23, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 0.1f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(200.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(0), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(1), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 4.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(2), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 3.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	m_pResult->SetResultScore(0, CScore::GetInstnce()->GetScore(0));
	m_pResult->SetResultScore(1, CScore::GetInstnce()->GetScore(1));
	m_pResult->SetResultScore(2, CScore::GetInstnce()->GetScore(2));
}

void CMain::ReadyDraw()
{
	//テクスチャのインスタンス生成
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	//レディー
	if (m_Ready)
	{
		if (m_Height<275.0f)
		{
			m_Height += 2.0f;
		}
		pTextureManager->Draw(19, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.5f, m_Height, 0.0f), 1, m_Alpha.x, D3DXVECTOR3(600.0f, 200.0f, 0.0f));
	}
	if (m_Go)
	{
		//ゴー
		pTextureManager->Draw(20, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.5f, 275.0f, 0.0f), 1, m_Alpha.y, D3DXVECTOR3(600.0f, 200.0f, 0.0f));
		time += 1;
		if (m_Alpha.y <= 0.0f)
		{
			m_Go = FALSE;
			time = 0;
			m_Value.x = 0.01f;
		}
	}
}

//====================================
//関数名	:PlayerAndEnemyHitCheck
//引数		:
//戻り値	:TRUE:当たった FALSE:当たっていない
//内容		:プレイヤーとゾンビが接触しているかチェック
//====================================
BOOL CMain::PlayerAndEnemyHitCheck()
{
	float deadDistance;

	if (m_pPlayer->GetHitMapNum() == 3) {
		deadDistance = 3.5f;
	}
	else {
		deadDistance = 1.0f;
	}
	static bool deadFlag = TRUE;

	if (m_pPlayer->GetEnterMesh(3)) {
		deadFlag = FALSE;
	}
	for(int i = 0; i < MaxZombie; i++)
	{
		if (Collider::GetInstance()->EasyCheck(m_pPlayer->GetPlayerPos(), m_pZombie->GetZombiePosition(i), deadDistance))
		{
			if (!deadFlag) {
				return FALSE;
			}
			return TRUE;
		}
	}
	deadFlag = TRUE;
	return FALSE;
}

BOOL CMain::isDeadCheck()
{
	if (m_pZombie->IsDeadCheck())
	{
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:GetLimitTime
//引数		:
//戻り値	:
//内容		:タイムが０のなったか判定
//====================================
BOOL CMain::GetLimitTime()
{
	return (m_Time == 0) ? TRUE : FALSE;
}

//====================================
//関数名	:AllGetWeapon
//引数		:
//戻り値	:
//内容		:武器を手に入れる処理
//====================================
void CMain::AllGetWeapon()
{
	if ((m_pInput->KeyDown(DIK_G)) || (m_pInput->IsButtonDown(SQUARE_BUTTON)))
	{
		if (m_BB.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_BB);
			return;
		}
		else if (m_BG.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_BG);
			return;
		}
		else if (m_WG.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_WG);
			return;
		}
	}
	m_pPlayer->SetHaveWeaponInfo(GetWeaponNumber());
}

//====================================
//関数名	:AllPutWeapon
//引数		:
//戻り値	:
//内容		:武器を地面に置く処理
//====================================
void CMain::AllPutWeapon()
{
	if ((m_pInput->KeyDown(DIK_H)) || (m_pInput->IsButtonDown(X_MARK_BUTTON)))
	{
		if (m_BB.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
		else if (m_BG.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
		else if (m_WG.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
	}
	m_pPlayer->SetHaveWeaponInfo(GetWeaponNumber());
}

//====================================
//関数名	:AllHitCheck
//引数		:num:ゾンビの番号
//戻り値	:
//内容		:遊具のあたり判定(ビーチボール以外)
//====================================
BOOL CMain::AllHitCheck(int num)
{

	if (m_WG.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		return TRUE;
	}
	if (m_BG.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		m_pSoundManager->Stop(8);
		m_pSoundManager->Play(8, FALSE);
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:BBHitCheck
//引数		:num : ゾンビの番号
//戻り値	:
//内容		:ビーチボールとゾンビのあたり判定
//====================================
BOOL CMain::BBHitCheck(int num)
{

	if (m_BB.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		return TRUE;
	}

	return FALSE;
}