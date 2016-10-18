#include "SoundManager.h"

//====================================
//�֐���	:EventNotifyProc
//����		:param:�A�h���X
//�߂�l	:
//���e		:�C�x���g�Ǘ��p�v���V�[�W��
//====================================
//DWORD WINAPI CSoundManager::EventNotifyProc(LPVOID param)
//{
//	CSoundManager* pSound = (CSoundManager*)param;
//
//	while (TRUE)
//	{
//		DWORD dwResult = WaitForMultipleObjects(
//			NUMSECONDARY + 1,	//�I�u�W�F�N�g�̃n���h����
//			pSound->hEvent,		//�I�u�W�F�N�g�n���h���̔z��ւ̃|�C���^
//			FALSE,				//�ҋ@�̎��(�ǂꂩ����V�O�i����ԂɂȂ����Ƃ�)
//			INFINITE			//�^�C���A�E�g���Ԃ��~���b�P��(�������݂������܂ő҂�)
//			);
//		if (dwResult == NUMSECONDARY)
//		{
//			ExitThread(0);
//		}
//		else
//		{
//			pSound->pDSoundSecondaryBuffer[dwResult].SetPlay(TRUE);
//		}
//	}
//	return 0;
//}

//====================================
//�֐���	:CSoundManager
//����		:hWnd:�E�B���h�E�n���h��
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CSoundManager::CSoundManager(HWND hWnd)
{
	UNREFERENCED_PARAMETER(hWnd);
	
	//�T�E���h�o�b�t�@
	pDSoundBuffer = new CSoundBuffer[NUMEFFECTSOUND];
	//�Z�J���_���o�b�t�@
	pDSoundSecondaryBuffer = new CSoundBuffer[NUMSECONDARY];

	//��~���ɔ�������C�x���g���쐬
	for (DWORD i = 0; i < NUMSECONDARY; i++)
	{
		hEvent[i] = ::CreateEvent(
			NULL,	//
			FALSE,	//
			FALSE,	//
			NULL	//
			);
	}
	//DWORD ThreadID;
	//hThread = CreateThread(
	//	NULL,				//
	//	0,					//
	//	EventNotifyProc,	//
	//	this,				//
	//	0,					//
	//	&ThreadID			//
	//	);
}

//====================================
//�֐���	:~CSoundManager
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CSoundManager::~CSoundManager()
{
	DWORD i;



	for (i = 0; i < NUMEFFECTSOUND; i++)
	{
		pDSoundBuffer[i].Release();
	}
	if (pDSoundBuffer)
	{
		delete[] pDSoundBuffer;
	}

	for (i = 0; i < NUMSECONDARY; i++)
	{
		pDSoundSecondaryBuffer[i].Release();
	}
	if (pDSoundSecondaryBuffer)
	{
		delete[] pDSoundSecondaryBuffer;
	}
}

//====================================
//�֐���	:LoadWave
//����		:setPosition:�i�[�ԍ�
//			 fileName	:�T�E���h�t�@�C����
//�߂�l	:TRUE:�����@FALSE:���s
//���e		:WAVE�t�@�C���̓ǂݍ���
//====================================
BOOL CSoundManager::LoadWave(DWORD setPosition, char *fileName)
{
	if (!pDSoundBuffer[setPosition].LoadWave(DirectSound, fileName))
	{
		return FALSE;
	}

	return TRUE;
}

//====================================
//�֐���	:Play
//����		:setPosition:���t�ԍ�
//			 loop		:�J��Ԃ����ۂ�
//�߂�l	:
//���e		:�T�E���h�Đ�
//====================================
void CSoundManager::Play(DWORD setPosition, BOOL loop)
{
	DWORD i;

	if (setPosition >= NUMEFFECTSOUND)
	{
		//�ǂݍ��񂾌��ȊO�̃T�E���h���I�����ꂽ
		return;
	}
	
	//�Z�J���_���o�b�t�@�ɁA�����T�E���h�����邩�m�F
	if (pDSoundBuffer[setPosition].IsAlive())
	{
		for (i = 0; i < NUMSECONDARY; i++)
		{
			//���ݍĐ�����Ă�����̂ƌ��ݓ����Ă�����̂����ǂݍ��񂾐����Ɠ����Ȃ�
			if (pDSoundSecondaryBuffer[i].GetPlay() && pDSoundSecondaryBuffer[i].GetNumPlay() == (int)setPosition)
			{
				//���[�v�𔲂���
				break;
			}
		}
		//�����T�E���h���݂������̂�
		if (i < NUMSECONDARY)
		{
			//�Đ�
			pDSoundSecondaryBuffer[i].Play(loop);
			pDSoundSecondaryBuffer[i].SetPlay(FALSE);
		}
		else
		{
			//�����T�E���h��������Ȃ������Ƃ��̏���
			for (i = 0; i < NUMSECONDARY; i++)
			{
				//���s�\�ȃZ�J���_���ޯ̧������
				if (pDSoundSecondaryBuffer[i].GetPlay())
				{
					//���s�\�ȃo�b�t�@�𔭌�
					break;
				}
			}
			//���s�\�ȃo�b�t�@���������̂ňȉ��̏���
			if (i < NUMSECONDARY)
			{
				//���݂̍Đ��ԍ���ύX
				pDSoundSecondaryBuffer[i].SetNumPlay((int)setPosition);
				//
				ChangeWave(i, setPosition);
				//
				//Notify.SetNotify(pDSoundSecondaryBuffer[i], hEvent[i]);
				//
				pDSoundSecondaryBuffer[i].Play(loop);
				//
				pDSoundSecondaryBuffer[i].SetPlay(FALSE);
			}
		}
	}
}

