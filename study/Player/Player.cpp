#include "Player.h"
#include"StateMachine.h"


BOOL CPlayer::InWater = NULL;

//====================================
//�֐���	:CPlayer
//����		:pDevice	:�`��f�o�C�X
//			 keyState	:�L�[���͂̊Ǘ�
//			 pFont		:�t�H���g
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CPlayer::CPlayer()
{

	/*�v���C���[�̏�񏉊���*/
	SetPlayerPos(D3DXVECTOR3(-155.0f, 70.0f, 20.0f));
	m_pInfo.vRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	//���������Ă��Ȃ�
	m_pInfo.playerState = STATE_NONE;
	vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�f�o�b�N�p
	//m_vPosition = D3DXVECTOR3(-150,62,30);
	m_Speed = 0.5f;
	ChangeCamera = FALSE;

	//m_pWeapon = NULL;
	m_pMathLib = nullptr;
	m_isHitMapNum = 0;
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i] = NULL;
	}
}

//====================================
//�֐���	:~CPlayer
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CPlayer::~CPlayer()
{
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		SAFE_DELETE(m_pYukiAnim[i]);
	}
	SAFE_DELETE(m_pMathLib);
	SAFE_DELETE(m_pWaterSlider);
	//m_pWeapon = NULL;

};

//====================================
//�֐���	:ResetPlayer
//����		:
//�߂�l	:
//���e		:�Q�[�����I�����܂��̓��g���C���ɏ������Z�b�g����
//====================================
void CPlayer::ResetPlayer()
{
	SetPlayerPos(D3DXVECTOR3(-155.0f, 62.0f, 20.0f));
	m_pInfo.vRotation = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_pInfo.playerState = STATE_NONE;
	ChangeCamera = FALSE;
	vViewAngle = D3DXVECTOR3(D3DX_PI/4.0f, 0.0f, 0.0f);
	CCamera::GetInstance()->SetViewAngle(vViewAngle);
	CCamera::GetInstance()->SetDistance(34.0f);
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i]->SetPosition(m_pInfo.vPosition);
		m_pYukiAnim[i]->set_angle(m_pInfo.vRotation);
	}
	m_pWaterSlider->SetIsSlider(FALSE);
	m_pWaterSlider->ResetValue();
	//m_pWeapon = NULL;
	state->ResetState();
	SetAnimationNum(0);
	//weaponNum = 0;
	
}

//====================================
//�֐���	:CreateResource
//����		:pDevice:�`��f�o�C�X
//�߂�l	:
//���e		:���\�[�X�̍쐬
//====================================
BOOL CPlayer::CreateResource(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput,CSoundManager* pSManager,CPlayer*pPlayer)
{
	//�`��f�o�C�X
	m_pDevice9 = pDevice9;
	m_pInput = pInput;
	m_pSoundManager = pSManager;

	//m_pSoundManager = pSManager;
	for (int i = 0; i < MAXMOTION_NUM; i++)
	{
		m_pYukiAnim[i] = new YukiAnimation(m_pDevice9,MOTIONOBJECT[i]);
		m_pYukiAnim[i]->SetPosition(m_pInfo.vPosition);
	}
	state = new CStateMachine();
	state->CreateResource(m_pInput, pPlayer,pSManager);
	//���w�v�Z���p�֐�
	m_pMathLib = new CMathLib();
	//�E�H�[�^�[�X���C�_�[
	m_pWaterSlider = new CWaterSlider();
	//SetAnimationNum(STATE_N_Wait);
	return TRUE;
}

//====================================
//�֐���	:Update
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CPlayer::Update()
{	
	MyOutputDebugString("X:%f�@Y:%f  Z:%f\n", m_pInfo.vPosition.x,m_pInfo.vPosition.y,m_pInfo.vPosition.z);

	vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	InWater = FALSE;
	//m_pInput->GetJoyStickState(&m_JoyState);
	//�E�H�[�^�[�X���C�_�̏����ɓ��邩�ǂ���
	if (CheckInWaterSlider())
	{
		InWater = TRUE;
	}
	//�E�H�[�^�[�X���C�_�[�̒��ɂ���ꍇ
	if (m_pWaterSlider->GetisSlider())
	{
		InWaterSlider();
	}
	else
	{
		//�n��Ɛ����ł̃A�b�v�f�[�g
		GroundAndInWaterUpdate();
	}
	state->Update();
	#ifdef _DEBUG
		CheckHitMapNum();
	#endif // DEBUG
	
	
	/*����X�V����*/
	//PlayerAction();

	//���L�����̕����E�������A�j���[�V�����ɓn��
	m_pYukiAnim[GetAnimationNum()]->set_angle(m_pInfo.vRotation);
	m_pYukiAnim[GetAnimationNum()]->SetPosition(m_pInfo.vPosition);
	//�A�j���[�V�������X�V
	m_pYukiAnim[m_AnimationNum]->Update();
}

