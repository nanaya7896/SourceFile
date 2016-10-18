#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"DirectSound.h"

//=====defineマクロ=====//
#define NUMSECONDARY	14
#define NUMEFFECTSOUND  14
//=====クラス宣言=====//
class CSoundManager
{
	//protectedメンバ変数
protected:
	//DirectSound Objects
	CDirectSound DirectSound;
	//サウンドバッファ
	CSoundBuffer* pDSoundBuffer;
	//セカンダリバッファ
	CSoundBuffer* pDSoundSecondaryBuffer;
	//演奏終了通知用
	CSoundBufferNotify	Notify;
	//イベントハンドル
	HANDLE				hEvent[NUMSECONDARY+1];
	//イベント管理スレッドハンドル
	HANDLE				hThread;
	//イベント管理スレッド
	//static DWORD WINAPI EventNotifyProc(LPVOID param);

	//privateメンバ変数
private:
	

	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CSoundManager(HWND);
	//デストラクタ
	~CSoundManager();
	BOOL LoadWave(DWORD, char*);
	void Play(DWORD setPosition, BOOL loop);
	void Stop(DWORD setPosition);
	void StopAllSound();
	BOOL ChangeWave(DWORD,DWORD);
	//=====ゲット関数====//
	CDirectSound* GetDirectSound()
	{
		return &DirectSound;
	}
};