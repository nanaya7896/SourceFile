#include "GameOver.h"

//====================================
//関数名	:CGameOver
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CGameOver::CGameOver(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
	m_pos = 370.0f;
	isPos = TRUE;
}

//====================================
//関数名	:~CGameOver
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CGameOver::~CGameOver()
{

}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void CGameOver::Update()
{
	
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void CGameOver::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(13, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	pTextureManager->Draw(21, D3DXVECTOR3(m_pos, WINDOW_HEIGHT-170.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	//pTextureManager->Draw(21, D3DXVECTOR3(800.0f, WINDOW_HEIGHT - 170.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
}

void CGameOver::isArrowPos(BOOL isleft)
{
	if (isleft)
	{
		m_pos = 370.0f;
		isPos = TRUE;
	}
	else 
	{
		m_pos = 800.0f;
		isPos = FALSE;
	}
}


void CGameOver::ResetValue()
{
	m_pos = 370.0f;
	isPos = TRUE;

}