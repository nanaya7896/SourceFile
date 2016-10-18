/*

ビーチボールの処理

*/

#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"AbstractWeapon.h"

//=====クラス宣言=====//
class Cbb : public CAbstractWeapon
{
	//privateメンバ変数
private:
	D3DXVECTOR3 m_BeachBallPos;
	D3DXVECTOR3 m_BeachBallRot;
	int			m_State;



	D3DXVECTOR3 direction_;  // 移動方向
	D3DXVECTOR3 old_1f_position_; // 1F前の座標

	FLOAT gravity_; // 重力
	FLOAT ball_speed_;  // ビーチボールの移動量

	const FLOAT kAccelerationGravity;  // 重力加速度
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
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