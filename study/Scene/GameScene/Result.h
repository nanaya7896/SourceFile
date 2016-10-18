#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Camera/Camera.h"
#include"../../Lib/DirectX/DInput.h"
#include"../../Graphic/TextureManager.h"

//=====クラス宣言=====//
class CResult
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9       m_pDevice9;

	//キー入力格納用配列
	CDInput*				m_pDInput;

	//色
	int color;

	int score[3] = { 0 };
	//publicメンバ変数
public:

	//privateメンバ関数
private:
	

	//publicメンバ関数
public:
	CResult(LPDIRECT3DDEVICE9, CDInput*);
	~CResult();
	BOOL CreateResource();
	void Update();
	void Draw();

	void SetResultScore(int num,int value)
	{
		score[num] = value;
	}
	

};