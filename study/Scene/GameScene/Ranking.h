#pragma once
//=====Include File=====//
#include"../../Global/Global.h"
#include"../../Graphic/ModelManager.h"
#include"../../Graphic/TextureManager.h"
#include"../Score.h"
//=====�N���X�錾=====//
class CRanking
{
	//private�����o�ϐ�
private:
	FILE* fp;
	int rank[3];

	//public�����o�ϐ�
public:
	

	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CRanking();
	//�f�X�g���N�^
	~CRanking();
	//�X�V
	void Update();
	//�`��
	void Draw();

	void RankinCheck();
};