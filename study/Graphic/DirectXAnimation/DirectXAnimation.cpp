#include "DirectXAnimation.h"

//====================================
//�֐���	:CDirectXAnimation
//����		:pDevice : �`��f�o�C�X
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CDirectXAnimation::CDirectXAnimation(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice9 = pDevice;
	ZeroMemory(&m_TrackDesc, sizeof(D3DXTRACK_DESC));
	LoadAnimXFile();
	
}

//====================================
//�֐���	:~CDirectXAnimation
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CDirectXAnimation::~CDirectXAnimation()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pDevice9);
}

//====================================
//�֐���	:LoadAnimXFile
//����		:pDevice:�`��f�o�C�X
//�߂�l	:
//���e		:�A�j���[�V����XFile�̃��[�h
//====================================
void CDirectXAnimation::LoadAnimXFile()
{
	CModelManager* pModeleManager = CModelManager::GetInstance();
	//�����̐ݒ�
	SetPos(D3DXVECTOR3(0,0,-10));
	SetRot(D3DXVECTOR3(0, 0, 0));
	SetScale(D3DXVECTOR3(0.05f,0.05f,0.05f));

	//�C���X�^���X�̎擾
	CCamera* pCamera = CCamera::GetInstance();
	m_CameraPos = D3DXVECTOR3(m_PlayerPos.x, m_PlayerPos.y + 3.0f, m_PlayerPos.z - 10);
	pCamera->View(m_CameraPos, D3DXVECTOR3(m_PlayerRot.x, m_PlayerRot.y + D3DX_PI, m_PlayerRot.z));

	//�A�j���[�V�����̃t�@�C���̏������֐�
	///��������Manager���g���A�j���[�V�����t�@�C�������
	m_SkinMesh.InitThing(m_pDevice9,pModeleManager->GetXFileName(1));

	LPD3DXBUFFER pErrorMsg;
	//======�V�F�[�_�[�̍쐬=====//
	HRESULT hr = D3DXCreateEffectFromFile(
		m_pDevice9,
		"Effect\\xfile2.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErrorMsg
		);

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsg->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsg);
		return;
	}
}

//====================================
//�֐���	:FreeAnim
//����		:pFrame:Frame�\����
//�߂�l	:
//���e		:�Z��E�q�t���[���̉��
//====================================
void CDirectXAnimation::FreeAnim(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		FreeAnim(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		FreeAnim(pFrame->pFrameFirstChild);
	}
}

//====================================
//�֐���	:Render
//����		:
//�߂�l	:
//���e		:�`��
//====================================
int a = 0;
void CDirectXAnimation::Render()
{
	//�C���X�^���X�̎擾
	CCamera* pCamera = CCamera::GetInstance();
	//m_CameraPos = D3DXVECTOR3((m_PlayerPos.x +10.0f)*sinf(m_PlayerRot.y), m_PlayerPos.y + 3, (m_PlayerPos.z +10.0f)*cosf(m_PlayerRot.y));
	 m_CameraPos = D3DXVECTOR3(0 + 50.0f*cosf(m_PlayerRot.y), -2, 0 + 50 * sinf(m_PlayerRot.y));
	pCamera->View(m_CameraPos,D3DXVECTOR3(m_PlayerRot.x,m_PlayerRot.y,m_PlayerRot.z));

	//�RD�t�@�C���̍��W���`����
	D3DXMATRIX *m_World = new D3DXMATRIX();
	D3DXMATRIX *m_temp = new D3DXMATRIX();
	//�s��̏�����
	D3DXMatrixIdentity(m_World);
	D3DXMatrixIdentity(m_temp);

	//=====�ȉ��RD�̍��W�E�g��E�ړ��̌v�Z=====//
	//�g��̏���
	D3DXMatrixScaling(m_temp, m_PlayerScale.x, m_PlayerScale.y, m_PlayerScale.z);
	*m_World *= *m_temp;
	//���f���̍��W�ϊ�(���[�A�s�b�`�A���[�����w�肵�čs����쐬)
	D3DXMatrixRotationYawPitchRoll(m_temp, m_PlayerRot.y, m_PlayerRot.x, m_PlayerRot.z);
	*m_World *= *m_temp;
	//���f���ړ��̏���
	D3DXMatrixTranslation(m_temp, m_PlayerPos.x, m_PlayerPos.y, m_PlayerPos.z);
	//�S�Ă̍s��̉��Z���ʊi�[
	*m_World *= *m_temp;
	m_SkinMesh.Update(*m_World);
	m_SkinMesh.Draw(m_pDevice9);
	
	SAFE_DELETE(m_World);
	SAFE_DELETE(m_temp);
}


//====================================
//�֐���	:AnimationChange
//����		:Num : �A�j���[�V�����ԍ�
//�߂�l	:
//���e		:�A�j���[�V�����̐؂�ւ�
//====================================
void CDirectXAnimation::AnimationChange(DWORD Num)
{
	//�A�j���[�V�����̂��肩��
	m_SkinMesh.ChangeAnim(Num);
}

//====================================
//�֐���	:SetPos
//����		:pos:���W
//�߂�l	:
//���e		:���W�̃Z�b�g
//====================================
void CDirectXAnimation::SetPos(D3DXVECTOR3 pos)
{
	m_PlayerPos = pos;
}

//====================================
//�֐���	:SetRot
//����		:rot:��]
//�߂�l	:
//���e		:��]�̃Z�b�g
//====================================
void CDirectXAnimation::SetRot(D3DXVECTOR3 rot)
{
	m_PlayerRot = rot;
}

//====================================
//�֐���	:SetScale
//����		:Scale:�g��
//�߂�l	:
//���e		:�g�嗦�̑}��
//====================================
void CDirectXAnimation::SetScale(D3DXVECTOR3 Scale)
{
	m_PlayerScale = Scale;
}