//====================================
//�֐���	:InputKeyUpdate
//����		:viewAngle:�J�����������Ă������
//�߂�l	:
//���e		:���͂��ꂽ�L�[�ɉ����Ĉړ�����
//====================================
void CPlayer::InputKeyUpdate(D3DXVECTOR3 viewAngle)
{
	//���E��
	if (m_pInput->LAnalogState(LEFTRIGHT_UP))
	{
		MoveDirection(LEFTRIGHT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	else if (m_pInput->LAnalogState(RIGHTLEFT_UP))
	{
		MoveDirection(RIGHTLEFT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);
	}
	else if (m_pInput->LAnalogState(LEFTLEFT_UP))
	{
		MoveDirection(LEFTLEFT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	else if (m_pInput->LAnalogState(RIGHTRIGHT_UP))
	{
		MoveDirection(RIGHTRIGHT_UP);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);
	}
	else if (m_pInput->LAnalogState(LEFTLEFT_DOWN))
	{
		MoveDirection(LEFTLEFT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	else if (m_pInput->LAnalogState(RIGHTRIGHT_DOWN))
	{
		MoveDirection(RIGHTRIGHT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);

	}
	else if (m_pInput->LAnalogState(LEFTRIGHT_DOWN))
	{
		MoveDirection(LEFTRIGHT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	else if (m_pInput->LAnalogState(RIGHTLEFT_DOWN))
	{
		MoveDirection(RIGHTLEFT_DOWN);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);

	}
	//����
	else if ((m_pInput->KeyState(DIK_Q)) || (m_pInput->LAnalogState(UP_LEFT)))
	{
		MoveDirection(UP_LEFT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI / 2, 0.0f);
	}
	//�E��
	else if ((m_pInput->KeyState(DIK_E)) || (m_pInput->LAnalogState(UP_RIGHT)))
	{
		MoveDirection(UP_RIGHT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 + D3DX_PI, 0.0f);

	}
	//����
	else if ((m_pInput->KeyState(DIK_Z)) || (m_pInput->LAnalogState(DOWN_LEFT)))
	{
		MoveDirection(DOWN_LEFT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4, 0.0f);

	}
	//�E��
	else if ((m_pInput->KeyState(DIK_C)) || (m_pInput->LAnalogState(DOWN_RIGHT)))
	{
		MoveDirection(DOWN_RIGHT);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 4 - D3DX_PI / 2, 0.0f);
	}
	//��
	else if ((m_pInput->KeyState(DIK_W)) || (m_pInput->LAnalogState(UP_MOVE)))
	{
		MoveDirection(UP_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI, 0.0f);
	}
	//��
	else if ((m_pInput->KeyState(DIK_S)) || (m_pInput->LAnalogState(DOWN_MOVE)))
	{
		MoveDirection(DOWN_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + 0.0f, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + 0.0f, 0.0f);
	}
	//��
	else if ((m_pInput->KeyState(DIK_D)) || (m_pInput->LAnalogState(RIGHT_MOVE)))
	{
		MoveDirection(RIGHT_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + (-D3DX_PI / 2), 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + (-D3DX_PI / 2), 0.0f);
	}
	//�E
	else if ((m_pInput->KeyState(DIK_A)) || (m_pInput->LAnalogState(LEFT_MOVE)))
	{
		MoveDirection(LEFT_MOVE);
		m_pYukiAnim[GetAnimationNum()]->set_angle(D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 2, 0.0f));
		m_pInfo.vRotation = D3DXVECTOR3(0.0f, viewAngle.y + D3DX_PI / 2, 0.0f);
	}
}

//====================================
//�֐���	:MoveDirection
//����		:num:����
//�߂�l	:
//���e		:�����ɉ����Ĉړ��l�����肷��
//====================================
void CPlayer::MoveDirection(char num)
{
	float moveSpeed = 1.0f;
	if (num == UP_MOVE || num == DOWN_MOVE || num == RIGHT_MOVE || num == LEFT_MOVE)
	{
		//�΂߈ړ��ȊO
		moveSpeed = 1.0f;
	}
	//�΂߈ړ��̏ꍇ
	else
	{
		//�ړ��W��0.71��������
		moveSpeed = 0.71f;
	}


	switch (num)
	{
		/*�l����*/
	case UP_MOVE:
		m_vDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case DOWN_MOVE:
		m_vDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f)*moveSpeed;
		break;
	case RIGHT_MOVE:
		m_vDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f)*moveSpeed;
		break;
	case LEFT_MOVE:
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, 0.0f)*moveSpeed;
		break;
		/*������*/
	case UP_LEFT:
	//	m_vDirection = D3DXVECTOR3(-0.5f, 0.0f, 0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case UP_RIGHT:
		//m_vDirection = D3DXVECTOR3(0.5f, 0.0f, 0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(1.0f, 0.0f, 1.0f)*moveSpeed;
		break;
	case DOWN_LEFT:
		//m_vDirection = D3DXVECTOR3(-0.5f, 0.0f, -0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-1.0f, 0.0f, -1.0f)*moveSpeed;
		break;
	case DOWN_RIGHT:
		//m_vDirection = D3DXVECTOR3(0.5f, 0.0f, -0.5f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(1.0, 0.0f, -1.0)*moveSpeed;
		break;
		/*�\�Z����*/
	case LEFTRIGHT_UP:
		//m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, 0.75f)*moveSpeed;
		m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, 0.75f)*moveSpeed;
		break;
	case LEFTRIGHT_DOWN:
		m_vDirection = D3DXVECTOR3(-0.25f, 0.0f, -0.75f)*moveSpeed;
		break;

	case LEFTLEFT_UP:
		m_vDirection = D3DXVECTOR3(-0.75f, 0.0f, 0.25f)*moveSpeed;
		break;
	case LEFTLEFT_DOWN:
		m_vDirection = D3DXVECTOR3(-0.75f, 0.0f, -0.25f)*moveSpeed;
		break;
	
	case RIGHTLEFT_UP:
		m_vDirection = D3DXVECTOR3(0.25f, 0.0f, 0.75f)*moveSpeed;
		break;
	case RIGHTLEFT_DOWN:
		m_vDirection = D3DXVECTOR3(0.25f, 0.0f, -0.75f)*moveSpeed;
		break;
	
	case RIGHTRIGHT_UP:
		m_vDirection = D3DXVECTOR3(0.75f, 0.0f, 0.25f)*moveSpeed;
		break;
	case RIGHTRIGHT_DOWN:
		m_vDirection = D3DXVECTOR3(0.75f, 0.0f, -0.25f)*moveSpeed;
		break;

	}
	//Player�̈ړ�
	PlayerMove();

}

