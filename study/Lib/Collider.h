#pragma once
#include"../Global/Global.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/DirectX/DInput.h"

//Collider.h
#define PLAYER_HEIGHT 6

class Collider {

	//privateメンバ変数
private:

	static Collider* m_pColiderInstance;	//インスタンス変数

	struct MESH_STATE
	{
		LPD3DXMESH mesh;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rotate;
	};
	MESH_STATE pMesh[MAXFILE_NUM];

	int num = 0;								//登録した数
public:

private:


	//publicメンバ関数
public:
	Collider();	//コンストラクタ
	~Collider();//デストラクタ
	static void Create();//インスタンスを生成する
	static void Destory();//インスタンスを削除

						  //インスタンスを取得する
	static Collider* GetInstance() {
		return m_pColiderInstance;
	}

	//簡易的な当たり判定
	BOOL EasyCheck(D3DXVECTOR3 pos, D3DXVECTOR3 posCheck, float large);
	BOOL Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large);
	BOOL Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large, float *get, D3DXVECTOR3 *r_pos);

	//オブジェクトを読み込み頂点を取得格納
	bool SetObj(D3DXVECTOR3 pos, LPD3DXMESH pmesh);

	//現在の位置とy軸の当たり判定を決める
	BOOL GroundCol(D3DXVECTOR3 pos, D3DXVECTOR3 rotatepos, int *Hitnum, float *numreturn);
	BOOL RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate);
	BOOL RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, float * HitNum);
	BOOL DirectRayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, int objnum, float * HitNum);


};