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
//関数名	:Lerp
//引数		:posA	 :始点
//			 posB	 :終点
//			 alpha	 :割合(0.0f～1.0f)
//戻り値	:Vector値を返す
//内容		:線形補間
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
//関数名	:BezierCurve
//引数		:startPos	:始点
//			:ControlPos :制御点
//			:EndPos		:終点
//			:t			:分割数(0.0f～1.0f)
//戻り値	:始点から終点までの間の点
//内容		:始点から終点までの間に曲線を描くために点を打つ
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
