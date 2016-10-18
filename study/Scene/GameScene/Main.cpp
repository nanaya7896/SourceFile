#include "Main.h"

//====================================
//�֐���	:CMain
//����		:pDevice9	:�`��f�o�C�X
//			 pField		:�t�B�[���h
//			 byKeyState :�L�[�̊i�[���
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CMain::CMain(/*LPDIRECT3DDEVICE9 pDevice9,CDInput* pInput*/)
{
	m_pPlayer = nullptr;
	m_pField = nullptr;
	m_Ready = TRUE;
	m_Go = FALSE;
	m_pDecorate = NULL;
	//m_Value = D3DXVECTOR2(0.05f,0.05f);
	m_Alpha = D3DXVECTOR2(0.0f, 0.0f);
	CScore::Create();
	m_Height = 250.0f;
	//���ԗp�̃e�N�X�`���ԍ�(�����ݒ�60�b)
	m_Time = 90;
	
}

//====================================
//�֐���	:~CMain
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CMain::~CMain()
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pField);
	SAFE_DELETE(m_pZombie);
	SAFE_DELETE(m_pDecorate);
	CScore::GetInstnce()->Destroy();
}

//====================================
//�֐���	:CreateResource
//����		:pDevice9:�`��f�o�C�X
//�߂�l	:
//���e		:�쐬����x�����Ă΂��
//====================================
void CMain::CreateResource(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput, CSoundManager* pSManager)
{
	//�`��f�o�C�X
	m_pDevice9 = pDevice9;
	m_pInput = pInput;
	m_pSoundManager = pSManager;
	//Player�̃C���X�^���X�̐���
	m_pPlayer = (CPlayer*)m_ActorFactory.CreateActor("Player");
	m_pPlayer->CreateResource(m_pDevice9, m_pInput, m_pSoundManager,m_pPlayer);
	/*�r���ǉ�*/
	m_pPlayer->SetWeapon(&m_pWeapon);
	//�}�b�v�e�X�g�p
	m_pField = new CFiled(m_pDevice9);
	m_pField->CreateResource(m_pDevice9);
	//�]���r
	m_pZombie = new CZombie(m_pDevice9);
	m_pZombie->Initialize();
	m_pDecorate = new CDecorate();
	
}

//====================================
//�֐���	:AllGameReset
//����		:
//�߂�l	:
//���e		:����������
//====================================
void CMain::AllGameReset()
{
	m_Time = 90;
	m_Ready = TRUE;
	m_Go = FALSE;
	m_Height = 250.0f;
	m_Value = D3DXVECTOR2(0.02f, 0.02f);
	m_Alpha = D3DXVECTOR2(0.0f, 0.0f);
	/*�v���C���[�̏�����*/
	m_pPlayer->ResetPlayer();
	m_pZombie->ResetZombieInfo();
	//CCamera::GetInstance()->SetViewAngle(D3DXVECTOR3(, 0.0f, 0.0f));
	//m_pZombie->ResetZombieInfo();
	CScore::GetInstnce()->ResetScore();
	m_pDecorate->ResetDeco();
}

