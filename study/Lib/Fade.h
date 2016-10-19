#pragma once
#include"../Global/Global.h"
#include"../Graphic/TextureManager.h"

class Fade {

private:
	static Fade* m_pFadeInstance;	//インスタンス変数
	float		alpha = 0.0f;
	LPDIRECT3DDEVICE9   m_pDevice;
	LPD3DXEFFECT		m_pEffect;
public:


private:


public:
	 Fade(LPDIRECT3DDEVICE9);
	~Fade();

	static void Create(LPDIRECT3DDEVICE9);
	static void Destory();

	static Fade* GetInstance() {
		return m_pFadeInstance;
	}

	BOOL Init();
	BOOL Update(BOOL inOut, float fadeTime);
	void Draw();
};