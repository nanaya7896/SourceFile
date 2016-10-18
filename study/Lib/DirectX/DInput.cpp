
#include "DInput.h"



//====================================
//�֐���	:CDInput
//����		:hWnd:�E�B���h�E�n���h��
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CDInput::CDInput(HWND hWnd)
{
	m_pDInput = NULL;
	m_pDInputJoyStick = NULL;
	m_pDInputKeyBoard = NULL;
	ZeroMemory(&m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
	ZeroMemory(&m_KeyInputPreview, sizeof(BYTE)*BUFFER_SIZE);
	InitDirectInput(hWnd);
}

//====================================
//�֐���	:~CDInput
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CDInput::~CDInput()
{
	ReleaseDirectInput();
}

//====================================
//�֐���	:InitDirectInput
//����		:hWnd:�E�B���h�E�n���h��
//�߂�l	:
//���e		:DIRECTINPUT�̏�����
//====================================
void CDInput::InitDirectInput(HWND hWnd)
{
	HRESULT hr=E_FAIL;
	DirectInput8Create(
		(HINSTANCE)((UINT64)GetWindowLong(hWnd, GWL_HINSTANCE)),		//�v���O�����̃C���X�^���X�n���h�����w��
		DIRECTINPUT_VERSION,											//�o�[�W���������w��(�ʏ�w��)
		IID_IDirectInput8A,												//�ړI�̃C���^�[�t�F�[�X�̈�ӂȎ��ʎq(�ʏ�w��)
		(void**)&m_pDInput,												//DirectInput�V�X�e���̃A�h���X���w��
		NULL															//COM�W�听�ɑ΂��鐧��I�u�W�F�N�g��IUnknown�C���^�[�t�F�C�X�̃A�h���X�ւ̃|�C���^(�ʏ�w��)
		);

	//DirectInputDevice�R�l�N�^�쐬
	//�L�[�{�[�h
	m_pDInput->CreateDevice(
		GUID_SysKeyboard,		//���͋@���GUID���w��
		&m_pDInputKeyBoard,		//DirectInputDevice8�C���^�[�t�F�[�X���󂯎��ϐ��̃|�C���^
		NULL					//COM�W�听�ɑ΂��鐧��I�u�W�F�N�g��IUnknown�C���^�t�F�[�X�̃A�h���X
		);

	//�W���C�X�e�B�b�N�E�f�o�C�X�̗�
	//m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
	//�f�o�C�X�^�C�v���w��(�Q�[���R���g���[��))
	hr = m_pDInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,	//�f�o�C�X�^�C�v���w��(�Q�[���R���g���[��)
		EnumJoyStickCallback,	//�񋓃R�[���o�b�N�֐��̃A�h���X���w��
		(VOID*)this,			//�񋓃R�[���o�b�N�֐��ւ̈���
		DIEDFL_ATTACHEDONLY		//�񋓂͈̔͂��w�肷��t���O�l(�f�t�H���g)
		);
	//=====���̓f�[�^�`���w��=====//
	m_pDInputKeyBoard->SetDataFormat(
		&c_dfDIKeyboard		//DirectInput�f�o�C�X���Ԃ��ׂ��f�[�^�`�����L�q����\����
		//�L�[�{�[�h�p�f�[�^�t�H�[�}�b�g�B256�o�C�g�̔z��
		);

	//�����W���C�X�e�B�b�N���ڑ����ꂽ�Ȃ�
	if (m_pDInputJoyStick) 
	{
		//�W���C�X�e�B�b�N�f�[�^�`���ݒ�
		m_pDInputJoyStick->SetDataFormat(
			&c_dfDIJoystick2	//�W���C�X�e�B�b�N(��Ƀt�H�[�X�t�B�[�h�o�b�N)�p�f�[�^�t�H�[�}�b�g�Bxyz���̢�ʒu����]�p�����x�������x���p�����x���t�H�[�X���g���N��A�X���C�_��2�A���_�n�b�g4�A�{�^��128
			);
	}
	//=====���̓f�[�^�`���ݒ�I��=====//

	//=====�������[�h�̐ݒ�=====//
	//����L�[�{�[�h
	m_pDInputKeyBoard->SetCooperativeLevel(
		hWnd,									//�E�B���h�E�n���h��
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE	//�t�H�A�O���E���h�Ŕ�r�����[�h�Ŏw��
		);
	//---JoyStick
	if (m_pDInputJoyStick)
	{
		m_pDInputJoyStick->SetCooperativeLevel(
			hWnd,									//�E�B���h�E�n���h��
			DISCL_FOREGROUND | DISCL_EXCLUSIVE	//�t�H�A�O���E���h(���̂ݓ��͏����擾)�Ŕr��(�ق��̃A�v���P�[�V�����Ŕr���Ŏ擾�ł��Ȃ�)���[�h�Ŏw��
			);
	}

	if (m_pDInputJoyStick)
	{
		//�����Ƃɐݒ���s��
		m_pDInputJoyStick->EnumObjects(
			CDInput::EnumAxesCallback,		//�R�[���o�b�N�֐��̃A�h���X���w��
			(VOID*)m_pDInputJoyStick,		//�R�[���o�b�N�֐��ɓn������
			DIDFT_AXIS						//�񋓂���I�u�W�F�N�g�̃^�C�v���w��(�f�o�C�X�̎�������)
			);
	}

	//���͋@��̎擾
	//�L�[�{�[�h������͎擾���J�n
	m_pDInputKeyBoard->Acquire();

	if (m_pDInputJoyStick)
	{
		HRESULT hr;
		//�|�[�����O�Ώۂ���f�[�^���擾
		hr = m_pDInputJoyStick->Poll();
		//�����擾�Ɏ��s���Ă�����
		if (FAILED(hr))
		{
			//�W���C�X�e�B�b�N��������擾
			hr = m_pDInputJoyStick->Acquire();
			//�����f�o�C�X�����X�g������
			while (hr == DIERR_INPUTLOST)
			{
				//�f�[�^�̎擾���J�n
				hr = m_pDInputJoyStick->Acquire();
			}
		}
	}
	



}

