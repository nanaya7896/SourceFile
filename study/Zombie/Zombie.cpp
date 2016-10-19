#include "Zombie.h"


//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
CZombie::CZombie()
{
	
	
}

// �R���X�g���N�^
// ����		pDevice		�F	�f�o�C�X
CZombie::CZombie(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	value = 0;
	scale_ = D3DXVECTOR3(1.2f, 1.2f, 1.2f);
	//scale_ = D3DXVECTOR3(0.2f,0.2f, 0.2f);
	for (int i = 0; i < NumZombies; i++)
	{
		m_pWaterSlider[i] = NULL;
	}
	m_pMathlib = nullptr;
	m_pDecorate = NULL;
}

// �f�X�g���N�^
CZombie::~CZombie()
{
	SAFE_DELETE_ARRAY(m_ZombieInfo);
	for (int i = 0; i < MOTION_ZNUM; i++)
	{
		for (int j = 0; j < SKIN_ZNUM; j++) {
			SAFE_DELETE(m_pAnimation[j][i]);
		}
	}
	SAFE_DELETE(m_pMathlib);
	for (int i = 0; i < NumZombies; i++)
	{
		SAFE_DELETE(m_pWaterSlider[i]);
	}
	SAFE_DELETE(m_pDecorate);
}

// �������֐�
BOOL CZombie::Initialize()
{
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	// �V�F�[�_�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// �f�o�C�X
		"Effect/Zombie.fx",		// �t�@�C����
		NULL,			// �v���v���Z�b�T��`�ւ̃|�C���^
		NULL,			// �I�v�V�����̃C���^�[�t�F�C�X�|�C���^ 
		0,				// �R���p�C���I�v�V����
		NULL,			// �G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&m_pEffect,		// �G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^
		&pErrorMsgs);	// �G���[����ьx���̃��X�g���܂ރo�b�t�@

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	DWORD Time = 20;		// 20�~���b
	for (int i = 0; i < MOTION_ZNUM; i++)
	{
		for (int j = 0; j < SKIN_ZNUM; j++) {
			m_pAnimation[j][i] = new CAnimMesh(m_pDevice, Time, m_pEffect);

			if (!m_pAnimation[j][i]->LoadFromFile(Z_MOTIONOBJECT[j][i]))	// �]���r�̃t�@�C����
				return FALSE;
		}
	}

	for(int i=0;i<NumZombies;i++)
	m_pWaterSlider[i] = new CWaterSlider();
	m_ZombieInfo = new ZOMBIEINFO[MaxZombie];
	m_pMathlib = new CMathLib();
	ZombieInfoInitialize();
	m_pDecorate = new CDecorate();
	m_Speed = 0.4f;
	//�A�j���[�V���������]���r�P�̂��~���[�V��������ۑ�
	for (int i = 0; i < MaxZombie; i++) {

		//�v���O������0����X�^�[�g������̂ł��̂��߂�-1���g�p
		m_ZombieInfo[i].skin = Random(0, SKIN_ZNUM - 1);
		for (int j = 0; j < MOTION_ZNUM; j++) {
			m_ZombieInfo[i].AnimController[j] = m_pAnimation[m_ZombieInfo[i].skin][j]->GetAnimController();
		}
	}
	//�]���r���Ƀ����_���ōĐ��J�n�n�_��ʁX�ɂ���
	for (int i = 0; i < MaxZombie; i++) {
		FLOAT startTime = Random(0.0f, m_pAnimation[0][0]->GetAnimFinishTime());
		m_ZombieInfo[i].AnimController[0]->AdvanceTime(startTime, NULL);
	}
	return TRUE;
}

