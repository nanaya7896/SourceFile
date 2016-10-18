#include "Result.h"
#include<time.h>


//====================================
//�֐���	:CResult
//����		: keyState	:�L�[�̏��
//			 pDevice9:�`��f�o�C�X
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CResult::CResult(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput)
{
	m_pDevice9 = pDevice9;
	m_pDInput=pInput;
	CreateResource();
}

//====================================
//�֐���	:CResult
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CResult::~CResult()
{
}

BOOL CResult::CreateResource()
{
	return TRUE;
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CResult::Update()
{
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CResult::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(14, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	pTextureManager->Draw(score[2], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 9.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(score[1], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 10.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(score[0], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 11.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));

}


