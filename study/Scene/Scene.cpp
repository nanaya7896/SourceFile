#include "Scene.h"

//=====define値====//

/*
	コントローラー
	m_pJoyState->rgbButtons[]
	□:0	L2:6		PSボタン:12
	×:1	R2:7		スライドパッド:13
	〇:2	SHARE:8
	△:3	OPTIONS:9
	L1:4	L3:10
	R1:5	R3:11
*/

//====================================
//関数名	:CScene
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CScene::CScene()
{
	m_pJoyState = NULL;
	m_byKeyState = NULL;
	m_pDSystem = NULL;
	m_pFont = NULL;
	m_pTitle = NULL;
	m_pOperat = NULL;
	m_pMain = NULL;
	m_pGameOver = NULL;
	m_pResult = NULL;
	m_pCredit = NULL;
	m_isOnce = TRUE;
	m_NowGameScene = SCENE::TITLE;
	//m_NowGameScene = SCENE::MAIN;
	//m_NowGameScene = SCENE::GAMEOVER;
	//CameraPos = D3DXVECTOR3(0,-500,0);
	//CameraRot = D3DXVECTOR3(0, 0, 0);
}

//====================================
//関数名	:~CScene
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CScene::~CScene()
{
	SAFE_DELETE(m_pDSystem);
	
	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pOperat);
	SAFE_DELETE(m_pMain);
	SAFE_DELETE(m_pResult);
	SAFE_DELETE(m_pGameOver);
	SAFE_DELETE(m_pCredit);
	SAFE_DELETE(m_pRanking);

	CCamera::Destroy();
	//ModelManagerのデリート用に追加
	CModelManager* pModelManager = CModelManager::GetInstance();
	pModelManager->Delete();
	//TextureManagerのデリート
	CTextureManager* pTexturemanager = CTextureManager::GetInstance();
	pTexturemanager->Delete();
	//インスタンスのデストロイ
	CTextureManager::Destroy();
	CModelManager::Destory();
	//当たり判定の消失
	Collider::Destory();
	//フェード初期化
	Fade::Destory();
}

//====================================
//関数名	:CleanupObject
//引数		:
//戻り値	:
//内容		:
//====================================
void CScene::CleanupObject()
{
		
}

//====================================
//関数名	:CreateObject
//引数		:
//戻り値	:
//内容		:
//====================================
BOOL CScene::CreateObject()
{
	return TRUE;
}

//====================================
//関数名	:AllGameInitialize
//引数		:
//戻り値	:
//内容		:ゲームの初期化
//====================================
void CScene::AllGameInitialize()
{
	m_pTitle->AllReset();
	m_pMain->AllGameReset();
	//サウンドをすべて止める
	m_pSoundManager->StopAllSound();
	//サウンド0を流す
	m_pSoundManager->Play(0, TRUE);
	m_isOnce = TRUE;
}

//====================================
//関数名	:CreateResource
//引数		:
//戻り値	:
//内容		:リソースファイルの作成(一度しか呼ばれない)
//====================================
BOOL CScene::CreateResource(LPDIRECT3DDEVICE9 pDevice9, HWND hWnd, BYTE* byKeyState, DIJOYSTATE2* JoyStick,CDXFont* pFont, CDInput* pInput)
{

	m_pDevice9 = pDevice9;
	m_pFont = pFont;
	m_pDInput = pInput;
	m_pJoyState = JoyStick;
	m_byKeyState = byKeyState;
	//カメラの生成
	CCamera::Create(m_pDevice9, hWnd);
	//================================
	//
	//各システムを動的にメモリ確保
	//
	//================================
	//DirectSystemの作成
	m_pDSystem = new CDSystem(m_pDevice9);
	//サウンドマネージャー
	m_pDSystem->CreateSoundManager(hWnd);
	//サウンドマネージャーの取得

	m_pSoundManager = m_pDSystem->GetSoundManager();

	//テクスチャマネージャー
	CTextureManager::Create(m_pDevice9);
	//テクスチャの一括取得
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->AllLoadTexture();
	//モデルマネージャー
	CModelManager::Create(m_pDevice9);
	CModelManager* pModelManager = CModelManager::GetInstance();
	//モデルの一括読み込み
	pModelManager->AllLoadXfile();
	//フェードインアウトの処理
	Fade::Create();

	/*当たり判定*/
	Collider::Create();
	Collider* pColManager = Collider::GetInstance();
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		D3DXVECTOR3 tmp = D3DXVECTOR3(0, 0, 0);
		if (i == 3)tmp = D3DXVECTOR3(-166.0f, 13.0f, -25.0f);
		if (i == 12)tmp = D3DXVECTOR3(29.0f, 7.0f, 111.0f);
		pColManager->SetObj(tmp, pModelManager->GetMesh(i));
	}


	//シーン
	m_pTitle = new CTitle(m_pDevice9);
	m_pOperat = new COperating(m_pDevice9);
	m_pMain = new CMain();
	m_pMain->CreateResource(m_pDevice9, m_pDInput,m_pSoundManager);
	m_pGameOver = new CGameOver(m_pDevice9);
	m_pResult = new CResult(m_pDevice9, m_pDInput);
	m_pCredit = new CCredit(m_pDevice9);
	m_pRanking = new CRanking();

	m_pMain->SetResult(m_pResult);
	//ゲームの初期化
	AllGameInitialize();


	return TRUE;
}

