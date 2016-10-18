#pragma once
//=====Include File=====//
#include<dsound.h>
#include"../../Global/Global.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dsound.lib")

//=====マクロdefine====//
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2
//=====クラス宣言=====//
class CWaveFile
{
	//privateメンバ変数
private:
	WAVEFORMATEX*	m_pwfx;
	HMMIO			m_hmmio;
	MMCKINFO		m_ckRiff;
	MMCKINFO		m_ck;
	//publicメンバ変数
public:


	//protectedメンバ関数
protected:
	
	//privateメンバ関数
private:


	//publicメンバ関数
public:
	CWaveFile();
	~CWaveFile();
	BOOL Open(char*);
	BOOL StartRead();
	BOOL Read(LPVOID, DWORD, DWORD*);
	BOOL Close();
	WAVEFORMATEX* GetFormat() { return m_pwfx; }
	DWORD CkSize() { return m_ck.cksize; }
};