//====================================
//�֐���	:Upadate
//����		:
//�߂�l	:
//���e		:�X�V
//====================================
void CMain::Update()
{
	if (!m_Ready && !m_Go)
	{
		//Player�X�V
		m_pPlayer->Update();
		//
		for (int i = 0; i < MaxZombie; i++)
		{
			if (AllHitCheck(i))
			{
				//���b�~�܂�A�j���[�V����
				m_pZombie->SetScale(i, { 0.1f,0.1f,0.1f });
				m_pZombie->SetAniamtionNum(i, 6);
				m_pZombie->SetState(i, HITSTOP);
			}
			else if (BBHitCheck(i) && m_BB.GetState() == Air)
			{
				//���ʏ���
				m_pZombie->SetAniamtionNum(i, 2);
				m_pZombie->SetState(i, DEATH);
			}
			else if (m_pZombie->GetState(i) != HITSTOP)
			{
				m_pZombie->SetScale(i, { 1.2f,1.2f,1.2f });
			}
			//
			m_pZombie->Update(m_pPlayer->GetPlayerPos(), i);
		}

		m_pField->Update();
		CScore::GetInstnce()->Update();
		/*���Ԃ̍X�V*/
		if (!m_pPlayer->GetSlider())
		{
			TimeFrame++;
		}
		//GetWeapon();
		//if (m_pPlayer->GetWeapon() == NULL)
		//{
		//	//MyOutputDebugString("�E���܂�\n");
		//	AllGetWeapon();
		//}
		//else
		//{
		//	//MyOutputDebugString("�u���܂�\n");
		//	AllPutWeapon();
		//}
		//m_BB.BUpdate({ m_pPlayer->GetPlayerPos().x,m_pPlayer->GetPlayerPos().y + 10.0f ,m_pPlayer->GetPlayerPos().z }, m_pPlayer->GetPlayerAngle());
		//if ((m_pInput->KeyState(DIK_SPACE)) || m_pInput->IsButtonDown(CIRCLE_BUTTON))
		//{
		//	//if(m_pPlayer->GetWeapon() == &m_BB)
		//	//{
		//	//	//m_pPlayer->SetPlayerState(STATE_BB_SHOT);
		//	//}
		//	//else if(m_pPlayer->GetWeapon() == &m_BG)
		//	//{
		//	//	//m_pPlayer->SetPlayerState(STATE_BG_SHOT);
		//	//}
		//	//else if(m_pPlayer->GetWeapon() == &m_WG)
		//	//{
		//	////	m_pPlayer->SetPlayerState(STATE_WG_SHOT);
		//	//}
		//	m_BG.SetWPos({ m_pPlayer->GetPlayerPos().x,m_pPlayer->GetPlayerPos().y + 5.0f,m_pPlayer->GetPlayerPos().z });
		//	m_BG.SetRot(m_pPlayer->GetPlayerAngle());
		//}
		for (int i = 0; i < 53; i++)
		{
			m_pDecorate->Update(m_pPlayer->GetPlayerPos(), m_pZombie->GetZombiePosition(i));
		}
		//�f�o�b�N�p�`�[�g�{�^��
		//if (m_pInput->KeyState(DIK_L))
		//{
		//	TimeFrame = 60;
		//}
		
		/*�X�V�����I��*/
	}
	else
	{
		if (m_Ready == TRUE)
		{
			m_Alpha.x += m_Value.x;
		}
		if (m_Alpha.x >= 1.0f)
		{
			m_Ready = FALSE;
			m_Go = TRUE;
		}
		if (m_Alpha.x >= 1.0f)
		{
			m_Value.x = -1.0f;

		}
		if (m_Go == TRUE)
		{
			m_Alpha.y += m_Value.y;
		}
		if (m_Alpha.y >= 1.0f)
		{
			m_Value.y = -1.0f;
		}
	}
}

//====================================
//�֐���	:Draw
//����		:
//�߂�l	:
//���e		:�`��
//====================================
void CMain::Draw()
{
	m_pField->Draw();
	m_pPlayer->Draw();
	m_pDecorate->FalshScreen();
	for (int i = 0; i < MaxZombie; i++)
	{
		m_pZombie->Draw(i);
		m_pZombie->AnimationPlay(i);
	}
	//m_BB.Draw();
	//m_BG.Draw();
	//m_WG.Draw();
	ZombieValueDraw();
	TimeUpdate(MAX_DIGIT, TRUE);

	if (!m_Ready && !m_Go)
	{

	}
	else
	{
		ReadyDraw();
	}

}

