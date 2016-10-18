//=====IncudeFile=====//
#include"TimeThread.h"

//�n���h���̎��Ԃ��`
HANDLE CTimeThread::m_hTimeEvent = NULL;

//====================================
//�֐���	:CTimeThread
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CTimeThread::CTimeThread()
{
	m_hTimeThread = NULL;
	m_bTimeThreadSleep = NULL;
	m_pDXDevice = NULL;
	m_hTimeEvent = NULL;
}

//====================================
//�֐���	:~CTimeThread
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CTimeThread::~CTimeThread()
{
	//�C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ�
	ResetEvent(m_hTimeEvent);
	//================================
	//���̂����ꂩ����������Ɛ����Ԃ�
	//�E�w�肳�ꂽ�I�u�W�F�N�g���V�O�i����ԂɂȂ�
	//�E�^�C���A�E�g���Ԃ��o�߂���
	//����:�I�u�W�F�N�g�̃n���h���A�^�C���A�E�g�̌o�ߎ���
	//WAIT_TIMEOUT:�^�C���A�E�g���Ԃ��o�߂��A�w�肳�ꂽ�I�u�W�F�N�g����V�O�i����Ԃł��������Ƃ��Ӗ����܂��B
	//================================
	while (WaitForSingleObject(m_hTimeThread, 0) == WAIT_TIMEOUT)
	{
		Sleep(1);
	}
	m_hTimeThread = NULL;
}

//====================================
//�֐���	:Start
//����		:pDXDevice:�f�o�C�X
//�߂�l	:
//���e		:�^�C�}�[�X���b�h�̍쐬
//====================================
void CTimeThread::Start(CDXDevice *pDXDevice)
{
	m_pDXDevice = pDXDevice;
	//�X���b�h�J�n
	//�X���b�h�pID
	DWORD	threadID;
	m_hTimeThread = CreateThread(
		0,								//�Z�L�����e�B����(����)
		0,								//�X���b�h�����X�^�b�N�̃T�C�Y���A�o�C�g�P�ʂŎw��(�f�t�H���g)
		(LPTHREAD_START_ROUTINE)Run,	//�X���b�h�̊J�n�A�h���X���w��
		(VOID* )this,					//�X���b�h�ւ̈����w��(�A�h���X)
		0,								//����t���O���w��(�ݒ�Ȃ�)
		&threadID						//�X���b�hID���i�[����Ă���|�C��t���w��
		);

	//�C�x���g�I�u�W�F�N�g�쐬
	m_hTimeEvent = CreateEvent(
		0,		//�Z�L�����e�B�����i�ݒ�Ȃ��j
		TRUE,	//�蓮/���Z�b�g�I�u�W�F�N�g�쐬
		TRUE,	//�C�x���g�I�u�W�F�N�g�̏����ݒ��Ԏw��i�V�O�i����ԁj
		0		//�C�x���g�I�u�W�F�N�g�̖��O
		);
	//�I�u�W�F�N�g���V�O�i����Ԃɐݒ�
	SetEvent(m_hTimeEvent);
}

//====================================
//�֐���	:Run
//����		:pTimeTread:�^�C�}�[�X���b�h
//�߂�l	:
//���e		:�A�v���P�[�V�����̎��s
//====================================
void CTimeThread::Run(CTimeThread *pTimeThread)
{
	//���ԕۑ��p
	static DWORD dwOldTime, dwNowTime;

	//GameSpeed��0.02�b�Ԋu�Ɏw��
	DWORD dwTimer = 20;

	while (TRUE)
	{
		//�V�O�i����Ԃ������̓^�C���A�E�g���Ă�����
		if (WaitForSingleObject(m_hTimeEvent, 0) == WAIT_TIMEOUT)
		{
			break;
		}

		//�^�C�}�[�X���b�h���ҋ@��ԂȂ�
		if (pTimeThread->m_bTimeThreadSleep)
		{
			continue;
		}

		//�f�o�C�X���������Ă��Ȃ����
		if (!pTimeThread->m_pDXDevice->getDeviceLost())
		{
			//���݂̎��Ԃ��擾
			dwNowTime = timeGetTime();
			
			//���݂̎���-�ߋ��̎����̍���0.02�b�ȉ��̏ꍇ
			if (dwNowTime - dwOldTime < dwTimer)
			{
				//���ԑ҂�
				Sleep(dwTimer - (dwNowTime - dwOldTime));
				//���݂̎������擾
				dwNowTime = timeGetTime();
			}
			//���݂̎��Ԃ��ߋ��̎��Ԃɒu��������
			dwOldTime = dwNowTime;

			//��ʂ̍X�V
			if (pTimeThread->m_pDXDevice && pTimeThread->m_pDXDevice->getDirect3D())
			{
				HRESULT hr = S_OK;

				if ((pTimeThread->m_pDXDevice->getWindowed()) || !pTimeThread->m_pDXDevice->getWindowed())
				{
					pTimeThread->m_pDXDevice->getDInput()->Update();
				}

				// �����_�����O����
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
//�֐���	:sertTimeThread
//����		:bTimeThreadSleep:�ҋ@�̗L��
//�߂�l	:
//���e		:�^�C�}�[�X���b�h�ҋ@�̎w��
//====================================
void CTimeThread::setTimeThreadSleep(BOOL bTimeThreadSleep)
{
	m_bTimeThreadSleep = bTimeThreadSleep;
}

//====================================
//�֐���	:getTimeThread
//����		:
//�߂�l	:�^�C�}�[�X���b�h�ҋ@�̗L��
//���e		:�^�C�}�[�̃X���b�h�ҋ@�̎擾
//====================================
BOOL CTimeThread::getTimeThreadSleep()
{
	return m_bTimeThreadSleep;
}
