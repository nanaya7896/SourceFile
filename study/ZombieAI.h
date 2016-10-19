/*
	ƒ]ƒ“ƒr‚ÌAI‚ğŠÇ—‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX
	


	2016/10/19 S.Ikehara
*/

#pragma once
#include"Zombie\Zombie.h"

class CZombie;

typedef enum ZombieAIName
{
	CHASER_Mode,
	DEATH_Mode,
	FLOW_Mode,
	WALLOW_Mode,
	STAIRS_Mode,
	HITSTOP_Mode
};

class CZombieAI : public CZombie
{

	CZombieAI();
	~CZombieAI();

	void Update();

	void ZombieAISelect(ZombieAIName name);
	void test(int num);
};


