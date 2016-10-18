/*
	�`�[���̃��S�ƃ^�C�g����ʂ�\������N���X
	

*/
#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/PointSprite.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"

//=====�N���X�錾=====//
class CTitle
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9	m_pDevice9;

	
	//public�����o�ϐ�
public:
	//���t�F�[�h
	float fadeTime;
	bool isTitle;
	float timeCount;
	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CTitle(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CTitle();
	//���\�[�X����
	void CreateResource(LPDIRECT3DDEVICE9 pDevice9);
	void AllReset();
	//�X�V
	void Update();
	//�`��
	void Draw();
};