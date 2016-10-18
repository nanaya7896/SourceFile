#pragma once
//=====Include File=====//
#include<dsound.h>
#include"../../Global/Global.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dsound.lib")

//=====�}�N��define====//
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2
//=====�N���X�錾=====//
class CWaveFile
{
	//private�����o�ϐ�
private:
	WAVEFORMATEX*	m_pwfx;
	HMMIO			m_hmmio;
	MMCKINFO		m_ckRiff;
	MMCKINFO		m_ck;
	//public�����o�ϐ�
public:


	//protected�����o�֐�
protected:
	
	//private�����o�֐�
private:


	//public�����o�֐�
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