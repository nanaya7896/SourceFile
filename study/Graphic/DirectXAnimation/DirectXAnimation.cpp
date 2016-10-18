#include "DirectXAnimation.h"

//====================================
//関数名	:CDirectXAnimation
//引数		:pDevice : 描画デバイス
//戻り値	:
//内容		:コンストラクタ
//====================================
CDirectXAnimation::CDirectXAnimation(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice9 = pDevice;
	ZeroMemory(&m_TrackDesc, sizeof(D3DXTRACK_DESC));
	LoadAnimXFile();
	
}

//====================================
//関数名	:~CDirectXAnimation
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CDirectXAnimation::~CDirectXAnimation()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pDevice9);
}

//====================================
//関数名	:LoadAnimXFile
//引数		:pDevice:描画デバイス
//戻り値	:
//内容		:アニメーションXFileのロード
//====================================
void CDirectXAnimation::LoadAnimXFile()
{
	CModelManager* pModeleManager = CModelManager::GetInstance();
	//初期の設定
	SetPos(D3DXVECTOR3(0,0,-10));
	SetRot(D3DXVECTOR3(0, 0, 0));
	SetScale(D3DXVECTOR3(0.05f,0.05f,0.05f));

	//インスタンスの取得
	CCamera* pCamera = CCamera::GetInstance();
	m_CameraPos = D3DXVECTOR3(m_PlayerPos.x, m_PlayerPos.y + 3.0f, m_PlayerPos.z - 10);
	pCamera->View(m_CameraPos, D3DXVECTOR3(m_PlayerRot.x, m_PlayerRot.y + D3DX_PI, m_PlayerRot.z));

	//アニメーションのファイルの初期化関数
	///第二引数にManagerを使いアニメーションファイルを入力
	m_SkinMesh.InitThing(m_pDevice9,pModeleManager->GetXFileName(1));

	LPD3DXBUFFER pErrorMsg;
	//======シェーダーの作成=====//
	HRESULT hr = D3DXCreateEffectFromFile(
		m_pDevice9,
		"Effect\\xfile2.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErrorMsg
		);

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsg->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsg);
		return;
	}
}

//====================================
//関数名	:FreeAnim
//引数		:pFrame:Frame構造体
//戻り値	:
//内容		:兄弟・子フレームの解放
//====================================
void CDirectXAnimation::FreeAnim(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		FreeAnim(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		FreeAnim(pFrame->pFrameFirstChild);
	}
}

//====================================
//関数名	:Render
//引数		:
//戻り値	:
//内容		:描画
//====================================
int a = 0;
void CDirectXAnimation::Render()
{
	//インスタンスの取得
	CCamera* pCamera = CCamera::GetInstance();
	//m_CameraPos = D3DXVECTOR3((m_PlayerPos.x +10.0f)*sinf(m_PlayerRot.y), m_PlayerPos.y + 3, (m_PlayerPos.z +10.0f)*cosf(m_PlayerRot.y));
	 m_CameraPos = D3DXVECTOR3(0 + 50.0f*cosf(m_PlayerRot.y), -2, 0 + 50 * sinf(m_PlayerRot.y));
	pCamera->View(m_CameraPos,D3DXVECTOR3(m_PlayerRot.x,m_PlayerRot.y,m_PlayerRot.z));

	//３Dファイルの座標を定義する
	D3DXMATRIX *m_World = new D3DXMATRIX();
	D3DXMATRIX *m_temp = new D3DXMATRIX();
	//行列の初期化
	D3DXMatrixIdentity(m_World);
	D3DXMatrixIdentity(m_temp);

	//=====以下３Dの座標・拡大・移動の計算=====//
	//拡大の処理
	D3DXMatrixScaling(m_temp, m_PlayerScale.x, m_PlayerScale.y, m_PlayerScale.z);
	*m_World *= *m_temp;
	//モデルの座標変換(ヨー、ピッチ、ロールを指定して行列を作成)
	D3DXMatrixRotationYawPitchRoll(m_temp, m_PlayerRot.y, m_PlayerRot.x, m_PlayerRot.z);
	*m_World *= *m_temp;
	//モデル移動の処理
	D3DXMatrixTranslation(m_temp, m_PlayerPos.x, m_PlayerPos.y, m_PlayerPos.z);
	//全ての行列の演算結果格納
	*m_World *= *m_temp;
	m_SkinMesh.Update(*m_World);
	m_SkinMesh.Draw(m_pDevice9);
	
	SAFE_DELETE(m_World);
	SAFE_DELETE(m_temp);
}


//====================================
//関数名	:AnimationChange
//引数		:Num : アニメーション番号
//戻り値	:
//内容		:アニメーションの切り替え
//====================================
void CDirectXAnimation::AnimationChange(DWORD Num)
{
	//アニメーションのきりかえ
	m_SkinMesh.ChangeAnim(Num);
}

//====================================
//関数名	:SetPos
//引数		:pos:座標
//戻り値	:
//内容		:座標のセット
//====================================
void CDirectXAnimation::SetPos(D3DXVECTOR3 pos)
{
	m_PlayerPos = pos;
}

//====================================
//関数名	:SetRot
//引数		:rot:回転
//戻り値	:
//内容		:回転のセット
//====================================
void CDirectXAnimation::SetRot(D3DXVECTOR3 rot)
{
	m_PlayerRot = rot;
}

//====================================
//関数名	:SetScale
//引数		:Scale:拡大
//戻り値	:
//内容		:拡大率の挿入
//====================================
void CDirectXAnimation::SetScale(D3DXVECTOR3 Scale)
{
	m_PlayerScale = Scale;
}
