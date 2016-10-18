#include "MyMathLib.h"

CMathLib::CMathLib()
{

}

CMathLib::~CMathLib()
{
}

D3DXVECTOR3 CMathLib::Divide(D3DXVECTOR3 stPos, D3DXVECTOR3 endPos, float divide)
{
	D3DXVECTOR3 tmp,value;
	value = endPos - stPos;
	tmp = value / divide;

	return tmp;
}


//====================================
//�֐���	:Lerp
//����		:posA	 :�n�_
//			 posB	 :�I�_
//			 alpha	 :����(0.0f�`1.0f)
//�߂�l	:Vector�l��Ԃ�
//���e		:���`���
//====================================
D3DXVECTOR3 CMathLib::Lerp(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float alpha)
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
//���e		:�n�_����I�_�܂ł̊ԂɋȐ���`�����߂ɓ_��ł�(�񎟌�)
//====================================
D3DXVECTOR3 CMathLib::BezierCurve(D3DXVECTOR3 startPos, D3DXVECTOR3 ControlPos, D3DXVECTOR3 EndPos, float t)
{
	float pT = 1 - t;
	D3DXVECTOR3 resultPos;
	resultPos.x = t*t*EndPos.x + 2 * t*pT*ControlPos.x + pT*pT*startPos.x;
	resultPos.y = t*t*EndPos.y + 2 * t*pT*ControlPos.y + pT*pT*startPos.y;
	resultPos.z = t*t*EndPos.z + 2 * t*pT*ControlPos.z + pT*pT*startPos.z;

	return resultPos;
}

D3DXVECTOR3 CMathLib::BezierCurve(D3DXVECTOR3 startPos, D3DXVECTOR3 ControlPos, D3DXVECTOR3 ControlPos2, D3DXVECTOR3 EndPos, float t)
{
	float pT = 1 - t;
	D3DXVECTOR3 pos;
	pos = startPos*pow(pT, 3) + 3 * t*(ControlPos*pow(pT, 2)) + ControlPos2 * 3 * pow(t, 2)*pT + EndPos * (pow(t, 3));

	return pos;
}

//====================================
//�֐���	:MyVecAngle
//����		:pPos	   :���̂̍��W
//			:centerPos :���S�_
//			:angle	   :�������p�x
//�߂�l	:2D�ł̊p�x
//���e		:�ǂ̂��炢�X���Ă��邩���Z�o����
//====================================
D3DXVECTOR2 CMathLib::MyVecAngle(D3DXVECTOR3 pPos, D3DXVECTOR3 centerPos,float angle)
{
	//���W�ꎞ�ۊǗp
	D3DXVECTOR2 tmp;
	//���S�_����v���C���[�ւ̕����x�N�g�����Z�o
	tmp.x = pPos.x - centerPos.x;
	tmp.y = pPos.z - centerPos.y;

	//�v�Z���ʂ����ƂɊp�x���Z�o
	tmp.x = tmp.x* cosf(angle) - tmp.y*sinf(angle);
	tmp.y = tmp.x* sinf(angle) + tmp.y*cosf(angle);


	return tmp;
}
