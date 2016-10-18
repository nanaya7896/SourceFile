#pragma once
//=====define�̐ݒ�=====//
//#define DIRECTSOUND_VERSION		0x900

//=====Include File=====//
#include<dsound.h>
#include"../../Global/Global.h"
#include"WaveFile.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dsound.lib")
//=====�N���X�錾=====//
class CDirectSound;


//�T�E���h�o�b�t�@�֘A�N���X
class CSoundBuffer
{
protected:
	LONG	Volume;				// ����
	LONG	Pan;				// ���E�̃I�[�f�B�I�`�����l���̑��΃{�����[��
	DWORD	Frequency;			// ���g��
	DWORD	BufferBytes;		// �T�E���h�o�b�t�@�T�C�Y
	WAVEFORMATEX Wfx;			// �t�H�[�}�b�g
	LPDIRECTSOUNDBUFFER	buffer;	// �T�E���h�o�b�t�@
	BOOL	bPlay;				// ���t���̗L��
	int		NumPlay;			// ���t�ԍ�


	//private�����o�ϐ�
private:
	
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CSoundBuffer();
	//�f�X�g���N�^
	~CSoundBuffer();
	
	BOOL Create(CDirectSound&, WAVEFORMATEX*, DWORD);
	BOOL CreatePrimary(CDirectSound&, DWORD dwFreq = 22050);
	//���
	HRESULT Release();
	//�f�o�C�X����
	HRESULT Restore();

 	BOOL LoadWave(CDirectSound&,char*);

	BOOL Play(BOOL loop = FALSE);
	BOOL Stop();

	//=====��Ԃ̊m�F====//
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

	//======�Q�b�g�֐�====//
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

	//=====�Z�b�g�֐�=====//
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

//DirectSound�N���X
class CDirectSound
{
protected:
	//======================================
	//�T�E���h�C���^�[�t�F�C�X
	LPDIRECTSOUND	m_pDSound;
	//�T�E���h�̃o�b�t�@�[(�v���C�}���[�o�b�t�@�[)
	CSoundBuffer	Primary;
	//private�����o�ϐ�
private:

	//public�����o�ϐ�
public:
	////���b�N���J�n����A�o�b�t�@�̐擪����̃o�C�g�����w��
	//DWORD					m_dwOffset;
	////���b�N����u���b�N�̃o�C�g����ݒ�
	//DWORD					m_dwBlockByts;
	////�ŏ��̃u���b�N�̃|�C���^
	//LPVOID					m_lpVPtr1;
	////�ŏ��̃u���b�N�̃T�C�Y
	//DWORD					m_dwByts1;
	////2�Ԗڂ̃u���b�N�̃|�C���^
	//LPVOID					m_lpVPtr2;
	////2�Ԗڂ̃u���b�N�̃T�C�Y
	//DWORD					m_dwByts2;


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CDirectSound();
	//�f�X�g���N�^
	~CDirectSound();
	BOOL Create(HWND, int freq = 22050);
	void RelaseObject();

	//�X�s�[�J�[�̎��ʗp
	DWORD SpeakerConfig(HRESULT);


	LPDIRECTSOUND operator -> ()
	{
		return m_pDSound;
	}
};

//�X�g���[���Đ��p�N���X
class CSoundBufferNotify {
	//private�����o�ϐ�
protected:
	LPDIRECTSOUNDNOTIFY m_pDSNotify;
private:


	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CSoundBufferNotify();
	//�f�X�g���N�^
	~CSoundBufferNotify();
	BOOL SetNotify(CSoundBuffer&, HANDLE);


};