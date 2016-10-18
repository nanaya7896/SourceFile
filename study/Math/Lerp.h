/*
線形補間とベジェ曲線などの演算を計算してます
2016/06/10
*/


#pragma once
//=====Include File=====//
#include"../Global/Global.h"



//=====クラス宣言=====//
class CLerp
{
	//privateメンバ変数
private:
	//publicメンバ変数
public:


	//privateメンバ関数
private:

	
	//publicメンバ関数
public:
	CLerp();
	~CLerp();
	void Update();
	void Draw();

	D3DXVECTOR3 Lerp(D3DXVECTOR3 posA,D3DXVECTOR3 posB,float divValue);
	D3DXVECTOR3 BezierCurve(D3DXVECTOR3 startPos,D3DXVECTOR3 ControlPos,D3DXVECTOR3 EndPos,float t);


};