#include "Score.h"

//�B��̃C���X�^���X�̐���
CScore* CScore::m_pInstance = NULL;

//====================================
//�֐���	:CScore
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CScore::CScore()
{
	ZombieNum = 0;
}

//====================================
//�֐���	:~CScore
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CScore::~CScore()
{
	
}

void CScore::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CScore;
	}
}

void CScore::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CScore::Update()
{
	SetScore();
}

//====================================
//�֐���	:SetScore
//����		:
//�߂�l	:
//���e		:�X�R�A��ϐ��ɃZ�b�g����
//====================================
void CScore::SetScore()
{
	for (int i = 0; i < 3; i++)
	{
		score[i] = ZombieNum / (int)pow(10.0f, i);
		score[i] = score[i] % 10;
	}
}

//====================================
//�֐���	:ResetScore
//����		:
//�߂�l	:
//���e		:�X�R�A�����Z�b�g����
//====================================
void CScore::ResetScore()
{
	ZombieNum = 0;
	score[0] = 0;
	score[1] = 0;
	score[2] = 0;
}


