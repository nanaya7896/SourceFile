#include "Scene.h"

//=====define�l====//

/*
	�R���g���[���[
	m_pJoyState->rgbButtons[]
	��:0	L2:6		PS�{�^��:12
	�~:1	R2:7		�X���C�h�p�b�h:13
	�Z:2	SHARE:8
	��:3	OPTIONS:9
	L1:4	L3:10
	R1:5	R3:11
*/

//====================================
//�֐���	:CScene
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CScene::CScene()
{
	m_pJoyState = NULL;
	m_byKeyState = NULL;
	m_pDSystem = NULL;
	m_pFont = NULL;
	m_pTitle = NULL;
	m_pOperat = NULL;
	m_pMain = NULL;
	m_pGameOver = NULL;
	m_pResult = NULL;
	m_pCredit = NULL;
	m_isOnce = TRUE;
	m_NowGameScene = SCENE::TITLE;
	//m_NowGameScene = SCENE::MAIN;
	//m_NowGameScene = SCENE::GAMEOVER;
	//CameraPos = D3DXVECTOR3(0,-500,0);
	//CameraRot = D3DXVECTOR3(0, 0, 0);
}

//====================================
//�֐���	:~CScene
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CScene::~CScene()
{
	SAFE_DELETE(m_pDSystem);
	
	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pOperat);
	SAFE_DELETE(m_pMain);
	SAFE_DELETE(m_pResult);
	SAFE_DELETE(m_pGameOver);
	SAFE_DELETE(m_pCredit);
	SAFE_DELETE(m_pRanking);

	CCamera::Destroy();
	//ModelManager�̃f���[�g�p�ɒǉ�
	CModelManager* pModelManager = CModelManager::GetInstance();
	pModelManager->Delete();
	//TextureManager�̃f���[�g
	CTextureManager* pTexturemanager = CTextureManager::GetInstance();
	pTexturemanager->Delete();
	//�C���X�^���X�̃f�X�g���C
	CTextureManager::Destroy();
	CModelManager::Destory();
	//�����蔻��̏���
	Collider::Destory();
	//�t�F�[�h������
	Fade::Destory();
}

//====================================
//�֐���	:CleanupObject
//����		:
//�߂�l	:
//���e		:
//====================================
void CScene::CleanupObject()
{
		
}

//====================================
//�֐���	:CreateObject
//����		:
//�߂�l	:
//���e		:
//====================================
BOOL CScene::CreateObject()
{
	return TRUE;
}

//====================================
//�֐���	:AllGameInitialize
//����		:
//�߂�l	:
//���e		:�Q�[���̏�����
//====================================
void CScene::AllGameInitialize()
{
	m_pTitle->AllReset();
	m_pMain->AllGameReset();
	//�T�E���h�����ׂĎ~�߂�
	m_pSoundManager->StopAllSound();
	//�T�E���h0�𗬂�
	m_pSoundManager->Play(0, TRUE);
	m_isOnce = TRUE;
}

