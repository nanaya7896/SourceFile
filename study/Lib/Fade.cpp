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
	
	// �V�F�[�_�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// �f�o�C�X
		"Effect/ColorEdit.fx",		// �t�@�C����
		NULL,			// �v���v���Z�b�T��`�ւ̃|�C���^
		NULL,			// �I�v�V�����̃C���^�[�t�F�C�X�|�C���^ 
		0,				// �R���p�C���I�v�V����
		NULL,			// �G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&m_pEffect,		// �G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^
		&pErrorMsgs);	// �G���[����ьx���̃��X�g���܂ރo�b�t�@
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

void Fade::Draw()
{
	if (m_pEffect)
	{
		// �e�N�j�b�N�̑I��
		m_pEffect->SetTechnique("textured");
		// �p�X���w��
		m_pEffect->Begin(0, 0);
		


		// �V�F�[�_�ݒ���O���{�ɍX�V
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