//====================================
//関数名	:SceneChange
//引数		:
//戻り値	:
//内容		:シーンの変更
//====================================
void CScene::SceneChange()
{

	Fade* pFadeManager = Fade::GetInstance();
	static BOOL fadeCheck = FALSE;

	switch (m_NowGameScene)
	{
	case SCENE::TITLE:
		//更新
		if (fadeCheck) {
			if (pFadeManager->Update(TRUE, 0.5f)) {
				fadeCheck = FALSE;
				m_pSoundManager->Play(5, FALSE);
				m_NowGameScene = SCENE::OPERATE;
			}
		}
		else {
			pFadeManager->Update(FALSE, 1.5f);
		}

		m_pTitle->Update();
		//描画
		m_pTitle->Draw();
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			fadeCheck = TRUE;
		}

		m_pMain->AllGameReset();
		break;

	case SCENE::OPERATE:

		//音楽
		if (fadeCheck) {
			if (pFadeManager->Update(TRUE, 0.3f)) {
				fadeCheck = FALSE;
				m_pSoundManager->Play(5, FALSE);
				m_pSoundManager->StopAllSound();
				//メインゲーム音楽
				m_pSoundManager->Play(1, TRUE);
				m_pSoundManager->Play(9, FALSE);

				m_NowGameScene = SCENE::MAIN;
			}
		}
		else {
			pFadeManager->Update(FALSE, 0.3f);
		}
		m_pOperat->Update();
		//描画
		m_pOperat->Draw();
		//もしボタンが押されていたら
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			fadeCheck = TRUE;
		}
		break;

	case SCENE::MAIN:
		pFadeManager->Update(FALSE, 0.3f);
		m_pDInput->GetJoyStickState(m_pJoyState);
		if (m_pMain->GetisSlider())
		{
			CCamera::GetInstance()->SetDistance(5.0f);
			CCamera::GetInstance()->SetCamera(D3DXVECTOR3(0.0f,240.0f,-50.0f));
			CCamera::GetInstance()->SetViewAngle(D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f));
			if (m_isOnce)
			{
				m_pSoundManager->Play(13, FALSE);
				m_isOnce = FALSE;
			}
		}
		else 
		{
			m_isOnce = TRUE;
			//CCamera::GetInstance()->SetDistance(25.0f);
			CCamera::GetInstance()->SetCamera(m_pMain->GetpPlayer()->GetPlayerPos());
		}
		//敵と接触したら
	#ifndef _DEBUG
		if (m_pMain->PlayerAndEnemyHitCheck())
		{
			pFadeManager->Update(TRUE, 0.0);
			m_pSoundManager->StopAllSound();
			m_pSoundManager->Play(2, TRUE);
			m_NowGameScene = SCENE::GAMEOVER;
			m_pMain->AllGameReset();
			break;
		}
	#endif // ! _DEBUG



		//更新
		m_pMain->Update();
		//描画
		m_pMain->Draw();

		//制限時間が終了したら
		if (m_pMain->GetLimitTime())
		{
			pFadeManager->Update(TRUE, 0.0);
			m_pSoundManager->StopAllSound();
			m_pSoundManager->Play(3, TRUE);
			m_NowGameScene = SCENE::RESULT;
			break;
		}

		break;
	case SCENE::GAMEOVER:
		//更新
		m_pGameOver->Update();
		pFadeManager->Update(FALSE, 0.5f);

		if (m_pDInput->KeyDown(DIK_LEFT) || (m_pDInput->LAnalogState(2)))
		{
			m_pSoundManager->Play(5, FALSE);
			m_pGameOver->isArrowPos(TRUE);
		}
		else if (m_pDInput->KeyDown(DIK_RIGHT) || (m_pDInput->LAnalogState(3)))
		{
			m_pSoundManager->Play(5, FALSE);

			m_pGameOver->isArrowPos(FALSE);
		}
		//描画
		m_pGameOver->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			if (m_pGameOver->GetReturnPos() == TRUE)
			{
				//タイトルに戻る処理
				AllGameInitialize();
				m_pGameOver->ResetValue();
				m_NowGameScene = SCENE::TITLE;
			}
			else
			{
				//リトライの処理
				m_pMain->AllGameReset();
				m_pSoundManager->StopAllSound();
				m_pSoundManager->Play(1, TRUE);
				m_pSoundManager->Play(9, FALSE);
				m_pGameOver->ResetValue();
				m_NowGameScene = SCENE::MAIN;
			}
		}


		break;

	case SCENE::RESULT:
		//更新
		m_pResult->Update();
		//描画
		m_pResult->Draw();

		
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_pRanking->RankinCheck();
			m_NowGameScene = SCENE::RANKING;
		}
		break;
	case SCENE::RANKING:

		m_pRanking->Update();
		m_pRanking->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_NowGameScene = SCENE::CREDIT;
		}
		break;

	case SCENE::CREDIT:
		//更新
		m_pCredit->Update();
		//描画
		m_pCredit->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_pTitle->AllReset();
			AllGameInitialize();
			m_NowGameScene = SCENE::TITLE;
		}
		break;
	default:
		m_NowGameScene = SCENE::TITLE;
	}
	pFadeManager->Draw();
}

//====================================
//関数名	:OnLostDevice
//引数		:
//戻り値	:
//内容		:
//====================================
void CScene::OnLostDevice()
{
	
}

//====================================
//関数名	:OnResetDevice
//引数		:
//戻り値	:
//内容		:
//====================================
void CScene::OnResetDevice()
{

}




