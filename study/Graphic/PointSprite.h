#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"DSystem.h"
#include"../Camera/Camera.h"
//=====構造体=====//
struct Vertex3 
{
	D3DXVECTOR3 pos;	//位置	
	float       size;	//サイズ
	DWORD		color;	//色
};

//=====クラス宣言=====//
class CPointSprite
{
	//privateメンバ変数
private:
	LPDIRECT3DVERTEXBUFFER9 m_pPointBuffer;
	LPDIRECT3DTEXTURE9		m_pTexture;
	D3DXMATRIX				m_World;
	LPDIRECT3DDEVICE9       m_pDevice9;
	//シェーダ
	LPD3DXEFFECT		m_pEffect;


	//publicメンバ変数
public:
	float ColorChange;
	float Value;
	float Speed;
	D3DXVECTOR3 m_Pos[9];
	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CPointSprite(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CPointSprite();
	//
	BOOL PointLoadTexture(const TCHAR *FileName);
	//初期化
	BOOL InitPointSprite(LPDIRECT3DDEVICE9);
	//ポイントスプライト描画
	void DrawPointSprite(Vertex3[],int, bool);
	void Update();

	//=====セット関数====//
	void SetD3DTexture(LPDIRECT3DTEXTURE9 pTexture)
	{
		m_pTexture = pTexture;
	}

	void SetPosition(D3DXVECTOR3 pos,int num)
	{
		m_Pos[num] = pos;
	}

	void SetSpeed(float speed)
	{
		Speed = speed;
	}

};