#include "Score.h"

//唯一のインスタンスの生成
CScore* CScore::m_pInstance = NULL;

//====================================
//関数名	:CScore
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CScore::CScore()
{
	ZombieNum = 0;
}

//====================================
//関数名	:~CScore
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CScore::~CScore()
{
	
}

void CScore::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CScore;
	}
}

void CScore::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CScore::Update()
{
	SetScore();
}

//====================================
//関数名	:SetScore
//引数		:
//戻り値	:
//内容		:スコアを変数にセットする
//====================================
void CScore::SetScore()
{
	for (int i = 0; i < 3; i++)
	{
		score[i] = ZombieNum / (int)pow(10.0f, i);
		score[i] = score[i] % 10;
	}
}

//====================================
//関数名	:ResetScore
//引数		:
//戻り値	:
//内容		:スコアをリセットする
//====================================
void CScore::ResetScore()
{
	ZombieNum = 0;
	score[0] = 0;
	score[1] = 0;
	score[2] = 0;
}


