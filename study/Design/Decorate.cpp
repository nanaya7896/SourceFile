#include "Decorate.h"

CDecorate::CDecorate()
{
	alphaValue = 0.0f;
	alphaFlashScore=0.0f;
	
	value=0.001f;
	isFlash=FALSE;
}

CDecorate::~CDecorate()
{
}

//====================================
//�֐���	:Update
//����		:pPos:�v���C���[�̍��W
//			 ePos:�]���r�̍��W
//�߂�l	:
//���e		:�X�V
//====================================
void CDecorate::Update(D3DXVECTOR3 pPos, D3DXVECTOR3 ePos)
{
	//�]���r�ƃv���C���[�̍��W�̍���}��
	D3DXVECTOR3 diff = pPos - ePos;
	//�ꎞ�ϐ��ۊǗp
	FLOAT tmp;
	//���������߂�
	FLOAT length = D3DXVec3Length(&diff);
	if (length <= 10.0f)
	{
		//�������瓧���x�����߂�
		alphaValue = length - 10.0f;
	}
}

//====================================
//�֐���	:UpdateeFlashScore
//����		:
//�߂�l	:
//���e		:�X�R�A�����点��
//====================================
void CDecorate::UpdateeFlashScore()
{
	if (isFlash)
	{
		alphaFlashScore += value;
		if (alphaFlashScore > 1.0f)
		{
			value *= -1.0f;
		}
		else if(alphaFlashScore<0.0f)
		{
			value *= -1.0f;
			alphaFlashScore = 0.0f;
			isFlash = FALSE;
		}
	}
}

//====================================
//�֐���	:FalshScreen
//����		:
//�߂�l	:
//���e		:�X�N���[�������点�鏈��
//====================================
void CDecorate::FalshScreen()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();

	pTextureManager->Draw(24, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, -alphaValue, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
}

void CDecorate::FlashScore()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	if (isFlash)
	{
		pTextureManager->Draw(25, D3DXVECTOR3(-10.0f, -10.0f, 0.0f), 0, alphaFlashScore, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 8.0f, 150.0f, 0.0f));
	}
}

void CDecorate::ResetDeco()
{
	alphaValue = 0.0f;
	alphaFlashScore = 0.0f;
	float value = 0.01f;
	BOOL isFlash = FALSE;
}
