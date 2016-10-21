#include "Fade.h"

Fade* Fade::m_pFadeInstance = NULL;

Fade::Fade(LPDIRECT3DDEVICE9 pDevice) 
{
	m_pDevice = pDevice;
}

Fade::~Fade() {

	//SAFE_DELETE(m_pFadeInstance);
}

void Fade::Create(LPDIRECT3DDEVICE9 pDevice) {

	if (!m_pFadeInstance) {
		m_pFadeInstance = new Fade(pDevice);
	}
}

void Fade::Destory() {
	SAFE_DELETE(m_pFadeInstance);
}

BOOL Fade::Init() {

	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;
	
	// シェーダの読み込み
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// デバイス
		"Effect/ColorEdit.fx",		// ファイル名
		NULL,			// プリプロセッサ定義へのポインタ
		NULL,			// オプションのインターフェイスポインタ 
		0,				// コンパイルオプション
		NULL,			// エフェクトプールオブジェクトへのポインタ
		&m_pEffect,		// エフェクトオブジェクトへのポインタ
		&pErrorMsgs);	// エラーおよび警告のリストを含むバッファ
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}


	return TRUE;
}
/*
BOOL inOut = FALSE 明るくなる
			　TRUE　くらくなる

*/
BOOL Fade::Update(BOOL inOut, float fadeTime) {

	static BOOL		nowInOut = TRUE;
	static DWORD	initTime;

	//BOOL rBoolen = FALSE;

	if (nowInOut != inOut) {
		initTime = timeGetTime();
		nowInOut = inOut;
	}

	//経過したミリ秒を計算
	float time = (double)(timeGetTime() - initTime) / 1000.0;
	//アルファ値の２５５段階に変換し、それをフェードをさせる時間を割る
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

void Fade::Draw()
{
	if (m_pEffect)
	{
		// テクニックの選択
		m_pEffect->SetTechnique("textured");
		// パスを指定
		m_pEffect->Begin(0, 0);
		


		// シェーダ設定をグラボに更新
		m_pEffect->CommitChanges();
		CTextureManager* pTextureManager = CTextureManager::GetInstance();
		pTextureManager->Draw(22, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, alpha, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
		//
		m_pEffect->EndPass();
	}
	//CTextureManager* pTextureManager = CTextureManager::GetInstance();
	//pTextureManager->Draw(22, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, alpha, D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f));
	//pTextureManager->Draw(22, D3DXVECTOR3((WINDOW_WIDTH / 20.0f) * 3.0f, 0.0f, 0.0f), 1, 1.0f, D3DXVECTOR3(100.0f, 100.0f, 0.0f));
}

