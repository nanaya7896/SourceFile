#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Player/Player.h"
#include"../Score.h"
#include"../../Camera/Camera.h"
#include"../../Lib/DirectX/DInput.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
#include"../Field.h"
#include"../../Graphic/DirectXAnimation/AllocateHierarchy.h"
#include"../../Zombie/Zombie.h"
#include"Result.h"
#include"../../Design/Decorate.h"
#include"../../Weapon/AbstractWeapon.h"
#include"../../Weapon/BB.h"
#include"../../Weapon/BG.h"
#include"../../Weapon/WG.h"
#include"../../Actor/ActorFactory.h"

//=====define�l=====//
#define MAX_TIME		120
#define MAX_DIGIT		2
#define ZOMBIE_NUM		20
//=====�N���X�錾=====//
class CMain
{
	//private�����o�ϐ�
private:
	LPDIRECT3DDEVICE9		m_pDevice9;
	//�v���C���[�̓ǂݍ���
	CPlayer*				m_pPlayer;
	//�L�[���͗p
	CDInput*				m_pInput;
		//�t�B�[���h�`��p
	CFiled*					m_pField;
	//���U���g
	CResult*				m_pResult;
	//�]���r
	CZombie*				m_pZombie;
	//�T�E���h�}�l�[�W���[
	CSoundManager*			m_pSoundManager;
	//�f�R���[�^�[
	CDecorate*				m_pDecorate;
	//����Ǘ��N���X
	CAbstractWeapon	m_pWeapon;
	//�r�[�`�{�[��
	Cbb		m_BB;
	//���S�C
	Cwg     m_WG;
	//�V���{����
	Cbg		m_BG;

	CActorFactory			 m_ActorFactory;

	//���Ԑi�s�p
	int TimeFrame;

	//���ԊǗ�
	int m_Time;

	BOOL m_Ready;
	BOOL m_Go;
	int time;
	//
	D3DXVECTOR2 m_Alpha;
	D3DXVECTOR2 m_Value;
	float       m_Height;
	//public�����o�ϐ�
public:

	//private�����o�֐�
private:
	enum HavePlayer
	{
		None,
		BBall,
		WGun,
		BGun,


	};

	//public�����o�֐�
public:
	//�R���X�g���N�^
	CMain(/*LPDIRECT3DDEVICE9, CDInput**/);
	//�f�X�g���N�^
	~CMain();
	//���\�[�X�쐬
	void CreateResource(LPDIRECT3DDEVICE9,CDInput*,CSoundManager*);
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�Q�[���̃��Z�b�g
	void AllGameReset();
	//���ԍX�V
	void TimeUpdate(int digit, BOOL isPull);
	//�y���񂾃]���r�̐���`��
	void ZombieValueDraw();
	//Ready����
	void ReadyDraw();

	BOOL PlayerAndEnemyHitCheck();
	BOOL isDeadCheck();

	void AllGetWeapon();
	void AllPutWeapon();
	BOOL AllHitCheck(int num);
	BOOL BBHitCheck(int num);

	void SetResult(CResult* pResult)
	{
		m_pResult = pResult;
	}
	//�Q�b�g�֐�
	BOOL GetLimitTime();
	CPlayer* GetpPlayer() 
	{
		return m_pPlayer; 
	}

	BOOL GetisSlider()
	{
		return m_pPlayer->GetSlider();
	}
	int GetisNum()
	{
		return m_pPlayer->GetNum();
	}

	int GetWeaponNumber()
	{
		if (m_pPlayer->GetWeapon() == NULL){return None;}//���������Ă��Ȃ�
		if (m_pPlayer->GetWeapon() == &m_BB){return BBall;}
		if (m_pPlayer->GetWeapon() == &m_WG){return WGun;}
		if (m_pPlayer->GetWeapon() == &m_BG){return BGun;}
	}

};
