#include "DXDevice.h"

//====================================
//関数名	: CDXDevice
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CDXDevice::CDXDevice()
{
	m_pDInput = NULL;
	m_pDirect3D = NULL;
	m_pDevice9 = NULL;
	m_bDeviceLost = FALSE;
	m_bChangeDisplayMode = FALSE;
	m_bCleanupObject = FALSE;
	m_pFont = NULL;
}

//====================================
//関数名	:~CDXDevice
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CDXDevice::~CDXDevice()
{
		SAFE_DELETE(m_pScene);
		SAFE_DELETE(m_pDInput);
		SAFE_DELETE(m_pFont);
	
}
//====================================
//関数名	:CreateResource
//引数		:
//戻り値	: TRUE:成功  FALSE:問題あり
//内容		:リソースの作成 
//			 デバイス消失に影響されないリソース
//====================================
BOOL CDXDevice::CreateResource()
{
	//シーンの作成
	m_pScene = new CScene();
	//フォントの作成
	m_pFont = new CDXFont(m_pDevice9);

	m_pScene->CreateResource(m_pDevice9, m_hWnd, m_byKeyState, &m_JoyState,m_pFont,m_pDInput);
	return TRUE;
}

//====================================
//関数名	:Initialize
//引数		:
//戻り値	:
//内容		:初期化
//====================================
HRESULT CDXDevice::Initialize(HWND hWnd, BOOL bWindowed)
{
	//Direct3Dシステムの作成
	m_pDirect3D = ::Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pDirect3D == NULL)
	{
		return E_FAIL;
	}

	//プレゼントパラメーターの初期化
	InitPresentParameters(hWnd, bWindowed);

	//Direct3DDeviceの作成
	HRESULT hResult = CreateDevice(hWnd, m_d3dpp);
	if (FAILED(hResult))
	{
		return E_FAIL;
	}

	//DirectInputの作成
	m_pDInput = new CDInput(hWnd);

	//既にリソースが存在したら
	if (!CreateResource())
	{
		//リソースを解放
		CleanupResource();
		return E_FAIL;
	}

	
	//
	if (!CreateObject())
	{
		//
		CleanupObject();
		return E_FAIL;
	}


	return S_OK;
}

//====================================
//関数名	:InitPresentParameters
//引数		:
//戻り値	:
//内容		:
//====================================
void CDXDevice::InitPresentParameters(HWND hWnd, BOOL bWindowed)
{
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));	// メモリのゼロクリア
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;// フリッピング方法:IDirect3DDevice9::Present処理がバックバッファの内容に一切影響しない
	m_d3dpp.BackBufferCount = 1;					// バックバッファ数を1に設定
	m_d3dpp.EnableAutoDepthStencil = TRUE;				// 深度バッファを管理を有効
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		// 8ビットのステンシルチャンネルを指定
	m_d3dpp.hDeviceWindow = hWnd;					// ウィンドウハンドル
													//	D3DDISPLAYMODE BaseFullScreenMode = {800,600,60,D3DFMT_R5G6B5};	// 基本モード
													//	D3DDISPLAYMODE FullScreenMode = {800,600,75,D3DFMT_X8R8G8B8};	// 指定モード
	D3DDISPLAYMODE BaseFullScreenMode = { 1280,720,60,D3DFMT_R5G6B5 };	// 基本モード
	D3DDISPLAYMODE FullScreenMode = { 1280,720,75,D3DFMT_X8R8G8B8 };	// 指定モード
	m_bWindowed = bWindowed;
	m_hWnd = hWnd;

	m_Mode = CheckFullScreenAdapter(FullScreenMode);

	if (m_Mode.Format == D3DFMT_UNKNOWN)
	{
		m_Mode = BaseFullScreenMode;
	}

	//ディスプレイモードを取得
	m_pDirect3D->GetAdapterDisplayMode( 
		D3DADAPTER_DEFAULT,	  //ディスプレイアダプター標準指定
		&m_d3ddm				//ディスプレイモードを保存する構造体
		);

	//ウィンドウモードなら
	if (bWindowed)
	{
		//ウィンドウモードに設定
		m_d3dpp.Windowed = TRUE;

		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		//バックバッファのサイズを変更
		m_d3dpp.BackBufferWidth = rcClient.right - rcClient.left;
		m_d3dpp.BackBufferHeight = rcClient.bottom - rcClient.top;

		if (m_d3ddm.Format == D3DFMT_X8R8G8B8)
		{
			//バックバッファの色数をARGB8bitに指定
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		}
		else if (m_d3ddm.Format == D3DFMT_R5G6B5)
		{
			//色数:A1bitRGB5bitに指定
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;
		}
		else
		{
			//色数をGetAdapterDisplayModeで取得したフォーマットを指定
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;
		}
		//リフレッシュシートを指定
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
	}
	//フルスクリーンモードなら
	else
	{
		//フルスクリーンモードに設定
		m_d3dpp.Windowed = FALSE;
		//バックバッファ水平解像度
		m_d3dpp.BackBufferWidth = m_Mode.Width;
		//バックバッファ垂直解像度
		m_d3dpp.BackBufferHeight = m_Mode.Height;
		//リフレッシュレートを指定
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;
		//色数を指定
		m_d3dpp.BackBufferFormat = m_Mode.Format;

	}

}

