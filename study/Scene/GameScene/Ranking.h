#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
#include"../Score.h"
//=====クラス宣言=====//
class CRanking
{
	//privateメンバ変数
private:
	FILE* fp;
	int rank[3];

	//publicメンバ変数
public:
	

	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CRanking();
	//デストラクタ
	~CRanking();
	//更新
	void Update();
	//描画
	void Draw();

	void RankinCheck();
};