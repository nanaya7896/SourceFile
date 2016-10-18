#pragma once
//=====Include File=====//
#include"..\Global\Global.h"
#include"DirectX\DXDevice.h"
//=====クラス宣言=====//
class CTimeThread
{
//privateメンバ変数
private:
	//ハンドル
	HANDLE			m_hTimeThread;
	//スレッド管理用イベント
	static HANDLE	m_hTimeEvent;
	//タイマースレッドの待機有無
	BOOL			m_bTimeThreadSleep;

//publicメンバ変数
public:
	CDXDevice*		m_pDXDevice;

//privateメンバ関数
private:
	static void Run(CTimeThread*);

//publicメンバ関数
public:
	//コンストラクタ
	CTimeThread();
	//デストラクタ
	virtual ~CTimeThread();

	void Start(CDXDevice*);

	//=====セット関数=====//
	void setTimeThreadSleep(BOOL);

	//=====ゲット関数=====//
	BOOL getTimeThreadSleep();
};