//====================================
//�֐���	:PositionCheck
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�����_���ŃG�l�~�[�̍��W��ݒ肷��
//====================================
void CZombie::PositionCheck(int num)
{
	float check;
	int hitNum;
	int value;
	//int value;
	//value = (int)Random(0, 10000);
	/*�[�̍��W�l��203�O��Ȃ̂ł���ɍ��킹�č쐬*/
	//2�̂�����O�ł䂫�����̂��΂ɐݒ�
	if (0 == num) {
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(-155.5f, 54.0f, -15.5f);
		//m_ZombieInfo[num].vAngle = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	}
	else if (1 == num) {
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(-155.5f, 49.0f, -25.5f);
	}
	//�܂����̏�Ԍ�قǂȂ����܂��B�B�B
	else {
		do {
			value = (int)Random(0, 10000);
			if (value % 2 == 0)
			{
				/*�[�̍��W�l��203�O��Ȃ̂ł���ɍ��킹�č쐬*/
				m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-200.0f, 200.0f), 14.0f, Random(150.0f, 200.0f));
			}
			else
			{
				m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-200.0f, 200.0f), 14.0f, Random(-150.0f, -200.0f));
			}
			Collider::GetInstance()->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &hitNum, &check);
		} while (hitNum!=0);
	}

	if (Collider::GetInstance()->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &hitNum, &check))
	{
		m_ZombieInfo[num].vPosition.y += check;
	}

}

//====================================
//�֐���	:ZombieInfoInitialize
//����		:
//�߂�l	:
//���e		:�]���r�̏����l�ݒ�B���Z�b�g�����d�l����
//====================================
void CZombie::ZombieInfoInitialize()
{
	/*�]���r�ɃX�e�[�^�X����^����*/
	for (DWORD i = 0; i < NumZombies; i++)
	{
		PositionCheck(i);
		m_ZombieInfo[i].isAlive = TRUE;		
		/*�ꏊ�E����*/
		m_ZombieInfo[i].vAngle = D3DXVECTOR3(0.0f, 0.0f,1.0f);

		/*�ǐՃ��[�h*/
		m_ZombieInfo[i].state = CHASER;
		/*/���S���̐�������*/
		m_ZombieInfo[i].LifeTime = 120.0f;
		m_ZombieInfo[i].animNum = 0;
		m_ZombieInfo[i].ZombieStaNum = 0;
		m_ZombieInfo[i].z_num = 0;
		/*�K�i�ɂ̂ڂ邩�ǂ����̔���*/
		m_ZombieInfo[i].isStairs = FALSE;
		/*�K�i����鎎�s��*/
		m_ZombieInfo[i].val = 0;
		/*�傫��*/
		m_ZombieInfo[i].vScale = { 1.2f,1.2f,1.2f };


		
		//m_ZombieInfo[i].AnimController= m_pAnimation[m_ZombieInfo[i].animNum]->Anim
	}
	for (DWORD i = NumZombies; i < MaxZombie; i++)
	{
		if (i == NumZombies)
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(100.0f, 7.5f, 80.0f);
		//	m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, 110.0f);
		}
		else if (i == NumZombies + 1)
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, 100.0f);
		}
		else
		{
			m_ZombieInfo[i].vPosition = D3DXVECTOR3(0.0f, 7.5f, -60.0f);
		}
		m_ZombieInfo[i].isAlive = TRUE;

		/*�ꏊ�E����*/
		m_ZombieInfo[i].vAngle = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		/*�ǐՃ��[�h*/
		m_ZombieInfo[i].state = CHASER;
		/*/���S���̐�������*/
		m_ZombieInfo[i].LifeTime = 120.0f;
		m_ZombieInfo[i].animNum = 4;

		/*for (int j = 0; j < MOTION_ZNUM; j++) 
		{
			m_ZombieInfo[i].AnimController[j] = m_pAnimation[j]->GetAnimController();
		}*/
	}
	
}

