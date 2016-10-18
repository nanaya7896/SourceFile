#include "DXDevice.h"

//====================================
//�֐���	: CDXDevice
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CDXDevice::CDXDevice()
{
	m_pDInput = NULL;
	m_pDirect3D = NULL;
	m_pDevice9 = NULL;
	m_bDeviceLost = FALSE;
	m_bChangeDisplayMode = FALSE;
	m_bCleanupObject = FALSE;
	m_pFont = NULL;
}

//====================================
//�֐���	:~CDXDevice
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CDXDevice::~CDXDevice()
{
		SAFE_DELETE(m_pScene);
		SAFE_DELETE(m_pDInput);
		SAFE_DELETE(m_pFont);
	
}
//====================================
//�֐���	:CreateResource
//����		:
//�߂�l	: TRUE:����  FALSE:��肠��
//���e		:���\�[�X�̍쐬 
//			 �f�o�C�X�����ɉe������Ȃ����\�[�X
//====================================
BOOL CDXDevice::CreateResource()
{
	//�V�[���̍쐬
	m_pScene = new CScene();
	//�t�H���g�̍쐬
	m_pFont = new CDXFont(m_pDevice9);

	m_pScene->CreateResource(m_pDevice9, m_hWnd, m_byKeyState, &m_JoyState,m_pFont,m_pDInput);
	return TRUE;
}

//====================================
//�֐���	:Initialize
//����		:
//�߂�l	:
//���e		:������
//====================================
HRESULT CDXDevice::Initialize(HWND hWnd, BOOL bWindowed)
{
	//Direct3D�V�X�e���̍쐬
	m_pDirect3D = ::Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pDirect3D == NULL)
	{
		return E_FAIL;
	}

	//�v���[���g�p�����[�^�[�̏�����
	InitPresentParameters(hWnd, bWindowed);

	//Direct3DDevice�̍쐬
	HRESULT hResult = CreateDevice(hWnd, m_d3dpp);
	if (FAILED(hResult))
	{
		return E_FAIL;
	}

	//DirectInput�̍쐬
	m_pDInput = new CDInput(hWnd);

	//���Ƀ��\�[�X�����݂�����
	if (!CreateResource())
	{
		//���\�[�X�����
		CleanupResource();
		return E_FAIL;
	}

	
	//
	if (!CreateObject())
	{
		//
		CleanupObject();
		return E_FAIL;
	}


	return S_OK;
}

