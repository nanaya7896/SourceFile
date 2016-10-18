/*バブルがんの処理*/

#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"AbstractWeapon.h"

//=====クラス宣言=====//
class Cbg : public CAbstractWeapon
{
	//privateメンバ変数
private:
	D3DXVECTOR3 gunPos;
	D3DXVECTOR3 rotate;
	int m_State;
	//座標
	D3DXVECTOR3 m_waterPos;
	float value;
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	Cbg();
	~Cbg();
	void BG_ResetState();
	BOOL GetWeapon(D3DXVECTOR3 pos);
	BOOL PutWeapon(D3DXVECTOR3 pos);
	void Update(D3DXVECTOR3 pos,D3DXVECTOR3 rotate);
	BOOL HitCheck(D3DXVECTOR3 checkPos);
	void UpdateWater(D3DXVECTOR3 pos);
	void Attack();
	void Draw();
	FLOAT ChangeTF(FLOAT value, FLOAT rot, BOOL isSin);

	void SetWPos(D3DXVECTOR3 pos)
	{
		m_waterPos = pos;
	}
	void SetPos(D3DXVECTOR3 pos)
	{
		gunPos = pos;
	}
	void SetRot(D3DXVECTOR3 rot)
	{
		rotate = rot;
	}
	int GetState()
	{
		return m_State;
	}



};