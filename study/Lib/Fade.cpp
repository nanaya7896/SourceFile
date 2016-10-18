#include "Fade.h"

Fade* Fade::m_pFadeInstance = NULL;

Fade::Fade() {

}

Fade::~Fade() {

	//SAFE_DELETE(m_pFadeInstance);
}

void Fade::Create() {

	if (!m_pFadeInstance) {
		m_pFadeInstance = new Fade();
	}
}

void Fade::Destory() {
	SAFE_DELETE(m_pFadeInstance);
}

BOOL Fade::Init() {

	return TRUE;
}
/*
BOOL inOut = FALSE ���邭�Ȃ�
			�@TRUE�@���炭�Ȃ�

*/
BOOL Fade::Update(BOOL inOut, float fadeTime) {

	static BOOL		nowInOut = TRUE;
	static DWORD	initTime;

	//BOOL rBoolen = FALSE;

	if (nowInOut != inOut) {
		initTime = timeGetTime();
		nowInOut = inOut;
	}


	//�o�߂����~���b���v�Z
	float time = (double)(timeGetTime() - initTime) / 1000.0;
	//�A���t�@�l�̂Q�T�T�i�K�ɕϊ����A������t�F�[�h�������鎞�Ԃ�����
	alpha = time / fadeTime;

	if (nowInOut) {
		if (alpha > 1.0f) { 
			alpha = 1.0f; 
			return TRUE;
		}
	}
	else{
		alpha = 1.0f - alpha;
		if (alpha < 0.0f) {
			alpha = 0.0f;
			return TRUE;
		}
	}
	return FALSE;
}

void Fade::Draw() {
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	pTextureManager->Draw(22, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, alpha, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	//pTextureManager->Draw(22, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 3.0f, 0.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
}