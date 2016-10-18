#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/Fade.h"
#include"../Lib/DirectX/DInput.h"
#include"../Lib/DirectX/DXFont.h"
#include"GameScene/Title.h"
#include"GameScene/Operating.h"
#include"GameScene/Main.h"
#include"GameScene/Result.h"
#include"GameScene/GameOver.h"
#include"GameScene/Credit.h"
#include"GameScene/Ranking.h"

//=====�N���X�錾=====//
class CScene
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;
	LPDIRECT3DTEXTURE9      m_ShadowMapTexture;
	////�t�H���g
	CDXFont*				m_pFont;
	//�L�[���͊i�[�p�z��
	BYTE*					m_byKeyState;
	//�W���C�X�e�B�b�N�̏��
	DIJOYSTATE2*			m_pJoyState;
	//�e�N�X�`���E�T�E���h�̃|�C���^
	CDSystem*				m_pDSystem;
	//�L�[���
	CDInput*				m_pDInput;
	//�T�E���h�}�l�[�W���[
	CSoundManager*			m_pSoundManager;
	//���݂̃Q�[���V�[��
	DWORD					m_NowGameScene;

	//�}�b�v
	//=====�e�V�[���̓ǂݍ���=====//
	CTitle*					m_pTitle;
	COperating*				m_pOperat;
	CMain*					m_pMain;
	CResult*				m_pResult;
	CGameOver*				m_pGameOver;
	CCredit*				m_pCredit;
	CRanking*				m_pRanking;


	//�T�E���h�����炷�p
	BOOL					m_isOnce;
	////public�����o�ϐ�
public:
	D3DXVECTOR3 CameraPos;
	D3DXVECTOR3 CameraRot;
	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CScene();
	//�f�X�g���N�^
	~CScene();

	//=====�e�����̊֐�=====//
	void CleanupObject();
	BOOL CreateObject();
	BOOL CreateResource(LPDIRECT3DDEVICE9, HWND, BYTE*, DIJOYSTATE2*, CDXFont*, CDInput*);


	//�Q�[����Ԃ̏�����
	void AllGameInitialize();
	//���C���Q�[���̎��s����
	void MainGame();
	//�Q�[�������̓���
	void SceneChange();
	//

	void OnLostDevice();
	void OnResetDevice();
};