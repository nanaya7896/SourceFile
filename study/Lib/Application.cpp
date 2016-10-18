#include "Application.h"
#include"../resource.h"
//インスタンスの実態
CApplication CApplication::m_pAppInstance;
//====================================
//関数名	:getInstance
//引数		:
//戻り値	:アプリケーションのインスタンス
//
//====================================
CApplication * CApplication::getInstance()
{
	return &m_pAppInstance;
}

//====================================
//関数名	:CApplication
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CApplication::CApplication()
{
	m_ptWindowShowPosition.x = -1;
}

//====================================
//関数名	:~CApplication
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CApplication::~CApplication()
{

}

//=====================================================================
//関数名:initialize
//引数 : hInstance		インスタンスハンドル
//		 WindowTitle	ウィンドウ名
//		 WindowSize		ウィンドウサイズ
//		 bWindowed		TRUE:ウィンドウモード FALSE:フルスクリーンモード
//戻り値 :ウィンドウハンドル  
//=====================================================================
HWND CApplication::Initialize(HINSTANCE hInstance, LPCTSTR Windowtitle, SIZE WindowSize, BOOL bWindowed)
{
	//ウィンドウの初期化
	HWND hWnd=InitWindow(hInstance,Windowtitle,WindowSize,bWindowed);
	if (hWnd == NULL)
	{
		return NULL;
	}

	//ゲームデバイスを初期化する
	m_pDXDevice = new CDXDevice();
	HRESULT hResult = m_pDXDevice->Initialize(hWnd, bWindowed);
	if (FAILED(hResult))
	{
		delete m_pDXDevice;
		return NULL;
	}

	//タイマーの分解能度を1msに設定する
	timeBeginPeriod(1);

	//ウィンドウの表示
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	return hWnd;
}

//====================================
//関数名	:Run
//引数		:hWnd:ウィンドウハンドル
//戻り値	:アプリケーション終了値
//内容		:アプリケーションの実行
//====================================
int CApplication::Run(HWND hWnd)
{
	//ゲームスピード管理スレッド作成/実行
	CTimeThread* pTimeThread = new CTimeThread();
	pTimeThread->Start(m_pDXDevice);


	//メッセージループ
	MSG msg;
	do {
	//メッセージ処理
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}

		if (pTimeThread->getTimeThreadSleep())
		{
			if (m_pDXDevice->getDeviceLost())
			{
				if (m_pDXDevice->getDevice9()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				{
					if (pTimeThread->m_pDXDevice->RestoreDevice())
					{
							//デバイスが復元した
						pTimeThread->m_pDXDevice->setDeviceLost(FALSE);
						m_pDXDevice->setDeviceLost(TRUE);
					}
				}
			}
			else
			{
				//ウィンドウかフルスクリーンか取得
				BOOL bWindowed = m_pDXDevice->getWindowed();
				bWindowed ^= TRUE;
				//セットする
				m_pDXDevice->setWindowed(bWindowed);
				//表示モード切替
				m_pDXDevice->ChangeDisplaymode();
				
				//ウィンドウスタイルの指定
				DWORD WindowStyle;
				if (bWindowed)
				{
					//ウィンドウスタイルを指定
					WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX;
				}
				else
				{
					//ウィンドウスタイルをポップアップに指定
					WindowStyle = WS_POPUP | WS_VISIBLE;
				}

				::SetWindowLong(hWnd, GWL_STYLE, WindowStyle);
				//ウィンドウスタイルの変更

				//ウィンドウの配置
				if (bWindowed)
				{
					//ウィンドウモード時の処理
					if (m_ptWindowShowPosition.x == -1)
					{
						//クライアントサイズの変更
						setClientSize(hWnd, m_WindowSize);
						//ウィンドウの表示位置を画面の中心に変更
						WindowMoveToCenter(hWnd);
					}
					::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,m_WindowSize.cx,m_WindowSize.cy,SWP_SHOWWINDOW);
				}
			}
			pTimeThread->setTimeThreadSleep(FALSE);
		}
		Sleep(1);
	}while(msg.message !=WM_QUIT);

	//_CrtDumpMemoryLeaks();
	//ゲームスピード管理スレッド終了
	delete pTimeThread;

	//リソース開放
	m_pAppInstance.m_pDXDevice->CleanupResource();

	//ゲームスレッド終了
	delete m_pDXDevice;

	// タイマーの分解度を1msに設定する
	timeEndPeriod(1);

	return (int)msg.wParam;
}