//====================================
//関数名	:CheckFullScreenAdapter
//引数		: FullScreenMode フルスクリーンモードの情報
//戻り値	: 実行できるフルスクリーンモード
//内容		: グラフィックアダプターの取得
//====================================
D3DDISPLAYMODE CDXDevice::CheckFullScreenAdapter(D3DDISPLAYMODE FullScreenMode)
{
	//アダプタを識別する情報を保持する構造体
	D3DADAPTER_IDENTIFIER9 Adapter;
	//引数1:幅 2:高さ 3:リフレッシュレート 4:フォーマット
	D3DDISPLAYMODE ResultMode = { 0,0,0,D3DFMT_UNKNOWN };
	//
	D3DDISPLAYMODE Mode;
	//
	int iLevel = 1000000;
	//
	int iNumber, iCount, i, j;
	//
	//UINT uMaxRefreshRate = 0;
	//
	//int iSlelectNumber = -1;

	//アダプターの識別色数を配列にセット
	D3DFORMAT AdapterFormat[TotalAdapter]= 
	{
		D3DFMT_A2R10G10B10,		//色数:A2R10G10B10
		D3DFMT_X8R8G8B8,		//色数:X8R8G8B8
		D3DFMT_A8R8G8B8,		//色数:A8R8G8B8
		D3DFMT_X1R5G5B5,		//色数:X1R5G5B5
		D3DFMT_A1R5G5B5,		//色数:A1R5G5B5
		D3DFMT_R5G6B5			//色数:R5G6B5
	};
	//システムにあるアダプタの数を取得
	iNumber = m_pDirect3D->GetAdapterCount();

	//システムにアダプタがある場合
	if (iNumber != -1)
	{
		//システムに存在していた物理的なディスプレイアダプタを記述
		m_pDirect3D->GetAdapterIdentifier
			(
				0,			//ディスプレイアダプタを示す序数を指定(DEFAULTを指定)
				0,			//通常は０に設定するパラメータ
				&Adapter	//指定したアダプタを記述する情報が格納される構造体のポインタを指定
				);
		for (j = 0; j < TotalAdapter; j++)
		{
			iCount = 0;
			//アダプタで利用可能なディスプレイモードの数を取得
			iCount = m_pDirect3D->GetAdapterModeCount
				(
					D3DADAPTER_DEFAULT,		//アダプタで利用可能なディスプレイモードの数を取得
					AdapterFormat[j]		//ディスプレイフォーマット(16bit)
					);

			//アダプタのディスプレイモードを列挙
			for (i = 0; i < iCount; i++)
			{
				//紹介するディスプレイアダプタを示す序数を指定
				if (m_pDirect3D->EnumAdapterModes(
					0,					//照会するディスプレイアダプタを示す序数を指定(DEFAULTを指定)
					AdapterFormat[j],	//ディスプレイフォーマット
					i,					//列挙するモードを示す順序数
					&Mode				//D3DDISPLAYMODE構造体へのポインタ
					) == D3D_OK)
				{
					if (Mode.Format == FullScreenMode.Format)
					{
						//取得したディスプレイモードとフルスクリーンのディスプレイの情報を比較して絶対値で値を取得
						int l = abs((int)(Mode.Width - FullScreenMode.Width)) + abs((int)(Mode.Height - FullScreenMode.Height)) + abs((int)(Mode.RefreshRate - FullScreenMode.RefreshRate));

						//iLevel以下なら
						if (l < iLevel) 
						{
							//今まで取得してきたディスプレイ情報をコピー
							memcpy(&ResultMode, &Mode, sizeof(D3DDISPLAYMODE));
							//iLevelを置き換え
							iLevel = l;
						}
					}
				}
			}
		}
	}

	return ResultMode;
}

