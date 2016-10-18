
#include "DInput.h"



//====================================
//関数名	:CDInput
//引数		:hWnd:ウィンドウハンドル
//戻り値	:
//内容		:コンストラクタ
//====================================
CDInput::CDInput(HWND hWnd)
{
	m_pDInput = NULL;
	m_pDInputJoyStick = NULL;
	m_pDInputKeyBoard = NULL;
	ZeroMemory(&m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
	ZeroMemory(&m_KeyInputPreview, sizeof(BYTE)*BUFFER_SIZE);
	InitDirectInput(hWnd);
}

//====================================
//関数名	:~CDInput
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CDInput::~CDInput()
{
	ReleaseDirectInput();
}

//====================================
//関数名	:InitDirectInput
//引数		:hWnd:ウィンドウハンドル
//戻り値	:
//内容		:DIRECTINPUTの初期化
//====================================
void CDInput::InitDirectInput(HWND hWnd)
{
	HRESULT hr=E_FAIL;
	DirectInput8Create(
		(HINSTANCE)((UINT64)GetWindowLong(hWnd, GWL_HINSTANCE)),		//プログラムのインスタンスハンドルを指定
		DIRECTINPUT_VERSION,											//バージョン情報を指定(通常指定)
		IID_IDirectInput8A,												//目的のインターフェースの一意な識別子(通常指定)
		(void**)&m_pDInput,												//DirectInputシステムのアドレスを指定
		NULL															//COM集大成に対する制御オブジェクトのIUnknownインターフェイスのアドレスへのポインタ(通常指定)
		);

	//DirectInputDeviceコネクタ作成
	//キーボード
	m_pDInput->CreateDevice(
		GUID_SysKeyboard,		//入力機器のGUIDを指定
		&m_pDInputKeyBoard,		//DirectInputDevice8インターフェースを受け取る変数のポインタ
		NULL					//COM集大成に対する制御オブジェクトのIUnknownインタフェースのアドレス
		);

	//ジョイスティック・デバイスの列挙
	//m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
	//デバイスタイプを指定(ゲームコントローラ))
	hr = m_pDInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,	//デバイスタイプを指定(ゲームコントローラ)
		EnumJoyStickCallback,	//列挙コールバック関数のアドレスを指定
		(VOID*)this,			//列挙コールバック関数への引数
		DIEDFL_ATTACHEDONLY		//列挙の範囲を指定するフラグ値(デフォルト)
		);
	//=====入力データ形式指定=====//
	m_pDInputKeyBoard->SetDataFormat(
		&c_dfDIKeyboard		//DirectInputデバイスが返すべきデータ形式を記述する構造体
		//キーボード用データフォーマット。256バイトの配列
		);

	//もしジョイスティックが接続されたなら
	if (m_pDInputJoyStick) 
	{
		//ジョイスティックデータ形式設定
		m_pDInputJoyStick->SetDataFormat(
			&c_dfDIJoystick2	//ジョイスティック(主にフォースフィードバック)用データフォーマット。xyz軸の｢位置｣｢回転角｣｢速度｣｢加速度｣｢角加速度｣｢フォース｣｢トルク｣、スライダ軸2、視点ハット4、ボタン128
			);
	}
	//=====入力データ形式設定終了=====//

	//=====協調モードの設定=====//
	//ｰｰｰキーボード
	m_pDInputKeyBoard->SetCooperativeLevel(
		hWnd,									//ウィンドウハンドル
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE	//フォアグラウンドで非排他モードで指定
		);
	//---JoyStick
	if (m_pDInputJoyStick)
	{
		m_pDInputJoyStick->SetCooperativeLevel(
			hWnd,									//ウィンドウハンドル
			DISCL_FOREGROUND | DISCL_EXCLUSIVE	//フォアグラウンド(時のみ入力情報を取得)で排他(ほかのアプリケーションで排他で取得できない)モードで指定
			);
	}

	if (m_pDInputJoyStick)
	{
		//軸ごとに設定を行う
		m_pDInputJoyStick->EnumObjects(
			CDInput::EnumAxesCallback,		//コールバック関数のアドレスを指定
			(VOID*)m_pDInputJoyStick,		//コールバック関数に渡す引数
			DIDFT_AXIS						//列挙するオブジェクトのタイプを指定(デバイスの軸だけ列挙)
			);
	}

	//入力機器の取得
	//キーボードから入力取得を開始
	m_pDInputKeyBoard->Acquire();

	if (m_pDInputJoyStick)
	{
		HRESULT hr;
		//ポーリング対象からデータを取得
		hr = m_pDInputJoyStick->Poll();
		//もし取得に失敗していたら
		if (FAILED(hr))
		{
			//ジョイスティックから情報を取得
			hr = m_pDInputJoyStick->Acquire();
			//もしデバイスがロストしたら
			while (hr == DIERR_INPUTLOST)
			{
				//データの取得を開始
				hr = m_pDInputJoyStick->Acquire();
			}
		}
	}
	



}

