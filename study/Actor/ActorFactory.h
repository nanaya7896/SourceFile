#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"Actor.h"
#include"../Player/Player.h"
#include"../Zombie/Zombie.h"

//=====クラス宣言=====//
class CActorFactory
{
	//privateメンバ変数
private:


	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	CActorFactory();
	~CActorFactory();

	CActor* CreateActor(const std::string& name);
};