//====================================
//�֐���	:TimeUpdate
//����		:digit: ����
//			:isPull:���Z�����Z��
//�߂�l	:
//���e		:���ԍX�V
//====================================
void CMain::TimeUpdate(int digit, BOOL isPull)
{
	float posValue = 9.5f;
	//�ꎞ���ԕۑ��p
	int tmp;
	if (TimeFrame >= 60)
	{
		/*������ ? �^ : �U*/
		(isPull) ? m_Time-- : m_Time++;
		TimeFrame = 0;
	}

	for (int i = 0; i < digit; i++)
	{
		tmp = m_Time / (int)pow(10, i);
		tmp %= 10;
		//�e�N�X�`���̃C���X�^���X����
		CTextureManager* pTextureManager = CTextureManager::GetInstance();
		float pos = (WINDOW_WIDTH / 20.0f)*posValue;
		pTextureManager->Draw(tmp, D3DXVECTOR3(pos, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		posValue -= 1.2f;
	}
}

//====================================
//�֐���	:ZombieValueDraw
//����		:
//�߂�l	:
//���e		:�]���r�̓|���ꂽ���𐔂���
//====================================
void CMain::ZombieValueDraw()
{

	//�e�N�X�`���̃C���X�^���X����
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(23, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 0.1f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(200.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(0), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(1), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 4.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(CScore::GetInstnce()->GetScore(2), D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 3.0f, 5.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	m_pResult->SetResultScore(0, CScore::GetInstnce()->GetScore(0));
	m_pResult->SetResultScore(1, CScore::GetInstnce()->GetScore(1));
	m_pResult->SetResultScore(2, CScore::GetInstnce()->GetScore(2));
}

void CMain::ReadyDraw()
{
	//�e�N�X�`���̃C���X�^���X����
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	//���f�B�[
	if (m_Ready)
	{
		if (m_Height<275.0f)
		{
			m_Height += 2.0f;
		}
		pTextureManager->Draw(19, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.5f, m_Height, 0.0f), 1, m_Alpha.x, D3DXVECTOR3(600.0f, 200.0f, 0.0f));
	}
	if (m_Go)
	{
		//�S�[
		pTextureManager->Draw(20, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 5.5f, 275.0f, 0.0f), 1, m_Alpha.y, D3DXVECTOR3(600.0f, 200.0f, 0.0f));
		time += 1;
		if (m_Alpha.y <= 0.0f)
		{
			m_Go = FALSE;
			time = 0;
			m_Value.x = 0.01f;
		}
	}
}

//====================================
//�֐���	:PlayerAndEnemyHitCheck
//����		:
//�߂�l	:TRUE:�������� FALSE:�������Ă��Ȃ�
//���e		:�v���C���[�ƃ]���r���ڐG���Ă��邩�`�F�b�N
//====================================
BOOL CMain::PlayerAndEnemyHitCheck()
{
	float deadDistance;

	if (m_pPlayer->GetHitMapNum() == 3) {
		deadDistance = 3.5f;
	}
	else {
		deadDistance = 1.0f;
	}
	static bool deadFlag = TRUE;

	if (m_pPlayer->GetEnterMesh(3)) {
		deadFlag = FALSE;
	}
	for(int i = 0; i < MaxZombie; i++)
	{
		if (Collider::GetInstance()->EasyCheck(m_pPlayer->GetPlayerPos(), m_pZombie->GetZombiePosition(i), deadDistance))
		{
			if (!deadFlag) {
				return FALSE;
			}
			return TRUE;
		}
	}
	deadFlag = TRUE;
	return FALSE;
}

BOOL CMain::isDeadCheck()
{
	if (m_pZombie->IsDeadCheck())
	{
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:GetLimitTime
//����		:
//�߂�l	:
//���e		:�^�C�����O�̂Ȃ���������
//====================================
BOOL CMain::GetLimitTime()
{
	return (m_Time == 0) ? TRUE : FALSE;
}

//====================================
//�֐���	:AllGetWeapon
//����		:
//�߂�l	:
//���e		:�������ɓ���鏈��
//====================================
void CMain::AllGetWeapon()
{
	if ((m_pInput->KeyDown(DIK_G)) || (m_pInput->IsButtonDown(SQUARE_BUTTON)))
	{
		if (m_BB.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_BB);
			return;
		}
		else if (m_BG.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_BG);
			return;
		}
		else if (m_WG.GetWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(&m_WG);
			return;
		}
	}
	m_pPlayer->SetHaveWeaponInfo(GetWeaponNumber());
}

//====================================
//�֐���	:AllPutWeapon
//����		:
//�߂�l	:
//���e		:�����n�ʂɒu������
//====================================
void CMain::AllPutWeapon()
{
	if ((m_pInput->KeyDown(DIK_H)) || (m_pInput->IsButtonDown(X_MARK_BUTTON)))
	{
		if (m_BB.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
		else if (m_BG.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
		else if (m_WG.PutWeapon(m_pPlayer->GetPlayerPos()))
		{
			m_pPlayer->SetWeapon(NULL);
			return;
		}
	}
	m_pPlayer->SetHaveWeaponInfo(GetWeaponNumber());
}

//====================================
//�֐���	:AllHitCheck
//����		:num:�]���r�̔ԍ�
//�߂�l	:
//���e		:�V��̂����蔻��(�r�[�`�{�[���ȊO)
//====================================
BOOL CMain::AllHitCheck(int num)
{

	if (m_WG.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		return TRUE;
	}
	if (m_BG.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		m_pSoundManager->Stop(8);
		m_pSoundManager->Play(8, FALSE);
		return TRUE;
	}
	return FALSE;
}

//====================================
//�֐���	:BBHitCheck
//����		:num : �]���r�̔ԍ�
//�߂�l	:
//���e		:�r�[�`�{�[���ƃ]���r�̂����蔻��
//====================================
BOOL CMain::BBHitCheck(int num)
{

	if (m_BB.HitCheck(m_pZombie->GetZombiePosition(num)))
	{
		return TRUE;
	}

	return FALSE;
}