// �`��֐�
// ����		Number		�F	�]���r�̔z��ԍ�
void CZombie::Draw(DWORD Number)
{

	//MyOutputDebugString("ZOmbie[50]:%f\n", m_ZombieInfo[50].vPosition.x);
	D3DXMATRIX matView, matProjection, matScaling;
	CCamera* pCamera = CCamera::GetInstance();
	matView = pCamera->getView();
	matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);
	D3DXMATRIX matWorld, matPosition, matRotation;
	//
	if (m_ZombieInfo[Number].isAlive)
	{
		if (m_pEffect)
		{
			// �e�N�j�b�N�̑I��
			m_pEffect->SetTechnique("ShaderTechnique");
			// �p�X���w��
			m_pEffect->Begin(0, 0);
			//�X�P�[���ϊ�
			D3DXMatrixScaling(&matScaling, m_ZombieInfo[Number].vScale.x, m_ZombieInfo[Number].vScale.y, m_ZombieInfo[Number].vScale.z);

			// ���s�ړ��s����쐬
			D3DXMatrixTranslation(
				&matPosition,		// ���Z���ʂ̍s��ւ̃A�h���X
				m_ZombieInfo[Number].vPosition.x,		// X���W�̃I�t�Z�b�g
				m_ZombieInfo[Number].vPosition.y,		// X���W�̃I�t�Z�b�g
				m_ZombieInfo[Number].vPosition.z);		// X���W�̃I�t�Z�b�g
									// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
			D3DXMatrixRotationY(
				&m_ZombieInfo[Number].matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
				m_ZombieInfo[Number].DirectionRadian);
			matWorld =matScaling *  m_ZombieInfo[Number].matRotation * matPosition;
			// ���[���h�s����w��
			m_pEffect->SetMatrix("matWorld", &matWorld);
			// �r���[�s����w��
			m_pEffect->SetMatrix("matView", &matView);
			// �v���W�F�N�V�����s����w��
			m_pEffect->SetMatrix("matProjection", &matProjection);
			// �J�����̈ʒu���w��
			m_pEffect->SetVector("ViewPosition", &vViewPosition);
			// ���C�g�������w��
			m_pEffect->SetVector("DirectionalLight", &vDirectionalLight);
			// �V�F�[�_�ݒ���O���{�ɍX�V
			m_pEffect->CommitChanges();
			//
			m_pAnimation[m_ZombieInfo[Number].skin][m_ZombieInfo[Number].animNum]->Draw();
			//
			m_pEffect->EndPass();
		}
	}
}
// �A�j���[�V�����Đ��֐�
// ����		Number		�F	�]���r�̔z��ԍ�
void CZombie::AnimationPlay(DWORD Number)
{
	/*�X�R�A���Z�����点�鏈��*/
	m_pDecorate->UpdateeFlashScore();
	m_pDecorate->FlashScore();
	FLOAT Time = 0.0005f * 50;
	m_ZombieInfo[Number].AnimController[m_ZombieInfo[Number].animNum]->AdvanceTime(Time, NULL);
	//m_ZombieInfo[Number].AnimController->AdvanceTime(Time, NULL);
}