//====================================
//�֐���	:Stop
//����		:setPosition:���t�ԍ�
//�߂�l	:
//���e		:�T�E���h���~�߂�
//====================================
void CSoundManager::Stop(DWORD setPosition)
{
	DWORD i;

	if (setPosition >=NUMEFFECTSOUND)
	{
		return;
	}
	//�Z�J���_���o�b�t�@�̂ǂ��ōĐ�����Ă��邩����
	for (i = 0; i < NUMSECONDARY; i++)
	{
		if (pDSoundSecondaryBuffer[i].GetNumPlay() == (int)setPosition)
		{
			break;
		}
	}
		if (i < NUMSECONDARY)
		{
			pDSoundSecondaryBuffer[i].Stop();
			pDSoundSecondaryBuffer[i].SetPlay(TRUE);
		}
	
}

//====================================
//�֐���	:StopAllSound
//����		:
//�߂�l	:
//���e		:�S�Ă�BGM���~�߂�
//====================================
void CSoundManager::StopAllSound()
{
	for (DWORD i = 0; i < NUMSECONDARY; i++)
	{
		if (pDSoundSecondaryBuffer[i].GetNumPlay() != -1)
		{
			pDSoundSecondaryBuffer[i].Stop();
			pDSoundSecondaryBuffer[i].SetPlay(TRUE);
			pDSoundSecondaryBuffer[i].SetNumPlay(-1);
		}
	}
}

//====================================
//�֐���	:ChangeWave
//����		:dwDest	:�i�[�ԍ�
//			 dwSrc	:���t�ԍ�
//�߂�l	:TRUE:���� ���s:FALSE
//���e		:WAVE�����Z�J���_���o�b�t�@�ɃR�s�[
//====================================
BOOL CSoundManager::ChangeWave(DWORD dwDest, DWORD dwSrc)
{
	if (dwDest >= NUMSECONDARY || dwSrc >= NUMEFFECTSOUND)
	{
		return FALSE;
	}

	pDSoundSecondaryBuffer[dwDest].Release();

	if (!pDSoundSecondaryBuffer[dwDest].Create(DirectSound, &pDSoundBuffer[dwSrc].GetSetWaveFormat(), pDSoundBuffer[dwSrc].GetBufferBytes()))
	{
		return FALSE;
	}

	//��o�b�t�@�Ƀo�b�t�@�f�[�^����������
	LPVOID pFirstDest, pSecondDest, pFirstSrc, pSecondSrc;
	DWORD dwFirstDestBufferBytes, dwSecondDestBufferBytes;
	DWORD dwFirstSrcBufferBytes, dwSecondSrcBufferBytes;

	pDSoundSecondaryBuffer[dwDest]->Lock(
		0,
		pDSoundSecondaryBuffer[dwDest].GetBufferBytes(),
		&pFirstDest,
		&dwFirstDestBufferBytes,
		&pSecondDest,
		&dwSecondDestBufferBytes,
		0L
		);

	pDSoundBuffer[dwSrc]->Lock(
		0,
		pDSoundBuffer[dwSrc].GetBufferBytes(),
		&pFirstSrc,
		&dwFirstSrcBufferBytes,
		&pSecondSrc,
		&dwSecondSrcBufferBytes,
		0L
		);

	//�v���C�}���o�b�t�@�̒��g���R�s�[
	memcpy(pFirstDest, pFirstSrc, dwFirstSrcBufferBytes);

	//�������ӂꂽ�u���b�N����������
	if (dwSecondDestBufferBytes)
	{
		memcpy(pSecondDest, pSecondSrc, dwSecondDestBufferBytes);
	}
	pDSoundBuffer[dwSrc]->Unlock(pFirstSrc, dwFirstSrcBufferBytes, pSecondSrc, dwSecondSrcBufferBytes);
	pDSoundSecondaryBuffer[dwDest]->Unlock(pFirstDest, dwFirstDestBufferBytes, pSecondDest, dwSecondDestBufferBytes);

	return TRUE;
}
