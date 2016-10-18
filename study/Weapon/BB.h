/*

�r�[�`�{�[���̏���

*/

#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"AbstractWeapon.h"

//=====�N���X�錾=====//
class Cbb : public CAbstractWeapon
{
	//private�����o�ϐ�
private:
	D3DXVECTOR3 m_BeachBallPos;
	D3DXVECTOR3 m_BeachBallRot;
	int			m_State;



	D3DXVECTOR3 direction_;  // �ړ�����
	D3DXVECTOR3 old_1f_position_; // 1F�O�̍��W

	FLOAT gravity_; // �d��
	FLOAT ball_speed_;  // �r�[�`�{�[���̈ړ���

	const FLOAT kAccelerationGravity;  // �d�͉����x
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	Cbb();
	~Cbb();
	void BB_ResetState();
	BOOL PutWeapon(D3DXVECTOR3 pos);
	BOOL GetWeapon(D3DXVECTOR3 pPos);

	void Draw();
	void RayPick(const LPD3DXMESH & target_mesh, const D3DXMATRIX & target_world_matrix);
	void SetPos(D3DXVECTOR3 pos)
	{
		m_BeachBallPos = pos;
	}
	void SetRot(D3DXVECTOR3 rot)
	{
		m_BeachBallRot = rot;
	}


	void Ground();
	void BHave();
	void BThrow();
	void BAir();

	void BUpdate(D3DXVECTOR3 pos, D3DXVECTOR3 rotate);

	//void Update(D3DXVECTOR3 pos, D3DXVECTOR3 rotate);
	void Attack();
	int GetState()
	{
		return m_State;
	}
	BOOL HitCheck(D3DXVECTOR3 checkPos);


};