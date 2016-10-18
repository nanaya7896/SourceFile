#include "Title.h"
#include<time.h>

//====================================
//�֐���	:CTitle
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CTitle::CTitle(LPDIRECT3DDEVICE9 pDevice9)
{
	isTitle = false;
	fadeTime = 0.0f;
	timeCount = 0.005f;
	CreateResource(pDevice9);
}

//====================================
//�֐���	:~CTitle
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CTitle::~CTitle()
{
}

//====================================
//�֐���	:CreateResource
//����		:
//�߂�l	:
//���e		:���\�[�X�̍쐬�B��x�����Ă΂�Ȃ�
//====================================
void CTitle::CreateResource(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
}

//====================================
//�֐���	:AllGameReset
//����		:
//�߂�l	:
//���e		:Title�̃��Z�b�g�֐�
//====================================
void CTitle::AllReset()
{
	isTitle = false;
	fadeTime = 0.0f;
	timeCount = 0.005f;
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V����
//====================================

void CTitle::Update()
{
	if (fadeTime > 1.5f)
	{
		timeCount = -0.05f;
	}
	if (fadeTime < 0)
	{
		isTitle = true;
	}
	fadeTime += timeCount;
}


//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CTitle::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	if (isTitle)
	{
		//�^�C�g���̕`��
		pTextureManager->Draw(10, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	}
	else
	{
		//�^�C�g�����S�̕`��
		pTextureManager->Draw(17, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, fadeTime, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	}
	

}