// �]���r�̕�������(���ݕ��ʈړ�)
// ����		PlayerPos	�F	�v���C���[�̍��W
//			Number		�F	�]���r�̔z��ԍ�
void CZombie::ZombieWalk(D3DXVECTOR3 PlayerPos, DWORD Number)
{

	D3DXVECTOR3 Position;
	D3DXVECTOR3 vDirection = PlayerPos - m_ZombieInfo[Number].vPosition;
	m_ZombieInfo[Number].vAngle = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// �x�N�g���̒P�ʉ�
	D3DXVec3Normalize(&vDirection, &vDirection);
	D3DXVec3Normalize(&m_ZombieInfo[Number].vAngle, &m_ZombieInfo[Number].vAngle);

	// 2��3D�x�N�g���̓��ς��v�Z
	FLOAT Dot = D3DXVec3Dot(&vDirection, &m_ZombieInfo[Number].vAngle);

	m_ZombieInfo[Number].DirectionRadian = acosf(Dot);		// �A�[�N�R�T�C���Ŋp�x�֕ϊ�

	//----- �O�� -----//
	D3DXVECTOR3 vNormal;
	// 2��3D�x�N�g���̊O�ς��v�Z
	D3DXVec3Cross(&vNormal, &vDirection, &m_ZombieInfo[Number].vAngle);
	// �P�ʉ�
	D3DXVec3Normalize(&vNormal, &vNormal);
	// 0�`�΂܂ł̌ʓx��0�`2�΂܂łɕύX
	if (vNormal.y >= 0.0f)
	{
		m_ZombieInfo[Number].DirectionRadian = (D3DX_PI - m_ZombieInfo[Number].DirectionRadian) + D3DX_PI;
	}
	D3DXVec3TransformCoord(&vDirection, &vDirection, &m_ZombieInfo[Number].matRotation);
	vDirection.x = PlayerPos.x - m_ZombieInfo[Number].vPosition.x;
	vDirection.y = 0.0f;
	vDirection.z = PlayerPos.z - m_ZombieInfo[Number].vPosition.z;
	// ���W�̍X�V
	D3DXVec3Normalize(&vDirection, &vDirection);
	vDirection *= m_Speed;
	Position = m_ZombieInfo[Number].vPosition + vDirection;
	//m_ZombieInfo[Number].vPosition += vDirection;
	/**/
	/*�����蔻��*/
	float num;
	int hitNum;

	Collider *pColManager = Collider::GetInstance();

	for (DWORD i = 0; i < NumZombies; i++)
	{
		if (Number == i)
		{
			continue;
		}
		//����ѓ��m�̓����蔻��Ƃ��̒���
		pColManager->Distance(Position, GetZombiePosition(i), 5.0f, &num, &Position);
		//
		pColManager->Distance(Position, D3DXVECTOR3(-174.0f, 12.0f, -12.5), 15.0f, &num, &Position);
	}

	if (!pColManager->GroundCol(Position, D3DXVECTOR3(0, 0, 0), &hitNum, &num))
	{
		if (Number < NumZombies)
		{
			if (hitNum != 1)
			{
				float no;
				if (pColManager->RayCast(Position, m_ZombieInfo[Number].vAngle, &no))
				{
					
				}
				else {
					m_ZombieInfo[Number].vPosition = Position;
					m_ZombieInfo[Number].vPosition.y += num;
				}
			}
			//�C�̒n�`��������
			else if (hitNum == 1)
			{
				//�ړ����̒l��߂�
				//	m_ZombieInfo[Number].vPosition = Position - vDirection*2;
			}
		}
		else if (Number >= NumZombies)
		{
			if (hitNum != 0)
			{
				m_ZombieInfo[Number].vPosition = Position;
				m_ZombieInfo[Number].vPosition.y += num;
				//m_ZombieInfo[Number].hitMesh = hitNum;
				D3DXVECTOR2 tmp;
				tmp = m_pMathlib->MyVecAngle(m_ZombieInfo[Number].vPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DX_PI / 1000);
				m_ZombieInfo[Number].vPosition = D3DXVECTOR3(tmp.x, m_ZombieInfo[Number].vPosition.y, tmp.y);
			}
			else if (hitNum == 0)
			{
				//�ړ����̒l��߂�
				m_ZombieInfo[Number].vPosition = Position - vDirection;
			}

			if (pColManager->Distance(Position, D3DXVECTOR3(108.0f, 7.5f, -5.0f), 10)) 
			{
				//�ړ����̒l��߂�
				Position.z = Position.z - vDirection.z;
			}
		}
	}
}

