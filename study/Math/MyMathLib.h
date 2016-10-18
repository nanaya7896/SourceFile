/*
���`��Ԃƃx�W�F�Ȑ��Ȃǂ̉��Z���v�Z���Ă܂�
2016/06/10
*/


#pragma once
//=====Include File=====//
#include"../Global/Global.h"



//=====�N���X�錾=====//
class CMathLib
{
	//private�����o�ϐ�
private:
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	CMathLib();
	~CMathLib();
	D3DXVECTOR3 Divide(D3DXVECTOR3 stPos,D3DXVECTOR3 endPos,float divide);
	D3DXVECTOR3 Lerp(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float divValue);
	D3DXVECTOR3 BezierCurve(D3DXVECTOR3 startPos, D3DXVECTOR3 ControlPos, D3DXVECTOR3 EndPos, float t);
	D3DXVECTOR3 BezierCurve(D3DXVECTOR3 startPos, D3DXVECTOR3 ControlPos,D3DXVECTOR3 ControlPos2, D3DXVECTOR3 EndPos, float t);
	D3DXVECTOR2 MyVecAngle(D3DXVECTOR3 pPos,D3DXVECTOR3 centerPos, float angle);

};