//====================================
//関数名	:WndProc
//引数		:
//戻り値	:
//内容		:ウィンドウプロシージャ
//====================================
LRESULT CApplication::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//メッセージが送られてきた場合
	switch (msg)
	{
		//ウィンドウを閉じるとき
	case WM_CLOSE:
	case WM_DESTROY:

		//ウィンドウ終了通知
		PostQuitMessage(0);
		break;

	}
	//
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//====================================
//関数名	:InitWindow
//引数		:hInstance :インスタンスハンドル
//			 WindowTitle:タイトル名
//			 WindowSize:ウィンドウサイズ
//			 bWindowed:TRUE:ウィンドウモード、FALSE:フルスクリーンモード
//戻り値	:ウィンドウハンドル
//内容		:ウィンドウの初期化
//====================================
HWND CApplication::InitWindow(HINSTANCE hInstance, LPCTSTR WindowTitle, SIZE WindowSize, BOOL bWindowed)
{
	// ウィンドウクラスの登録
	WNDCLASSEX wcex;	// ウィンドウクラスの情報格納用構造体変数
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);					// 構造体のサイズを設定
	wcex.style = CS_HREDRAW | CS_VREDRAW;				// ウィンドウスタイル(幅/高さ変更許可)
	wcex.lpfnWndProc = (WNDPROC)WndProc;						// ウィンドウプロシージャの指定
	wcex.cbClsExtra = 0;									// 追加領域は使わない
	wcex.cbWndExtra = 0;									// 追加領域は使わない
	wcex.hInstance = hInstance;							// このインスタンスのハンドル
														//	wcex.hIcon			= LoadIcon(hInstance,IDI_APPLICATION);	// ラージアイコン
	wcex.hIcon = LoadIcon(hInstance, (LPCSTR)IDI_ICON3);	// ラージアイコン
	//wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// カーソルスタイル
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// 背景色 黒色
	wcex.lpszMenuName = NULL;									// 表示メニューのセレクト(メニューなし)
	wcex.lpszClassName = TEXT("MainWindowClass");				// ウィンドウクラス名
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON3);// スモールアイコン

	::RegisterClassEx(&wcex);									// ウィンドウクラスの登録


	HWND hWnd = ::CreateWindow(
		"MainWindowClass",		// クラス名
		WindowTitle,			// タイトル
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	// スタイル
		CW_USEDEFAULT,			// ｘ座標(Windowsに任せる)
		CW_USEDEFAULT,			// ｙ座標(Windowsに任せる)ｘｙで左上角の座標
		WindowSize.cx,			// 幅
		WindowSize.cy,			// 高さ
		HWND_DESKTOP,			// 無し
		(HMENU)NULL,			// メニュー無し
		hInstance,				// このプログラムのインスタンスのハンドル
		NULL					//追加引数なし
		);

	
	if (hWnd == NULL)
	{
		return NULL;
	}
	
	//ウィンドウモードの場合
	if (bWindowed)
	{
		//クライアントサイズの変更
		setClientSize(hWnd, WindowSize);
		//ウィンドウ表示位置を画面の中心に変更
		WindowMoveToCenter(hWnd);
	}
	else
	{
		DWORD WindowStyle;
		//ウィンドウスタイルをポップアップに指定
		WindowStyle = WS_POPUP | WS_VISIBLE;

		//ウィンドスタイルの変更
		::SetWindowLong(hWnd, GWL_STYLE, WindowStyle);

		memcpy(&m_WindowSize, &WindowSize, sizeof(SIZE));
	}

	return hWnd;
}

//====================================
//関数名	:getClientSize
//引数		:
//戻り値	:
//内容		:クライアント領域のサイズを求める
//====================================
SIZE CApplication::getClientSize(HWND hWnd)
{
	//クライアント領域の矩形を取得
	RECT rcClient;
	::GetClientRect(hWnd,&rcClient);

	//サイズを求める
	SIZE result;
	result.cx = rcClient.right - rcClient.left;
	result.cy = rcClient.bottom - rcClient.top;

	return result;
}

//====================================
//関数名	:getWindowSize
//引数		:
//戻り値	:
//内容		:ウィンドウのサイズを求める
//====================================
SIZE CApplication::getWindowSize(HWND hWnd)
{
	//ウィンドウの矩形を取得
	RECT rcWindow;
	::GetWindowRect(hWnd, &rcWindow);

	//サイズを求める
	SIZE result;
	result.cx = rcWindow.right - rcWindow.left;
	result.cy = rcWindow.bottom - rcWindow.top;

	return result;
}

//====================================
//関数名	:setClientSize
//引数		:
//戻り値	:
//内容		:クライアント領域のサイズをセットする
//====================================
void CApplication::setClientSize(HWND hWnd, SIZE size)
{
	//クライアント領域サイズの取得
	SIZE ClientSize = getClientSize(hWnd);
	//クライアント領域と設定するサイズの差を取得
	SIZE CompensationSize;
	CompensationSize.cx = size.cx - ClientSize.cx;
	CompensationSize.cy = size.cy - ClientSize.cy;

	//ウィンドウ矩形の幅と高さを取得
	SIZE WindowSize = getWindowSize(hWnd);

	//ウィンドウの幅と高さを指定
	m_WindowSize.cx = WindowSize.cx + CompensationSize.cx;
	m_WindowSize.cy = WindowSize.cy + CompensationSize.cy;

	//サイズを指定
	::SetWindowPos(hWnd, NULL, 0, 0, m_WindowSize.cx, m_WindowSize.cy, SWP_HIDEWINDOW);
}

//====================================
//関数名	:WindowMoveToCenter
//引数		:
//戻り値	:
//内容		:ウィンドウの位置を中央にする
//====================================
void CApplication::WindowMoveToCenter(HWND hWnd)
{
	//デスクトップの矩形を取得
	RECT rcDesktop;
	//ウィンドウの矩形情報を取得
	::GetWindowRect(::GetDesktopWindow(), (LPRECT)&rcDesktop);
	RECT rcWindow;
	::GetWindowRect(hWnd, (LPRECT)&rcWindow);

	// ウィンドウの中心座標を求め最終的に位置を決定
	m_ptWindowShowPosition.x = (rcDesktop.right - (rcWindow.right - rcWindow.left)) / 2;
	m_ptWindowShowPosition.y = (rcDesktop.bottom - (rcWindow.bottom - rcWindow.top)) / 2;
	// ポジションを移動する
	::SetWindowPos(hWnd, HWND_TOP, m_ptWindowShowPosition.x, m_ptWindowShowPosition.y, (rcWindow.right - rcWindow.left), (rcWindow.bottom - rcWindow.top), SWP_HIDEWINDOW);
}


