#include "WaterBalloon.h"

CWaterBalloon* CWaterBalloon::m_pCWaterBalloonInstance = NULL;

//====================================
//�֐���	:CWaterBalloon
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CWaterBalloon::CWaterBalloon()
{
	m_isShot = FALSE;
	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
	value = 0.0f;
	gunPos = D3DXVECTOR3(-125.0f, 13.25f, -130.0f);
	state = STATE_NONE;
}

//====================================
//�֐���	:~CWaterBalloon
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CWaterBalloon::~CWaterBalloon()
{

}

void CWaterBalloon::Create()
{
	if (!m_pCWaterBalloonInstance)
	{
		m_pCWaterBalloonInstance = new CWaterBalloon();
	}
}

void CWaterBalloon::Destory()
{
	SAFE_DELETE(m_pCWaterBalloonInstance);
}

//====================================
//�֐���	:CreateResource
//����		:
//�߂�l	:
//���e		:��x�����Ă΂�郊�\�[�X�̍쐬
//====================================
void CWaterBalloon::CreateResource(LPDIRECT3DDEVICE9 pDevice, CDInput* pDInput)
{
	m_pDevice9 = pDevice;
	m_pDInput = pDInput;
}

//====================================
//�֐���	:UpdateWater
//����		:
//�߂�l	:
//���e		:�V���{���ʂ�Sin�l�ŏ㉺������
//====================================
void CWaterBalloon::UpdateWater()
{
	m_waterPos.y += ChangeTF(0.1f, value, TRUE);
	m_waterPos.x += ChangeTF(0.1f, m_playerRot.y - D3DX_PI, TRUE);
	m_waterPos.z += ChangeTF(0.1f, m_playerRot.y - D3DX_PI, FALSE);

	value += 0.1f;
	float num;
	if (Collider::GetInstance()->RayCast(m_waterPos, m_playerRot, &num)) {
		m_isShot = FALSE;
		value = 0.0f;
	}

}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�ʏ��̍X�V����
//====================================
void CWaterBalloon::Update(D3DXVECTOR3 vPos, D3DXVECTOR3 /*vRot*/)
{
	if (STATE_NONE == state)
	{
		//�擾�����߂�
		//GetWaterGun(vPos, vRot);
		return;
	}

	//�������Ă���Ȃ�
	//�ʒu�X�V
	gunPos = vPos;

	if (m_isShot)
	{
		UpdateWater();
	}
	//if (m_isShot == TRUE)
	//{
	//	UpdateWater();
	//}
	////�K�������ꍇ
	//if (m_isGun==FALSE)
	//{
	//	//m_pPlayer->SetAnimationNum(13);
	//	m_isGun = TRUE;
	//	
	//}
	////�K����u���ꍇ
	//if (m_isGun==TRUE)
	//{
	//	m_isGun = FALSE;
	//}
	//�V���{���ʂ��o�����Ă���Ԃ�����
	//UpdateWater();	 
	//value = 0.0f;
	//�K���������Ă��邩���v���C���[���ɑ���
	//m_pPlayer->SetWaterGunHoldCheck(m_isGun);
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CWaterBalloon::Draw()
{
	CModelManager* pModelManager = CModelManager::GetInstance();
	if (state != STATE_HAVE_BG &&state !=STATE_BG_SHOT)
	{
		pModelManager->Draw(9, gunPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI/4), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 1.0f);
	}
	if (m_isShot)
	{
		//�V���{����
		pModelManager->Draw(5, m_waterPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.0001f);
	}

	//pModelManager->Draw(0, gunPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);
}

//====================================
//�֐���	:GetWaterGun
//����		:vPos:���L�����̍��W
//			 vRot:���L�����̊p�x
//�߂�l	:
//���e		:�e����ɓ���鏈��
//====================================
BOOL CWaterBalloon::GetWaterGun(D3DXVECTOR3 vPos, D3DXVECTOR3 /*vRot*/)
{
	Collider* pColManager = Collider::GetInstance();
	BOOL check = pColManager->EasyCheck(vPos, gunPos, 10.0f);
	if (check)
	{
		state = STATE_HAVE_BG;
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:DisGun
//����		:
//�߂�l	:
//���e		:�e��u������
//====================================
void CWaterBalloon::DisGun()
{
	state = STATE_NONE;
}

//====================================
//�֐���	:ChangeTF
//����		:value	:�������v�Z���邽�߂̒l(�W��1.0f)
//			 rot	:�U�ꕝ�����߂�l
//			 isSin  :TRUE:Sin�l�@FALSE:Cos�l
//�߂�l	:�O�p�֐�(Sin,Cos)�̌v�Z����
//���e		:Sin,Cos�̒l���v�Z����
//====================================
FLOAT CWaterBalloon::ChangeTF(FLOAT value, FLOAT rot, BOOL isSin)
{
	FLOAT va = 0.0f;
	if (isSin)
	{
		//Sin���v�Z�������ꍇ
		va = value*(sinf(rot));
	}
	else
	{
		//Cos���v�Z�������ꍇ
		va = value*(cosf(rot));
	}
	return va;
}

void CWaterBalloon::ResetWaterBullon()
{
	m_isShot = FALSE;


	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
	value = 0.0f;
	gunPos = D3DXVECTOR3(-125.0f, 13.25f, -130.0f);
	state = STATE_NONE;
}

BOOL CWaterBalloon::HitCheck(D3DXVECTOR3 checkPos) {

	if (Collider::GetInstance()->EasyCheck(m_waterPos, checkPos, 5.0f)) {
		state = STATE_HAVE_BG;
		return TRUE;
	}
	return FALSE;
}