#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"Actor.h"
#include"../Player/Player.h"
#include"../Zombie/Zombie.h"

//=====�N���X�錾=====//
class CActorFactory
{
	//private�����o�ϐ�
private:


	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	CActorFactory();
	~CActorFactory();

	CActor* CreateActor(const std::string& name);
};