#pragma once

//=====includeFile=====
#include"../Global/Global.h"
#include"TimeThread.h"
#include"DirectX\DXDevice.h"

//======クラス宣言=====
class CApplication
{
private:
	//インスタンス
	static CApplication m_pAppInstance;

	// 最小化の有無
	BOOL					m_bWindowActive;
	
	// 表示座標	
	POINT					m_ptWindowShowPosition;

	//サイズ
	SIZE					m_WindowSize;

	//ゲームデバイス
	CDXDevice*				m_pDXDevice;

public:
	static CApplication* getInstance();
	HWND Initialize(HINSTANCE, LPCTSTR, SIZE, BOOL);

	int	Run(HWND);
	virtual ~CApplication();

private:
	CApplication();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	HWND InitWindow(HINSTANCE, LPCTSTR, SIZE, BOOL);
	SIZE getClientSize(HWND);
	SIZE getWindowSize(HWND);
	void WindowMoveToCenter(HWND);
	void setClientSize(HWND, SIZE);


};