//====================================
//�֐���	:ReleaseDirectInput
//����		:
//�߂�l	:
//���e		:DirectInput�̉��
//====================================
void CDInput::ReleaseDirectInput()
{
	//DirectInput�֘A�I�u�W�F�N�g���
	if (m_pDInput != NULL)
	{
		//����Device�L�[�{�[�h�����݂���Ȃ�
		if (m_pDInputKeyBoard != NULL)
		{
			//�L�[�{�[�h�̎擾���~
			m_pDInputKeyBoard->Unacquire();
			SAFE_RELEASE(m_pDInputKeyBoard);
		}
		//����Device�W���C�X�e�B�b�N������Ȃ�
		if (m_pDInputJoyStick != NULL) 
		{
			//�W���C�X�e�B�b�N�̏��擾�𒆎~
			m_pDInputJoyStick->Unacquire();
			SAFE_RELEASE(m_pDInputJoyStick);
		}
		//DirectInput�V�X�e���̉��
		SAFE_RELEASE(m_pDInput);
	}
}

//====================================
//�֐���	:EnumJoyStickCallback
//����		:pdidInstance	:�f�o�C�X�ւ̃|�C���^
//			 lpContext		:�����Ƃ��Ďw�肵��32�r�b�g�l�ւ̃|�C���^
//�߂�l	:DIENUM_CONTINUE:���� DIENUM_STOP:���s
//���e		:�W���C�X�e�B�b�N���
//====================================
BOOL CDInput::EnumJoyStickCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID lpContext)
{
	CDInput*		pDInput = (CDInput *)lpContext;
	LPDIRECTINPUT8	pDI = pDInput->GetDInput();
	LPDIRECTINPUTDEVICE8* pDIJoyStick = pDInput->GetDInputJoyStick();

	HRESULT hr = E_FAIL;
	//�񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�[�X���擾
	hr = pDI->CreateDevice(
		pdidInstance->guidInstance,	//���͋@���GUID���w��(�f�o�C�X�C���X�^���X�ւ̃|�C���^���w��)
		pDIJoyStick,				//DirectInput�R�l�N�^�̃A�h���X���w��
		NULL						//COM�W�听�ɑ΂��鐧��I�u�W�F�N�g��IUnknown�C���^�t�F�[�X�̃A�h���X
		);

	//
	if (FAILED(hr)) 
	{
		//�񋓂͑S�ďI���Ύ����I�ɏI������̂ŁA�񋓂�������
		return DIENUM_CONTINUE;
	}
	//��ŏ\��
	return DIENUM_STOP;

}

