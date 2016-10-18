#pragma once

#define BUFFER_SIZE			256
//=====Include File=====//
#include"../../Global/Global.h"

//=====クラス宣言=====//
class CDInput
{
	//privateメンバ変数
private:
	//DirectInputシステム
	LPDIRECTINPUT8			m_pDInput;
	//DIRECTINPUTDEVICEキーボード
	LPDIRECTINPUTDEVICE8	m_pDInputKeyBoard;
	//DIRECTINPUTDEVICEジョイスティック
	LPDIRECTINPUTDEVICE8	m_pDInputJoyStick;

	BYTE m_KeyInput[BUFFER_SIZE];
	BYTE m_KeyInputPreview[BUFFER_SIZE];
	//publicメンバ変数
public:


	//privateメンバ関数
private:
	void InitDirectInput(HWND);
	void ReleaseDirectInput();

	static BOOL CALLBACK EnumJoyStickCallback(LPCDIDEVICEINSTANCE, LPVOID);

	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE, LPVOID);

	//列挙する軸の範囲を設定

	//publicメンバ関数
public:
	//コンストラクタ
	CDInput(HWND);
	//デストラクタ
	virtual ~CDInput();

	//更新
	HRESULT Update();
	//キーボードの状態の取得
	//void GetKeyBoardState(LPBYTE);
	//ジョイスティックの入力情報の取得
	void GetJoyStickState(DIJOYSTATE2*);
	//値取得関数
	bool KeyState(int code);
	bool KeyStatePreview(int code);
	bool KeyDown(int code);
	bool KeyUp(int code);
	int KeyCount(int code);

	BOOL IsButtonDown(int pos);
	BOOL IsButtonPressed(int pos);
	BOOL LAnalogState(int num);
	BOOL AllControllCheck();
	BOOL WASDKeyUpCheck();
	BOOL WASDKeyDownCheck();
	BOOL RAnalogState(int num);
	DIJOYSTATE2	padData;
	//パッド情報（前フレーム）
	DIJOYSTATE2 lastpaddata;
	//=====ゲット関数=====//
	LPDIRECTINPUT8 GetDInput()
	{
		return m_pDInput;
	}

	LPDIRECTINPUTDEVICE8* GetDInputJoyStick()
	{
		return &m_pDInputJoyStick;
	}
};