//====================================
//�֐���	:InitPresentParameters
//����		:
//�߂�l	:
//���e		:
//====================================
void CDXDevice::InitPresentParameters(HWND hWnd, BOOL bWindowed)
{
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));	// �������̃[���N���A
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;// �t���b�s���O���@:IDirect3DDevice9::Present�������o�b�N�o�b�t�@�̓��e�Ɉ�؉e�����Ȃ�
	m_d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@����1�ɐݒ�
	m_d3dpp.EnableAutoDepthStencil = TRUE;				// �[�x�o�b�t�@���Ǘ���L��
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;		// 8�r�b�g�̃X�e���V���`�����l�����w��
	m_d3dpp.hDeviceWindow = hWnd;					// �E�B���h�E�n���h��
													//	D3DDISPLAYMODE BaseFullScreenMode = {800,600,60,D3DFMT_R5G6B5};	// ��{���[�h
													//	D3DDISPLAYMODE FullScreenMode = {800,600,75,D3DFMT_X8R8G8B8};	// �w�胂�[�h
	D3DDISPLAYMODE BaseFullScreenMode = { 1280,720,60,D3DFMT_R5G6B5 };	// ��{���[�h
	D3DDISPLAYMODE FullScreenMode = { 1280,720,75,D3DFMT_X8R8G8B8 };	// �w�胂�[�h
	m_bWindowed = bWindowed;
	m_hWnd = hWnd;

	m_Mode = CheckFullScreenAdapter(FullScreenMode);

	if (m_Mode.Format == D3DFMT_UNKNOWN)
	{
		m_Mode = BaseFullScreenMode;
	}

	//�f�B�X�v���C���[�h���擾
	m_pDirect3D->GetAdapterDisplayMode( 
		D3DADAPTER_DEFAULT,	  //�f�B�X�v���C�A�_�v�^�[�W���w��
		&m_d3ddm				//�f�B�X�v���C���[�h��ۑ�����\����
		);

	//�E�B���h�E���[�h�Ȃ�
	if (bWindowed)
	{
		//�E�B���h�E���[�h�ɐݒ�
		m_d3dpp.Windowed = TRUE;

		RECT rcClient;
		::GetClientRect(hWnd, &rcClient);
		//�o�b�N�o�b�t�@�̃T�C�Y��ύX
		m_d3dpp.BackBufferWidth = rcClient.right - rcClient.left;
		m_d3dpp.BackBufferHeight = rcClient.bottom - rcClient.top;

		if (m_d3ddm.Format == D3DFMT_X8R8G8B8)
		{
			//�o�b�N�o�b�t�@�̐F����ARGB8bit�Ɏw��
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		}
		else if (m_d3ddm.Format == D3DFMT_R5G6B5)
		{
			//�F��:A1bitRGB5bit�Ɏw��
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;
		}
		else
		{
			//�F����GetAdapterDisplayMode�Ŏ擾�����t�H�[�}�b�g���w��
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;
		}
		//���t���b�V���V�[�g���w��
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
	}
	//�t���X�N���[�����[�h�Ȃ�
	else
	{
		//�t���X�N���[�����[�h�ɐݒ�
		m_d3dpp.Windowed = FALSE;
		//�o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.BackBufferWidth = m_Mode.Width;
		//�o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.BackBufferHeight = m_Mode.Height;
		//���t���b�V�����[�g���w��
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;
		//�F�����w��
		m_d3dpp.BackBufferFormat = m_Mode.Format;

	}

}

