/*
	���������`��E��������N���X

*/
#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
//=====�N���X�錾=====//
class COperating
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	COperating(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~COperating();

	void Update();
	void Draw();
};