//====================================
//関数名	:ReleaseDirectInput
//引数		:
//戻り値	:
//内容		:DirectInputの解放
//====================================
void CDInput::ReleaseDirectInput()
{
	//DirectInput関連オブジェクト解放
	if (m_pDInput != NULL)
	{
		//もしDeviceキーボードが存在するなら
		if (m_pDInputKeyBoard != NULL)
		{
			//キーボードの取得を停止
			m_pDInputKeyBoard->Unacquire();
			SAFE_RELEASE(m_pDInputKeyBoard);
		}
		//もしDeviceジョイスティックがあるなら
		if (m_pDInputJoyStick != NULL) 
		{
			//ジョイスティックの情報取得を中止
			m_pDInputJoyStick->Unacquire();
			SAFE_RELEASE(m_pDInputJoyStick);
		}
		//DirectInputシステムの解放
		SAFE_RELEASE(m_pDInput);
	}
}

//====================================
//関数名	:EnumJoyStickCallback
//引数		:pdidInstance	:デバイスへのポインタ
//			 lpContext		:引数として指定した32ビット値へのポインタ
//戻り値	:DIENUM_CONTINUE:成功 DIENUM_STOP:失敗
//内容		:ジョイスティックを列挙
//====================================
BOOL CDInput::EnumJoyStickCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID lpContext)
{
	CDInput*		pDInput = (CDInput *)lpContext;
	LPDIRECTINPUT8	pDI = pDInput->GetDInput();
	LPDIRECTINPUTDEVICE8* pDIJoyStick = pDInput->GetDInputJoyStick();

	HRESULT hr = E_FAIL;
	//列挙されたジョイスティックへのインターフェースを取得
	hr = pDI->CreateDevice(
		pdidInstance->guidInstance,	//入力機器のGUIDを指定(デバイスインスタンスへのポインタを指定)
		pDIJoyStick,				//DirectInputコネクタのアドレスを指定
		NULL						//COM集大成に対する制御オブジェクトのIUnknownインタフェースのアドレス
		);

	//
	if (FAILED(hr)) 
	{
		//列挙は全て終われば自動的に終了するので、列挙し続ける
		return DIENUM_CONTINUE;
	}
	//一つで十分
	return DIENUM_STOP;

}

//====================================
//関数名	:EnumAxesCallback
//引数		:pdidoi		:デバイスオブジェクトのインスタンス
//			 lpContext	:引数として指定した32ビット値へのポインタ
//戻り値	:DIENUM_CONTINUE:成功 DIENUM_STOP:失敗
//内容		: 列挙する軸の範囲を設定
//====================================
BOOL CDInput::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID lpContext)
{
	//
	LPDIRECTINPUTDEVICE8 pDIJoyStick = (LPDIRECTINPUTDEVICE8)lpContext;
	
	HRESULT hr;
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		//デバイス内のオブジェクトの範囲に関する情報を格納する構造体
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		//dwObjectメンバの解釈方法を指定する値(オブジェクトタイプまたはインスタンス識別子)
		diprg.diph.dwHow = DIPH_BYID;

		//アクセスするプロパティが属するオブジェクト(dwHowメンバに指定されている値に依存)
		diprg.diph.dwObj = pdidoi->dwType;
		//範囲の下限
		diprg.lMin = -1000;
		//範囲の上限
		diprg.lMax = 1000;

		//デバイスの動作を定義するプロパティを設定
		hr = pDIJoyStick->SetProperty(
			DIPROP_RANGE,	//GUID情報を指定(オブジェクトが報告可能な値の範囲を設定)
			&diprg.diph		//タイプ固有のDIPROPHEADERの構造体のアドレス
			);

		if (FAILED(hr)) 
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:キーのアップデート関数
//====================================
HRESULT CDInput::Update()
{
	HRESULT hr = m_pDInputKeyBoard->Acquire();
	if ((hr == DI_OK || (hr == S_FALSE)))
	{
		//前回のキーの入力情報を退避
		memcpy(m_KeyInputPreview, m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
		memcpy(&lastpaddata, &padData, sizeof(DIJOYSTATE2));
		//状態
		BYTE tKeyInputWork[BUFFER_SIZE];
		m_pDInputKeyBoard->GetDeviceState(sizeof(m_KeyInput), &tKeyInputWork);
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])
			{
				//押されているカウントUP
				if (m_KeyInput[i] < 255)
				{
					m_KeyInput[i]++;
				}
				else
				{
					m_KeyInput[i] = 255;
				}
			}
			else {
				m_KeyInput[i] = 0;
			}
		}
	}

	return E_NOTIMPL;
}