//====================================
//�֐���	:CreateResource
//����		:
//�߂�l	:
//���e		:���\�[�X�t�@�C���̍쐬(��x�����Ă΂�Ȃ�)
//====================================
BOOL CScene::CreateResource(LPDIRECT3DDEVICE9 pDevice9, HWND hWnd, BYTE* byKeyState, DIJOYSTATE2* JoyStick,CDXFont* pFont, CDInput* pInput)
{

	m_pDevice9 = pDevice9;
	m_pFont = pFont;
	m_pDInput = pInput;
	m_pJoyState = JoyStick;
	m_byKeyState = byKeyState;
	//�J�����̐���
	CCamera::Create(m_pDevice9, hWnd);
	//================================
	//
	//�e�V�X�e���𓮓I�Ƀ������m��
	//
	//================================
	//DirectSystem�̍쐬
	m_pDSystem = new CDSystem(m_pDevice9);
	//�T�E���h�}�l�[�W���[
	m_pDSystem->CreateSoundManager(hWnd);
	//�T�E���h�}�l�[�W���[�̎擾

	m_pSoundManager = m_pDSystem->GetSoundManager();

	//�e�N�X�`���}�l�[�W���[
	CTextureManager::Create(m_pDevice9);
	//�e�N�X�`���̈ꊇ�擾
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->AllLoadTexture();
	//���f���}�l�[�W���[
	CModelManager::Create(m_pDevice9);
	CModelManager* pModelManager = CModelManager::GetInstance();
	//���f���̈ꊇ�ǂݍ���
	pModelManager->AllLoadXfile();
	//�t�F�[�h�C���A�E�g�̏���
	Fade::Create();

	/*�����蔻��*/
	Collider::Create();
	Collider* pColManager = Collider::GetInstance();
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		D3DXVECTOR3 tmp = D3DXVECTOR3(0, 0, 0);
		if (i == 3)tmp = D3DXVECTOR3(-166.0f, 13.0f, -25.0f);
		if (i == 12)tmp = D3DXVECTOR3(29.0f, 7.0f, 111.0f);
		pColManager->SetObj(tmp, pModelManager->GetMesh(i));
	}


	//�V�[��
	m_pTitle = new CTitle(m_pDevice9);
	m_pOperat = new COperating(m_pDevice9);
	m_pMain = new CMain();
	m_pMain->CreateResource(m_pDevice9, m_pDInput,m_pSoundManager);
	m_pGameOver = new CGameOver(m_pDevice9);
	m_pResult = new CResult(m_pDevice9, m_pDInput);
	m_pCredit = new CCredit(m_pDevice9);
	m_pRanking = new CRanking();

	m_pMain->SetResult(m_pResult);
	//�Q�[���̏�����
	AllGameInitialize();


	return TRUE;
}

