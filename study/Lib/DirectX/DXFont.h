#pragma once
//=====Include File=====//
#include"../../Global/Global.h"

//=====クラス宣言=====//
class CDXFont
{
	//privateメンバ変数
private:
	//フォント
	LPD3DXFONT m_pFont;


	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CDXFont(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CDXFont();
	void CreateFont(LPDIRECT3DDEVICE9);
	void DrawText(int, int, const char*, D3DCOLOR,...);
	//=====ゲット関数====//
	LPD3DXFONT getDXFont()
	{
		return m_pFont;
	}
};