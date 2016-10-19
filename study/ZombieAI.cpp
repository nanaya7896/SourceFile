#include"ZombieAI.h"

CZombieAI::CZombieAI()
{
}

CZombieAI::~CZombieAI()
{
}

void CZombieAI::Update()
{
}

/*

CHASER_Mode,
DEATH_Mode,
FLOW_Mode,
WALLOW_Mode,
STAIRS_Mode,
HITSTOP_Mode

*/
void CZombieAI::ZombieAISelect(ZombieAIName name)
{
	switch (name)
	{
	CHASER_Mode:
		test(1);
		break;
	DEATH_Mode:
		break;
	FLOW_Mode:
		break;
	WALLOW_Mode:
		break;
	STAIRS_Mode:
		break;
	default:
		
		break;

	}
}

void CZombieAI::test(int num)
{
	m_ZombieInfo[num].animNum = 2;
	//if (m_ZombieInfo[num].)
	if (m_pAnimation[m_ZombieInfo[num].skin][m_ZombieInfo[num].animNum]->GetAnimFinishTime() <= m_ZombieInfo[num].AnimController[m_ZombieInfo[num].animNum]->GetTime())
	{
		m_ZombieInfo[num].isAlive = FALSE;
		value++;
		CScore::GetInstnce()->SetScoreNum(value);
		m_pDecorate->SetisFlash(TRUE);
		Zombierevival(num);
		m_pWaterSlider[num]->ResetValue();
	}

}