//====================================
//�֐���	:EnumAxesCallback
//����		:pdidoi		:�f�o�C�X�I�u�W�F�N�g�̃C���X�^���X
//			 lpContext	:�����Ƃ��Ďw�肵��32�r�b�g�l�ւ̃|�C���^
//�߂�l	:DIENUM_CONTINUE:���� DIENUM_STOP:���s
//���e		: �񋓂��鎲�͈̔͂�ݒ�
//====================================
BOOL CDInput::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID lpContext)
{
	//
	LPDIRECTINPUTDEVICE8 pDIJoyStick = (LPDIRECTINPUTDEVICE8)lpContext;
	
	HRESULT hr;
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		//�f�o�C�X���̃I�u�W�F�N�g�͈̔͂Ɋւ�������i�[����\����
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		//dwObject�����o�̉��ߕ��@���w�肷��l(�I�u�W�F�N�g�^�C�v�܂��̓C���X�^���X���ʎq)
		diprg.diph.dwHow = DIPH_BYID;

		//�A�N�Z�X����v���p�e�B��������I�u�W�F�N�g(dwHow�����o�Ɏw�肳��Ă���l�Ɉˑ�)
		diprg.diph.dwObj = pdidoi->dwType;
		//�͈͂̉���
		diprg.lMin = -1000;
		//�͈͂̏��
		diprg.lMax = 1000;

		//�f�o�C�X�̓�����`����v���p�e�B��ݒ�
		hr = pDIJoyStick->SetProperty(
			DIPROP_RANGE,	//GUID�����w��(�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂�ݒ�)
			&diprg.diph		//�^�C�v�ŗL��DIPROPHEADER�̍\���̂̃A�h���X
			);

		if (FAILED(hr)) 
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�L�[�̃A�b�v�f�[�g�֐�
//====================================
HRESULT CDInput::Update()
{
	HRESULT hr = m_pDInputKeyBoard->Acquire();
	if ((hr == DI_OK || (hr == S_FALSE)))
	{
		//�O��̃L�[�̓��͏���ޔ�
		memcpy(m_KeyInputPreview, m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
		memcpy(&lastpaddata, &padData, sizeof(DIJOYSTATE2));
		//���
		BYTE tKeyInputWork[BUFFER_SIZE];
		m_pDInputKeyBoard->GetDeviceState(sizeof(m_KeyInput), &tKeyInputWork);
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])
			{
				//������Ă���J�E���gUP
				if (m_KeyInput[i] < 255)
				{
					m_KeyInput[i]++;
				}
				else
				{
					m_KeyInput[i] = 255;
				}
			}
			else {
				m_KeyInput[i] = 0;
			}
		}
	}

	return E_NOTIMPL;
}

//====================================
//�֐���	:GetKeyBoardState
//����		:pKeyData : �L�[���z��̃A�h���X
//�߂�l	:
//���e		:�L�[�{�[�h�̓��͏�Ԃ̎擾
//====================================
//void CDInput::GetKeyBoardState(LPBYTE pKeyData)
//{
//	//���\�b�h���s���l���
//	HRESULT hr;
//	//�z����O�ŏ�����
//	ZeroMemory(pKeyData, sizeof(BYTE)*BUFFER_SIZE);
//	
//	//���ړ��̓f�[�^�擾����
//	hr = m_pDInputKeyBoard->GetDeviceState(
//		BUFFER_SIZE,	//�i�[����v�f�̐����w��
//		pKeyData		//�i�[�p�z��A�h���X���w��
//		);
//	//�G���[:�L�[�{�[�h�̏��擾�Ɏ��s/���̓f�o�C�X�̃��X�g
//	if ((hr == DIERR_NOTACQUIRED) || (hr == DIERR_INPUTLOST))
//	{
//		//���͏��擾�ĊJ
//		m_pDInputKeyBoard->Acquire();
//	}
//	//HRESULT hr = m_pDInputKeyBoard->Acquire();
//	//if ((hr == DI_OK || (hr == S_FALSE)))
//	//{
//	//	//�O��̃L�[�̓��͏���ޔ�
//	//	memcpy(m_KeyInputPreview, m_KeyInput, sizeof(BYTE)*BUFFER_SIZE);
//	//	//���
//	//	BYTE tKeyInputWork[BUFFER_SIZE];
//	//	m_pDInputKeyBoard->GetDeviceState(sizeof(m_KeyInput), &tKeyInputWork);
//	//	for (int i = 0; i < BUFFER_SIZE; i++)
//	//	{
//	//		if (tKeyInputWork[i])
//	//		{
//	//			//������Ă���J�E���gUP
//	//			if (m_KeyInput[i] < 255)
//	//			{
//	//				m_KeyInput[i]++;
//	//			}
//	//			else
//	//			{
//	//				m_KeyInput[i] = 255;
//	//			}
//	//		}
//	//		else {
//	//			m_KeyInput[i] = 0;
//	//		}
//	//	}
//	//}
//}

//====================================
//�֐���	:GetJoyStickState
//����		:pJoyStick:���ւ̃A�h���X
//�߂�l	:
//���e		:�W���C�X�e�B�b�N�̏����擾
//====================================
void CDInput::GetJoyStickState(DIJOYSTATE2 *pJoyStick)
{
	HRESULT hr;
	//�W���C�X�e�B�b�N�̏�Ԃ��O�ŏ�����
	//DIJOYSTATE2 js2 = { 0 };
	//padData = { 0 };
	//�����W���C�X�e�B�b�N���Ȃ������ꍇ
	if (NULL == m_pDInputJoyStick)
	{
		//�󂯎�����A�h���X�ɃW���C�X�e�B�b�N���Ȃ���Ԃ�n��
		memcpy(pJoyStick,&padData,sizeof(DIJOYSTATE2));
		return;
	}

	//�|�[�����O�ΏۃI�u�W�F�N�g����f�[�^���擾
	hr = m_pDInputJoyStick->Poll();
	//�����擾�Ɏ��s������
	if (FAILED(hr))
	{
		//�W���C�X�e�B�b�N������͎擾���J�n
		m_pDInputJoyStick->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			//���͏��̎擾���ĊJ
			hr = m_pDInputJoyStick->Acquire();
		}
		return;
	}



	//�f�o�C�X���璼�ڃf�[�^���擾
	hr = m_pDInputJoyStick->GetDeviceState
	(
		sizeof(DIJOYSTATE2),	//�o�b�t�@�T�C�Y(�o�C�g�P��)
		&padData				//�f�o�C�X�̌��݂̏�Ԃ��󂯎��\���̂̃A�h���X
	);


	//
	//memcpy(
	//	pJoyStick,				//�R�s�[��̃A�h���X
	//	&padData,					//�R�s�[��
	//	sizeof(DIJOYSTATE2)		//DIJOYSTATE2�\����
	//	);

}

