#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Camera/Camera.h"
#include"../../Lib/DirectX/DInput.h"
#include"../../Graphic/TextureManager.h"

//=====�N���X�錾=====//
class CResult
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9       m_pDevice9;

	//�L�[���͊i�[�p�z��
	CDInput*				m_pDInput;

	//�F
	int color;

	int score[3] = { 0 };
	//public�����o�ϐ�
public:

	//private�����o�֐�
private:
	

	//public�����o�֐�
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