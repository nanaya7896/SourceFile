#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/TextureManager.h"
//=====�N���X�錾=====//
class CDecorate
{
	//private�����o�ϐ�
private:
	FLOAT alphaValue=0.0f;
	FLOAT alphaFlashScore=0.0f;
	FLOAT value=0.001f;
	BOOL isFlash=FALSE;
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
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