//====================================
//�֐���	:PlayerMove
//����		:
//�߂�l	:
//���e		:�v���C���[�̈ړ������̊֐�
//====================================
void CPlayer::PlayerMove()
{
	D3DXMATRIX matRotation;
	D3DXVECTOR3 newPostion;

	D3DXMatrixRotationY(&matRotation, CCamera::GetInstance()->GetViewAngle().y);
	D3DXVec3TransformCoord(&m_vDirection, &m_vDirection, &matRotation);
	m_vDirection *= m_Speed;
	m_pInfo.vPosition = m_pYukiAnim[GetAnimationNum()]->GetPosition();
	newPostion = m_pInfo.vPosition + m_vDirection;


	///����ύX��(2016/07/15)
	///*�����蔻��*/
	float meshDistance;
	m_beforeIsHitMapNum = m_isHitMapNum;

	Collider* pColManager = Collider::GetInstance();

	D3DXVECTOR3 checkrota(-sin(m_pInfo.vRotation.y), 0.0f, -cos(m_pInfo.vRotation.y));

	if (!pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &meshDistance)) {

		if (meshDistance < 4.0f) {

			if (pColManager->RayCast(newPostion, m_pInfo.vRotation) ||
				pColManager->RayCast(newPostion, m_pInfo.vRotation + D3DXVECTOR3(0, 0.5, 0)) ||
				pColManager->RayCast(newPostion, m_pInfo.vRotation - D3DXVECTOR3(0, 0.5, 0))
				) {
				if (pColManager->RayCast(newPostion, D3DXVECTOR3(0, D3DX_PI / 2, 0)) ||
					pColManager->RayCast(newPostion, D3DXVECTOR3(0, -D3DX_PI / 2, 0))) {
					newPostion.x = newPostion.x - m_vDirection.x;
				}
				if (pColManager->RayCast(newPostion, D3DXVECTOR3(0, 0, 0)) ||
					pColManager->RayCast(newPostion, D3DXVECTOR3(0, D3DX_PI, 0))) {
					newPostion.z = newPostion.z - m_vDirection.z;
				}
			}
		}
		m_pInfo.vPosition = newPostion;
		m_pInfo.vPosition.y += meshDistance;
	}
	///�����܂�

	//Collider* pColManager = Collider::GetInstance();
	////pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num);
	//if (pColManager->GroundCol(newPostion, D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num))
	//{
	//	//	/*	m_vPosition = newPostion;
	//	//	m_vPosition.y += num;*/
	//}
	//else
	//{
	//	if (num < 4.0f)
	//	{
	//		float no;
	//		if (pColManager->RayCast(newPostion, m_pInfo.vRotation, &no))
	//		{
	//			return;
	//		}
	//	}
	//	m_pInfo.vPosition = newPostion;
	//	m_pInfo.vPosition.y += num;
	//}
	//m_pInfo.vPosition = newPostion;
	//m_pInfo.vPosition.y += num;
	m_pYukiAnim[GetAnimationNum()]->SetPosition(newPostion);
	//SE�̊J�n
	/*if (m_beforeIsHitMapNum != 1 && m_isHitMapNum == 1)
	{
		m_pSoundManager->Play(11, TRUE);
	}
	if (m_beforeIsHitMapNum == 1 && m_isHitMapNum != 1)
	{
		m_pSoundManager->Stop(6);
		m_pSoundManager->Stop(11);
		m_pSoundManager->Play(6, FALSE);

	}*/
}


