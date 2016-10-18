#include "Title.h"
#include<time.h>

//====================================
//関数名	:CTitle
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CTitle::CTitle(LPDIRECT3DDEVICE9 pDevice9)
{
	isTitle = false;
	fadeTime = 0.0f;
	timeCount = 0.005f;
	CreateResource(pDevice9);
}

//====================================
//関数名	:~CTitle
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CTitle::~CTitle()
{
}

//====================================
//関数名	:CreateResource
//引数		:
//戻り値	:
//内容		:リソースの作成。一度しか呼ばれない
//====================================
void CTitle::CreateResource(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
}

//====================================
//関数名	:AllGameReset
//引数		:
//戻り値	:
//内容		:Titleのリセット関数
//====================================
void CTitle::AllReset()
{
	isTitle = false;
	fadeTime = 0.0f;
	timeCount = 0.005f;
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新処理
//====================================

void CTitle::Update()
{
	if (fadeTime > 1.5f)
	{
		timeCount = -0.05f;
	}
	if (fadeTime < 0)
	{
		isTitle = true;
	}
	fadeTime += timeCount;
}


//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CTitle::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	if (isTitle)
	{
		//タイトルの描画
		pTextureManager->Draw(10, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	}
	else
	{
		//タイトルロゴの描画
		pTextureManager->Draw(17, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, fadeTime, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	}
	

}
