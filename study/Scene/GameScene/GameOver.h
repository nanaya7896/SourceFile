#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
//=====�N���X�錾=====//
class CGameOver
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9	m_pDevice9;

	float				m_pos;
	BOOL				isPos;
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CGameOver(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CGameOver();

	//�X�V
	void Update();
	//�`��
	void Draw();

	void isArrowPos(BOOL isleft);

	void ResetValue();
	BOOL GetReturnPos()
	{
		return isPos;
	}
};