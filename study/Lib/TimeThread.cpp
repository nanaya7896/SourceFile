//=====IncudeFile=====//
#include"TimeThread.h"

//ハンドルの実態を定義
HANDLE CTimeThread::m_hTimeEvent = NULL;

//====================================
//関数名	:CTimeThread
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CTimeThread::CTimeThread()
{
	m_hTimeThread = NULL;
	m_bTimeThreadSleep = NULL;
	m_pDXDevice = NULL;
	m_hTimeEvent = NULL;
}

//====================================
//関数名	:~CTimeThread
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CTimeThread::~CTimeThread()
{
	//イベントオブジェクトを非シグナル状態に設定
	ResetEvent(m_hTimeEvent);
	//================================
	//次のいずれかが成立すると制御を返す
	//・指定されたオブジェクトがシグナル状態になる
	//・タイムアウト時間が経過した
	//引数:オブジェクトのハンドル、タイムアウトの経過時間
	//WAIT_TIMEOUT:タイムアウト時間が経過し、指定されたオブジェクトが非シグナル状態であったことを意味します。
	//================================
	while (WaitForSingleObject(m_hTimeThread, 0) == WAIT_TIMEOUT)
	{
		Sleep(1);
	}
	m_hTimeThread = NULL;
}

//====================================
//関数名	:Start
//引数		:pDXDevice:デバイス
//戻り値	:
//内容		:タイマースレッドの作成
//====================================
void CTimeThread::Start(CDXDevice *pDXDevice)
{
	m_pDXDevice = pDXDevice;
	//スレッド開始
	//スレッド用ID
	DWORD	threadID;
	m_hTimeThread = CreateThread(
		0,								//セキュリティ属性(無し)
		0,								//スレッドが持つスタックのサイズを、バイト単位で指定(デフォルト)
		(LPTHREAD_START_ROUTINE)Run,	//スレッドの開始アドレスを指定
		(VOID* )this,					//スレッドへの引数指定(アドレス)
		0,								//制御フラグを指定(設定なし)
		&threadID						//スレッドIDが格納されているポインtを指定
		);

	//イベントオブジェクト作成
	m_hTimeEvent = CreateEvent(
		0,		//セキュリティ属性（設定なし）
		TRUE,	//手動/リセットオブジェクト作成
		TRUE,	//イベントオブジェクトの初期設定状態指定（シグナル状態）
		0		//イベントオブジェクトの名前
		);
	//オブジェクトをシグナル状態に設定
	SetEvent(m_hTimeEvent);
}

//====================================
//関数名	:Run
//引数		:pTimeTread:タイマースレッド
//戻り値	:
//内容		:アプリケーションの実行
//====================================
void CTimeThread::Run(CTimeThread *pTimeThread)
{
	//時間保存用
	static DWORD dwOldTime, dwNowTime;

	//GameSpeedを0.02秒間隔に指定
	DWORD dwTimer = 20;

	while (TRUE)
	{
		//シグナル状態もしくはタイムアウトしていたら
		if (WaitForSingleObject(m_hTimeEvent, 0) == WAIT_TIMEOUT)
		{
			break;
		}

		//タイマースレッドが待機状態なら
		if (pTimeThread->m_bTimeThreadSleep)
		{
			continue;
		}

		//デバイスを消失していなければ
		if (!pTimeThread->m_pDXDevice->getDeviceLost())
		{
			//現在の時間を取得
			dwNowTime = timeGetTime();
			
			//現在の時刻-過去の時刻の差が0.02秒以下の場合
			if (dwNowTime - dwOldTime < dwTimer)
			{
				//時間待ち
				Sleep(dwTimer - (dwNowTime - dwOldTime));
				//現在の時刻を取得
				dwNowTime = timeGetTime();
			}
			//現在の時間を過去の時間に置き換える
			dwOldTime = dwNowTime;

			//画面の更新
			if (pTimeThread->m_pDXDevice && pTimeThread->m_pDXDevice->getDirect3D())
			{
				HRESULT hr = S_OK;

				if ((pTimeThread->m_pDXDevice->getWindowed()) || !pTimeThread->m_pDXDevice->getWindowed())
				{
					pTimeThread->m_pDXDevice->getDInput()->Update();
				}

				// レンダリング処理
				hr = pTimeThread->m_pDXDevice->Render();
				if (hr == D3DERR_DEVICELOST) 
				{
					pTimeThread->m_pDXDevice->setDeviceLost(TRUE);
					pTimeThread->setTimeThreadSleep(TRUE);
				}
				if (hr == D3DERR_DEVICENOTRESET)
				{
					pTimeThread->m_pDXDevice->setDeviceLost(TRUE);
					pTimeThread->setTimeThreadSleep(TRUE);
				}
				if (pTimeThread->m_pDXDevice->getChangeDisplayMode())
				{
					pTimeThread->setTimeThreadSleep(TRUE);
				}
			}
		}
		else
		{
			if (pTimeThread->m_pDXDevice->RestoreDevice())
			{
				pTimeThread->m_pDXDevice->setDeviceLost(FALSE);
				pTimeThread->m_pDXDevice->setChangeDisplayMode(FALSE);
			}
		}
		Sleep(1);
	}
}

//====================================
//関数名	:sertTimeThread
//引数		:bTimeThreadSleep:待機の有無
//戻り値	:
//内容		:タイマースレッド待機の指定
//====================================
void CTimeThread::setTimeThreadSleep(BOOL bTimeThreadSleep)
{
	m_bTimeThreadSleep = bTimeThreadSleep;
}

//====================================
//関数名	:getTimeThread
//引数		:
//戻り値	:タイマースレッド待機の有無
//内容		:タイマーのスレッド待機の取得
//====================================
BOOL CTimeThread::getTimeThreadSleep()
{
	return m_bTimeThreadSleep;
}
