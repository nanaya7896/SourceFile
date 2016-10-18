#include "WaterBalloon.h"

CWaterBalloon* CWaterBalloon::m_pCWaterBalloonInstance = NULL;

//====================================
//関数名	:CWaterBalloon
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CWaterBalloon::CWaterBalloon()
{
	m_isShot = FALSE;
	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
	value = 0.0f;
	gunPos = D3DXVECTOR3(-125.0f, 13.25f, -130.0f);
	state = STATE_NONE;
}

//====================================
//関数名	:~CWaterBalloon
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CWaterBalloon::~CWaterBalloon()
{

}

void CWaterBalloon::Create()
{
	if (!m_pCWaterBalloonInstance)
	{
		m_pCWaterBalloonInstance = new CWaterBalloon();
	}
}

void CWaterBalloon::Destory()
{
	SAFE_DELETE(m_pCWaterBalloonInstance);
}

//====================================
//関数名	:CreateResource
//引数		:
//戻り値	:
//内容		:一度だけ呼ばれるリソースの作成
//====================================
void CWaterBalloon::CreateResource(LPDIRECT3DDEVICE9 pDevice, CDInput* pDInput)
{
	m_pDevice9 = pDevice;
	m_pDInput = pDInput;
}

//====================================
//関数名	:UpdateWater
//引数		:
//戻り値	:
//内容		:シャボン玉をSin値で上下させる
//====================================
void CWaterBalloon::UpdateWater()
{
	m_waterPos.y += ChangeTF(0.1f, value, TRUE);
	m_waterPos.x += ChangeTF(0.1f, m_playerRot.y - D3DX_PI, TRUE);
	m_waterPos.z += ChangeTF(0.1f, m_playerRot.y - D3DX_PI, FALSE);

	value += 0.1f;
	float num;
	if (Collider::GetInstance()->RayCast(m_waterPos, m_playerRot, &num)) {
		m_isShot = FALSE;
		value = 0.0f;
	}

}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:通所の更新処理
//====================================
void CWaterBalloon::Update(D3DXVECTOR3 vPos, D3DXVECTOR3 /*vRot*/)
{
	if (STATE_NONE == state)
	{
		//取得を決める
		//GetWaterGun(vPos, vRot);
		return;
	}

	//所持しているなら
	//位置更新
	gunPos = vPos;

	if (m_isShot)
	{
		UpdateWater();
	}
	//if (m_isShot == TRUE)
	//{
	//	UpdateWater();
	//}
	////ガンを持つ場合
	//if (m_isGun==FALSE)
	//{
	//	//m_pPlayer->SetAnimationNum(13);
	//	m_isGun = TRUE;
	//	
	//}
	////ガンを置く場合
	//if (m_isGun==TRUE)
	//{
	//	m_isGun = FALSE;
	//}
	//シャボン玉が出現している間ずっと
	//UpdateWater();	 
	//value = 0.0f;
	//ガンを持っているかをプレイヤー側に送る
	//m_pPlayer->SetWaterGunHoldCheck(m_isGun);
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CWaterBalloon::Draw()
{
	CModelManager* pModelManager = CModelManager::GetInstance();
	if (state != STATE_HAVE_BG &&state !=STATE_BG_SHOT)
	{
		pModelManager->Draw(9, gunPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI/4), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 1.0f);
	}
	if (m_isShot)
	{
		//シャボン玉
		pModelManager->Draw(5, m_waterPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.0001f);
	}

	//pModelManager->Draw(0, gunPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);
}

//====================================
//関数名	:GetWaterGun
//引数		:vPos:ユキちゃんの座標
//			 vRot:ユキちゃんの角度
//戻り値	:
//内容		:銃を手に入れる処理
//====================================
BOOL CWaterBalloon::GetWaterGun(D3DXVECTOR3 vPos, D3DXVECTOR3 /*vRot*/)
{
	Collider* pColManager = Collider::GetInstance();
	BOOL check = pColManager->EasyCheck(vPos, gunPos, 10.0f);
	if (check)
	{
		state = STATE_HAVE_BG;
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:DisGun
//引数		:
//戻り値	:
//内容		:銃を置く処理
//====================================
void CWaterBalloon::DisGun()
{
	state = STATE_NONE;
}

//====================================
//関数名	:ChangeTF
//引数		:value	:周期を計算するための値(標準1.0f)
//			 rot	:振れ幅を決める値
//			 isSin  :TRUE:Sin値　FALSE:Cos値
//戻り値	:三角関数(Sin,Cos)の計算結果
//内容		:Sin,Cosの値を計算する
//====================================
FLOAT CWaterBalloon::ChangeTF(FLOAT value, FLOAT rot, BOOL isSin)
{
	FLOAT va = 0.0f;
	if (isSin)
	{
		//Sinを計算したい場合
		va = value*(sinf(rot));
	}
	else
	{
		//Cosを計算したい場合
		va = value*(cosf(rot));
	}
	return va;
}

void CWaterBalloon::ResetWaterBullon()
{
	m_isShot = FALSE;


	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
	value = 0.0f;
	gunPos = D3DXVECTOR3(-125.0f, 13.25f, -130.0f);
	state = STATE_NONE;
}

BOOL CWaterBalloon::HitCheck(D3DXVECTOR3 checkPos) {

	if (Collider::GetInstance()->EasyCheck(m_waterPos, checkPos, 5.0f)) {
		state = STATE_HAVE_BG;
		return TRUE;
	}
	return FALSE;
}