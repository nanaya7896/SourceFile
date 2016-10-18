#include "Application.h"
#include"../resource.h"
//�C���X�^���X�̎���
CApplication CApplication::m_pAppInstance;
//====================================
//�֐���	:getInstance
//����		:
//�߂�l	:�A�v���P�[�V�����̃C���X�^���X
//
//====================================
CApplication * CApplication::getInstance()
{
	return &m_pAppInstance;
}

//====================================
//�֐���	:CApplication
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CApplication::CApplication()
{
	m_ptWindowShowPosition.x = -1;
}

//====================================
//�֐���	:~CApplication
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CApplication::~CApplication()
{

}

//=====================================================================
//�֐���:initialize
//���� : hInstance		�C���X�^���X�n���h��
//		 WindowTitle	�E�B���h�E��
//		 WindowSize		�E�B���h�E�T�C�Y
//		 bWindowed		TRUE:�E�B���h�E���[�h FALSE:�t���X�N���[�����[�h
//�߂�l :�E�B���h�E�n���h��  
//=====================================================================
HWND CApplication::Initialize(HINSTANCE hInstance, LPCTSTR Windowtitle, SIZE WindowSize, BOOL bWindowed)
{
	//�E�B���h�E�̏�����
	HWND hWnd=InitWindow(hInstance,Windowtitle,WindowSize,bWindowed);
	if (hWnd == NULL)
	{
		return NULL;
	}

	//�Q�[���f�o�C�X������������
	m_pDXDevice = new CDXDevice();
	HRESULT hResult = m_pDXDevice->Initialize(hWnd, bWindowed);
	if (FAILED(hResult))
	{
		delete m_pDXDevice;
		return NULL;
	}

	//�^�C�}�[�̕���\�x��1ms�ɐݒ肷��
	timeBeginPeriod(1);

	//�E�B���h�E�̕\��
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	return hWnd;
}

//====================================
//�֐���	:Run
//����		:hWnd:�E�B���h�E�n���h��
//�߂�l	:�A�v���P�[�V�����I���l
//���e		:�A�v���P�[�V�����̎��s
//====================================
int CApplication::Run(HWND hWnd)
{
	//�Q�[���X�s�[�h�Ǘ��X���b�h�쐬/���s
	CTimeThread* pTimeThread = new CTimeThread();
	pTimeThread->Start(m_pDXDevice);


	//���b�Z�[�W���[�v
	MSG msg;
	do {
	//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}

		if (pTimeThread->getTimeThreadSleep())
		{
			if (m_pDXDevice->getDeviceLost())
			{
				if (m_pDXDevice->getDevice9()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				{
					if (pTimeThread->m_pDXDevice->RestoreDevice())
					{
							//�f�o�C�X����������
						pTimeThread->m_pDXDevice->setDeviceLost(FALSE);
						m_pDXDevice->setDeviceLost(TRUE);
					}
				}
			}
			else
			{
				//�E�B���h�E���t���X�N���[�����擾
				BOOL bWindowed = m_pDXDevice->getWindowed();
				bWindowed ^= TRUE;
				//�Z�b�g����
				m_pDXDevice->setWindowed(bWindowed);
				//�\�����[�h�ؑ�
				m_pDXDevice->ChangeDisplaymode();
				
				//�E�B���h�E�X�^�C���̎w��
				DWORD WindowStyle;
				if (bWindowed)
				{
					//�E�B���h�E�X�^�C�����w��
					WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX;
				}
				else
				{
					//�E�B���h�E�X�^�C�����|�b�v�A�b�v�Ɏw��
					WindowStyle = WS_POPUP | WS_VISIBLE;
				}

				::SetWindowLong(hWnd, GWL_STYLE, WindowStyle);
				//�E�B���h�E�X�^�C���̕ύX

				//�E�B���h�E�̔z�u
				if (bWindowed)
				{
					//�E�B���h�E���[�h���̏���
					if (m_ptWindowShowPosition.x == -1)
					{
						//�N���C�A���g�T�C�Y�̕ύX
						setClientSize(hWnd, m_WindowSize);
						//�E�B���h�E�̕\���ʒu����ʂ̒��S�ɕύX
						WindowMoveToCenter(hWnd);
					}
					::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,m_WindowSize.cx,m_WindowSize.cy,SWP_SHOWWINDOW);
				}
			}
			pTimeThread->setTimeThreadSleep(FALSE);
		}
		Sleep(1);
	}while(msg.message !=WM_QUIT);

	//_CrtDumpMemoryLeaks();
	//�Q�[���X�s�[�h�Ǘ��X���b�h�I��
	delete pTimeThread;

	//���\�[�X�J��
	m_pAppInstance.m_pDXDevice->CleanupResource();

	//�Q�[���X���b�h�I��
	delete m_pDXDevice;

	// �^�C�}�[�̕���x��1ms�ɐݒ肷��
	timeEndPeriod(1);

	return (int)msg.wParam;
}


