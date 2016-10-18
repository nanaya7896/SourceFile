#ifndef ___Class_Camera___
#define ___Class_Camera___

#pragma once
//=====Include File=====//
#include"..\Global\Global.h"

//=====クラス宣言=====//
class CCamera
{
	//privateメンバ変数
private:
	//シングルトン
	static CCamera*			m_pInstance;
	//描画デバイス
	LPDIRECT3DDEVICE9		m_pDevice9;
	//カメラの基準位置
	D3DXVECTOR3				m_vViewPosition;
	//倍率(カメラから原点までの距離)
	FLOAT					m_fDistance;
	//カメラのアングル
	D3DXVECTOR3				m_vViewAngle;
	D3DXMATRIX				m_View;
	D3DXMATRIX				m_Proj;
	D3DXMATRIX				m_Screen;
	//環境光源
	D3DXVECTOR3				m_vDirectionalLight;
	//環境光源の場所
	D3DXVECTOR3				m_vLightPosition;
	D3DXVECTOR3				m_vEyePt2;
	//注視点
	D3DXVECTOR3				m_vLookatPt;
	
	D3DXVECTOR3 vViewBasePosition;	// カメラの基準位置
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CCamera(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CCamera();

	static void Create(LPDIRECT3DDEVICE9, HWND);
	static void Destroy();

	void SetCamera(D3DXVECTOR3 pos);

	//=======================
	//
	//セット関数
	//
	//=======================s
	void SetViewAngle(D3DXVECTOR3 Value)
	{
		m_vViewAngle = Value;
	}
	void SetDistance(FLOAT dis)
	{
		m_fDistance = dis;
	}
	//=======================
	//
	//ゲット関数
	//
	//=======================

	//カメラのインスタンス取得
	static CCamera* GetInstance()
	{
		return m_pInstance;
	}
	//View行列の取得
	D3DXMATRIX getView() 
	{
		return m_View; 
	}
	//
	D3DXMATRIX getProj()
	{
		return m_Proj;
	}
	//スクリーン行列の取得
	D3DXMATRIX getScreen() 
	{
		return m_Screen;
	}
	//カメラの位置の取得
	D3DXVECTOR3 GetViewPosition() 
	{
		return m_vViewPosition; 
	}
	//環境光源の取得
	D3DXVECTOR3 GetDirectionalLight()
	{
		return m_vDirectionalLight;
	}
	//環境光源の場所を取得
	D3DXVECTOR3 GetLightPosition() 
	{
		return m_vLightPosition; 
	}
	//注視点の取得
	D3DXVECTOR3 GetLookatPt() 
	{
		return m_vLookatPt;
	}
	//
	D3DXVECTOR3 GetViewAngle()
	{
		return m_vViewAngle;
	}
	D3DXVECTOR3 GetViewPos()
	{
		return vViewBasePosition;
	}

};

#endif