//====================================
//関数名	:GetKeyBoardState
//引数		:pKeyData : キー情報配列のアドレス
//戻り値	:
//内容		:キーボードの入力状態の取得
//====================================
//void CDInput::GetKeyBoardState(LPBYTE pKeyData)
//{
//	//メソッド失敗時値代入
//	HRESULT hr;
//	//配列を０で初期化
//	ZeroMemory(pKeyData, sizeof(BYTE)*BUFFER_SIZE);
//	
//	//直接入力データ取得処理
//	hr = m_pDInputKeyBoard->GetDeviceState(
//		BUFFER_SIZE,	//格納する要素の数を指定
//		pKeyData		//格納用配列アドレスを指定
//		);
//	//エラー:キーボードの情報取得に失敗/入力デバイスのロスト
//	if ((hr == DIERR_NOTACQUIRED) || (hr == DIERR_INPUTLOST))
//	{
//		//入力情報取得再開
//		m_pDInputKeyBoard->Acquire();
//	}
//	//HRESULT hr = m_pDInputKeyBoard->Acquire();
//	//if ((hr == DI_OK || (hr == S_FALSE)))
//	//{
//	//	//前回のキーの入力情報を退避
//	//	memcpy(m_KeyInputPreview, m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
//	//	//状態
//	//	BYTE tKeyInputWork[BUFFER_SIZE];
//	//	m_pDInputKeyBoard->GetDeviceState(sizeof(m_KeyInput), &tKeyInputWork);
//	//	for (int i = 0; i < BUFFER_SIZE; i++)
//	//	{
//	//		if (tKeyInputWork[i])
//	//		{
//	//			//押されているカウントUP
//	//			if (m_KeyInput[i] < 255)
//	//			{
//	//				m_KeyInput[i]++;
//	//			}
//	//			else
//	//			{
//	//				m_KeyInput[i] = 255;
//	//			}
//	//		}
//	//		else {
//	//			m_KeyInput[i] = 0;
//	//		}
//	//	}
//	//}
//}

//====================================
//関数名	:GetJoyStickState
//引数		:pJoyStick:情報へのアドレス
//戻り値	:
//内容		:ジョイスティックの情報を取得
//====================================
void CDInput::GetJoyStickState(DIJOYSTATE2 *pJoyStick)
{
	HRESULT hr;
	//ジョイスティックの状態を０で初期化
	//DIJOYSTATE2 js2 = { 0 };
	//padData = { 0 };
	//もしジョイスティックがなかった場合
	if (NULL == m_pDInputJoyStick)
	{
		//受け取ったアドレスにジョイスティックがない状態を渡す
		memcpy(pJoyStick,&padData,sizeof(DIJOYSTATE2));
		return;
	}

	//ポーリング対象オブジェクトからデータを取得
	hr = m_pDInputJoyStick->Poll();
	//もし取得に失敗したら
	if (FAILED(hr))
	{
		//ジョイスティックから入力取得を開始
		m_pDInputJoyStick->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			//入力情報の取得を再開
			hr = m_pDInputJoyStick->Acquire();
		}
		return;
	}



	//デバイスから直接データを取得
	hr = m_pDInputJoyStick->GetDeviceState
	(
		sizeof(DIJOYSTATE2),	//バッファサイズ(バイト単位)
		&padData				//デバイスの現在の状態を受け取る構造体のアドレス
	);


	//
	//memcpy(
	//	pJoyStick,				//コピー先のアドレス
	//	&padData,					//コピー元
	//	sizeof(DIJOYSTATE2)		//DIJOYSTATE2構造体
	//	);

}

//====================================
//関数名	:KeyState
//引数		:code:キーの番号
//戻り値	:
//内容		:キーが入力され続けている状態か
//====================================
bool CDInput::KeyState(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		if (m_KeyInput[code] > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//====================================
//関数名	:KeyStatePreview
//引数		:code:キーの番号
//戻り値	:
//内容		:前回のキーの状態
//====================================
bool CDInput::KeyStatePreview(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return false;
}

//====================================
//関数名	:KeyDown
//引数		:code
//戻り値	:
//内容		:キーが押された瞬間か
//====================================
bool CDInput::KeyDown(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return (m_KeyInput[code] >= 1) && (m_KeyInputPreview[code] == 0);
	}
	return false;
}

//====================================
//関数名	:KeyUp
//引数		:code
//戻り値	:
//内容		:キーから離された瞬間か
//====================================
bool CDInput::KeyUp(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return (m_KeyInput[code] == 0) && (m_KeyInputPreview[code] >= 1);
	}
	return false;
}