//====================================
//�֐���	:WndProc
//����		:
//�߂�l	:
//���e		:�E�B���h�E�v���V�[�W��
//====================================
LRESULT CApplication::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//���b�Z�[�W�������Ă����ꍇ
	switch (msg)
	{
		//�E�B���h�E�����Ƃ�
	case WM_CLOSE:
	case WM_DESTROY:

		//�E�B���h�E�I���ʒm
		PostQuitMessage(0);
		break;

	}
	//
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//====================================
//�֐���	:InitWindow
//����		:hInstance :�C���X�^���X�n���h��
//			 WindowTitle:�^�C�g����
//			 WindowSize:�E�B���h�E�T�C�Y
//			 bWindowed:TRUE:�E�B���h�E���[�h�AFALSE:�t���X�N���[�����[�h
//�߂�l	:�E�B���h�E�n���h��
//���e		:�E�B���h�E�̏�����
//====================================
HWND CApplication::InitWindow(HINSTANCE hInstance, LPCTSTR WindowTitle, SIZE WindowSize, BOOL bWindowed)
{
	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex;	// �E�B���h�E�N���X�̏��i�[�p�\���̕ϐ�
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);					// �\���̂̃T�C�Y��ݒ�
	wcex.style = CS_HREDRAW | CS_VREDRAW;				// �E�B���h�E�X�^�C��(��/�����ύX����)
	wcex.lpfnWndProc = (WNDPROC)WndProc;						// �E�B���h�E�v���V�[�W���̎w��
	wcex.cbClsExtra = 0;									// �ǉ��̈�͎g��Ȃ�
	wcex.cbWndExtra = 0;									// �ǉ��̈�͎g��Ȃ�
	wcex.hInstance = hInstance;							// ���̃C���X�^���X�̃n���h��
														//	wcex.hIcon			= LoadIcon(hInstance,IDI_APPLICATION);	// ���[�W�A�C�R��
	wcex.hIcon = LoadIcon(hInstance, (LPCSTR)IDI_ICON3);	// ���[�W�A�C�R��
	//wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// �J�[�\���X�^�C��
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// �w�i�F ���F
	wcex.lpszMenuName = NULL;									// �\�����j���[�̃Z���N�g(���j���[�Ȃ�)
	wcex.lpszClassName = TEXT("MainWindowClass");				// �E�B���h�E�N���X��
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON3);// �X���[���A�C�R��

	::RegisterClassEx(&wcex);									// �E�B���h�E�N���X�̓o�^


	HWND hWnd = ::CreateWindow(
		"MainWindowClass",		// �N���X��
		WindowTitle,			// �^�C�g��
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	// �X�^�C��
		CW_USEDEFAULT,			// �����W(Windows�ɔC����)
		CW_USEDEFAULT,			// �����W(Windows�ɔC����)�����ō���p�̍��W
		WindowSize.cx,			// ��
		WindowSize.cy,			// ����
		HWND_DESKTOP,			// ����
		(HMENU)NULL,			// ���j���[����
		hInstance,				// ���̃v���O�����̃C���X�^���X�̃n���h��
		NULL					//�ǉ������Ȃ�
		);

	
	if (hWnd == NULL)
	{
		return NULL;
	}
	
	//�E�B���h�E���[�h�̏ꍇ
	if (bWindowed)
	{
		//�N���C�A���g�T�C�Y�̕ύX
		setClientSize(hWnd, WindowSize);
		//�E�B���h�E�\���ʒu����ʂ̒��S�ɕύX
		WindowMoveToCenter(hWnd);
	}
	else
	{
		DWORD WindowStyle;
		//�E�B���h�E�X�^�C�����|�b�v�A�b�v�Ɏw��
		WindowStyle = WS_POPUP | WS_VISIBLE;

		//�E�B���h�X�^�C���̕ύX
		::SetWindowLong(hWnd, GWL_STYLE, WindowStyle);

		memcpy(&m_WindowSize, &WindowSize, sizeof(SIZE));
	}

	return hWnd;
}

