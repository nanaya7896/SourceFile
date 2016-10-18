#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/TextureManager.h"
//=====クラス宣言=====//
class CDecorate
{
	//privateメンバ変数
private:
	FLOAT alphaValue=0.0f;
	FLOAT alphaFlashScore=0.0f;
	FLOAT value=0.001f;
	BOOL isFlash=FALSE;
	//publicメンバ変数
public:


	//privateメンバ関数
private:


	//publicメンバ関数
public:
	CDecorate();
	~CDecorate();
	void Update(D3DXVECTOR3 pPos,D3DXVECTOR3 ePos);
	void UpdateeFlashScore();
	void FalshScreen();
	void FlashScore();

	void ResetDeco();

	void SetisFlash(BOOL is)
	{
		isFlash = is;
	}

};