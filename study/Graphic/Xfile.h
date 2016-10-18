#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Camera/Camera.h"
#include"../Math/MyMathLib.h"
//=====クラス宣言=====//
class CXfile
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9	m_pDevice9;
	//メッシュ
	LPD3DXMESH			m_pMesh;
	// 質感
	D3DMATERIAL9*		m_pMaterials;
	// 質感
	D3DMATERIAL9*		m_pOriginalMaterials;
	// 質感の数
	DWORD				m_NumMaterials;
	//テクスチャ
	LPDIRECT3DTEXTURE9*	m_pTextures;
	//中心
	D3DXVECTOR3			m_vCenter;
	//半径
	FLOAT				m_Radius;
	//シェーダ
	LPD3DXEFFECT		m_pEffect;

	// スライダーのテクスチャUV
	D3DXVECTOR2 TextureUV;
	//D3DXVECTOR2 TextureUVpool;
	BOOL slider;
	BOOL pool;
	CMathLib*			m_Math;


	D3DXMATRIX	matRotCop;
	D3DXMATRIX  matWorldCopy;
	//環境光源を指定
	D3DXVECTOR4 vDirectionalLight;
	//publicメンバ変数
public:
	int value_Light;
	//privateメンバ関数
private:

	//publicメンバ関数
public:
	//コンストラクタ
	CXfile(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CXfile();

	//Xfileの読み込み
	BOOL LoatXfile(const TCHAR*);
	
	//描画用
	void Draw(D3DXVECTOR3, D3DXVECTOR3,D3DXVECTOR3, float);
	//描画用
	void Draw(D3DXVECTOR3*, D3DXVECTOR3*, D3DXVECTOR3*, D3DXCOLOR, int);

	LPD3DXMESH CXfile::GetMesh()
	{
		return m_pMesh;
	}
	void SetSlider(BOOL Value) { slider = Value; }
	void Setpool(BOOL Value) { pool = Value; }


	//UV値のゲッター
	D3DXVECTOR2 GetSliderUV()
	{
		return TextureUV;
	}
	D3DXMATRIX GetMatRot()
	{
		return matRotCop;
	}
	D3DXMATRIX GetMatWorld()
	{
		return matWorldCopy;
	}
};