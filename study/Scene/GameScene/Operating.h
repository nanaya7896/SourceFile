/*
	操作説明を描画・説明するクラス

*/
#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
//=====クラス宣言=====//
class COperating
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9		m_pDevice9;
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	COperating(LPDIRECT3DDEVICE9);
	//デストラクタ
	~COperating();

	void Update();
	void Draw();
};

