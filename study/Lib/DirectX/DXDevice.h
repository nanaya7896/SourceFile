#pragma once

//=====IncludeFile=====//
#include"../../Global/Global.h"
#include"../../Scene/Scene.h"
//=====�N���X�錾=====//
class CDXDevice
{
	//private�����o�ϐ�
private:
	//Direct3D�V�X�e��
	LPDIRECT3D9				m_pDirect3D;
	
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;

	//�v���[���g�p�����[�^�[���
	D3DPRESENT_PARAMETERS	m_d3dpp;

	//�f�B�X�v���C���
	D3DDISPLAYMODE			m_d3ddm;

	//DX��
	D3DCAPS9				m_d3dCaps;

	//�f�o�C�X�����̗L��
	BOOL					m_bDeviceLost;

	//ObjectCleanup���s�L��
	BOOL					m_bCleanupObject;

	//�E�B���h�E/�t���X�N���[��
	BOOL					m_bWindowed;

	//�f�B�X�v���C���[�h�ύX���s�̗L��
	BOOL					m_bChangeDisplayMode;

	//�E�B���h�E�n���h��
	HWND					m_hWnd;

	//���[�h���
	D3DDISPLAYMODE			m_Mode;

	//�V�[���̏��
	CScene*					m_pScene;

	//�t�H���g
	CDXFont*				m_pFont;

	//DirectInput
	CDInput*				m_pDInput;

	//�L�[���͊i�[�p�z��
	BYTE					m_byKeyState[BUFFER_SIZE];

	//�W���C�X�e�B�b�N
	DIJOYSTATE2				m_JoyState;
	// �A�j���[�V������
	BYTE					m_byAnimMethod;			

	//public�����o�ϐ�
public:

	//private�����o�֐�
private:
	void		InitPresentParameters(HWND,BOOL);
	D3DDISPLAYMODE	CheckFullScreenAdapter(D3DDISPLAYMODE);
	HRESULT		CreateDevice(HWND,D3DPRESENT_PARAMETERS);
	BOOL		CreateResource();
	BOOL		CreateObject();
	void		CleanupObject();
	//public�����o�֐�
public:
	BOOL		BeginScene();
	HRESULT		EndScene();
	HRESULT		Initialize(HWND, BOOL);
	BOOL		RestoreDevice();
	//�R���X�g���N�^
	CDXDevice();
	//�f�X�g���N�^
	virtual		~CDXDevice();
	void		CleanupResource();
	void		ChangeDisplaymode();
	HRESULT		Render();

	//=====�Z�b�g�֐�=====//
	void		setDeviceLost(BOOL);
	void		setWindowed(BOOL);
	void		setChangeDisplayMode(BOOL);

	//=====�Q�b�g�֐�=====//
	BOOL		getDeviceLost();
	LPDIRECT3D9	getDirect3D();
	LPDIRECT3DDEVICE9	getDevice9();
	CDInput*			getDInput();
	BOOL		getWindowed();
	BOOL		getChangeDisplayMode();
	LPBYTE		getKeyStateArray();
	HWND		GetWindowHandle()
	{
		return m_hWnd;
	}
};