//====================================
//関数名	: CreateDevice
//引数		: hWnd  : ウィンドウハンドル 
//			  d3dpp : プレゼントパラメーター 
//戻り値	: S_OK : 登録に成功 E_FAIL : 登録に失敗
//内容		: 描画デバイスの作成
//====================================
HRESULT CDXDevice::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	UNREFERENCED_PARAMETER(d3dpp);

	// デバイスの作成 描画:HAL & 3D計算:HARDWARE で試行
	if (FAILED(m_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,						// ディスプレイアダプター指定(標準指定)
		D3DDEVTYPE_HAL,							// 描画デバイス指定(ハードウェア(HAL)指定)
		hWnd,									// ウインドウハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ３Ｄ計算処理(ハードウェアで実施を指定)
		&m_d3dpp,								// プレゼンテーションパラメータ構造体格納アドレス指定
		&m_pDevice9								// 描画デバイス格納アドレス指定
		))) 
	{							
		// 失敗したので 描画:HAL & 3D計算:SOFTWARE で試行											
		if (FAILED(m_pDirect3D->CreateDevice(
			D3DADAPTER_DEFAULT,					// ディスプレイアダプター指定(標準指定)
			D3DDEVTYPE_HAL,						// 描画デバイス指定(ハードウェア(HAL)指定)
			hWnd,								// ウインドウハンドル
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,// ３Ｄ計算処理(ソフトウェアで実施を指定)
			&m_d3dpp,							// プレゼンテーションパラメータ構造体格納アドレス指定
			&m_pDevice9							// 描画デバイス格納アドレス指定
			)))
		{					
			MessageBox(hWnd, TEXT("Direct3Dの初期化に失敗しました。True Color(24ﾋﾞｯﾄ)からHigh Color(16ﾋﾞｯﾄ/32ﾋﾞｯﾄ)に変更して、実行してください。"), TEXT("確認"), MB_OK | MB_ICONSTOP);
			//LPDIRECT3D9の解放
			SAFE_RELEASE(m_pDirect3D);
			return E_FAIL;
		}
	}

	//指定されたデバイスに関するデバイス固有の情報を取得する　
	m_pDevice9->GetDeviceCaps(&m_d3dCaps);

	//当店単位のインデックスを使ってインデックス付けできる最大の行列インデックスを示すDWORD値>1
	if (m_d3dCaps.MaxVertexBlendMatrixIndex > 1)
	{
		m_byAnimMethod = D3DINDEXED;
	}
	else
	{
		//複数の行列による頂点ブレンディングを実行する際にデバイスが適用できる行列の最大数>1
		if (m_d3dCaps.MaxVertexBlendMatrices > 1)
			m_byAnimMethod = D3DNONINDEXED;
		else 
		{
			//アニメーションメソッドが頂点でもインデックスでもない場合
			m_byAnimMethod = SOFTWARE;
			SAFE_RELEASE(m_pDevice9);

			// 描画:HAL & 3D計算:SOFTWARE で試行
			if (FAILED(m_pDirect3D->CreateDevice(
				D3DADAPTER_DEFAULT,					// ディスプレイアダプター指定(標準指定)
				D3DDEVTYPE_HAL,						// 描画デバイス指定(ハードウェア(HAL)指定)
				hWnd,								// ウインドウハンドル
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,// ３Ｄ計算処理(ソフトウェアで実施を指定)
				&m_d3dpp,							// プレゼンテーションパラメータ構造体格納アドレス指定
				&m_pDevice9							// 描画デバイス格納アドレス指定
				)))
			{						
				MessageBox(hWnd, TEXT("Direct3Dの初期化に失敗しました。"), TEXT("確認"), MB_OK | MB_ICONSTOP);
				//LPDIRECT3Dの解放
				SAFE_RELEASE(m_pDirect3D);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}



//====================================
//関数名	:CleanupResource
//引数		:
//戻り値	:
//内容		:リソース削除
//====================================
void CDXDevice::CleanupResource()
{
	//オブジェクトの解放
	CleanupObject();
	if (m_pFont != NULL)
	{
		SAFE_DELETE(m_pFont);
	}
	if (m_pScene != NULL)
	{
		SAFE_DELETE(m_pScene);
	}
	if (m_pDInput != NULL)
	{
		SAFE_DELETE(m_pDInput);
	}


}

//====================================
//関数名	:CreateObject
//引数		:
//戻り値	:TRUE:成功 FALSE:問題あり
//内容		:オブジェクトの作成 
//			 デバイス消失に影響されるリソース
//====================================
BOOL CDXDevice::CreateObject()
{
	//フォントがあるかチェック
	if (m_pFont)
	{
		m_pFont->getDXFont()->OnResetDevice();
	}
	//シーンがあるかチェック
	if (m_pScene) 
	{
		m_pScene->CreateObject();
	}
	
	return TRUE;
}

//====================================
//関数名	:CleanupObject
//引数		:
//戻り値	:
//内容		:オブジェクト解放
//====================================
void CDXDevice::CleanupObject()
{
	//フォントがあるかチェック
	if (m_pFont)
	{
		m_pFont->getDXFont()->OnLostDevice();
	}
	//シーンのチェック
	if (m_pScene) 
	{
		m_pScene->CleanupObject();
	}

	
}



//====================================
//関数名	:BeginScene
//引数		:
//戻り値	:TRUE:成功 FALSE:問題発生
//内容		:レンダリング処理開始
//====================================
BOOL CDXDevice::BeginScene()
{
	//シーンの開始
	if (FAILED(m_pDevice9->BeginScene()))
	{
		return FALSE;
	}

	return TRUE;
}

//====================================
//関数名	:EndScene
//引数		:
//戻り値	:
//内容		:レンダリング処理終了
//====================================
HRESULT CDXDevice::EndScene()
{
	m_pDevice9->EndScene();

	return TRUE;
}


//====================================
//関数名	:RestoreDevice
//引数		:
//戻り値	:TRUE:復元成功 FALSE:復元失敗
//内容		:デバイスの復元
//====================================
BOOL CDXDevice::RestoreDevice()
{

	HRESULT hr;
	
	//どちらか一方がなければ
	if (!m_pDirect3D || !m_pDevice9)
	{
		//失敗
		return FALSE;
	}

	//消失したデバイスの復元処理
	//デバイスが消失していたら
	if (m_bDeviceLost)
	{
		//=====================================================================
		//デバイスの状態をチェック(協調レベルステータスを返す)
		//戻り値 :成功:D3D_OK
		//		  失敗:D3DERR_DEVICELOST デバイスを消失していて、現在リセットできない(レンダリング不可)
		//			   D3DERR_DEVICENOTRESET デバイスは消失しているが、現在リセットできる
		//=====================================================================
		hr = m_pDevice9->TestCooperativeLevel();

		if (FAILED(hr))
		{
			//D3DERR_DEVICELOSTの場合
			if (hr == D3DERR_DEVICELOST)
			{
				//s消失していてリセットできない
				return FALSE;
			}
			//リセットできない状態なら
			if (hr != D3DERR_DEVICENOTRESET)
			{
				return FALSE;
			}

			//オブジェクトのクリーンアップが行わrていなければ
			if (!m_bCleanupObject)
			{
				//オブジェクトの解放
				CleanupObject();
				//クリーンアップ完了
				m_bCleanupObject = TRUE;
			}
			//デバイスの復元が可能なので復元を試みる
			hr = m_pDevice9->Reset(&m_d3dpp);
			
			//それでも復元失敗したら
			if (FAILED(hr))
			{
				//デバイスは消失しているが復元できない状態
				if (hr == D3DERR_DEVICELOST)
				{
					return FALSE;
				}
			}
			//オブジェクトの再生成
			CreateObject();
			//クリーンアップしていない状態にする
			m_bCleanupObject = FALSE;
		}
	}

	return TRUE;
}

//====================================
//関数名	:ChangeDisplaymode
//引数		:
//戻り値	:
//内容		:表示モードの切り替え
//====================================
void CDXDevice::ChangeDisplaymode()
{
	//オブジェクトの解放
	CleanupObject();

	// プレゼンテーションパラメータ設定

	// メモリをゼロクリア
	ZeroMemory(
		&m_d3dpp,												//格納するプレゼンテーションパラメータのポインタ
		sizeof(D3DPRESENT_PARAMETERS
			));							

		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				// フリッピング方法:IDirect3DDevice9::Present処理がバックバッファの内容に一切影響しない
		m_d3dpp.BackBufferCount = 1;							// バックバッファ数を２に設定
		m_d3dpp.EnableAutoDepthStencil = TRUE;					// 深度バッファを管理を有効
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			// 8ビットのステンシルチャンネルを指定
		m_d3dpp.hDeviceWindow = m_hWnd;							// ウィンドウハンドル

	// 表示モードの切り替え
	// ウィンドウモードの場合
	if (m_bWindowed) 
	{
		// ウィンドウモードに設定
		m_d3dpp.Windowed = TRUE;					
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		// バックバッファの幅
		m_d3dpp.BackBufferWidth = rcClient.right - rcClient.left;
		// バックバッファの高さ
		m_d3dpp.BackBufferHeight = rcClient.bottom - rcClient.top;	

		if (m_d3ddm.Format == D3DFMT_X8R8G8B8)
		{
			// 色数:ARGB8bit
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		}
		else if (m_d3ddm.Format == D3DFMT_R5G6B5)
		{
			// 色数:A1bitRGB5bit
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	
		}
		else
		{
			// 色数:GetAdapterDisplayModeで取得したFormatを指定
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;
		}

		m_d3dpp.FullScreen_RefreshRateInHz = 0;		// リフレッシュレートを指定
	}
	else 
	{											// フルスクリーンモードの場合
		m_d3dpp.Windowed = FALSE;						// フルスクリーンモードに設定
		m_d3dpp.BackBufferWidth = m_Mode.Width;			// バックバッファ水平解像度
		m_d3dpp.BackBufferHeight = m_Mode.Height;		// バックバッファ垂直解像度
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// リフレッシュレートを指定
		m_d3dpp.BackBufferFormat = m_Mode.Format;		// 色数を指定
	}

	m_pDevice9->Reset(&m_d3dpp);				// リセット
	m_bDeviceLost = TRUE;
	PostMessage(m_hWnd, UM_SETCURSOR, 0L, 0L);		// カーソル設定
	CreateObject();							// オブジェクト再作成

}

//====================================
//関数名	:Render
//引数		:
//戻り値	:転送結果
//内容		:レンダリング処理
//====================================
HRESULT CDXDevice::Render()
{
	//シーンの開始
	if (SUCCEEDED(BeginScene()))
	{
		//ビューポートまたはビューポート内の矩形セットを指定した RGBA色にクリアし、深度バッファをクリアし、ステンシルバッファを削除
		m_pDevice9->Clear(
			0,															//消去する矩形数指定(バックバッファ全体書き換え指定)
			NULL,														//矩形の座標指定(画像全体書き換え指定)
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//レンダリングターゲット/深度バッファ/ステンシルバッファのクリア
			D3DCOLOR_XRGB(0x00,0x00,0x00),									//レンダリングターゲットサーフェスを塗りつぶす32bitRGBAカラー値を指定(黒色)
			1.0f,														//このメソッドが深度バッファに保存する新しいZ値
			0															//各ステンシルバッファのエントリに保存する値
			);

		//ゲームの処理の実行
		m_pDInput->GetJoyStickState(&m_JoyState);
		m_pScene->SceneChange();


		//シーンの終了
		EndScene();
	}
	//デバイスが所有するバックバッファのシーケンスの中で次のコンテンツを提示する
	//バックバッファの内容をウィンドウに転送
	return m_pDevice9->Present(
		NULL,	//バックバッファ矩形領域を指定(バックバッファ全体をフロントバッファに転送なのでNULL)
		NULL,	//フロントバッファ矩形領域を指定(バックバッファ全体をフロントバッファに転送で指定なのでNULL)
		NULL,	//ウィンドウハンドル(ウィンドウを1画面しか使っていないのでNULLを指定)
		NULL	//DirectX9では用意されていないのでNULL
		);
}

//////////////////////////////////////
//
//セット関数
//
//////////////////////////////////////

//====================================
//関数名	:setDeviceLost
//引数		:bDeviceLost : デバイス消失の有無
//戻り値	:
//内容		:
//====================================
void CDXDevice::setDeviceLost(BOOL bDeviceLost)
{
	m_bDeviceLost = bDeviceLost;
}

//====================================
//関数名	:setWindowed
//引数		:bWindowed :ウィンドウモード/フルスクリーンモード
//戻り値	:
//内容		:ウィンドウモードの指定
//====================================
void CDXDevice::setWindowed(BOOL bWindowed)
{
	m_bWindowed = bWindowed;
}

//====================================
//関数名	:setChangeDisplayMode
//引数		:bChangeDisplayMode : ディスプレイモード変更実行の有無
//戻り値	:
//内容		:ディスプレイモード変更実行の有無
//====================================
void CDXDevice::setChangeDisplayMode(BOOL bChangeDisplayMode)
{
	m_bChangeDisplayMode = bChangeDisplayMode;
}


/////////////////////////////////////////
//
//ゲット関数
//
/////////////////////////////////////////

//====================================
//関数名	:getDeviceLost
//引数		:
//戻り値	:デバイスデバイス消失の有無
//内容		:デバイス消失の取得
//====================================
BOOL CDXDevice::getDeviceLost()
{
	return m_bDeviceLost;
}

//====================================
//関数名	:getDirect3D
//引数		:
//戻り値	:Direct3Dシステム
//内容		:Direct3Dシステムの取得
//====================================
LPDIRECT3D9 CDXDevice::getDirect3D()
{
	return m_pDirect3D;
}

//====================================
//関数名	:getDevice9
//引数		:
//戻り値	:描画デバイス
//内容		:描画デバイスの取得
//====================================
LPDIRECT3DDEVICE9 CDXDevice::getDevice9()
{
	return m_pDevice9;
}

//====================================
//関数名	:getDInput
//引数		:
//戻り値	:m_pDInput
//内容		:DirectInput
//====================================
CDInput * CDXDevice::getDInput()
{
	return m_pDInput;
}

//====================================
//関数名	:getWindowed
//引数		:
//戻り値	:ウィンドウモード/フルスクリーンモード
//内容		:ウィンドウモードの取得
//====================================
BOOL CDXDevice::getWindowed()
{
	return m_bWindowed;
}

//====================================
//関数名	:getChangeDisplayMode
//引数		:
//戻り値	:ディスプレイモード変更実行の有無
//内容		:ディスプレイモード変更の取得
//====================================
BOOL CDXDevice::getChangeDisplayMode()
{
	return m_bChangeDisplayMode;
}

//====================================
//関数名	:getKeyStateArray
//引数		:
//戻り値	:
//内容		:キー入力情報格納配列のアドレス
//====================================
LPBYTE CDXDevice::getKeyStateArray()
{
	return m_byKeyState;
}