// �ő�ƍŏ��̊Ԃ̒l�������_���ɕԂ��w���p�[�֐�
float CZombie::Random(float min, float max)
{
	return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

// �ő�ƍŏ��̊Ԃ̒l�������_���ɕԂ��w���p�[�֐�
int CZombie::Random(int min, int max)
{
	return lroundf(((float)rand() / (float)RAND_MAX) * (max - min) + min);
}

//====================================
//�֐���	:Update
//����		:playerPos:�v���C���[�̍��W
//			 num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�X�V����
//====================================
void CZombie::Update(D3DXVECTOR3 playerPos, DWORD num)
{

	//double time = m_pAnimation[m_ZombieInfo[num].animNum]->GetTime();
	//�X�V�O�ɃX�e�[�g���`�F�b�N
	//StateCheck(playerPos, m_ZombieInfo[num].vPosition, num);
	Collider* pColManager = Collider::GetInstance();
	float nums;
	int hitNum;


	pColManager->GroundCol(m_ZombieInfo[num].vPosition, D3DXVECTOR3(0, 0, 0), &hitNum, &nums);

	if ((m_ZombieInfo[num].vPosition.y<=11.0f && m_ZombieInfo[num].state !=DEATH ) && num<NumZombies)
	{
		m_ZombieInfo[num].animNum = 1;
		m_ZombieInfo[num].state = WALLOW;
	}
	else if (m_ZombieInfo[num].vPosition.y > 62 && m_ZombieInfo[num].vPosition.z > 25 && m_ZombieInfo[num].vPosition.z < 27 )
	{
		m_pWaterSlider[num]->SetIsSlider(TRUE);
		m_ZombieInfo[num].state = FLOW;
		m_ZombieInfo[num].animNum = 3;	
	}
	else if (pColManager->EasyCheck(m_ZombieInfo[num].vPosition, D3DXVECTOR3(-174.5f, 12.72f, 13.5f), 10.0f) && m_ZombieInfo[num].state != STAIRS)
	{
		//�K�i����鏈��
		m_ZombieInfo[num].state = STAIRS;
		//m_ZombieInfo[num].vPosition = STAIRSPOSITION[0];
	}
	if (m_ZombieInfo[num].isAlive)
	{
		ExcuteState(num,playerPos);
	}
}

//====================================
//�֐���	:ExcuteState
//����		:num:�G�l�~�[�̔ԍ�
//			 playerPos:�v���C���[�̍��W
//�߂�l	:
//���e		:�G�l�~�[�̃X�e�[�g����������ӏ�
//====================================
void CZombie::ExcuteState(int num,D3DXVECTOR3 playerPos)
{

	//�X�e�[�g�ɂ���ē����ύX
	switch (m_ZombieInfo[num].state)
	{
	case CHASER:
		ZombieWalk(playerPos, num);
		break;
	case DEATH:
		ZombieDeathState(num);
		break;
		//����鏈��
	case FLOW:
		ZombieFlowState(num);
		break;
		//�M��鏈��
	case WALLOW:
		ZombieWalowState(num);
		break;
		//�K�i����鏈��
	case STAIRS:
		ZombieWalkUp(num);
		break;
		//�V��łԂ������Ƃ��̏���
	case HITSTOP:
		HitStop(num);
		break;
	case STAIRS_Walk:
		
		break;
	default:
	#ifdef _DEBUG
		assert(!"���݂��Ȃ��X�e�[�g�ł�");
	#endif // DEBUG	
		break;
	}
}

//====================================
//�֐���	:ZombieDeathState
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�G�l�~�[�̎��S���̏���
//====================================
void CZombie::ZombieDeathState(int num)
{
	m_ZombieInfo[num].animNum = 2;
	//if (m_ZombieInfo[num].)
	if (m_pAnimation[m_ZombieInfo[num].skin][m_ZombieInfo[num].animNum]->GetAnimFinishTime() <= m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->GetTime())
	{
		m_ZombieInfo[num].isAlive = FALSE;
		value++;
		CScore::GetInstnce()->SetScoreNum(value);
		m_pDecorate->SetisFlash(TRUE);
		Zombierevival(num);
		m_pWaterSlider[num]->ResetValue();
	}
}

//====================================
//�֐���	:ZombieFlowState
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�G�l�~�[������鎞�ɓ��鏈��
//====================================
void CZombie::ZombieFlowState(int num)
{
	m_pWaterSlider[num]->SetPlayerPos(m_ZombieInfo[num].vPosition);
	m_pWaterSlider[num]->SetPlayerRot(m_ZombieInfo[num].vAngle);
	m_ZombieInfo[num].vPosition = m_pWaterSlider[num]->WaterSliderAction(TRUE);
	m_ZombieInfo[num].DirectionRadian = m_pWaterSlider[num]->GetDirectionRadian();
	m_ZombieInfo[num].ZombieStaNum = 0;
}

//====================================
//�֐���	:ZombieWalowState
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�G�l�~�[���M��鏈��
//====================================
void CZombie::ZombieWalowState(int num)
{
	//
	int v = 0;

	if (m_ZombieInfo[num].LifeTime < 0.0f)
	{
		if (m_ZombieInfo[num].vPosition.y < -10.0f)
		{
			m_ZombieInfo[num].state = DEATH;
		}
		else
		{
			m_ZombieInfo[num].vPosition.y -= 0.1f;
			m_ZombieInfo[num].vPosition.x += 0.2f;
		}
	}
	else
	{
		m_ZombieInfo[num].LifeTime--;
		m_ZombieInfo[num].vPosition.y = 1.0f;

		v = Random(0.0f, 1000.0f);
		if (v % 2 == 0)
		{
			m_ZombieInfo[num].vPosition.z += 0.2f;
		}
		else
		{
			m_ZombieInfo[num].vPosition.x += 0.2f;
		}
	}
	m_ZombieInfo[num].ZombieStaNum = 0;
}

//====================================
//�֐���	:AllZombiCheckCollider
//����		:
//�߂�l	:
//���e		:�G�l�~�[�̃q�b�g�`�F�b�N
//====================================
BOOL CZombie::AllZombiCheckCollider(D3DXVECTOR3 pos,int num)
{
	Collider *pColManager = Collider::GetInstance();
	for (int i = 0; i < MaxZombie; i++) 
	{
		if (num == i)continue;

		if (pColManager->Distance(pos, m_ZombieInfo[i].vPosition, 5.0f)) 
		{
			return TRUE;
		}	
	}
	return FALSE;
}

//====================================
//�֐���	:IsDeadCheck
//����		:
//�߂�l	:
//���e		:�G�l�~�[�̎��S����
//====================================
BOOL CZombie::IsDeadCheck()
{
	for (int i = 0; i < MaxZombie; i++)
	{
		if (m_ZombieInfo[i].isAlive)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//====================================
//�֐���	:ZombieWalkUp
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�]���r���K�i�����Ƃ��̏���
//====================================
void CZombie::ZombieWalkUp(int num)
{
	//�ꎞ�ۑ��p�ϐ�
	D3DXVECTOR3 zPos,prevPos,vDirection,direction;
	//���݂̍��W��ۑ�
	prevPos = m_ZombieInfo[num].vPosition;


	if (m_ZombieInfo[num].isStairs)
	{
		m_ZombieInfo[num].vPosition += ((STAIRSPOSITION[m_ZombieInfo[num].ZombieStaNum + 1] - STAIRSPOSITION[m_ZombieInfo[num].ZombieStaNum]) / dividValue[m_ZombieInfo[num].ZombieStaNum]);

		zPos = m_ZombieInfo[num].vPosition;
		//
		vDirection = zPos - prevPos;

		// �x�N�g���̒P�ʉ�
		D3DXVec3Normalize(&vDirection, &vDirection);
		D3DXVec3Normalize(&m_ZombieInfo[num].vAngle, &m_ZombieInfo[num].vAngle);


		// 2��3D�x�N�g���̓��ς��v�Z
		FLOAT Dot = D3DXVec3Dot(&vDirection, &m_ZombieInfo[num].vAngle);

		m_ZombieInfo[num].DirectionRadian = acosf(Dot);
		//----- �O�� -----//
		D3DXVECTOR3 vNormal;
		// 2��3D�x�N�g���̊O�ς��v�Z
		D3DXVec3Cross(&vNormal, &vDirection, &m_ZombieInfo[num].vAngle);
		// �P�ʉ�
		D3DXVec3Normalize(&vNormal, &vNormal);
		// 0�`�΂܂ł̌ʓx��0�`2�΂܂łɕύX
		if (vNormal.y >= 0.0f)
		{
			m_ZombieInfo[num].DirectionRadian = (D3DX_PI - m_ZombieInfo[num].DirectionRadian) + D3DX_PI;
		}
		m_ZombieInfo[num].z_num++;
		if (m_ZombieInfo[num].z_num >= dividValue[m_ZombieInfo[num].ZombieStaNum])
		{
			m_ZombieInfo[num].ZombieStaNum++;
			m_ZombieInfo[num].z_num = 0;
			if (m_ZombieInfo[num].ZombieStaNum >= 3)
			{
				m_ZombieInfo[num].z_num = 0;
				m_ZombieInfo[num].state = FLOW;
			}
		}
	}
	else
	{

		m_ZombieInfo[num].vPosition+=(STAIRSPOSITION[0] - m_ZombieInfo[num].vPosition) / 10.0f;
		m_ZombieInfo[num].val++;
		if (m_ZombieInfo[num].val==10)
		{
			m_ZombieInfo[num].isStairs = TRUE;
			m_ZombieInfo[num].val = 0;
		}



	}
}

//====================================
//�֐���	:Zombierevival
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�]���r�̕�������
//====================================
void CZombie::Zombierevival(int num)
{
	//�]���r�ĕ���
	m_ZombieInfo[num].isAlive = TRUE;
	m_ZombieInfo[num].state = CHASER;
	int value = (int)Random(0.0f, 10000.0f);
	if (value % 3 == 0)
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(200.0f,13.0f,Random(-42,43));
	}
	else if (value % 3 == 1)
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-45.0f, 45.0f), 13.0f, 195.45f);
	}
	else
	{
		m_ZombieInfo[num].vPosition = D3DXVECTOR3(Random(-45.0f, 45.0f), 13.0f, -200.155f);
	}
	/*/���S���̐�������*/
	m_ZombieInfo[num].LifeTime = 120.0f;
	m_ZombieInfo[num].animNum = 0;
	m_ZombieInfo[num].ZombieStaNum = 0;
	m_ZombieInfo[num].z_num = 0;
	m_ZombieInfo[num].DirectionRadian = 0.0f;
	m_ZombieInfo[num].ZombieStaNum = 0;
	m_ZombieInfo[num].isStairs = FALSE;
	m_ZombieInfo[num].val = 0;
}

