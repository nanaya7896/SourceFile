/*
WaterSliderの流れる処理
2016/06/09	S.Ikehara
*/

#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Math/MyMathLib.h"
#include"../Camera/Camera.h"
#include"../Graphic/ModelManager.h"

#define W_POINT   50
#define MAX_CURVE			37
#define FINISH_SLIDER		33
//=====クラス宣言=====//
class CWaterSlider
{
	//privateメンバ変数
private:
	CMathLib*			m_pMathLib;

	BOOL m_setSlider;
	BOOL m_isValueSet;
	int m_Curve;
	int m_PointValue = 0;
	D3DXVECTOR3 m_vbezir[W_POINT] = {};
	D3DXVECTOR3 m_Angle[W_POINT] = {};
	//プレイヤーの座標
	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3     m_vRotation;

	D3DXVECTOR3 nowPos;
	D3DXVECTOR3	prevPos;
	D3DXVECTOR3 vDirection;
	FLOAT		DirectionRadian;
	FLOAT disAn;

	//ウォータスライダーに設置する点
	D3DXVECTOR3 waterSliderCurvePos[MAX_CURVE] =
	{
		D3DXVECTOR3(-152.390f,62.912f,42.066f),		//0
		D3DXVECTOR3(-150.394f,62.719f,61.966f),		//1
		D3DXVECTOR3(-141.571f,61.930f,74.068f),		//2
		D3DXVECTOR3(-130.843f,61.857f,89.750f),		//3
		D3DXVECTOR3(-117.607f,60.112f,95.673f),			//4
		D3DXVECTOR3(-100.721f,59.103f,103.230f),		//5
		D3DXVECTOR3(-86.269f,58.389f,103.705f),			//6
		D3DXVECTOR3(-68.776f,57.480f,103.193f),		//7
		D3DXVECTOR3(-55.661f,56.751f, 96.043f),			//8
		D3DXVECTOR3(-42.442f,55.905f, 86.413f),		//9
		D3DXVECTOR3(-35.980f,55.154f,74.129f),			//10
		D3DXVECTOR3(-31.018f,54.347f,62.113f),		//11
		D3DXVECTOR3(-30.159f,53.652f,48.659f),			//12
		D3DXVECTOR3(-30.153f,52.695f,31.670f),		//13
		D3DXVECTOR3(-38.192f,50.725f,11.435f),			//14
		D3DXVECTOR3(-60.454f,48.925f,1.220f),		//15
		D3DXVECTOR3(-84.827f,46.812f,6.304f),			//16
		D3DXVECTOR3(-98.543f,44.757f,24.146f),		//17
		D3DXVECTOR3(-96.062f,42.679f,45.902f),			//18
		D3DXVECTOR3(-78.146f,40.587f,60.486f),			//19
		{ -51.960f,38.957f,64.551f },	//20
		{ -28.985f,38.474f,55.529f },	//21
		{ -10.859f,37.795f,41.965f },	//22
		{ -0.617f,37.060f,27.819f },	//23
		{ 9.664f,36.210f,10.981f },	//24
		{ 25.133f,35.182f,1.18f },	//25
		{ 43.472f,33.532f,10.468f },	//26
		{ 51.356f,31.921f,31.067f },	//27
		{ 58.511154f,29.411896f,50.646042f },//28
		{ 80.860268f,26.656462f,53.394531f },//29
		{ 88.937500f,24.972832f,28.739809f },//30
		{ 103.808083f,20.710445f,0.073458f },//31
		{ 117.247269f,19.578993f,0.398026f },//32
		{ 124.682861f,9.000f,19.972950f },	 //33
		{ 124.682861f,7.0f,19.972950f },	 //34
		{ 124.682861f,7.0f,19.972950f },	 //35
		{ 124.682861f,7.0f,19.972950f }		 //36
	};

	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	CWaterSlider();
	~CWaterSlider();
	void CreateResoure();

	void ResetValue();
	void HitPoint();
	void HitRotate();
	void ZombieRotate();
	D3DXVECTOR3 WaterSliderAction(BOOL);


	//=====セット関数=====//
	void SetPlayerPos(D3DXVECTOR3 pos)
	{
		m_vPosition = pos;
	}

	void SetIsSlider(BOOL isSli)
	{
		m_setSlider = isSli;
	}
	void SetPlayerRot(D3DXVECTOR3 rot)
	{
		m_vRotation = rot;
	}


	//====ゲット関数=====//
	BOOL GetisSlider()
	{
		return m_setSlider;
	}
	int GetNum()
	{
		return m_Curve;
	}

	float GetRot()
	{
		return disAn;
	}

	FLOAT GetDirectionRadian()
	{
		return DirectionRadian;
	}
};