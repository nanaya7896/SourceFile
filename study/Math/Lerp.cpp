#include "Lerp.h"

CLerp::CLerp()
{

}

CLerp::~CLerp()
{
}

void CLerp::Update()
{



}

void CLerp::Draw()
{
}

//====================================
//�֐���	:Lerp
//����		:posA	 :�n�_
//			 posB	 :�I�_
//			 alpha	 :����(0.0f�`1.0f)
//�߂�l	:Vector�l��Ԃ�
//���e		:���`���
//====================================
D3DXVECTOR3 CLerp::Lerp(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float alpha)
{
	D3DXVECTOR3 tmp = {};

	tmp = posB - posA;
	tmp *= alpha;
	tmp += posA;
	
	return tmp;	
}

//====================================
//�֐���	:BezierCurve
//����		:startPos	:�n�_
//			:ControlPos :����_
//			:EndPos		:�I�_
//			:t			:������(0.0f�`1.0f)
//�߂�l	:�n�_����I�_�܂ł̊Ԃ̓_
//���e		:�n�_����I�_�܂ł̊ԂɋȐ���`�����߂ɓ_��ł�
//====================================
D3DXVECTOR3 CLerp::BezierCurve(D3DXVECTOR3 startPos, D3DXVECTOR3 ControlPos, D3DXVECTOR3 EndPos, float t)
{
	float pT = 1 - t;
	D3DXVECTOR3 resultPos;
	resultPos.x = t*t*EndPos.x + 2 * t*pT*ControlPos.x + pT*pT*startPos.x;
	resultPos.y = t*t*EndPos.y + 2 * t*pT*ControlPos.y + pT*pT*startPos.y;
	resultPos.z = t*t*EndPos.z + 2 * t*pT*ControlPos.z + pT*pT*startPos.z;

	return resultPos;
}
