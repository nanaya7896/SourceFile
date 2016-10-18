#include "DirectSound.h"

//====================================
//�֐���	:CDirectSound
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CDirectSound::CDirectSound()
{
	m_pDSound = 0;
}

//====================================
//�֐���	:~CDirectSound
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CDirectSound::~CDirectSound()
{
	RelaseObject();
}

//====================================
//�֐���	:Create
//����		:hWnd:�E�B���h�E�n���h��
//			 freq:���g��
//�߂�l	:TRUE:���� FALSE:���s
//���e		:�T�E���h�C���^�[�t�F�C�X�̍쐬
//====================================
BOOL CDirectSound::Create(HWND hWnd, int freq)
{
	HRESULT hr;
	//DirectSound�C���^�t�F�[�X�̎擾
	if (FAILED(hr = DirectSoundCreate(NULL, &m_pDSound, NULL)))
	{
		return FALSE;
	}
	//=====�������x���̐ݒ�=====//
	//DSSCL_NORMAL		:�W���������x��
	//DSSCL_PRIORITY	:�D�拦�����x��
	//DSSCL_WRITEPRIMARY:�������ݗD�拦�����x��
	if (FAILED(hr = m_pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return FALSE;
	}


	//�v���C�}���[�o�b�t�@�̍쐬
	return Primary.CreatePrimary(*this, freq);

	//return TRUE;
}

//====================================
//�֐���	:RelaseObject
//����		:
//�߂�l	:
//���e		:�T�E���h�C���^�[�t�F�C�X�̉��
//====================================
void CDirectSound::RelaseObject()
{
	//�����T�E���h�C���^�t�F�[�X�����݂�����
	if (m_pDSound)
	{
		Primary.Release();
		m_pDSound->Release();
		m_pDSound = NULL;
	}
}

//====================================
//�֐���	:SpeakerConfig
//����		:hr:HRESULT�^
//�߂�l	:
//���e		:�X�s�[�J�[�𒲍�
//====================================
DWORD CDirectSound::SpeakerConfig(HRESULT hr)
{
	//=====�X�s�[�J�[�̎���=====//
	DWORD dwSpeakerConfig;
	hr = m_pDSound->GetSpeakerConfig(&dwSpeakerConfig);
	//���������ꍇ
	if (SUCCEEDED(hr))
	{
		switch ((DSSPEAKER_CONFIG(dwSpeakerConfig)))
		{
		case DSSPEAKER_5POINT1_BACK:
			//�T�u�E�[�t�@�[�t����5�T���E���h�E�X�s�[�J�[
			break;
		case DSSPEAKER_5POINT1_SURROUND:
			//�z�[���V�A�^�[�����̃T�u�E�[�t�@�[�t���T�T���E���h�E�X�s�[�J�[
			break;
		case DSSPEAKER_DIRECTOUT:
			//�X�s�[�J�[�p�̍\���͍s���Ȃ�
			break;
		case DSSPEAKER_HEADPHONE:
			//�w�b�h�z��
			break;
		case DSSPEAKER_MONO:
			//���m�����X�s�[�J�[
			break;
		case DSSPEAKER_QUAD:
			//4�`�����l���X�s�[�J�[
			break;
		case DSSPEAKER_STEREO:
			//�X�e���I�X�s�[�J�[
			//�W�I���g���\��(�Q�̃X�s�[�J�[�̔z�u)
			switch (DSSPEAKER_GEOMETRY(dwSpeakerConfig))
			{
			case DSSPEAKER_GEOMETRY_WIDE:
				//���X�i�[�𒆐S�Ƃ���20�x�̊p�x
				break;
			case DSSPEAKER_GEOMETRY_NARROW:
				//���X�i�[�𒆐S�Ƃ���10�x�̊p�x
				break;
			case DSSPEAKER_GEOMETRY_MIN:
				//���X�i�[�𒆐S�Ƃ���5�x�̊p�x
				break;
			case DSSPEAKER_GEOMETRY_MAX:
				//���X�i�[�𒆐S�Ƃ���180�x�̊p�x
				break;
			}
			break;
		case DSSPEAKER_SURROUND:
			//�T���E���h�X�s�[�J�[
			break;
		case DSSPEAKER_7POINT1_WIDE:
			//�T�u�E�[�t�@�[�t����7�T���E���h�X�s�[�J�[
			break;
		case DSSPEAKER_7POINT1_SURROUND:
			//�z�[���V�A�^�[�����̃T�u�E�[�t�@�[�t���V�T���E���h�X�s�[�J�[
			break;
		}
	}
	return dwSpeakerConfig;
}

//======================================CSoundBuffer�N���X======================================//

//====================================
//�֐���	:CSoundBuffer
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CSoundBuffer::CSoundBuffer()
{
	buffer = NULL;
	Volume = 0;
	Pan = 0;
	Frequency = 0;
	bPlay = TRUE;
	NumPlay = -1;
}

//====================================
//�֐���	:~CSoundBuffer
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CSoundBuffer::~CSoundBuffer()
{
	if (buffer)
	{
		buffer->Release();
	}

}

//====================================
//�֐���	:Create
//����		:dSound			:�_�C���N�g�T�E���h�̃A�h���X
//			 pFormat		:WAVEFORMATEX�\����
//			 dwBufferBytes	:�o�b�t�@�T�C�Y
//�߂�l	:TRUE:���� FALSE:���s
//���e		:�T�E���h�o�b�t�@�̍쐬
//====================================
BOOL CSoundBuffer::Create(CDirectSound& dSound, WAVEFORMATEX *pFormat, DWORD dwBufferBytes)
{
	HRESULT hr;
	DSBUFFERDESC dsbd;

	//DSBUFFERDESC�\���̂̐ݒ�
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_GETCURRENTPOSITION2 |
		DSBCAPS_GLOBALFOCUS |
		DSBCAPS_LOCDEFER |
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLFREQUENCY |
		DSBCAPS_CTRLPOSITIONNOTIFY;
	dsbd.dwBufferBytes = dwBufferBytes;
	dsbd.lpwfxFormat = pFormat;
	//���̃o�b�N�A�b�v
	BufferBytes = dwBufferBytes;
	memcpy(&Wfx,pFormat,sizeof(WAVEFORMATEX));
	
	//�T�E���h�o�b�t�@�̍쐬
	if (FAILED(hr = dSound->CreateSoundBuffer(&dsbd, &buffer, NULL)))
	{
		return FALSE;
	}

	//���̎擾
	buffer->GetVolume(&Volume);
	buffer->GetPan(&Pan);
	buffer->GetFrequency(&Frequency);


	return TRUE;
}

//====================================
//�֐���	:CreatePrimary
//����		:dSound			:�_�C���N�g�T�E���h�̃A�h���X
//			 dwPrimaryFreq	:���g��
//�߂�l	:TRUE:���� FALSE:���s
//���e		:�v���C�}���o�b�t�@�̍쐬
//====================================
BOOL CSoundBuffer::CreatePrimary(CDirectSound &dSound, DWORD dwPrimaryFreq)
{
	HRESULT hr;

	//DSBUFFERDESC�\���̂̐ݒ�
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags =
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN |
		DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	//�T�E���h�o�b�t�@�̍쐬
	if (FAILED(hr = dSound->CreateSoundBuffer(&dsbd, &buffer, NULL)))
	{
		return FALSE;
	}

	//WAVE�t�H�[�}�b�g�̐ݒ�
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	//�v���C�}���o�b�t�@��Wave�t�H�[�}�b�g��ݒ�
	//�D�拦�����x���ȏ�̋������x�����ݒ肳��Ă���K�v������B
	//WAVE_FORMAT_PCM:�P�`�����l��PCM�܂���2�`�����l��PCM
	//WAVE_FORMAT_EXTENSIBLE:WAVEFORMATEXTENSIBLE���g�p
	//
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	//�`�����l���� 1:���m����2:�X�e���I
	//2�`�����l��(�X�e���I)
	wfx.nChannels = 2;
	//11.025kHz:11025 22.05kHz:22050(�w���c�P�ʂŎw��)
	//�T���v�����O���[�g
	wfx.nSamplesPerSec = dwPrimaryFreq;
	
	//1�T���v��������̃r�b�g��
	//WAVE_FORMAT_PCM�̏ꍇ:�8��܂��͢16�
	//16�r�b�g
	wfx.wBitsPerSample = 16;

	//�u���b�N�A���C�����g
	//WAVE_FORMAT_PCM�̏ꍇ:nChannels*wBitsPerSample/8�ɓ�����
	wfx.nBlockAlign = 4;

	//1�b�Ԃɓ]������o�C�g��
	//WAVE_FORMAT_PCM�̏ꍇ:nSamplesPerSec*nBlockAlign�ɓ�����
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	if (FAILED(hr = buffer->SetFormat(&wfx)))
	{
		//�ݒ�Ɏ��s
		return FALSE;
	}

	buffer->GetVolume(&Volume);
	buffer->GetPan(&Pan);
	buffer->GetFrequency(&Frequency);

	return TRUE;
}

//====================================
//�֐���	:Release
//����		:
//�߂�l	:
//���e		:�o�b�t�@�̉��
//====================================
HRESULT CSoundBuffer::Release()
{

	if (buffer)
	{
		HRESULT hr;

		hr= buffer->Release();
	}
	buffer = NULL;

	return S_OK;
}

//====================================
//�֐���	:Restore
//����		:
//�߂�l	:
//���e		:�o�b�t�@�̕���
//====================================
HRESULT CSoundBuffer::Restore()
{
	HRESULT hr;
	if (buffer)
	{
		hr= buffer->Restore();
	}

	return S_OK;
}

//====================================
//�֐���	:LoadWave
//����		:dSound:�_�C���N�g�T�E���h�̃A�h���X
//			 pFileName:�T�E���h�t�@�C����
//�߂�l	:TRUE:�ǂݍ��ݐ��� FALSE:���s
//���e		:Wave�t�@�C���̓ǂݍ���
//====================================
BOOL CSoundBuffer::LoadWave(CDirectSound &dSound, char *pFileName)
{
	CWaveFile file;
	
	//�����t�@�C�����J���Ȃ��A
	if (!file.Open(pFileName) || !file.StartRead())
	{
		return FALSE;
	}

	//�o�b�t�@������΃����[�X
	Release();

	//�V����WAVE�T�C�Y�̃o�b�t�@�ō쐬
	if (!Create(dSound, file.GetFormat(), file.CkSize()))
	{
		return FALSE;
	}

	//�o�b�t�@��WAVE�f�[�^�ɏ�������
	LPVOID pFirstDest, pSecondDest;
	DWORD dwFirstDestBufferBytes, dwSecondDestBufferBytes;

	HRESULT hr = buffer->Lock(
		0,							//�I�t�Z�b�g�l���o�C�g�P��
		file.CkSize(),				//���b�N�����o�b�t�@�����̃T�C�Y���o�C�g�P��
		&pFirstDest,				//�ŏ��Ƀ��b�N���ꂽ�����ւ̃|�C���^
		&dwFirstDestBufferBytes,	//�u���b�N�̃o�C�g��
		&pSecondDest,				//���ӂꂽ�u���b�N�̃��b�N���ꂽ�����ւ̃|�C���^
		&dwSecondDestBufferBytes,	//���ӂꂽ�u���b�N�̃o�C�g��
		0L
		);

	if (hr == DSERR_BUFFERLOST)
	{
		//�f�o�C�X�̕���
		Restore();
		hr = buffer->Lock(
			0,							// �I�t�Z�b�g�l���o�C�g�P��
			file.CkSize(),				// ���b�N�����o�b�t�@�����̃T�C�Y���o�C�g�P��
			&pFirstDest,				// �ŏ��Ƀ��b�N���ꂽ�����ւ̃|�C���^
			&dwFirstDestBufferBytes,	// �u���b�N�̃o�C�g��
			&pSecondDest,				// ��ꂽ�u���b�N�̃��b�N���ꂽ�����ւ̃|�C���^
			&dwSecondDestBufferBytes,	// ��ꂽ�u���b�N�̃o�C�g��
			0L);
	}

	//���������ꍇ
	if (SUCCEEDED(hr))
	{
		BOOL ReaderErr = FALSE;
		DWORD dwSizeRead;

		if (!file.Read((LPBYTE)pFirstDest, dwFirstDestBufferBytes, &dwSizeRead) || dwFirstDestBufferBytes != dwSizeRead)
		{
			ReaderErr = TRUE;
		}

		if (dwSecondDestBufferBytes)
		{
			if (!file.Read((LPBYTE)pSecondDest, dwSecondDestBufferBytes, &dwSizeRead) || dwSecondDestBufferBytes != dwSizeRead)
			{
				ReaderErr = TRUE;
			}
		}
		//���b�N�͑��߂ɉ�������
		hr = buffer->Unlock(
			pFirstDest,
			dwFirstDestBufferBytes,
			pSecondDest,
			dwSecondDestBufferBytes
			);

		if (ReaderErr)
		{
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

//======================================CSoundBufferNotify�N���X======================================//

//====================================
//�֐���	:CSoundBufferNotify
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CSoundBufferNotify::CSoundBufferNotify()
{
	m_pDSNotify = NULL;
}

//====================================
//�֐���	:~CSoundBufferNotify
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CSoundBufferNotify::~CSoundBufferNotify()
{
	
}

//====================================
//�֐���	:SetNotify
//����		:buffer:�T�E���h�o�b�t�@
//			 hEvent:���t�I�����ɔ�������C�x���g�n���h��
//�߂�l	:TRUE:���� FALSE:���s
//���e		:���t�I���ʒm�C�x���g�Z�b�g
//====================================
BOOL CSoundBufferNotify::SetNotify(CSoundBuffer &buffer, HANDLE hEvent )
{
	HRESULT hr;

	//�C���^�[�t�F�C�X���w���|�C���^���擾����
	hr = buffer->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&m_pDSNotify);
	if (FAILED(hr))
	{
		//�C���^�[�t�F�C�X�擾�Ɏ��s
		return FALSE;
	}

	//
	DSBPOSITIONNOTIFY PositionNotify;

	//�Đ��I�����w��
	PositionNotify.dwOffset = DSBPN_OFFSETSTOP;
	PositionNotify.hEventNotify = hEvent;
	hr = m_pDSNotify->SetNotificationPositions(
		1,				//DSBPOSITIONNOTIFY�\���̂̐�
		&PositionNotify	//DSBPOSITIONNOTIFY�\���̂̃A�h���X
		);

	SAFE_RELEASE(m_pDSNotify);
	if (hr == S_OK)
	{
		return TRUE;
	}
	return FALSE;
}
