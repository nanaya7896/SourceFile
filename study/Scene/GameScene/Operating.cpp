#include "Operating.h"

//====================================
//�֐���	:COperating
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
COperating::COperating(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
}

//====================================
//�֐���	:~COperating
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
COperating::~COperating()
{
	
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void COperating::Update()
{
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void COperating::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(11, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));

}
