#include "Credit.h"

//====================================
//�֐���	:CCredit
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CCredit::CCredit(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
}

//====================================
//�֐���	:~CCredit
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CCredit::~CCredit()
{
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CCredit::Update()
{

}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CCredit::Draw()
{
	//�C���X�^���X����
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(15, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
}
