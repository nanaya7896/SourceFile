/*
	�X�R�A��ۑ��`�悷��֐�


*/
#pragma once
//=====Include File=====//
#include"../Global/Global.h"

//=====�N���X�錾=====//
class CScore
{
	//private�����o�ϐ�
private:
	int ZombieNum;
	//int tenPosition;
	//int onePosition;
	int score[3] = {0};
	//public�����o�ϐ�
public:
	

	//private�����o�֐�
private:
	//�V���O���g��
	static CScore*		m_pInstance;

	//public�����o�֐�
public:
	CScore();
	~CScore();

	static void Create();
	static void Destroy();

	void Update();

	void ResetScore();
	//�Z�b�g�֐�
	/*�X�R�A���Z�p*/
	void SetAddScore(int num)
	{
		ZombieNum += num;
	}
	void SetScoreNum(int num)
	{
		ZombieNum = num;
	}
	void SetScore();

	//�Q�b�g�֐�
	int GetScore(int num)
	{
		return score[num];
	}
	static CScore* GetInstnce()
	{
		return m_pInstance;
	}

};