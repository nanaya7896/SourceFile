#include "Field.h"

CFiled::CFiled(LPDIRECT3DDEVICE9 /*pDevice9*/)
{	
	m_pWall = NULL;
	m_vBaburuPos = D3DXVECTOR3(60.0f, 13.0f, 0.0f);
	m_vBallPos = D3DXVECTOR3(-60.0f, 13.0f, 0.0f);
	m_vWatergunPos = D3DXVECTOR3(0.0f, 13.0f, -60.0f);
}

CFiled::~CFiled()
{
	SAFE_DELETE(m_pWall);
	//SAFE_DELETE(m_pool);
}

BOOL CFiled::CreateResource(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
	m_pWall = new CWall(m_pDevice9);
	m_pWall->Initialize(1.0f,"Model/kabe.X");

	//m_pool = new YukiAnimation(m_pDevice9,"Model/pool.X");
	//m_pool->SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
	return TRUE;
}
void CFiled::Update()
{
	m_WaterRot += 0.001f;
	m_WaterUpDownValue = sinf(m_WaterRot);
	//m_pool->Update();
}

void CFiled::Draw()
{

	CModelManager* pModelManager = CModelManager::GetInstance();
	//フィールド描画用
	pModelManager->Draw(0, D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(1.0f,1.0f,1.0f), 1.0f);
	//水
	//m_pool->Draw();
	pModelManager->Draw(1, D3DXVECTOR3(0.0f, m_WaterUpDownValue, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	//スライダー
	pModelManager->Draw(2, D3DXVECTOR3(0.0f, 0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	//階段
	pModelManager->Draw(3, D3DXVECTOR3(-166.0f, 13.0f, -25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	pModelManager->Draw(4, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);

	//pModelManager->Draw(5, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	pModelManager->Draw(6, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	pModelManager->Draw(7, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);
	
	//橋
	pModelManager->Draw(12, D3DXVECTOR3(29.0f, 7.0f, 111.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1.0f);

	//壁生成
	m_pWall->Draw();
	//ボール
	//pModelManager->Draw(8, m_vBallPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), 1.0f);
	//バブル
	//pModelManager->Draw(9, m_vBaburuPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), 1.0f, 3);
	//ウォーター
	//pModelManager->Draw(10, m_vWatergunPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), 1.0f, 3);


}

