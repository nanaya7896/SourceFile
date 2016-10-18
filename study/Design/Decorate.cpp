#include "Decorate.h"

CDecorate::CDecorate()
{
	alphaValue = 0.0f;
	alphaFlashScore=0.0f;
	
	value=0.001f;
	isFlash=FALSE;
}

CDecorate::~CDecorate()
{
}

//====================================
//関数名	:Update
//引数		:pPos:プレイヤーの座標
//			 ePos:ゾンビの座標
//戻り値	:
//内容		:更新
//====================================
void CDecorate::Update(D3DXVECTOR3 pPos, D3DXVECTOR3 ePos)
{
	//ゾンビとプレイヤーの座標の差を図る
	D3DXVECTOR3 diff = pPos - ePos;
	//一時変数保管用
	FLOAT tmp;
	//距離を求める
	FLOAT length = D3DXVec3Length(&diff);
	if (length <= 10.0f)
	{
		//距離から透明度を求める
		alphaValue = length - 10.0f;
	}
}

//====================================
//関数名	:UpdateeFlashScore
//引数		:
//戻り値	:
//内容		:スコアを光らせる
//====================================
void CDecorate::UpdateeFlashScore()
{
	if (isFlash)
	{
		alphaFlashScore += value;
		if (alphaFlashScore > 1.0f)
		{
			value *= -1.0f;
		}
		else if(alphaFlashScore<0.0f)
		{
			value *= -1.0f;
			alphaFlashScore = 0.0f;
			isFlash = FALSE;
		}
	}
}

//====================================
//関数名	:FalshScreen
//引数		:
//戻り値	:
//内容		:スクリーンを光らせる処理
//====================================
void CDecorate::FalshScreen()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();

	pTextureManager->Draw(24, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, -alphaValue, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
}

void CDecorate::FlashScore()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	if (isFlash)
	{
		pTextureManager->Draw(25, D3DXVECTOR3(-10.0f, -10.0f, 0.0f), 0, alphaFlashScore, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 8.0f, 150.0f, 0.0f));
	}
}

void CDecorate::ResetDeco()
{
	alphaValue = 0.0f;
	alphaFlashScore = 0.0f;
	float value = 0.01f;
	BOOL isFlash = FALSE;
}
