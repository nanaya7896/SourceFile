#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"DirectSound.h"

//=====define�}�N��=====//
#define NUMSECONDARY	14
#define NUMEFFECTSOUND  14
//=====�N���X�錾=====//
class CSoundManager
{
	//protected�����o�ϐ�
protected:
	//DirectSound Objects
	CDirectSound DirectSound;
	//�T�E���h�o�b�t�@
	CSoundBuffer* pDSoundBuffer;
	//�Z�J���_���o�b�t�@
	CSoundBuffer* pDSoundSecondaryBuffer;
	//���t�I���ʒm�p
	CSoundBufferNotify	Notify;
	//�C�x���g�n���h��
	HANDLE				hEvent[NUMSECONDARY+1];
	//�C�x���g�Ǘ��X���b�h�n���h��
	HANDLE				hThread;
	//�C�x���g�Ǘ��X���b�h
	//static DWORD WINAPI EventNotifyProc(LPVOID param);

	//private�����o�ϐ�
private:
	

	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CSoundManager(HWND);
	//�f�X�g���N�^
	~CSoundManager();
	BOOL LoadWave(DWORD, char*);
	void Play(DWORD setPosition, BOOL loop);
	void Stop(DWORD setPosition);
	void StopAllSound();
	BOOL ChangeWave(DWORD,DWORD);
	//=====�Q�b�g�֐�====//
	CDirectSound* GetDirectSound()
	{
		return &DirectSound;
	}
};