//====================================
//�֐���	:ResetZombieInfo
//����		:
//�߂�l	:
//���e		:�]���r�̏�������������(�Q�[�����Z�b�g���g�p)
//====================================
void CZombie::ResetZombieInfo()
{
	ZombieInfoInitialize();
	for (int i = 0; i < NumZombies; i++)
	{
		m_pWaterSlider[i]->SetIsSlider(FALSE);
		m_pWaterSlider[i]->ResetValue();
	}
	value = 0;
}

//====================================
//�֐���	:HitStop
//����		:num:�G�l�~�[�̔ԍ�
//�߂�l	:
//���e		:�]���r���V��ɓ��������ۂɎ~�܂鏈��
//====================================
void CZombie::HitStop(int num)
{
	if (m_pAnimation[m_ZombieInfo[num].skin][m_ZombieInfo[num].animNum]->GetAnimFinishTime() <= m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->GetTime())
	{
		m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->ResetTime();
		//m_pAnimation[m_ZombieInfo[num].animNum]->SetTime(0.15f);
		m_ZombieInfo[num].animNum = 0;
		m_ZombieInfo[num].state = CHASER;
	}
}

void CZombie::STAIRS_WalkState(int num)
{

}


//void CZombie::StateCheck(D3DXVECTOR3 playerPos, D3DXVECTOR3 enemyPos,DWORD num)
//{
//	//�ꎞ�ۑ��p�ϐ�
//	double temp,temp2,temp3,finaltemp, vTemp,angle;
//
//	//�ŏ��p�x
//	temp2 = atan2(100.0f,100.0f+enemyPos.x);
//	temp2 = D3DXToDegree(temp2);
//	//�ő�p�x
//	temp3 = atan2(100.0f , enemyPos.x-100.0f);
//	temp3 = D3DXToDegree(temp3);
//	//finaltemp = temp3 - temp2;
//
//	//���݂̃v���C���[�̈ʒu�ƓG�̈ʒu�̊p�x���`�F�b�N
//	temp=atan2((playerPos.z - enemyPos.z), (playerPos.x - enemyPos.x));
//	temp=D3DXToDegree(temp);
//	//�G�l�~�[�ƃv���C���[�Ƃ̋����̍������o
//	vTemp = sqrt(pow((playerPos.z - enemyPos.z), 2) + pow((playerPos.x - enemyPos.x), 2));
//	//��Βl
//	vTemp = abs(vTemp);
//	//�����̒��������o
//	if ((temp>temp2 && temp<temp3) && (vTemp>0 && vTemp<100))
//	{
//		//�ǐՃ��[�h�ɕύX
//		m_ZombieInfo[num].state = CHASER;
//	}
//	else
//	{
//		//���[�h�ɕύX
//		m_ZombieInfo[num].state = CHASER;
//	}
//}