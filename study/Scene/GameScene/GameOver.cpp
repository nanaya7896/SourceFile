#include "GameOver.h"

//====================================
//�֐���	:CGameOver
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CGameOver::CGameOver(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
	m_pos = 370.0f;
	isPos = TRUE;
}

//====================================
//�֐���	:~CGameOver
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CGameOver::~CGameOver()
{

}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CGameOver::Update()
{
	
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CGameOver::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(13, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	pTextureManager->Draw(21, D3DXVECTOR3(m_pos, WINDOW_HEIGHT-170.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	//pTextureManager->Draw(21, D3DXVECTOR3(800.0f, WINDOW_HEIGHT - 170.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
}

void CGameOver::isArrowPos(BOOL isleft)
{
	if (isleft)
	{
		m_pos = 370.0f;
		isPos = TRUE;
	}
	else 
	{
		m_pos = 800.0f;
		isPos = FALSE;
	}
}


void CGameOver::ResetValue()
{
	m_pos = 370.0f;
	isPos = TRUE;

}