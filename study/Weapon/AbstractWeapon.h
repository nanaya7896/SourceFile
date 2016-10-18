#pragma once
//=====Include File=====//
#include"../Global/Global.h"

//#include"../Graphic/ModelManager.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/Collider.h"
//=====�N���X�錾=====//
class CAbstractWeapon
{
	//private�����o�ϐ�
private:


	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
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
