#pragma once
//=====Include File=====//
#include"..\Global\Global.h"
#include"DirectX\DXDevice.h"
//=====�N���X�錾=====//
class CTimeThread
{
//private�����o�ϐ�
private:
	//�n���h��
	HANDLE			m_hTimeThread;
	//�X���b�h�Ǘ��p�C�x���g
	static HANDLE	m_hTimeEvent;
	//�^�C�}�[�X���b�h�̑ҋ@�L��
	BOOL			m_bTimeThreadSleep;

//public�����o�ϐ�
public:
	CDXDevice*		m_pDXDevice;

//private�����o�֐�
private:
	static void Run(CTimeThread*);

//public�����o�֐�
public:
	//�R���X�g���N�^
	CTimeThread();
	//�f�X�g���N�^
	virtual ~CTimeThread();

	void Start(CDXDevice*);

	//=====�Z�b�g�֐�=====//
	void setTimeThreadSleep(BOOL);

	//=====�Q�b�g�֐�=====//
	BOOL getTimeThreadSleep();
};