//====================================
//�֐���	:CheckFullScreenAdapter
//����		: FullScreenMode �t���X�N���[�����[�h�̏��
//�߂�l	: ���s�ł���t���X�N���[�����[�h
//���e		: �O���t�B�b�N�A�_�v�^�[�̎擾
//====================================
D3DDISPLAYMODE CDXDevice::CheckFullScreenAdapter(D3DDISPLAYMODE FullScreenMode)
{
	//�A�_�v�^�����ʂ������ێ�����\����
	D3DADAPTER_IDENTIFIER9 Adapter;
	//����1:�� 2:���� 3:���t���b�V�����[�g 4:�t�H�[�}�b�g
	D3DDISPLAYMODE ResultMode = { 0,0,0,D3DFMT_UNKNOWN };
	//
	D3DDISPLAYMODE Mode;
	//
	int iLevel = 1000000;
	//
	int iNumber, iCount, i, j;
	//
	//UINT uMaxRefreshRate = 0;
	//
	//int iSlelectNumber = -1;

	//�A�_�v�^�[�̎��ʐF����z��ɃZ�b�g
	D3DFORMAT AdapterFormat[TotalAdapter]= 
	{
		D3DFMT_A2R10G10B10,		//�F��:A2R10G10B10
		D3DFMT_X8R8G8B8,		//�F��:X8R8G8B8
		D3DFMT_A8R8G8B8,		//�F��:A8R8G8B8
		D3DFMT_X1R5G5B5,		//�F��:X1R5G5B5
		D3DFMT_A1R5G5B5,		//�F��:A1R5G5B5
		D3DFMT_R5G6B5			//�F��:R5G6B5
	};
	//�V�X�e���ɂ���A�_�v�^�̐����擾
	iNumber = m_pDirect3D->GetAdapterCount();

	//�V�X�e���ɃA�_�v�^������ꍇ
	if (iNumber != -1)
	{
		//�V�X�e���ɑ��݂��Ă��������I�ȃf�B�X�v���C�A�_�v�^���L�q
		m_pDirect3D->GetAdapterIdentifier
			(
				0,			//�f�B�X�v���C�A�_�v�^�������������w��(DEFAULT���w��)
				0,			//�ʏ�͂O�ɐݒ肷��p�����[�^
				&Adapter	//�w�肵���A�_�v�^���L�q�����񂪊i�[�����\���̂̃|�C���^���w��
				);
		for (j = 0; j < TotalAdapter; j++)
		{
			iCount = 0;
			//�A�_�v�^�ŗ��p�\�ȃf�B�X�v���C���[�h�̐����擾
			iCount = m_pDirect3D->GetAdapterModeCount
				(
					D3DADAPTER_DEFAULT,		//�A�_�v�^�ŗ��p�\�ȃf�B�X�v���C���[�h�̐����擾
					AdapterFormat[j]		//�f�B�X�v���C�t�H�[�}�b�g(16bit)
					);

			//�A�_�v�^�̃f�B�X�v���C���[�h���
			for (i = 0; i < iCount; i++)
			{
				//�Љ��f�B�X�v���C�A�_�v�^�������������w��
				if (m_pDirect3D->EnumAdapterModes(
					0,					//�Ɖ��f�B�X�v���C�A�_�v�^�������������w��(DEFAULT���w��)
					AdapterFormat[j],	//�f�B�X�v���C�t�H�[�}�b�g
					i,					//�񋓂��郂�[�h������������
					&Mode				//D3DDISPLAYMODE�\���̂ւ̃|�C���^
					) == D3D_OK)
				{
					if (Mode.Format == FullScreenMode.Format)
					{
						//�擾�����f�B�X�v���C���[�h�ƃt���X�N���[���̃f�B�X�v���C�̏����r���Đ�Βl�Œl���擾
						int l = abs((int)(Mode.Width - FullScreenMode.Width)) + abs((int)(Mode.Height - FullScreenMode.Height)) + abs((int)(Mode.RefreshRate - FullScreenMode.RefreshRate));

						//iLevel�ȉ��Ȃ�
						if (l < iLevel) 
						{
							//���܂Ŏ擾���Ă����f�B�X�v���C�����R�s�[
							memcpy(&ResultMode, &Mode, sizeof(D3DDISPLAYMODE));
							//iLevel��u������
							iLevel = l;
						}
					}
				}
			}
		}
	}

	return ResultMode;
}

