#include "ActorFactory.h"

CActorFactory::CActorFactory()
{
}

CActorFactory::~CActorFactory()
{
}

CActor * CActorFactory::CreateActor(const std::string & name)
{
	if (name == "Player") { return new CPlayer(); }
	if (name == "Zombie") { return new CZombie(); }
	return nullptr;
}
