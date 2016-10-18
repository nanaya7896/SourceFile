/*
	チームのロゴとタイトル画面を表示するクラス
	

*/
#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/PointSprite.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"

//=====クラス宣言=====//
class CTitle
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9	m_pDevice9;

	
	//publicメンバ変数
public:
	//仮フェード
	float fadeTime;
	bool isTitle;
	float timeCount;
	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CTitle(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CTitle();
	//リソース生成
	void CreateResource(LPDIRECT3DDEVICE9 pDevice9);
	void AllReset();
	//更新
	void Update();
	//描画
	void Draw();
};