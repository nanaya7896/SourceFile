#pragma once
#include"../Global/Global.h"
#include"../Graphic/TextureManager.h"

class Fade {

private:
	static Fade* m_pFadeInstance;	//インスタンス変数
	float		alpha = 0.0f;
public:


private:


public:
	 Fade();
	~Fade();

	static void Create();
	static void Destory();

	static Fade* GetInstance() {
		return m_pFadeInstance;
	}

	BOOL Init();
	BOOL Update(BOOL inOut, float fadeTime);
	void Draw();
};