#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
//=====クラス宣言=====//
class CCredit
{
	//privateメンバ変数
private:
	LPDIRECT3DDEVICE9			m_pDevice9;

	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CCredit(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CCredit();
	//更新
	void Update();
	//描画
	void Draw();
};