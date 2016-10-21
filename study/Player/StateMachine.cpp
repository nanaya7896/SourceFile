#include "StateMachine.h"
#include"../Scene/GameScene/Main.h"

//====================================
//�֐���	:event
//����		:num:�񎟌��z���Row�̏ꏊ��\��
//�߂�l	:
//���e		:�C�x���g�����s����
//====================================
void CStateMachine::event(int num)
{
	(this->*handler_[m_State_][num])();
}

//====================================
//�֐���	:CheckPlayerPositionWaterSlider
//����		:
//�߂�l	:
//���e		:�v���C���[���E�H�[�^�[�X���C�_�[�ɓ��������ǂ������ׂ�
//====================================
BOOL CStateMachine::CheckPlayerPositionWaterSlider()
{
	if (m_pPlayer->CheckInWaterSlider())
	{
		return TRUE;
	}

	return FALSE;
}

//====================================
//�֐���	:PlayerMoveing
//����		:
//�߂�l	:
//���e		:�v���C���[�������Ă��邩�ǂ���
//====================================
BOOL CStateMachine::PlayerMoveing()
{
	if ((m_pDInput->GetDInputJoyStick() != NULL && m_pDInput->AllControllCheck()) || m_pDInput->WASDKeyDownCheck())
	{
		stateNumber = 1;
		//event(1);
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:PlayerWait
//����		:
//�߂�l	:
//���e		:�v���C���[���~�܂��Ă��邩�ǂ���
//====================================
BOOL CStateMachine::PlayerWait()
{
	if ((m_pDInput->GetDInputJoyStick() !=NULL && m_pDInput->AllControllCheck()) || (m_pDInput->WASDKeyUpCheck()))
	{
		stateNumber = 0;
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:CheckPlayerPosition
//����		:
//�߂�l	:
//���e		:�v���C���[���ǂ̒n�`�ɂ��邩�`�F�b�N
//====================================
int CStateMachine::CheckPlayerPosition()
{
	return m_pPlayer->CheckHitMapNum();
}

//====================================
//�֐���	:CheckPlayerHaveWeapon
//����		:
//�߂�l	:
//���e		:�v���C���[���ǂ̗V��������Ă��邩
//====================================
void CStateMachine::CheckPlayerHaveWeapon()
{

	//
	switch (weaponNumber)
	{
	case 0:
		if (CheckPlayerPosition() == 0)
		{
			m_State_ = STATE_Ground;
		}
		break;
	case 1:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_BBall;
		break;
	case 2:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_WGun;
		break;
	case 3:
		if (m_State_ == STATE_Ground)
		{
			stateNumber = 0;
		}
		m_State_ = STATE_BGun;
		break;
	}
}

//====================================
//�֐���	:ResetState
//����		:
//�߂�l	:
//���e		:���v���C���ɕK�v�Ȓl��������
//====================================
void CStateMachine::ResetState()
{
	stateNumber = 0;
	weaponNumber = 0;
	m_State_ = STATE_Ground;
	int a = 0;
	int b = 0;
}

//====================================
//�֐���	:CreateResource
//����		:
//�߂�l	:
//���e		:��x�����Ăяo���č쐬
//====================================
BOOL CStateMachine::CreateResource(CDInput* pDInput,CPlayer* pPlayer,CSoundManager* pSManager)
{
	m_pDInput = pDInput;
	m_pPlayer = pPlayer;
	m_pSoundManager = pSManager;
	if (m_pDInput == NULL)
	{
		OutputDebugString("DirectInput is NULL");
		return FALSE;
	}
	//
	stateNumber = 0;



	if (m_pDInput->GetDInputJoyStick() == NULL)
	{
		isController = false;
	}
	else
	{
		isController = true;
	}

	return TRUE;
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CStateMachine::Update()
{
	//�E�H�[�^�[�X���C�_�[�ɓ����Ă��邩
	if (CheckPlayerPositionWaterSlider())
	{
		stateNumber = 2;
	}
	////����������Ă��ă{�^���������ꂽ�Ƃ�
	//if (weaponNumber != 0 && (m_pDInput->KeyDown(DIK_SPACE) || m_pDInput->IsButtonDown(CIRCLE_BUTTON)))
	//{
	//	stateNumber = 2;
	//}
	//�J�����̋�����ύX
	ChangeDistance();

	//if (m_State_ != STATE_Water)
	//{
	//	CheckPlayerHaveWeapon();
	//}
	//
	event(stateNumber);
}

//====================================
//�֐���	:handler[COLUMN][ROW]
//����		:
//�߂�l	:
//���e		:�񎟌��z��Ɋi�[����X�e�[�g�����肷��
//====================================
void (CStateMachine::*const CStateMachine::handler_[COLUMN][ROW])() = 
{
	//�j���[�g�����̃X�e�[�g
	{&CStateMachine::StateNeutralWait,&CStateMachine::StateNeutralWalk,&CStateMachine::StateWaterGSlider,&CStateMachine::StateNeutralPickUp},
	//�����̃X�e�[�g
	{&CStateMachine::StateInWaterWait,&CStateMachine::StateNeutralSwim,&CStateMachine::StateNeturalSwimUp},
	//�r�[�`�{�[���̃X�e�[�g
	{&CStateMachine::StateBallWait,&CStateMachine::StateBallWalk,&CStateMachine::StateBallThrow,&CStateMachine::StateBallThrowEnd},
	//�o�u���K���̃X�e�[�g
	{&CStateMachine::StateBubbleGWait,&CStateMachine::StateBubbleGWalk,&CStateMachine::StateBubbleShot},
	//�E�H�[�^�[�K���̃X�e�[�g
	{&CStateMachine::StateWaterGWait,&CStateMachine::StateWaterGWalk,&CStateMachine::StateWaterGHave,&CStateMachine::StateWaterGStandBy}

};

//====================================
//�֐���	:CheckKeyPickUpButton
//����		:
//�߂�l	:
//���e		:�E�����ݒ肳�ꂽ�{�^���������ꂽ��(Key:G JoyPad:SQUARE)
//====================================
BOOL CStateMachine::CheckKeyPickUpButton()
{
	//
	if ((m_pDInput->KeyDown(DIK_G) || m_pDInput->IsButtonDown(SQUARE_BUTTON)))
	{
		stateNumber = 3;
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:ChangeDistance
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::ChangeDistance()
{
	//���̒��ɂ���Ƃ��ɃJ�����̋�����ύX����
	if (m_State_ == STATE_Water)
	{
		CCamera::GetInstance()->SetDistance(13.0f);
	}
}
/*�ȉ��X�e�[�g�̏��*/
//====================================
//�֐���	:StateNeutralWait
//����		:
//�߂�l	:
//���e		:�ҋ@���[�V����
//====================================
void CStateMachine::StateNeutralWait()
{
	
	if (PlayerMoveing())
	{
		return;
	}
	//�E������
	CheckKeyPickUpButton();
	//�A�j���[�V������؂�ւ���
	SetAnimationNum(STATE_N_Wait);
	//MyOutputDebugString("�X�e�[�g�͑ҋ@�ł�\n");
}

//====================================
//�֐���	:StateNeutralWalk
//����		:
//�߂�l	:
//���e		:����
//====================================
void CStateMachine::StateNeutralWalk()
{
	//�v���C���[��������ԂȂ�
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
		SetAudio(13, FALSE);
	}
	//�v���C���[���ҋ@����
	if (PlayerWait())
	{
		return;
	}
	//�E������
	if (CheckKeyPickUpButton())
	{
		return;
	}

	SetAnimationNum(STATE_N_Walk);
	//MyOutputDebugString("�X�e�[�g�͕����ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateNeutralSwim()
{
	SetAnimationNum(STATE_N_Swim);
	
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition()==0)
	{
		
		m_State_ = STATE_Ground;
		SetAudio(6, FALSE);
		if (weaponNumber != 0)
		{
			CheckPlayerHaveWeapon();
		}
		//stateNumber = 0;
	}
	//MyOutputDebugString("�X�e�[�g�͉j���ł�\n");
}

//====================================
//�֐���	:StateNeturalSwimUp
//����		:
//�߂�l	:
//���e		:���ɏオ��ł�
//====================================
void CStateMachine::StateNeturalSwimUp()
{
	//MyOutputDebugString("�X�e�[�g�͗��ɏオ��ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateBallWait()
{
	if ((m_pDInput->KeyDown(DIK_H)) || m_pDInput->IsButtonDown(X_MARK_BUTTON))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_B_Wait);
	if (PlayerMoveing())
	{
		return;
	}
	//MyOutputDebugString("�X�e�[�g�̓r�[�`�{�[���ҋ@�ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateBallWalk()
{
	SetAnimationNum(STATE_B_Walk);
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
	}
//	MyOutputDebugString("�X�e�[�g�̓r�[�`�{�[�������ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateBallThrow()
{
	SetAnimationNum(STATE_B_Throw);
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 3;
		return;
	}
	//MyOutputDebugString("�X�e�[�g�̓r�[�`�{�[��������ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateBallThrowEnd()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_B_ThrowEnd);
	//MyOutputDebugString("�X�e�[�g�̓r�[�`�{�[�������I���ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateWaterGWait()
{
	SetAnimationNum(STATE_W_Wait);
	if (PlayerMoveing())
	{
		return;
	}
	if ((m_pDInput->KeyDown(DIK_H)) || m_pDInput->IsButtonDown(X_MARK_BUTTON))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
		return;
	}
//	MyOutputDebugString("�X�e�[�g�͐��e�ҋ@�ł�\n");
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateWaterGWalk()
{
	SetAnimationNum(STATE_W_Walk);
	if (PlayerWait())
	{
		return;
	}
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
	}
	//MyOutputDebugString("�X�e�[�g�͐��e�����ł�\n");
}

//====================================
//�֐���	:StateWaterGHave
//����		:
//�߂�l	:
//���e		:
//====================================
void CStateMachine::StateWaterGHave()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_W_Have);
	//MyOutputDebugString("�X�e�[�g�͐��e�����ł�\n");
}

//====================================
//�֐���	:StateWaterGStandBy
//����		:
//�߂�l	:
//���e		:�E�H�[�^�[�X���C�_�[�̃��[�V����
//====================================
void CStateMachine::StateWaterGStandBy()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_W_StandBy);
	//MyOutputDebugString("�X�e�[�g�͐��e�\���ł�\n");
}

//====================================
//�֐���	:StateWaterGSlider
//����		:
//�߂�l	:
//���e		:�X���C�_�[�𗬂��X�e�[�g�ł��B
//====================================
void CStateMachine::StateWaterGSlider()
{
	SetAudio(12, TRUE);
	SetAnimationNum(STATE_W_Slide);
	if (CheckPlayerPosition() == 1)
	{
		stateNumber = 0;
		m_State_ = STATE_Water;
		SetAudio(13, FALSE);
		return;
	}
	//MyOutputDebugString("�X�e�[�g�̓E�H�[�^�[�X���C�_�[����ł�\n");
}

//====================================
//�֐���	:StateBubbleGWait
//����		:
//�߂�l	:
//���e		:�o�u���K���ҋ@�̃X�e�[�g�ł�
//====================================
void CStateMachine::StateBubbleGWait()
{
	SetAnimationNum(STATE_BG_Wait);

	if (PlayerMoveing())
	{
		return;
	}
	if ((m_pDInput->KeyDown(DIK_H) || m_pDInput->IsButtonDown(X_MARK_BUTTON)))
	{
		m_State_ = STATE_Ground;
		stateNumber = 0;
	}
	//MyOutputDebugString("�X�e�[�g�̓o�u���K���ҋ@�ł�\n");
}

//====================================
//�֐���	:StateBubbleGWalk
//����		:
//�߂�l	:
//���e		:�o�u���K�����������Ă����Ԃŕ����Ă��܂��B
//====================================
void CStateMachine::StateBubbleGWalk()
{
	if (CheckPlayerPosition() == 1)
	{
		m_State_ = STATE_Water;
		stateNumber = 0;
		return;
	}

	SetAnimationNum(STATE_BG_Walk);
	if (PlayerWait())
	{
		return;
	}

//	MyOutputDebugString("�X�e�[�g�̓o�u���K�������ł�\n");
}

//====================================
//�֐���	:StateBubbleShot
//����		:
//�߂�l	:
//���e		:�o�u���K���̃V���b�g�X�e�[�g
//====================================
void CStateMachine::StateBubbleShot()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_BG_Shot);
//	MyOutputDebugString("�X�e�[�g�̓o�u���K���V���b�g�ł�\n");
}

