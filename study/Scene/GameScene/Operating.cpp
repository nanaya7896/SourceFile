#include "Operating.h"

//====================================
//関数名	:COperating
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
COperating::COperating(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
}

//====================================
//関数名	:~COperating
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
COperating::~COperating()
{
	
}

//====================================
//関数名	:Update
//引数		:
//戻り値	:
//内容		:更新
//====================================
void COperating::Update()
{
}

//====================================
//関数名	:Draw
//引数		:
//戻り値	:
//内容		:描画
//====================================
void COperating::Draw()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(11, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1.0f, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));

}
