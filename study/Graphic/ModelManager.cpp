#include "ModelManager.h"

//�B��̃C���X�^���X����
CModelManager*	CModelManager::m_pModelInstance = NULL;


//====================================
//�֐���	:CXfileManager
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CModelManager::CModelManager(LPDIRECT3DDEVICE9 pDevice9)
{
	//�`��f�o�C�X�̃R�s�[
	m_pDevice9 = pDevice9;
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		m_pXFile[i] = NULL;
	}
}

//====================================
//�֐���	:~CXfileManager
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CModelManager::~CModelManager()
{

}

//====================================
//�֐���	:Create
//����		:pDevice:�`��f�o�C�X
//�߂�l	:
//���e		:Manager�̃C���X�^���X�����݂��Ȃ��Ȃ���
//====================================
void CModelManager::Create(LPDIRECT3DDEVICE9 pDevice9)
{
	//XFileManager�̃C���X�^���X�����݂��Ȃ��Ȃ�
	if (!m_pModelInstance)
	{
		m_pModelInstance = new CModelManager(pDevice9);
	}
}

//====================================
//�֐���	:Destory
//����		:
//�߂�l	:
//���e		:�C���X�^���X�̏���
//====================================
void CModelManager::Destory()
{
	SAFE_DELETE(m_pModelInstance);
}

//====================================
//�֐���	:Delete
//����		:
//�߂�l	:
//���e		:XFIle�̃f���[�g
//====================================
void CModelManager::Delete()
{
	//XFILE�̍폜
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		SAFE_DELETE(m_pXFile[i]);
	}
}

//====================================
//�֐���	:AllLoadXfile
//����		:
//�߂�l	:
//���e		:�S�Ă�XFile�̈ꊇ�ǂݍ���
//====================================
BOOL CModelManager::AllLoadXfile()
{
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		m_pXFile[i] = new CXfile(m_pDevice9);
	}

	//�S�Ă�XFile�̃��[�h
	 for (int i = 0; i < MAXFILE_NUM; i++)
	 {	 
		 if (FAILED(m_pXFile[i]->LoatXfile(XFileName[i])))
		 {
			 OutputDebugString("XFile Load Error");
			 return FALSE;
		 }
	 }
	return TRUE;
}

//====================================
//�֐���	:GetXFileName
//����		:num:���f���ԍ�
//�߂�l	:
//���e		:�ǂݍ���ł���XFILE�̖��O���擾����
//====================================
CHAR* CModelManager::GetXFileName(DWORD num)
{
	return XFileName[num];
}

//====================================
//�֐���	:Draw
//����		:	num		:XFile�`��ԍ�
//				pos		:�ʒu
//				rot		:��]
//				sacle	:�傫��
//				t_Num	:�e�N�j�b�N�ԍ�
//				alpha	:�����x
//�߂�l	:
//���e		:�w�肳�ꂽXFile�̕`��(�����x�̂ݕύX�\)
//====================================
void CModelManager::Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scale, float alpha)
{
	if (num == 7)
	{
		m_pXFile[num]->SetSlider(TRUE);
	}
	else if (num == 1)
	{
		m_pXFile[num]->Setpool(TRUE);
	}
	else {
		m_pXFile[num]->SetSlider(FALSE);
		m_pXFile[num]->Setpool(FALSE);
	}
	m_pXFile[num]->Draw(pos, rot,scale, alpha);
}

//====================================
//�֐���	:Draw
//����		:	num		:XFile�`��ԍ�
//				pos		:�ʒu
//				rot		:��]
//				sacle	:�傫��
//				color	:RGB�F�̎w��
//				t_Num	:�e�N�j�b�N�ԍ�
//�߂�l	:
//���e		:�w�肳�ꂽXFile�̕`��(color�ύX��)
//====================================
//void CModelManager::Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale,D3DXCOLOR color, int t_Num)
//{
//	m_pXFile[num]->Draw(&pos, &rot, &scale,color,t_Num);
//}

LPD3DXMESH CModelManager::GetMesh(DWORD num) 
{
	return m_pXFile[num]->GetMesh();
}