//====================================
//�֐���	:KeyState
//����		:code:�L�[�̔ԍ�
//�߂�l	:
//���e		:�L�[�����͂��ꑱ���Ă����Ԃ�
//====================================
bool CDInput::KeyState(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		if (m_KeyInput[code] > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//====================================
//�֐���	:KeyStatePreview
//����		:code:�L�[�̔ԍ�
//�߂�l	:
//���e		:�O��̃L�[�̏��
//====================================
bool CDInput::KeyStatePreview(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return false;
}

//====================================
//�֐���	:KeyDown
//����		:code
//�߂�l	:
//���e		:�L�[�������ꂽ�u�Ԃ�
//====================================
bool CDInput::KeyDown(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return (m_KeyInput[code] >= 1) && (m_KeyInputPreview[code] == 0);
	}
	return false;
}

//====================================
//�֐���	:KeyUp
//����		:code
//�߂�l	:
//���e		:�L�[���痣���ꂽ�u�Ԃ�
//====================================
bool CDInput::KeyUp(int code)
{
	if (code >= 0 && code < BUFFER_SIZE)
	{
		return (m_KeyInput[code] == 0) && (m_KeyInputPreview[code] >= 1);
	}
	return false;
}

BOOL CDInput::WASDKeyUpCheck()
{
	if (KeyUp(DIK_W))
	{
		return TRUE;
	}
	if (KeyUp(DIK_A))
	{
		return TRUE;
	}
	if (KeyUp(DIK_S))
	{
		return TRUE;
	}
	if (KeyUp(DIK_D))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDInput::WASDKeyDownCheck()
{
	if (KeyState(DIK_W))
	{
		return TRUE;
	}
	if (KeyState(DIK_A))
	{
		return TRUE;
	}
	if (KeyState(DIK_S))
	{
		return TRUE;
	}
	if (KeyState(DIK_D))
	{
		return TRUE;
	}

	return FALSE;
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
//int CDInput::KeyCount(int code)
//{
//	return 0;
//}

//====================================
//�֐���	:IsButtonDown
//����		:pos:�L�[�ԍ�
//�߂�l	:TRUE:������Ă��� FALSE:������Ă��Ȃ�
//���e		:�{�^����������Ă��邩�ǂ���
//====================================
BOOL CDInput::IsButtonDown(int pos)
{
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	return padData.rgbButtons[pos];
}

//====================================
//�֐���	:IsButtonPressed
//����		:pos:�L�[�ԍ�
//�߂�l	:TRUE:������Ă��� FALSE:������Ă��Ȃ�
//���e		:�O�񉟂���Ă��Ȃ����񏉂߂ĉ����ꂽ��
//====================================
BOOL CDInput::IsButtonPressed(int pos)
{
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	if (padData.rgbButtons[pos] && !lastpaddata.rgbButtons[pos])
	{
		return TRUE;
	}

		return FALSE;
}

//====================================
//�֐���	:LAnalogState
//����		:num:
//�߂�l	:TRUE:�X���Ă��� FALSE:�j���[�g����
//���e		:���A�i���O�X�e�B�b�N�̌X�����擾
//====================================
BOOL CDInput::LAnalogState(int num)
{
	//�f�o�C�X�����邩�m�F
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}

	switch (num)
	{
	case 0://��
		if (padData.lY <= -1000)
		{
			return TRUE;
		}
		break;
	case 1://��
		if (padData.lY >= 1000)
		{
			return TRUE;
		}
		break;
	case 2://��
		if (padData.lX <= -1000)
		{
			return TRUE;
		}
		break;
	case 3://�E
		if (padData.lX >= 1000)
		{
			return TRUE;
		}
		break;
	case 4://����
		if (padData.lY <= -500 && padData.lX <= -500)
		{
			return TRUE;
		}
		break;
	case 5://�E��
		if (padData.lY <= -500 && padData.lX >= 500)
		{
			return TRUE;
		}
		break;
	case 6://����
		if (padData.lY >= 500 && padData.lX <= -500)
		{
			return TRUE;
		}
		break;
	case 7://�E��
		if (padData.lY >= 500 && padData.lX>=500)
		{
			return TRUE;
		}
		break;
	case 8://���E��
		if (padData.lY <= -750 && padData.lX <= -250)
		{
			return TRUE;
		}
		break;
	case 9://�E����
		if (padData.lY <= -750 && padData.lX >= 250)
		{
			return TRUE;
		}
		break;
	case 10://������
		if (padData.lY <= -250 && padData.lX <= -750)
		{
			return TRUE;
		}
		break;
	case 11://�E�E��
		if (padData.lY <= -250 && padData.lX >= 750)
		{
			return TRUE;
		}
		break;
	case 12://������
		if (padData.lY >= 250 && padData.lX <= -750)
		{
			return TRUE;
		}
		break;
	case 13://�E�E��
		if (padData.lY >= 250 && padData.lX >= 750)
		{
			return TRUE;
		}
		break;
	case 14://���E��
		if (padData.lY >= 750 && padData.lX <= -250)
		{
			return TRUE;
		}
		break;
	case 15://�E����
		if (padData.lY >= 750 && padData.lX >= 250)
		{
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CDInput::AllControllCheck()
{
	//�f�o�C�X�����邩�m�F
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}

	for (int i = 0; i < 16; i++)
	{
		if (LAnalogState(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}



BOOL CDInput::RAnalogState(int num)
{
	//�f�o�C�X�����邩�m�F
	if (m_pDInputJoyStick == NULL)
	{
		return FALSE;
	}
	switch (num)
	{
	case 0://��
		if (padData.lZ <= -1000)
		{
			return TRUE;
		}
		break;
	case 1://�E
		if (padData.lZ >= 1000)
		{
			return TRUE;
		}
		break;
	}
	return FALSE;
}