//====================================
//�֐���	: CreateDevice
//����		: hWnd  : �E�B���h�E�n���h�� 
//			  d3dpp : �v���[���g�p�����[�^�[ 
//�߂�l	: S_OK : �o�^�ɐ��� E_FAIL : �o�^�Ɏ��s
//���e		: �`��f�o�C�X�̍쐬
//====================================
HRESULT CDXDevice::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	UNREFERENCED_PARAMETER(d3dpp);

	// �f�o�C�X�̍쐬 �`��:HAL & 3D�v�Z:HARDWARE �Ŏ��s
	if (FAILED(m_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
		D3DDEVTYPE_HAL,							// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
		hWnd,									// �E�C���h�E�n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �R�c�v�Z����(�n�[�h�E�F�A�Ŏ��{���w��)
		&m_d3dpp,								// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
		&m_pDevice9								// �`��f�o�C�X�i�[�A�h���X�w��
		))) 
	{							
		// ���s�����̂� �`��:HAL & 3D�v�Z:SOFTWARE �Ŏ��s											
		if (FAILED(m_pDirect3D->CreateDevice(
			D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
			D3DDEVTYPE_HAL,						// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
			hWnd,								// �E�C���h�E�n���h��
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,// �R�c�v�Z����(�\�t�g�E�F�A�Ŏ��{���w��)
			&m_d3dpp,							// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
			&m_pDevice9							// �`��f�o�C�X�i�[�A�h���X�w��
			)))
		{					
			MessageBox(hWnd, TEXT("Direct3D�̏������Ɏ��s���܂����BTrue Color(24�ޯ�)����High Color(16�ޯ�/32�ޯ�)�ɕύX���āA���s���Ă��������B"), TEXT("�m�F"), MB_OK | MB_ICONSTOP);
			//LPDIRECT3D9�̉��
			SAFE_RELEASE(m_pDirect3D);
			return E_FAIL;
		}
	}

	//�w�肳�ꂽ�f�o�C�X�Ɋւ���f�o�C�X�ŗL�̏����擾����@
	m_pDevice9->GetDeviceCaps(&m_d3dCaps);

	//���X�P�ʂ̃C���f�b�N�X���g���ăC���f�b�N�X�t���ł���ő�̍s��C���f�b�N�X������DWORD�l>1
	if (m_d3dCaps.MaxVertexBlendMatrixIndex > 1)
	{
		m_byAnimMethod = D3DINDEXED;
	}
	else
	{
		//�����̍s��ɂ�钸�_�u�����f�B���O�����s����ۂɃf�o�C�X���K�p�ł���s��̍ő吔>1
		if (m_d3dCaps.MaxVertexBlendMatrices > 1)
			m_byAnimMethod = D3DNONINDEXED;
		else 
		{
			//�A�j���[�V�������\�b�h�����_�ł��C���f�b�N�X�ł��Ȃ��ꍇ
			m_byAnimMethod = SOFTWARE;
			SAFE_RELEASE(m_pDevice9);

			// �`��:HAL & 3D�v�Z:SOFTWARE �Ŏ��s
			if (FAILED(m_pDirect3D->CreateDevice(
				D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
				D3DDEVTYPE_HAL,						// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
				hWnd,								// �E�C���h�E�n���h��
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,// �R�c�v�Z����(�\�t�g�E�F�A�Ŏ��{���w��)
				&m_d3dpp,							// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
				&m_pDevice9							// �`��f�o�C�X�i�[�A�h���X�w��
				)))
			{						
				MessageBox(hWnd, TEXT("Direct3D�̏������Ɏ��s���܂����B"), TEXT("�m�F"), MB_OK | MB_ICONSTOP);
				//LPDIRECT3D�̉��
				SAFE_RELEASE(m_pDirect3D);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}



//====================================
//�֐���	:CleanupResource
//����		:
//�߂�l	:
//���e		:���\�[�X�폜
//====================================
void CDXDevice::CleanupResource()
{
	//�I�u�W�F�N�g�̉��
	CleanupObject();
	if (m_pFont != NULL)
	{
		SAFE_DELETE(m_pFont);
	}
	if (m_pScene != NULL)
	{
		SAFE_DELETE(m_pScene);
	}
	if (m_pDInput != NULL)
	{
		SAFE_DELETE(m_pDInput);
	}


}

//====================================
//�֐���	:CreateObject
//����		:
//�߂�l	:TRUE:���� FALSE:��肠��
//���e		:�I�u�W�F�N�g�̍쐬 
//			 �f�o�C�X�����ɉe������郊�\�[�X
//====================================
BOOL CDXDevice::CreateObject()
{
	//�t�H���g�����邩�`�F�b�N
	if (m_pFont)
	{
		m_pFont->getDXFont()->OnResetDevice();
	}
	//�V�[�������邩�`�F�b�N
	if (m_pScene) 
	{
		m_pScene->CreateObject();
	}
	
	return TRUE;
}

//====================================
//�֐���	:CleanupObject
//����		:
//�߂�l	:
//���e		:�I�u�W�F�N�g���
//====================================
void CDXDevice::CleanupObject()
{
	//�t�H���g�����邩�`�F�b�N
	if (m_pFont)
	{
		m_pFont->getDXFont()->OnLostDevice();
	}
	//�V�[���̃`�F�b�N
	if (m_pScene) 
	{
		m_pScene->CleanupObject();
	}

	
}



//====================================
//�֐���	:BeginScene
//����		:
//�߂�l	:TRUE:���� FALSE:��蔭��
//���e		:�����_�����O�����J�n
//====================================
BOOL CDXDevice::BeginScene()
{
	//�V�[���̊J�n
	if (FAILED(m_pDevice9->BeginScene()))
	{
		return FALSE;
	}

	return TRUE;
}

//====================================
//�֐���	:EndScene
//����		:
//�߂�l	:
//���e		:�����_�����O�����I��
//====================================
HRESULT CDXDevice::EndScene()
{
	m_pDevice9->EndScene();

	return TRUE;
}


//====================================
//�֐���	:RestoreDevice
//����		:
//�߂�l	:TRUE:�������� FALSE:�������s
//���e		:�f�o�C�X�̕���
//====================================
BOOL CDXDevice::RestoreDevice()
{

	HRESULT hr;
	
	//�ǂ��炩������Ȃ����
	if (!m_pDirect3D || !m_pDevice9)
	{
		//���s
		return FALSE;
	}

	//���������f�o�C�X�̕�������
	//�f�o�C�X���������Ă�����
	if (m_bDeviceLost)
	{
		//=====================================================================
		//�f�o�C�X�̏�Ԃ��`�F�b�N(�������x���X�e�[�^�X��Ԃ�)
		//�߂�l :����:D3D_OK
		//		  ���s:D3DERR_DEVICELOST �f�o�C�X���������Ă��āA���݃��Z�b�g�ł��Ȃ�(�����_�����O�s��)
		//			   D3DERR_DEVICENOTRESET �f�o�C�X�͏������Ă��邪�A���݃��Z�b�g�ł���
		//=====================================================================
		hr = m_pDevice9->TestCooperativeLevel();

		if (FAILED(hr))
		{
			//D3DERR_DEVICELOST�̏ꍇ
			if (hr == D3DERR_DEVICELOST)
			{
				//s�������Ă��ă��Z�b�g�ł��Ȃ�
				return FALSE;
			}
			//���Z�b�g�ł��Ȃ���ԂȂ�
			if (hr != D3DERR_DEVICENOTRESET)
			{
				return FALSE;
			}

			//�I�u�W�F�N�g�̃N���[���A�b�v���s��r�Ă��Ȃ����
			if (!m_bCleanupObject)
			{
				//�I�u�W�F�N�g�̉��
				CleanupObject();
				//�N���[���A�b�v����
				m_bCleanupObject = TRUE;
			}
			//�f�o�C�X�̕������\�Ȃ̂ŕ��������݂�
			hr = m_pDevice9->Reset(&m_d3dpp);
			
			//����ł��������s������
			if (FAILED(hr))
			{
				//�f�o�C�X�͏������Ă��邪�����ł��Ȃ����
				if (hr == D3DERR_DEVICELOST)
				{
					return FALSE;
				}
			}
			//�I�u�W�F�N�g�̍Đ���
			CreateObject();
			//�N���[���A�b�v���Ă��Ȃ���Ԃɂ���
			m_bCleanupObject = FALSE;
		}
	}

	return TRUE;
}

//====================================
//�֐���	:ChangeDisplaymode
//����		:
//�߂�l	:
//���e		:�\�����[�h�̐؂�ւ�
//====================================
void CDXDevice::ChangeDisplaymode()
{
	//�I�u�W�F�N�g�̉��
	CleanupObject();

	// �v���[���e�[�V�����p�����[�^�ݒ�

	// ���������[���N���A
	ZeroMemory(
		&m_d3dpp,												//�i�[����v���[���e�[�V�����p�����[�^�̃|�C���^
		sizeof(D3DPRESENT_PARAMETERS
			));							

		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				// �t���b�s���O���@:IDirect3DDevice9::Present�������o�b�N�o�b�t�@�̓��e�Ɉ�؉e�����Ȃ�
		m_d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�����Q�ɐݒ�
		m_d3dpp.EnableAutoDepthStencil = TRUE;					// �[�x�o�b�t�@���Ǘ���L��
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			// 8�r�b�g�̃X�e���V���`�����l�����w��
		m_d3dpp.hDeviceWindow = m_hWnd;							// �E�B���h�E�n���h��

	// �\�����[�h�̐؂�ւ�
	// �E�B���h�E���[�h�̏ꍇ
	if (m_bWindowed) 
	{
		// �E�B���h�E���[�h�ɐݒ�
		m_d3dpp.Windowed = TRUE;					
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		// �o�b�N�o�b�t�@�̕�
		m_d3dpp.BackBufferWidth = rcClient.right - rcClient.left;
		// �o�b�N�o�b�t�@�̍���
		m_d3dpp.BackBufferHeight = rcClient.bottom - rcClient.top;	

		if (m_d3ddm.Format == D3DFMT_X8R8G8B8)
		{
			// �F��:ARGB8bit
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		}
		else if (m_d3ddm.Format == D3DFMT_R5G6B5)
		{
			// �F��:A1bitRGB5bit
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	
		}
		else
		{
			// �F��:GetAdapterDisplayMode�Ŏ擾����Format���w��
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;
		}

		m_d3dpp.FullScreen_RefreshRateInHz = 0;		// ���t���b�V�����[�g���w��
	}
	else 
	{											// �t���X�N���[�����[�h�̏ꍇ
		m_d3dpp.Windowed = FALSE;						// �t���X�N���[�����[�h�ɐݒ�
		m_d3dpp.BackBufferWidth = m_Mode.Width;			// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.BackBufferHeight = m_Mode.Height;		// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// ���t���b�V�����[�g���w��
		m_d3dpp.BackBufferFormat = m_Mode.Format;		// �F�����w��
	}

	m_pDevice9->Reset(&m_d3dpp);				// ���Z�b�g
	m_bDeviceLost = TRUE;
	PostMessage(m_hWnd, UM_SETCURSOR, 0L, 0L);		// �J�[�\���ݒ�
	CreateObject();							// �I�u�W�F�N�g�č쐬

}

//====================================
//�֐���	:Render
//����		:
//�߂�l	:�]������
//���e		:�����_�����O����
//====================================
HRESULT CDXDevice::Render()
{
	//�V�[���̊J�n
	if (SUCCEEDED(BeginScene()))
	{
		//�r���[�|�[�g�܂��̓r���[�|�[�g���̋�`�Z�b�g���w�肵�� RGBA�F�ɃN���A���A�[�x�o�b�t�@���N���A���A�X�e���V���o�b�t�@���폜
		m_pDevice9->Clear(
			0,															//���������`���w��(�o�b�N�o�b�t�@�S�̏��������w��)
			NULL,														//��`�̍��W�w��(�摜�S�̏��������w��)
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//�����_�����O�^�[�Q�b�g/�[�x�o�b�t�@/�X�e���V���o�b�t�@�̃N���A
			D3DCOLOR_XRGB(0x00,0x00,0x00),									//�����_�����O�^�[�Q�b�g�T�[�t�F�X��h��Ԃ�32bitRGBA�J���[�l���w��(���F)
			1.0f,														//���̃��\�b�h���[�x�o�b�t�@�ɕۑ�����V����Z�l
			0															//�e�X�e���V���o�b�t�@�̃G���g���ɕۑ�����l
			);

		//�Q�[���̏����̎��s
		m_pDInput->GetJoyStickState(&m_JoyState);
		m_pScene->SceneChange();


		//�V�[���̏I��
		EndScene();
	}
	//�f�o�C�X�����L����o�b�N�o�b�t�@�̃V�[�P���X�̒��Ŏ��̃R���e���c��񎦂���
	//�o�b�N�o�b�t�@�̓��e���E�B���h�E�ɓ]��
	return m_pDevice9->Present(
		NULL,	//�o�b�N�o�b�t�@��`�̈���w��(�o�b�N�o�b�t�@�S�̂��t�����g�o�b�t�@�ɓ]���Ȃ̂�NULL)
		NULL,	//�t�����g�o�b�t�@��`�̈���w��(�o�b�N�o�b�t�@�S�̂��t�����g�o�b�t�@�ɓ]���Ŏw��Ȃ̂�NULL)
		NULL,	//�E�B���h�E�n���h��(�E�B���h�E��1��ʂ����g���Ă��Ȃ��̂�NULL���w��)
		NULL	//DirectX9�ł͗p�ӂ���Ă��Ȃ��̂�NULL
		);
}

//////////////////////////////////////
//
//�Z�b�g�֐�
//
//////////////////////////////////////

//====================================
//�֐���	:setDeviceLost
//����		:bDeviceLost : �f�o�C�X�����̗L��
//�߂�l	:
//���e		:
//====================================
void CDXDevice::setDeviceLost(BOOL bDeviceLost)
{
	m_bDeviceLost = bDeviceLost;
}

//====================================
//�֐���	:setWindowed
//����		:bWindowed :�E�B���h�E���[�h/�t���X�N���[�����[�h
//�߂�l	:
//���e		:�E�B���h�E���[�h�̎w��
//====================================
void CDXDevice::setWindowed(BOOL bWindowed)
{
	m_bWindowed = bWindowed;
}

//====================================
//�֐���	:setChangeDisplayMode
//����		:bChangeDisplayMode : �f�B�X�v���C���[�h�ύX���s�̗L��
//�߂�l	:
//���e		:�f�B�X�v���C���[�h�ύX���s�̗L��
//====================================
void CDXDevice::setChangeDisplayMode(BOOL bChangeDisplayMode)
{
	m_bChangeDisplayMode = bChangeDisplayMode;
}


/////////////////////////////////////////
//
//�Q�b�g�֐�
//
/////////////////////////////////////////

//====================================
//�֐���	:getDeviceLost
//����		:
//�߂�l	:�f�o�C�X�f�o�C�X�����̗L��
//���e		:�f�o�C�X�����̎擾
//====================================
BOOL CDXDevice::getDeviceLost()
{
	return m_bDeviceLost;
}

//====================================
//�֐���	:getDirect3D
//����		:
//�߂�l	:Direct3D�V�X�e��
//���e		:Direct3D�V�X�e���̎擾
//====================================
LPDIRECT3D9 CDXDevice::getDirect3D()
{
	return m_pDirect3D;
}

//====================================
//�֐���	:getDevice9
//����		:
//�߂�l	:�`��f�o�C�X
//���e		:�`��f�o�C�X�̎擾
//====================================
LPDIRECT3DDEVICE9 CDXDevice::getDevice9()
{
	return m_pDevice9;
}

//====================================
//�֐���	:getDInput
//����		:
//�߂�l	:m_pDInput
//���e		:DirectInput
//====================================
CDInput * CDXDevice::getDInput()
{
	return m_pDInput;
}

//====================================
//�֐���	:getWindowed
//����		:
//�߂�l	:�E�B���h�E���[�h/�t���X�N���[�����[�h
//���e		:�E�B���h�E���[�h�̎擾
//====================================
BOOL CDXDevice::getWindowed()
{
	return m_bWindowed;
}

//====================================
//�֐���	:getChangeDisplayMode
//����		:
//�߂�l	:�f�B�X�v���C���[�h�ύX���s�̗L��
//���e		:�f�B�X�v���C���[�h�ύX�̎擾
//====================================
BOOL CDXDevice::getChangeDisplayMode()
{
	return m_bChangeDisplayMode;
}

//====================================
//�֐���	:getKeyStateArray
//����		:
//�߂�l	:
//���e		:�L�[���͏��i�[�z��̃A�h���X
//====================================
LPBYTE CDXDevice::getKeyStateArray()
{
	return m_byKeyState;
}
