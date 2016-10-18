#include "WaterSlider.h"




CWaterSlider::CWaterSlider()
{
	m_pMathLib = nullptr;
	m_isValueSet = FALSE;
	m_setSlider = FALSE;
	disAn = 0.0f;
	CreateResoure();
}

CWaterSlider::~CWaterSlider()
{
	SAFE_DELETE(m_pMathLib);
}

void CWaterSlider::CreateResoure()
{
	m_pMathLib = new CMathLib();
}

void CWaterSlider::ResetValue()
{
	m_setSlider = FALSE;
	m_PointValue = 0;
	m_isValueSet = FALSE;
	m_Curve = 0;
	//disAn = 0.0f;
	prevPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	nowPos= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vDirection=D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CWaterSlider::HitPoint()
{
	int pointNumber = 0;
	for (float f = 0.0f; f < 1.0f; f += 0.02f)
	{
		//始点から終点までに点を５０個打つ
		m_vbezir[pointNumber] = m_pMathLib->BezierCurve(m_vPosition, waterSliderCurvePos[m_Curve], waterSliderCurvePos[m_Curve + 1], waterSliderCurvePos[m_Curve + 2], f);

		pointNumber++;
	}
}

void CWaterSlider::HitRotate()
{
	int pointNumber = 0;

	for (int i = 0; i<50; i++)
	{
		//過去の座標
		prevPos = m_vbezir[pointNumber];
		//現在のポジション
		nowPos = m_vbezir[pointNumber + 1];
		vDirection = nowPos - prevPos;
		//ユキちゃんなら
		//逆正接の角度（ラジアン）を求める
		disAn = atan2f(vDirection.z, vDirection.x);
		//0～2πまでの間にする
		disAn = (D3DX_PI - disAn) + D3DX_PI;

		//ここまで
		disAn -= D3DX_PI / 2;
		m_Angle[pointNumber].y = disAn;
		//検証用
		//FLOAT ang = D3DXToDegree(disAn);
		//MyOutputDebugString("YUKI=%f\n", ang);
		pointNumber++;
	}
}

void CWaterSlider::ZombieRotate()
{
	D3DXVECTOR3 vDir = waterSliderCurvePos[m_Curve + 1] - waterSliderCurvePos[m_Curve];
	//単位化
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&m_vRotation, &m_vRotation);
	//ゾンビなら
	// 2つの3Dベクトルの内積を計算
	FLOAT Dot = D3DXVec3Dot(&vDir, &m_vRotation);

	DirectionRadian = acosf(Dot);
	//----- 外積 -----//
	D3DXVECTOR3 vNormal;
	// 2つの3Dベクトルの外積を計算
	D3DXVec3Cross(&vNormal, &vDir, &m_vRotation);
	// 単位化
	D3DXVec3Normalize(&vNormal, &vNormal);
	// 0～πまでの弧度を0～2πまでに変更
	if (vNormal.y >= 0.0f)
	{
		//次の所までになる座標
		DirectionRadian = (D3DX_PI - DirectionRadian) + D3DX_PI;
	}
	//FLOAT an = D3DXToDegree(DirectionRadian);
	//MyOutputDebugString("  ZON=%f\n", an);
}


D3DXVECTOR3 CWaterSlider::WaterSliderAction(BOOL change)
{
	

	
	if (!m_isValueSet)
	{
		//ウォータースライダーでベジェ曲線させるための関数
		HitPoint();
		//ベジェ曲線で回す時に角度を回転させる
		HitRotate();
		m_isValueSet = TRUE;
	}
	else
	{
		//点をたどって移動
		m_vPosition = m_vbezir[m_PointValue];
		if (change == FALSE)
		{
			disAn = m_Angle[m_PointValue].y;
			if (m_PointValue >= W_POINT - 1)
			{
				m_isValueSet = FALSE;
				m_Curve += 3;
				if (m_Curve > FINISH_SLIDER)
				{
					//着水したのでカメラの視点を元に戻す
					CCamera::GetInstance()->SetViewAngle(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					ResetValue();
				}
				m_PointValue = 0;
			}
			else
			{
				m_PointValue++;
			}
		}
		else
		{
			//ゾンビの回転を求める
			ZombieRotate();
			if (m_PointValue >= W_POINT - 1)
			{
				m_isValueSet = FALSE;
				m_Curve += 3;
				m_PointValue = 0;
			}
			else
			{
				m_PointValue++;
			}
		}
		
		//m_vRotation.z = -disAn;
		

		
	}
	return m_vPosition;
}