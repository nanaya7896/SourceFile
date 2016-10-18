#pragma once
//=====defineの設定=====//
//#define DIRECTSOUND_VERSION		0x900

//=====Include File=====//
#include<dsound.h>
#include"../../Global/Global.h"
#include"WaveFile.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dsound.lib")
//=====クラス宣言=====//
class CDirectSound;


//サウンドバッファ関連クラス
class CSoundBuffer
{
protected:
	LONG	Volume;				// 音量
	LONG	Pan;				// 左右のオーディオチャンネルの相対ボリューム
	DWORD	Frequency;			// 周波数
	DWORD	BufferBytes;		// サウンドバッファサイズ
	WAVEFORMATEX Wfx;			// フォーマット
	LPDIRECTSOUNDBUFFER	buffer;	// サウンドバッファ
	BOOL	bPlay;				// 演奏中の有無
	int		NumPlay;			// 演奏番号


	//privateメンバ変数
private:
	
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CSoundBuffer();
	//デストラクタ
	~CSoundBuffer();
	
	BOOL Create(CDirectSound&, WAVEFORMATEX*, DWORD);
	BOOL CreatePrimary(CDirectSound&, DWORD dwFreq = 22050);
	//解放
	HRESULT Release();
	//デバイス復元
	HRESULT Restore();

 	BOOL LoadWave(CDirectSound&,char*);

	BOOL Play(BOOL loop = FALSE);
	BOOL Stop();

	//=====状態の確認====//
	BOOL IsAlive()
	{
		return buffer != 0;
	}
	BOOL IsSoundPlaying()
	{
		return bPlay;
	}
	//===================//

	LPDIRECTSOUNDBUFFER operator -> ()
	{
		return buffer;
	}

	//======ゲット関数====//
	WAVEFORMATEX GetSetWaveFormat()
	{
		return Wfx;
	}
	DWORD	GetBufferBytes() 
	{
		return BufferBytes;
	}
	BOOL GetPlay()
	{
		return bPlay;
	}
	int GetNumPlay()
	{
		return NumPlay;
	}

	//=====セット関数=====//
	void SetBufferBytes(DWORD NewBufferBytes)
	{
		BufferBytes = NewBufferBytes;
	}

	void SetPlay(BOOL bNewPlay)
	{
		bPlay = bNewPlay;
	}

	void SetNumPlay(int NewNumPlay)
	{
		NumPlay = NewNumPlay;
	}

};

inline BOOL CSoundBuffer::Play(BOOL loop)
{
	if (FAILED(buffer->Play(0, 0, loop ?
		DSBPLAY_LOOPING : 0)))
	{
		return FALSE;
	}
	return TRUE;
}

inline BOOL CSoundBuffer::Stop()
{
	return buffer->Stop() == DS_OK;
}

//DirectSoundクラス
class CDirectSound
{
protected:
	//======================================
	//サウンドインターフェイス
	LPDIRECTSOUND	m_pDSound;
	//サウンドのバッファー(プライマリーバッファー)
	CSoundBuffer	Primary;
	//privateメンバ変数
private:

	//publicメンバ変数
public:
	////ロックを開始する、バッファの先頭からのバイト数を指定
	//DWORD					m_dwOffset;
	////ロックするブロックのバイト数を設定
	//DWORD					m_dwBlockByts;
	////最初のブロックのポインタ
	//LPVOID					m_lpVPtr1;
	////最初のブロックのサイズ
	//DWORD					m_dwByts1;
	////2番目のブロックのポインタ
	//LPVOID					m_lpVPtr2;
	////2番目のブロックのサイズ
	//DWORD					m_dwByts2;


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CDirectSound();
	//デストラクタ
	~CDirectSound();
	BOOL Create(HWND, int freq = 22050);
	void RelaseObject();

	//スピーカーの識別用
	DWORD SpeakerConfig(HRESULT);


	LPDIRECTSOUND operator -> ()
	{
		return m_pDSound;
	}
};

//ストリーム再生用クラス
class CSoundBufferNotify {
	//privateメンバ変数
protected:
	LPDIRECTSOUNDNOTIFY m_pDSNotify;
private:


	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CSoundBufferNotify();
	//デストラクタ
	~CSoundBufferNotify();
	BOOL SetNotify(CSoundBuffer&, HANDLE);


};