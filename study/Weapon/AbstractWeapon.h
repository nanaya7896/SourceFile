#pragma once
//=====Include File=====//
#include"../Global/Global.h"

//#include"../Graphic/ModelManager.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/Collider.h"
//=====クラス宣言=====//
class CAbstractWeapon
{
	//privateメンバ変数
private:


	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:


	CAbstractWeapon();
	~CAbstractWeapon();
	virtual void Update(D3DXVECTOR3 pos, D3DXVECTOR3 rotate);
	virtual void Attack();
	virtual void SetPos(D3DXVECTOR3 pos);
	virtual void SetRot(D3DXVECTOR3 rot);
	virtual int  GetState();
	virtual BOOL HitCheck(D3DXVECTOR3 checkPos);

};
