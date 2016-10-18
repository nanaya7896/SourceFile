/*ウォーターガンの処理*/

#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"AbstractWeapon.h"
#include"../Lib/Collider.h"
#define	BULLET_MAX 64
#define BULLET_SPEED 5.0f
#define RECAST_TIME 0.2f
//=====クラス宣言=====//
class Cwg : public CAbstractWeapon
{
	//privateメンバ変数
private:
	D3DXVECTOR3 gunpos;
	int m_State;
	//回転率
	D3DXVECTOR3 rotate;

	//弾の構造体
	struct Bullet
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rotate;
		int			deg;
		bool		used;
	};
	Bullet bullet[BULLET_MAX];


	int start = 0;
	int end = 0;
	//弾の保存した数
	int bulletnum = 0;
	D3DXMATRIX				m_bullet;
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	Cwg();
	~Cwg();
	BOOL PutWeapon(D3DXVECTOR3 pos);
	BOOL GetWeapon(D3DXVECTOR3 pos);
	void Update(D3DXVECTOR3 pos,D3DXVECTOR3 rotate);
	void Attack();

	void Draw();
	BOOL HitCheck(D3DXVECTOR3 checkPos);


	void SetPos(D3DXVECTOR3 pos)
	{
		gunpos = pos;
	}
	void SetRot(D3DXVECTOR3 rot)
	{
		rotate = rot;
	}

	int GetState()
	{
		return m_State;
	}

	void WG_ResetState();
};
