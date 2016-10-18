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
		//�n�_����I�_�܂łɓ_���T�O�ł�
		m_vbezir[pointNumber] = m_pMathLib->BezierCurve(m_vPosition, waterSliderCurvePos[m_Curve], waterSliderCurvePos[m_Curve + 1], waterSliderCurvePos[m_Curve + 2], f);

		pointNumber++;
	}
}

void CWaterSlider::HitRotate()
{
	int pointNumber = 0;

	for (int i = 0; i<50; i++)
	{
		//�ߋ��̍��W
		prevPos = m_vbezir[pointNumber];
		//���݂̃|�W�V����
		nowPos = m_vbezir[pointNumber + 1];
		vDirection = nowPos - prevPos;
		//���L�����Ȃ�
		//�t���ڂ̊p�x�i���W�A���j�����߂�
		disAn = atan2f(vDirection.z, vDirection.x);
		//0�`2�΂܂ł̊Ԃɂ���
		disAn = (D3DX_PI - disAn) + D3DX_PI;

		//�����܂�
		disAn -= D3DX_PI / 2;
		m_Angle[pointNumber].y = disAn;
		//���ؗp
		//FLOAT ang = D3DXToDegree(disAn);
		//MyOutputDebugString("YUKI=%f\n", ang);
		pointNumber++;
	}
}

void CWaterSlider::ZombieRotate()
{
	D3DXVECTOR3 vDir = waterSliderCurvePos[m_Curve + 1] - waterSliderCurvePos[m_Curve];
	//�P�ʉ�
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&m_vRotation, &m_vRotation);
	//�]���r�Ȃ�
	// 2��3D�x�N�g���̓��ς��v�Z
	FLOAT Dot = D3DXVec3Dot(&vDir, &m_vRotation);

	DirectionRadian = acosf(Dot);
	//----- �O�� -----//
	D3DXVECTOR3 vNormal;
	// 2��3D�x�N�g���̊O�ς��v�Z
	D3DXVec3Cross(&vNormal, &vDir, &m_vRotation);
	// �P�ʉ�
	D3DXVec3Normalize(&vNormal, &vNormal);
	// 0�`�΂܂ł̌ʓx��0�`2�΂܂łɕύX
	if (vNormal.y >= 0.0f)
	{
		//���̏��܂łɂȂ���W
		DirectionRadian = (D3DX_PI - DirectionRadian) + D3DX_PI;
	}
	//FLOAT an = D3DXToDegree(DirectionRadian);
	//MyOutputDebugString("  ZON=%f\n", an);
}


D3DXVECTOR3 CWaterSlider::WaterSliderAction(BOOL change)
{
	

	
	if (!m_isValueSet)
	{
		//�E�H�[�^�[�X���C�_�[�Ńx�W�F�Ȑ������邽�߂̊֐�
		HitPoint();
		//�x�W�F�Ȑ��ŉ񂷎��Ɋp�x����]������
		HitRotate();
		m_isValueSet = TRUE;
	}
	else
	{
		//�_�����ǂ��Ĉړ�
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
					//���������̂ŃJ�����̎��_�����ɖ߂�
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
			//�]���r�̉�]�����߂�
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