//====================================
//�֐���	:SceneChange
//����		:
//�߂�l	:
//���e		:�V�[���̕ύX
//====================================
void CScene::SceneChange()
{

	Fade* pFadeManager = Fade::GetInstance();
	static BOOL fadeCheck = FALSE;

	switch (m_NowGameScene)
	{
	case SCENE::TITLE:
		//�X�V
		if (fadeCheck) {
			if (pFadeManager->Update(TRUE, 0.5f)) {
				fadeCheck = FALSE;
				m_pSoundManager->Play(5, FALSE);
				m_NowGameScene = SCENE::OPERATE;
			}
		}
		else {
			pFadeManager->Update(FALSE, 1.5f);
		}

		m_pTitle->Update();
		//�`��
		m_pTitle->Draw();
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			fadeCheck = TRUE;
		}

		m_pMain->AllGameReset();
		break;

	case SCENE::OPERATE:

		//���y
		if (fadeCheck) {
			if (pFadeManager->Update(TRUE, 0.3f)) {
				fadeCheck = FALSE;
				m_pSoundManager->Play(5, FALSE);
				m_pSoundManager->StopAllSound();
				//���C���Q�[�����y
				m_pSoundManager->Play(1, TRUE);
				m_pSoundManager->Play(9, FALSE);

				m_NowGameScene = SCENE::MAIN;
			}
		}
		else {
			pFadeManager->Update(FALSE, 0.3f);
		}
		m_pOperat->Update();
		//�`��
		m_pOperat->Draw();
		//�����{�^����������Ă�����
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			fadeCheck = TRUE;
		}
		break;

	case SCENE::MAIN:
		pFadeManager->Update(FALSE, 0.3f);
		m_pDInput->GetJoyStickState(m_pJoyState);
		if (m_pMain->GetisSlider())
		{
			CCamera::GetInstance()->SetDistance(5.0f);
			CCamera::GetInstance()->SetCamera(D3DXVECTOR3(0.0f,240.0f,-50.0f));
			CCamera::GetInstance()->SetViewAngle(D3DXVECTOR3(D3DX_PI / 4, 0.0f, 0.0f));
			if (m_isOnce)
			{
				m_pSoundManager->Play(13, FALSE);
				m_isOnce = FALSE;
			}
		}
		else 
		{
			m_isOnce = TRUE;
			//CCamera::GetInstance()->SetDistance(25.0f);
			CCamera::GetInstance()->SetCamera(m_pMain->GetpPlayer()->GetPlayerPos());
		}
		//�G�ƐڐG������
	#ifndef _DEBUG
		if (m_pMain->PlayerAndEnemyHitCheck())
		{
			pFadeManager->Update(TRUE, 0.0);
			m_pSoundManager->StopAllSound();
			m_pSoundManager->Play(2, TRUE);
			m_NowGameScene = SCENE::GAMEOVER;
			m_pMain->AllGameReset();
			break;
		}
	#endif // ! _DEBUG



		//�X�V
		m_pMain->Update();
		//�`��
		m_pMain->Draw();

		//�������Ԃ��I��������
		if (m_pMain->GetLimitTime())
		{
			pFadeManager->Update(TRUE, 0.0);
			m_pSoundManager->StopAllSound();
			m_pSoundManager->Play(3, TRUE);
			m_NowGameScene = SCENE::RESULT;
			break;
		}

		break;
	case SCENE::GAMEOVER:
		//�X�V
		m_pGameOver->Update();
		pFadeManager->Update(FALSE, 0.5f);

		if (m_pDInput->KeyDown(DIK_LEFT) || (m_pDInput->LAnalogState(2)))
		{
			m_pSoundManager->Play(5, FALSE);
			m_pGameOver->isArrowPos(TRUE);
		}
		else if (m_pDInput->KeyDown(DIK_RIGHT) || (m_pDInput->LAnalogState(3)))
		{
			m_pSoundManager->Play(5, FALSE);

			m_pGameOver->isArrowPos(FALSE);
		}
		//�`��
		m_pGameOver->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			if (m_pGameOver->GetReturnPos() == TRUE)
			{
				//�^�C�g���ɖ߂鏈��
				AllGameInitialize();
				m_pGameOver->ResetValue();
				m_NowGameScene = SCENE::TITLE;
			}
			else
			{
				//���g���C�̏���
				m_pMain->AllGameReset();
				m_pSoundManager->StopAllSound();
				m_pSoundManager->Play(1, TRUE);
				m_pSoundManager->Play(9, FALSE);
				m_pGameOver->ResetValue();
				m_NowGameScene = SCENE::MAIN;
			}
		}


		break;

	case SCENE::RESULT:
		//�X�V
		m_pResult->Update();
		//�`��
		m_pResult->Draw();

		
		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_pRanking->RankinCheck();
			m_NowGameScene = SCENE::RANKING;
		}
		break;
	case SCENE::RANKING:

		m_pRanking->Update();
		m_pRanking->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_NowGameScene = SCENE::CREDIT;
		}
		break;

	case SCENE::CREDIT:
		//�X�V
		m_pCredit->Update();
		//�`��
		m_pCredit->Draw();

		if ((m_pDInput->KeyDown(DIK_RETURN)) || (m_pDInput->IsButtonPressed(CIRCLE_BUTTON)))
		{
			m_pTitle->AllReset();
			AllGameInitialize();
			m_NowGameScene = SCENE::TITLE;
		}
		break;
	default:
		m_NowGameScene = SCENE::TITLE;
	}
	pFadeManager->Draw();
}

//====================================
//�֐���	:OnLostDevice
//����		:
//�߂�l	:
//���e		:
//====================================
void CScene::OnLostDevice()
{
	
}

//====================================
//�֐���	:OnResetDevice
//����		:
//�߂�l	:
//���e		:
//====================================
void CScene::OnResetDevice()
{

}