//====================================
//�֐���	:GroundAndInWaterUpdate
//����		:
//�߂�l	:
//���e		:�n��Ɛ������ł̃A�b�v�f�[�g
//====================================
void CPlayer::GroundAndInWaterUpdate()
{
	
	if (InWater)
	{
		InWaterUpdate();
	}
	vViewAngle = CCamera::GetInstance()->GetViewAngle();

	if (m_isHitMapNum != 1)
	{
		//���̒��ȊO�Ȃ�
		vViewAngle.x = D3DX_PI / 4;
	}
	else
	{
		vViewAngle.x = D3DX_PI / 10.0f;
	}
	Collider *pColManager = Collider::GetInstance();

	//���L�����̃A���O����ύX����Ƃ���
	if ((m_pInput->IsButtonDown(L1_BUTTON)) || (m_pInput->KeyState(DIK_UP)) || (m_pInput->RAnalogState(1)))
	{
		vViewAngle.y -= 0.05f;
	}
	if ((m_pInput->IsButtonDown(R1_BUTTON)) || (m_pInput->KeyState(DIK_DOWN)) || (m_pInput->RAnalogState(0)))
	{
		vViewAngle.y += 0.05f;
	}
	///����ύX��
	//�J�������g�̓����蔻��
	float getdis;
	//�ǂ݂̂̓����蔻����s��
	pColManager->DirectRayCast(m_pInfo.vPosition, vViewAngle, 4, &getdis);

	//�J�����̕ǂɂ߂荞�܂Ȃ��悤�ɒ��ߍX�V
	if (getdis >  QUARERVIEW_WALL_DIS)
	{
		getdis = QUARERVIEW_WALL_DIS;
	}
	CCamera::GetInstance()->SetDistance(getdis / (QUARERVIEW_WALL_DIS / QUATERVIEW_DIS));
	CCamera::GetInstance()->SetViewAngle(vViewAngle);

	//�L�[���͂̔���
	InputKeyUpdate(vViewAngle);

	if (m_isHitMapNum == 1)
	{
		PlayerMoveInWater();
	}

}

//====================================
//�֐���	:InWaterUpdate
//����		:
//�߂�l	:
//���e		:���̒��ɂ���ꍇ�̃A�b�v�f�[�g
//====================================
void CPlayer::InWaterUpdate()
{
	/*m_pSoundManager->Stop(12);
	m_pSoundManager->Stop(13);
	m_pSoundManager->Play(13, FALSE);*/
	InWater = FALSE;
}