BOOL CDInput::WASDKeyUpCheck()
{
	if (KeyUp(DIK_W))
	{
		return TRUE;
	}
	if (KeyUp(DIK_A))
	{
		return TRUE;
	}
	if (KeyUp(DIK_S))
	{
		return TRUE;
	}
	if (KeyUp(DIK_D))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDInput::WASDKeyDownCheck()
{
	if (KeyState(DIK_W))
	{
		return TRUE;
	}
	if (KeyState(DIK_A))
	{
		return TRUE;
	}
	if (KeyState(DIK_S))
	{
		return TRUE;
	}
	if (KeyState(DIK_D))
	{
		return TRUE;
	}

	return FALSE;
}

//====================================
//関数名	:
//引数		:
//戻り値	:
//内容		:
//====================================
//int CDInput::KeyCount(int code)
//{
//	return 0;
//}

//====================================
//関数名	:IsButtonDown
//引数		:pos:キー番号
//戻り値	:TRUE:押されている FALSE:押されていない
//内容		:ボタンが押されているかどうか
//====================================
BOOL CDInput::IsButtonDown(int pos)
{
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	return padData.rgbButtons[pos];
}

//====================================
//関数名	:IsButtonPressed
//引数		:pos:キー番号
//戻り値	:TRUE:押されている FALSE:押されていない
//内容		:前回押されていなく今回初めて押されたか
//====================================
BOOL CDInput::IsButtonPressed(int pos)
{
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	if (padData.rgbButtons[pos] && !lastpaddata.rgbButtons[pos])
	{
		return TRUE;
	}

		return FALSE;
}

//====================================
//関数名	:LAnalogState
//引数		:num:
//戻り値	:TRUE:傾いている FALSE:ニュートラル
//内容		:左アナログスティックの傾きを取得
//====================================
BOOL CDInput::LAnalogState(int num)
{
	//デバイスがあるか確認
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}

	switch (num)
	{
	case 0://上
		if (padData.lY <= -1000)
		{
			return TRUE;
		}
		break;
	case 1://下
		if (padData.lY >= 1000)
		{
			return TRUE;
		}
		break;
	case 2://左
		if (padData.lX <= -1000)
		{
			return TRUE;
		}
		break;
	case 3://右
		if (padData.lX >= 1000)
		{
			return TRUE;
		}
		break;
	case 4://左上
		if (padData.lY <= -500 && padData.lX <= -500)
		{
			return TRUE;
		}
		break;
	case 5://右上
		if (padData.lY <= -500 && padData.lX >= 500)
		{
			return TRUE;
		}
		break;
	case 6://左下
		if (padData.lY >= 500 && padData.lX <= -500)
		{
			return TRUE;
		}
		break;
	case 7://右下
		if (padData.lY >= 500 && padData.lX>=500)
		{
			return TRUE;
		}
		break;
	case 8://左右上
		if (padData.lY <= -750 && padData.lX <= -250)
		{
			return TRUE;
		}
		break;
	case 9://右左上
		if (padData.lY <= -750 && padData.lX >= 250)
		{
			return TRUE;
		}
		break;
	case 10://左左上
		if (padData.lY <= -250 && padData.lX <= -750)
		{
			return TRUE;
		}
		break;
	case 11://右右上
		if (padData.lY <= -250 && padData.lX >= 750)
		{
			return TRUE;
		}
		break;
	case 12://左左下
		if (padData.lY >= 250 && padData.lX <= -750)
		{
			return TRUE;
		}
		break;
	case 13://右右下
		if (padData.lY >= 250 && padData.lX >= 750)
		{
			return TRUE;
		}
		break;
	case 14://左右下
		if (padData.lY >= 750 && padData.lX <= -250)
		{
			return TRUE;
		}
		break;
	case 15://右左下
		if (padData.lY >= 750 && padData.lX >= 250)
		{
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CDInput::AllControllCheck()
{
	//デバイスがあるか確認
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}

	for (int i = 0; i < 16; i++)
	{
		if (LAnalogState(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}



BOOL CDInput::RAnalogState(int num)
{
	//デバイスがあるか確認
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	switch (num)
	{
	case 0://左
		if (padData.lZ <= -1000)
		{
			return TRUE;
		}
		break;
	case 1://右
		if (padData.lZ >= 1000)
		{
			return TRUE;
		}
		break;
	}
	return FALSE;
}
