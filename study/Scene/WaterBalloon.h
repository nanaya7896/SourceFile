#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/DirectX/DInput.h"

#include"../Lib/Collider.h"

//#define GROUND		0
//#define HAVE		1

//=====�N���X�錾=====//
class CWaterBalloon
{
	//private�����o�ϐ�
private:
	static CWaterBalloon* m_pCWaterBalloonInstance;

	LPDIRECT3DDEVICE9	m_pDevice9;
	CDInput*			m_pDInput;

	//���˂��ꂽ��
	BOOL m_isShot;

	//���W
	D3DXVECTOR3 m_waterPos;
	//�v���C���[�̍��W�E�A���O��
	D3DXVECTOR3 m_playerPos;
	D3DXVECTOR3 m_playerRot;
	//
	D3DXVECTOR3 gunPos;

	float value;

	int state;
	//public�����o�ϐ�
public:
	//

	//private�����o�֐�
private:


	//public�����o�֐�
public:
	CWaterBalloon();
	~CWaterBalloon();

	static void Create();//�C���X�^���X�𐶐�����
	static void Destory();//�C���X�^���X���폜

						  //�C���X�^���X���擾����
	static CWaterBalloon* GetInstance() {
		return m_pCWaterBalloonInstance;
	}

	void CreateResource(LPDIRECT3DDEVICE9, CDInput*);
	void UpdateWater();
	void Update(D3DXVECTOR3 vPos, D3DXVECTOR3 vRot);
	void Draw();

	BOOL GetWaterGun(D3DXVECTOR3 vPos, D3DXVECTOR3 vRot);
	void DisGun();
	BOOL HitCheck(D3DXVECTOR3 checkPos);

	FLOAT ChangeTF(FLOAT value, FLOAT rot, BOOL isSin);

	void ResetWaterBullon();
	//=====�Z�b�g�֐�=====/
	void SetisShot(BOOL isBool)
	{
		m_isShot = isBool;
	}
	void SetGunPos(D3DXVECTOR3 pos)
	{
		gunPos = pos;
	}

	void SetPlayerPos(D3DXVECTOR3 pos)
	{
		m_playerPos = pos;
	}

	void SetPlayerRot(D3DXVECTOR3 rot)
	{
		if (!m_isShot)
			m_playerRot = rot;
	}

	void SetWaterPos(D3DXVECTOR3 pos)
	{
		if (!m_isShot)
			m_waterPos = pos;
	}

	void SetState(int st)
	{
		state = st;
	}
	//=====�Q�b�g�֐�=====//
	int GetState()
	{
		return state;
	}
	BOOL Getishot()
	{
		return m_isShot;
	}
};