int CPlayer::CheckHitMapNum()
{
	float distance;
	int hitMap;
	Collider::GetInstance()->GroundCol(m_pInfo.vPosition, D3DXVECTOR3(0, 0, 0), &hitMap,&distance);
	//MyOutputDebugString("%d", hitMap);
	return hitMap;
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CPlayer::Draw()
{
	m_pYukiAnim[m_AnimationNum]->Draw();
}



//====================================
//�֐���	:PlayerAction
//����		:
//�߂�l	:
//���e		:�v���C���[���s������
//====================================
void CPlayer::PlayerAction()
{
	if ((m_pInput->KeyState(DIK_SPACE)) || m_pInput->IsButtonDown(CIRCLE_BUTTON))
	{
		if (m_pWeapon != NULL)
		{
			if (m_pWeapon->GetState() == Have)
			{
				m_pWeapon->SetPos(m_pInfo.vPosition);
				m_pWeapon->SetRot(m_pInfo.vRotation);
			}
			//����ōU������
			m_pWeapon->Attack();
		}
	}
}

//====================================
//�֐���	:PlayerMoveInWater
//����		:
//�߂�l	:
//���e		:�����v�[��(�����̏���)
//====================================
void CPlayer::PlayerMoveInWater()
{
	D3DXVECTOR2 tmp;
	tmp = m_pMathLib->MyVecAngle(m_pInfo.vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DX_PI / 1000);
	float num;
	Collider* pColManager = Collider::GetInstance();
	if (pColManager->GroundCol(D3DXVECTOR3(tmp.x, m_pInfo.vPosition.y, tmp.y), D3DXVECTOR3(0, 0, 0), &m_isHitMapNum, &num))
	{
		/*	m_vPosition = newPostion;
		m_vPosition.y += num;*/
	}
	else
	{
		//if (GetAnimationNum() == STATE_IW_Wait)
		//{
		//	m_pInfo.vPosition.y += (num - 4.0f);
		//}
		//else if (GetAnimationNum() == STATE_N_Swim)
		//{
		//	/*	m_vPosition = newPostion;*/
		//	m_pInfo.vPosition.y += num;
		//}
	}
	SetPlayerPos(D3DXVECTOR3(tmp.x, m_pInfo.vPosition.y, tmp.y));
}

//====================================
//�֐���	:CheckInWaterSlider
//����		:
//�߂�l	:
//���e		:�E�H�[�^�[�X���C�_�[�̒��ɂ��邩�ǂ������肷��
//====================================
BOOL CPlayer::CheckInWaterSlider()
{
	if (m_pInfo.vPosition.y > 62 && m_pInfo.vPosition.z > 25 && m_pInfo.vPosition.z < 27)
	{
	/*	m_pSoundManager->Stop(12);
		m_pSoundManager->Play(12, TRUE);*/
		m_pWaterSlider->SetIsSlider(TRUE);
		//SetAnimationNum(STATE_W_Slide);
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:InWaterSlider
//����		:
//�߂�l	:
//���e		:�E�H�[�^�[�X���C�_�[�Ƀv���C���[������ꍇ�Ƀv���C���[�̏�Ԃ��X�V����
//====================================
void CPlayer::InWaterSlider()
{
	m_pWaterSlider->SetPlayerPos(m_pInfo.vPosition);
	m_pWaterSlider->SetPlayerRot(m_pInfo.vRotation);
	m_pInfo.vPosition = m_pWaterSlider->WaterSliderAction(FALSE);
	m_pInfo.vRotation.y = m_pWaterSlider->GetRot();
	//if (m_pInfo.vPosition.y <= 10.0f)
	//{
	//	m_isHitMapNum = 1;
	//}
}

//====================================
//�֐���	:SetWeapon
//����		:pWeapon:����Ǘ��N���X
//�߂�l	:
//���e		:����Ǘ��N���X�֊e����̃A�h���X���Z�b�g����
//====================================
CAbstractWeapon * CPlayer::SetWeapon(CAbstractWeapon* pWeapon)
{
	m_pWeapon = pWeapon;
	return m_pWeapon;
}

void CPlayer::SetHaveWeaponInfo(int weapon)
{
	weaponNum = weapon;
}