//====================================
//�֐���	:StateInWaterWait
//����		:
//�߂�l	:
//���e		:���ҋ@�X�e�[�g
//====================================
void CStateMachine::StateInWaterWait()
{
	SetAnimationNum(STATE_IW_Wait);
	m_pPlayer->SetPlayerPosY(5.0f);

	if (PlayerMoveing())
	{
		return;
	}
	if (CheckPlayerPosition()==0)
	{
		stateNumber = 0;
		return;
	}
//	MyOutputDebugString("�X�e�[�g�͐��ҋ@�ł�\n");
}

//====================================
//�֐���	:StateNeutralPickUp
//����		:
//�߂�l	:
//���e		:�V����E���X�e�[�g
//====================================
void CStateMachine::StateNeutralPickUp()
{
	if (m_pPlayer->isAnimationFinishTime())
	{
		CheckPlayerHaveWeapon();
		stateNumber = 0;
		return;
	}
	SetAnimationNum(STATE_N_Pick);
	
//	MyOutputDebugString("�X�e�[�g�͏E���ł�\n");
}


void CStateMachine::SetAudio(int number, BOOL loop) {

	//���ݗ����Ă��鉹���i�[(�����ݒ��-1�Ƃ��Đݒ�j
	static int nowPlayNumber = -1;

	//�������Ă��鉹�ł���΁A���̂܂܏I��
	if (nowPlayNumber == number) {
		return;
	}

	//�����ݒ�ȊO�ł���΁A�������Ă��鉹�͎~�߂�
	if (nowPlayNumber != -1) {
		m_pSoundManager->Stop(nowPlayNumber);
	}

	//�I�����������Đ�����
	if (number > -1) {
		m_pSoundManager->Play(number, loop);
	}
	//���̔ԍ����i�[
	nowPlayNumber = number;

}