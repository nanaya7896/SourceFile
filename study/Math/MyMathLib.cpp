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
//関数名	:Lerp
//引数		:posA	 :始点
//			 posB	 :終点
//			 alpha	 :割合(0.0f～1.0f)
//戻り値	:Vector値を返す
//内容		:線形補間
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
//関数名	:BezierCurve
//引数		:startPos	:始点
//			:ControlPos :制御点
//			:EndPos		:終点
//			:t			:分割数(0.0f～1.0f)
//戻り値	:始点から終点までの間の点
//内容		:始点から終点までの間に曲線を描くために点を打つ(二次元)
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
//関数名	:MyVecAngle
//引数		:pPos	   :物体の座標
//			:centerPos :中心点
//			:angle	   :動かす角度
//戻り値	:2Dでの角度
//内容		:どのくらい傾いているかを算出する
//====================================
D3DXVECTOR2 CMathLib::MyVecAngle(D3DXVECTOR3 pPos, D3DXVECTOR3 centerPos,float angle)
{
	//座標一時保管用
	D3DXVECTOR2 tmp;
	//中心点からプレイヤーへの方向ベクトルを算出
	tmp.x = pPos.x - centerPos.x;
	tmp.y = pPos.z - centerPos.y;

	//計算結果をもとに角度を算出
	tmp.x = tmp.x* cosf(angle) - tmp.y*sinf(angle);
	tmp.y = tmp.x* sinf(angle) + tmp.y*cosf(angle);


	return tmp;
}
