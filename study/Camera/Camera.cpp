//=====Include File=====//
#include "Camera.h"

//�B��̃C���X�^���X�̎���
CCamera* CCamera::m_pInstance = NULL;

//====================================
//�֐���	:CCamera
//����		:pDevice9:�`��f�o�C�X
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice9)
{
/*	//�J�����̃A���O��
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�J�������猴�_�܂ł̋���
	m_fDistance = 6.0f;
	//�`��f�o�C�X�̊i�[
	m_pDevice9 = pDevice9;

	//�s��̏�����
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_Screen);

	m_vEyePt2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�����_�̐ݒ�
	m_vLookatPt = m_vEyePt2;

	//�J�����̊�ʒu
	m_vViewPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//���K��
	D3DXVec3Normalize(&m_vDirectionalLight, &m_vDirectionalLight);
	//�������̏ꏊ
	m_vLightPosition = m_vDirectionalLight*-20.0f;*/
	m_pDevice9 = pDevice9;
	m_fDistance = 10.0f;									// �{��(�J�������猴�_�܂ł̋���) 
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J�����̃A���O��
	m_vDirectionalLight = D3DXVECTOR3(1.0f, -2.0f, 1.0f);// �������̕���
	vViewBasePosition=D3DXVECTOR3(0.0f, 0.85f, -4.5f);
	D3DXVec3Normalize(&m_vDirectionalLight, &m_vDirectionalLight);

}

//====================================
//�֐���	:~CCamera
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CCamera::~CCamera()
{
}

//====================================
//�֐���	:Create
//����		:pDevice9:	�`��f�o�C�X 
//			 hWnd:		�E�B���h�E�n���h��
//�߂�l	:
//���e		:�J�����쐬�֐�
//====================================
void CCamera::Create(LPDIRECT3DDEVICE9 pDevice9, HWND hWnd)
{
	//�����J�����̃C���X�^���X�����݂��Ȃ����
	if (!m_pInstance)
	{
		//�J�����̃C���X�^���X�̐���
		m_pInstance = new CCamera(pDevice9);
	}

	//�X�N���[���s��쐬
	RECT rcWindow;
	::GetWindowRect(hWnd, (LPRECT)&rcWindow);

	FLOAT ClientWidth = (FLOAT)(rcWindow.right - rcWindow.left)*0.5f;
	FLOAT ClientHeight = (FLOAT)(rcWindow.bottom - rcWindow.top) * 0.5f;
	//�X�N���[���s��ݒ�
	//ClientWidth	,	0			,0	,0
	//0				,ClientHeight	,0	,0
	//0				,0				,0	,0
	//ClientWidth	,ClientHeight	,0	,0
	m_pInstance->m_Screen._11 = ClientWidth;
	m_pInstance->m_Screen._22 = -ClientHeight;
	m_pInstance->m_Screen._41 = ClientWidth;
	m_pInstance->m_Screen._42 = ClientHeight;


}

//====================================
//�֐���	:Destroy
//����		:
//�߂�l	:
//���e		:�C���X�^���X�̍폜
//====================================
void CCamera::Destroy()
{
	SAFE_DELETE(m_pInstance);
}


//====================================
//�֐���	:Destroy
//����		:
//�߂�l	:
//���e		:�C���X�^���X�̍폜
//====================================
void CCamera::SetCamera(D3DXVECTOR3 pos)
{
	if (!m_pDevice9)
	{
		return;
	}



	// �`�揈�����L�q����
	D3DXMATRIX matView;							// ���_�s��
	D3DXMATRIX matPers;							// ���e�ϊ��s��
	
	D3DXMATRIX matRotation;						// �I�u�W�F�N�g�s��̎���(��])
	D3DXVECTOR3 vViewUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vViewPosition = D3DXVECTOR3(0.0f, 0.0f, -m_fDistance);
	// 3D�x�N�g���̃X�P�[�����O
	D3DXVec3Scale(
		&m_vViewPosition,	// ���Z���ʂł���D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		&vViewBasePosition,	// �����̊�ɂȂ�D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		m_fDistance);		// �X�P�[�����O�l���w��

							// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// ���Z���ʂł���D3DXMATRIX�\���̂ւ̃|�C���^���w��
		m_vViewAngle.y,	// y���̃��[(���W�A���P��)���w��
		m_vViewAngle.x,	// x���̃s�b�`(���W�A���P��)���w��
		m_vViewAngle.z);// z���̃��[��(���W�A���P��)���w��

	D3DXVec3TransformCoord(
		&m_vViewPosition,
		&m_vViewPosition,
		&matRotation);

	// 3D�x�N�g�����s��ŕϊ�
	//D3DXVec3TransformCoord(
	//	&m_vViewPosition,		// �ϊ���̃x�N�g���ւ̃A�h���X���w��
	//	&m_vViewPosition,		// �ϊ��O�̃x�N�g���ւ̃A�h���X���w��
	//	&matRotation);			// �����̊�ɂȂ�s��ւ̃A�h���X���w��
	//							// 3D�x�N�g�����s��ŕϊ�
	//D3DXVec3TransformCoord(
	//	&vViewUp,		// �ϊ���̃x�N�g���ւ̃A�h���X���w��
	//	&vViewUp,		// �ϊ��O�̃x�N�g���ւ̃A�h���X���w��
	//	&matRotation);	// �����̊�ɂȂ�s��ւ̃A�h���X���w��

	m_vViewPosition += pos;
	m_vLookatPt = pos;
						// ������W�n�r���[�s����쐬
	D3DXMatrixLookAtLH(
		&matView,						// ���Z���ʂł���D3DXMATRIX�\���̂ւ̃|�C���^���w��
		(D3DXVECTOR3*)&m_vViewPosition,	// ���_���`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		&m_vLookatPt,	// �J�����̒����Ώۂ��`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		&vViewUp);						// �J�����g���[���h�̏���A��ʂɂ�[0, 1, 0]���`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��

	m_View = matView;

	D3DVIEWPORT9 vp;
	m_pDevice9->GetViewport(&vp);
	// ����(FOV)�Ɋ�Â��āA������W�n�p�[�X�y�N�e�B�u�ˉe�s����쐬
	D3DXMatrixPerspectiveFovLH(
		&matPers,			// ���Z���ʂł���D3DXMATRIX�\���̂ւ̃|�C���^���w��
		(FLOAT)D3DXToRadian(45.0), // ����p���w��(���W�A���P��)(45�x���w��) 
		(FLOAT)vp.Width / (FLOAT)vp.Height,// �A�X�y�N�g����w��
		1.0f,				// �߂��̃r���[�v���[����z�l���w��
		1000.0f);			// �����̃r���[�v���[����z�l���w��

	m_Proj = matPers;
}