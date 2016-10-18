/*
	スコアを保存描画する関数


*/
#pragma once
//=====Include File=====//
#include"../Global/Global.h"

//=====クラス宣言=====//
class CScore
{
	//privateメンバ変数
private:
	int ZombieNum;
	//int tenPosition;
	//int onePosition;
	int score[3] = {0};
	//publicメンバ変数
public:
	

	//privateメンバ関数
private:
	//シングルトン
	static CScore*		m_pInstance;

	//publicメンバ関数
public:
	CScore();
	~CScore();

	static void Create();
	static void Destroy();

	void Update();

	void ResetScore();
	//セット関数
	/*スコア加算用*/
	void SetAddScore(int num)
	{
		ZombieNum += num;
	}
	void SetScoreNum(int num)
	{
		ZombieNum = num;
	}
	void SetScore();

	//ゲット関数
	int GetScore(int num)
	{
		return score[num];
	}
	static CScore* GetInstnce()
	{
		return m_pInstance;
	}

};