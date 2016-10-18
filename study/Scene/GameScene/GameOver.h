#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
//=====クラス宣言=====//
class CGameOver
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9	m_pDevice9;

	float				m_pos;
	BOOL				isPos;
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CGameOver(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CGameOver();

	//更新
	void Update();
	//描画
	void Draw();

	void isArrowPos(BOOL isleft);

	void ResetValue();
	BOOL GetReturnPos()
	{
		return isPos;
	}
};