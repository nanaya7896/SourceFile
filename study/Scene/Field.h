#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/DirectX/DInput.h"
#include"../Graphic/Object/Object.h"
#include"../Player/YukiAnimation.h"

//=====クラス宣言=====//
class CFiled
{
	//privateメンバ変数
private:
	LPDIRECT3DDEVICE9		m_pDevice9;
	//オブジェクトの読み込み
	CWall*					m_pWall;

	D3DXVECTOR3 m_vWatergunPos;
	D3DXVECTOR3 m_vBallPos;
	D3DXVECTOR3 m_vBaburuPos;

	//仮
	//YukiAnimation*		m_pool;

	float m_WaterRot;
	float m_WaterUpDownValue;
	//publicメンバ変数
public:

private:


	//publicメンバ関数
public:
	CFiled(LPDIRECT3DDEVICE9);
	~CFiled();

	BOOL CreateResource(LPDIRECT3DDEVICE9);
	void Update();
	void Draw();

	

};