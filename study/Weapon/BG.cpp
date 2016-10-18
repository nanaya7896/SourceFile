#include "BG.h"

Cbg::Cbg()
{
	gunPos = { -125.0f, 13.25f, -130.0f };
	m_State = Land;
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
}

Cbg::~Cbg()
{
}

void Cbg::BG_ResetState()
{
	gunPos = { -125.0f, 13.25f, -130.0f };
	m_State = Land;
	m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);
}

BOOL Cbg::GetWeapon(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 dis = pos - gunPos;
	if (abs(D3DXVec3Length(&dis)) < 3.0f)
	{
		m_State = Have;
		return TRUE;
	}
	return FALSE;
}

BOOL Cbg::PutWeapon(D3DXVECTOR3 pos)
{
	if (m_State == Have)
	{
		m_State = Land;
		gunPos = pos;
		return TRUE;
	}

	return FALSE;
}

void Cbg::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{
	if(m_State==Shot)
	{
		UpdateWater(pos);
	}
	//MyOutputDebugString("バブルがん更新\n");
}


void Cbg::Attack()
{
	m_State = Shot;
	//MyOutputDebugString("バブルがんで攻撃\n");
}

void Cbg::Draw()
{
	CModelManager* pModelManager=CModelManager::GetInstance();
	if (m_State == Land)
	{
		pModelManager->Draw(9, gunPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 4), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 1.0f);
	}
	else if (m_State ==Shot)
	{
		D3DXVECTOR3 vec(0.0f,m_waterPos.y,-(1.0f));
		D3DXMATRIXA16 matWorld;
		D3DXMatrixRotationY(&matWorld,rotate.y);
		D3DXVECTOR3 myvec;
		D3DXVec3TransformCoord(&myvec, &vec, &matWorld);
		m_waterPos.x = m_waterPos.x + (myvec.x);
		m_waterPos.z = m_waterPos.z + (myvec.z);
	//	MyOutputDebugString("%f　", m_waterPos.x);
		//MyOutputDebugString(" %f\n", m_waterPos.z);
		//シャボン玉
		pModelManager->Draw(5, m_waterPos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0.5f, 0.5f, 0.5f), 0.0001f);
	}
}

//====================================
//関数名	:ChangeTF
//引数		:value	:周期を計算するための値(標準1.0f)
//			 rot	:振れ幅を決める値
//			 isSin  :TRUE:Sin値　FALSE:Cos値
//戻り値	:三角関数(Sin,Cos)の計算結果
//内容		:Sin,Cosの値を計算する
//====================================
FLOAT Cbg::ChangeTF(FLOAT value, FLOAT rot, BOOL isSin)
{
	FLOAT va = 0.0f;
	if (isSin)
	{
		//Sinを計算したい場合
		va = value*(sinf(rot));
	}
	else
	{
		//Cosを計算したい場合
		va = value*(cosf(rot));
	}
	return va;
}

BOOL Cbg::HitCheck(D3DXVECTOR3 checkPos)
{

	if (Collider::GetInstance()->EasyCheck(m_waterPos, checkPos, 5.0f)) 
	{
		m_waterPos = D3DXVECTOR3(1000.0f, 100.0f, 1000.0f);

		m_State = Have;
		return TRUE;
	}
	return FALSE;
}

//====================================
//関数名	:UpdateWater
//引数		:
//戻り値	:
//内容		:シャボン玉をSin値で上下させる
//====================================
void Cbg::UpdateWater(D3DXVECTOR3 pos)
{
	m_waterPos.y += ChangeTF(0.5f, value, TRUE);
	//m_waterPos.x += ChangeTF(0.5f, pos.y-D3DX_PI, TRUE);
	//m_waterPos.z += ChangeTF(0.5f, pos.y-D3DX_PI, FALSE);

	value += 0.1f;
	float num;
	if (Collider::GetInstance()->RayCast(m_waterPos, pos, &num))
	{
		m_State = Have;
		value = 0.0f;
	}

}