//====================================
//�֐���	:getClientSize
//����		:
//�߂�l	:
//���e		:�N���C�A���g�̈�̃T�C�Y�����߂�
//====================================
SIZE CApplication::getClientSize(HWND hWnd)
{
	//�N���C�A���g�̈�̋�`���擾
	RECT rcClient;
	::GetClientRect(hWnd,&rcClient);

	//�T�C�Y�����߂�
	SIZE result;
	result.cx = rcClient.right - rcClient.left;
	result.cy = rcClient.bottom - rcClient.top;

	return result;
}

//====================================
//�֐���	:getWindowSize
//����		:
//�߂�l	:
//���e		:�E�B���h�E�̃T�C�Y�����߂�
//====================================
SIZE CApplication::getWindowSize(HWND hWnd)
{
	//�E�B���h�E�̋�`���擾
	RECT rcWindow;
	::GetWindowRect(hWnd, &rcWindow);

	//�T�C�Y�����߂�
	SIZE result;
	result.cx = rcWindow.right - rcWindow.left;
	result.cy = rcWindow.bottom - rcWindow.top;

	return result;
}

//====================================
//�֐���	:setClientSize
//����		:
//�߂�l	:
//���e		:�N���C�A���g�̈�̃T�C�Y���Z�b�g����
//====================================
void CApplication::setClientSize(HWND hWnd, SIZE size)
{
	//�N���C�A���g�̈�T�C�Y�̎擾
	SIZE ClientSize = getClientSize(hWnd);
	//�N���C�A���g�̈�Ɛݒ肷��T�C�Y�̍����擾
	SIZE CompensationSize;
	CompensationSize.cx = size.cx - ClientSize.cx;
	CompensationSize.cy = size.cy - ClientSize.cy;

	//�E�B���h�E��`�̕��ƍ������擾
	SIZE WindowSize = getWindowSize(hWnd);

	//�E�B���h�E�̕��ƍ������w��
	m_WindowSize.cx = WindowSize.cx + CompensationSize.cx;
	m_WindowSize.cy = WindowSize.cy + CompensationSize.cy;

	//�T�C�Y���w��
	::SetWindowPos(hWnd, NULL, 0, 0, m_WindowSize.cx, m_WindowSize.cy, SWP_HIDEWINDOW);
}

//====================================
//�֐���	:WindowMoveToCenter
//����		:
//�߂�l	:
//���e		:�E�B���h�E�̈ʒu�𒆉��ɂ���
//====================================
void CApplication::WindowMoveToCenter(HWND hWnd)
{
	//�f�X�N�g�b�v�̋�`���擾
	RECT rcDesktop;
	//�E�B���h�E�̋�`�����擾
	::GetWindowRect(::GetDesktopWindow(), (LPRECT)&rcDesktop);
	RECT rcWindow;
	::GetWindowRect(hWnd, (LPRECT)&rcWindow);

	// �E�B���h�E�̒��S���W�����ߍŏI�I�Ɉʒu������
	m_ptWindowShowPosition.x = (rcDesktop.right - (rcWindow.right - rcWindow.left)) / 2;
	m_ptWindowShowPosition.y = (rcDesktop.bottom - (rcWindow.bottom - rcWindow.top)) / 2;
	// �|�W�V�������ړ�����
	::SetWindowPos(hWnd, HWND_TOP, m_ptWindowShowPosition.x, m_ptWindowShowPosition.y, (rcWindow.right - rcWindow.left), (rcWindow.bottom - rcWindow.top), SWP_HIDEWINDOW);
}


