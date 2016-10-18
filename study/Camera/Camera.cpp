//=====Include File=====//
#include "Camera.h"

//唯一のインスタンスの実態
CCamera* CCamera::m_pInstance = NULL;

//====================================
//関数名	:CCamera
//引数		:pDevice9:描画デバイス
//戻り値	:
//内容		:コンストラクタ
//====================================
CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice9)
{
/*	//カメラのアングル
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//カメラから原点までの距離
	m_fDistance = 6.0f;
	//描画デバイスの格納
	m_pDevice9 = pDevice9;

	//行列の初期化
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_Screen);

	m_vEyePt2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//注視点の設定
	m_vLookatPt = m_vEyePt2;

	//カメラの基準位置
	m_vViewPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//正規化
	D3DXVec3Normalize(&m_vDirectionalLight, &m_vDirectionalLight);
	//環境光源の場所
	m_vLightPosition = m_vDirectionalLight*-20.0f;*/
	m_pDevice9 = pDevice9;
	m_fDistance = 10.0f;									// 倍率(カメラから原点までの距離) 
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラのアングル
	m_vDirectionalLight = D3DXVECTOR3(1.0f, -2.0f, 1.0f);// 環境光源の方向
	vViewBasePosition=D3DXVECTOR3(0.0f, 0.85f, -4.5f);
	D3DXVec3Normalize(&m_vDirectionalLight, &m_vDirectionalLight);

}

//====================================
//関数名	:~CCamera
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CCamera::~CCamera()
{
}

//====================================
//関数名	:Create
//引数		:pDevice9:	描画デバイス 
//			 hWnd:		ウィンドウハンドル
//戻り値	:
//内容		:カメラ作成関数
//====================================
void CCamera::Create(LPDIRECT3DDEVICE9 pDevice9, HWND hWnd)
{
	//もしカメラのインスタンスが存在しなければ
	if (!m_pInstance)
	{
		//カメラのインスタンスの生成
		m_pInstance = new CCamera(pDevice9);
	}

	//スクリーン行列作成
	RECT rcWindow;
	::GetWindowRect(hWnd, (LPRECT)&rcWindow);

	FLOAT ClientWidth = (FLOAT)(rcWindow.right - rcWindow.left)*0.5f;
	FLOAT ClientHeight = (FLOAT)(rcWindow.bottom - rcWindow.top) * 0.5f;
	//スクリーン行列設定
	//ClientWidth	,	0			,0	,0
	//0				,ClientHeight	,0	,0
	//0				,0				,0	,0
	//ClientWidth	,ClientHeight	,0	,0
	m_pInstance->m_Screen._11 = ClientWidth;
	m_pInstance->m_Screen._22 = -ClientHeight;
	m_pInstance->m_Screen._41 = ClientWidth;
	m_pInstance->m_Screen._42 = ClientHeight;


}

//====================================
//関数名	:Destroy
//引数		:
//戻り値	:
//内容		:インスタンスの削除
//====================================
void CCamera::Destroy()
{
	SAFE_DELETE(m_pInstance);
}


//====================================
//関数名	:Destroy
//引数		:
//戻り値	:
//内容		:インスタンスの削除
//====================================
void CCamera::SetCamera(D3DXVECTOR3 pos)
{
	if (!m_pDevice9)
	{
		return;
	}



	// 描画処理を記述して
	D3DXMATRIX matView;							// 視点行列
	D3DXMATRIX matPers;							// 投影変換行列
	
	D3DXMATRIX matRotation;						// オブジェクト行列の実装(回転)
	D3DXVECTOR3 vViewUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vViewPosition = D3DXVECTOR3(0.0f, 0.0f, -m_fDistance);
	// 3Dベクトルのスケーリング
	D3DXVec3Scale(
		&m_vViewPosition,	// 演算結果であるD3DXVECTOR3構造体へのポインタを指定
		&vViewBasePosition,	// 処理の基になるD3DXVECTOR3構造体へのポインタを指定
		m_fDistance);		// スケーリング値を指定

							// ヨー、ピッチ、ロールを指定して行列を作成
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// 演算結果であるD3DXMATRIX構造体へのポインタを指定
		m_vViewAngle.y,	// y軸のヨー(ラジアン単位)を指定
		m_vViewAngle.x,	// x軸のピッチ(ラジアン単位)を指定
		m_vViewAngle.z);// z軸のロール(ラジアン単位)を指定

	D3DXVec3TransformCoord(
		&m_vViewPosition,
		&m_vViewPosition,
		&matRotation);

	// 3Dベクトルを行列で変換
	//D3DXVec3TransformCoord(
	//	&m_vViewPosition,		// 変換後のベクトルへのアドレスを指定
	//	&m_vViewPosition,		// 変換前のベクトルへのアドレスを指定
	//	&matRotation);			// 処理の基になる行列へのアドレスを指定
	//							// 3Dベクトルを行列で変換
	//D3DXVec3TransformCoord(
	//	&vViewUp,		// 変換後のベクトルへのアドレスを指定
	//	&vViewUp,		// 変換前のベクトルへのアドレスを指定
	//	&matRotation);	// 処理の基になる行列へのアドレスを指定

	m_vViewPosition += pos;
	m_vLookatPt = pos;
						// 左手座標系ビュー行列を作成
	D3DXMatrixLookAtLH(
		&matView,						// 演算結果であるD3DXMATRIX構造体へのポインタを指定
		(D3DXVECTOR3*)&m_vViewPosition,	// 視点を定義したD3DXVECTOR3構造体へのポインタを指定
		&m_vLookatPt,	// カメラの注視対象を定義したD3DXVECTOR3構造体へのポインタを指定
		&vViewUp);						// カレントワールドの上方、一般には[0, 1, 0]を定義したD3DXVECTOR3構造体へのポインタを指定

	m_View = matView;

	D3DVIEWPORT9 vp;
	m_pDevice9->GetViewport(&vp);
	// 視野(FOV)に基づいて、左手座標系パースペクティブ射影行列を作成
	D3DXMatrixPerspectiveFovLH(
		&matPers,			// 演算結果であるD3DXMATRIX構造体へのポインタを指定
		(FLOAT)D3DXToRadian(45.0), // 視野角を指定(ラジアン単位)(45度を指定) 
		(FLOAT)vp.Width / (FLOAT)vp.Height,// アスペクト比を指定
		1.0f,				// 近くのビュープレーンのz値を指定
		1000.0f);			// 遠くのビュープレーンのz値を指定

	m_Proj = matPers;
}