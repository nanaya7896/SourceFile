#include "Result.h"
#include<time.h>


//====================================
//関数名	:CResult
//引数		: keyState	:キーの情報
//			 pDevice9:描画デバイス
//戻り値	:
//内容		:コンストラクタ
//====================================
CResult::CResult(LPDIRECT3DDEVICE9 pDevice9, CDInput* pInput)
{
	m_pDevice9 = pDevice9;
	m_pDInput=pInput;
	CreateResource();
}

//====================================
//関数名	:CResult
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CResult::~CResult()
{
}

BOOL CResult::CreateResource()
{
	return TRUE;
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CResult::Update()
{
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CResult::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(14, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	pTextureManager->Draw(score[2], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 9.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(score[1], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 10.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
	pTextureManager->Draw(score[0], D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 11.0f, 500.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));

}


