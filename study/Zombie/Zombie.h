#pragma once

#include"../Global/Global.h"
#include"../Graphic/DirectXAnimation/AllocateHierarchy.h"
#include"../Camera/Camera.h"
#include"../Math/MyMathLib.h"
#include"../Lib/Collider.h"
#include"../Scene/Score.h"
#include"../Player/WaterSlider.h"
#include"../Design/Decorate.h"
#include"../Actor/Actor.h"
#define MaxZombie	53
#define NumZombies	50
#define SKIN_ZNUM	5
#define MOTION_ZNUM 7

enum  ZOMBIESTATE
{
	CHASER,
	DEATH,
	FLOW,
	WALLOW,
	STAIRS,
	HITSTOP,
	STAIRS_Walk
};



typedef struct
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vAngle;
	D3DXVECTOR3	vScale;
	BOOL		isAlive;
	FLOAT		LifeTime;
	D3DXMATRIX	matRotation;
	FLOAT		DirectionRadian;

	LPD3DXANIMATIONCONTROLLER	AnimController[MOTION_ZNUM];			// アニメーションコントローラ
																		/*池原追加*/
	INT			state;
	INT			animNum;
	INT			hitMesh;
	INT			ZombieStaNum;
	INT			z_num;
	BOOL        isStairs;
	INT			val;
	/*以上*/
	INT			skin;
}ZOMBIEINFO;


class CActor;


class CZombie : public CActor
{
private:
	//描画デバイス
	LPDIRECT3DDEVICE9   m_pDevice;
	
	
	CScore				m_Score;
	CMathLib*			m_pMathlib;
	
	LPD3DXEFFECT		m_pEffect;
	
	FLOAT				m_Speed;
	D3DXVECTOR3         scale_;
	D3DXVECTOR3         m_tmp;



	D3DXVECTOR3 STAIRSPOSITION[5] =
	{
		D3DXVECTOR3(-174.5f,13.7258f,13.8f),
		D3DXVECTOR3(-174.5f,40.49f,-55.49f),
		D3DXVECTOR3(-155.0f,40.493f,-55.49f),
		D3DXVECTOR3(-155.0f,63.232f,23.0f),
		D3DXVECTOR3(-155.0f,63.232f,23.0f)
	};
	FLOAT dividValue[3] = { 200.0f,120.0f,200.0f };
	BOOL isValueSet = FALSE;
	D3DXVECTOR3 bez[101];

	//アニメーション読み込み
	TCHAR Z_MOTIONOBJECT[SKIN_ZNUM][MOTION_ZNUM][MAX_PATH] =
	{
		//ゾンビのアニメーション
		{
			"Model/zonbi_hokou.X",					// 0
			"Model/zonbi_obore.X",					// 1
			//死亡
			"Model/zonbi_pool1.X",					// 2
			//滑っている途中
			"Model/zonbi_pool2.X",					// 3
			//うきわゾンビ
			"Model/zonbipoolukiwa.X",				// 4
			"Model/zonbidamezi01.X",				// 5
			"Model/zonbidamezi02.X"					// 6
		},

		{
			"Model/zonbi_hokou2.X",					// 0
			"Model/zonbi_obore2.X",					// 1
			//死亡
			"Model/zonbi_pool12.X",					// 2
			//滑っている途中
			"Model/zonbi_pool22.X",					// 3
			//うきわゾンビ
			"Model/zonbipoolukiwa.X",				// 4
			"Model/zonbidamezi01.X",				// 5
			"Model/zonbidamezi02.X"					// 6
		},

		{
			"Model/zonbi_hokou3.X",					// 0
			"Model/zonbi_obore3.X",					// 1
			//死亡
			"Model/zonbi_pool13.X",					// 2
			//滑っている途中
			"Model/zonbi_pool23.X",					// 3
			//うきわゾンビ
			"Model/zonbipoolukiwa.X",				// 4
			"Model/zonbidamezi01.X",				// 5
			"Model/zonbidamezi02.X"					// 6
		},

		{
			"Model/zonbi_hokou4.X",					// 0
			"Model/zonbi_obore4.X",					// 1
			//死亡
			"Model/zonbi_pool14.X",					// 2
			//滑っている途中
			"Model/zonbi_pool24.X",					// 3
			//うきわゾンビ
			"Model/zonbipoolukiwa.X",				// 4
			"Model/zonbidamezi01.X",				// 5
			"Model/zonbidamezi02.X"					// 6
		},

		{
			//ゾンビのアニメーション
			"Model/zonbi_hokou5.X",					// 0
			"Model/zonbi_obore5.X",					// 1
			//死亡
			"Model/zonbi_pool15.X",					// 2
			//滑っている途中
			"Model/zonbi_pool25.X",					// 3
			//うきわゾンビ
			"Model/zonbipoolukiwa.X",				// 4
			"Model/zonbidamezi01.X",				// 5
			"Model/zonbidamezi02.X"					// 6
		}
	};

protected:
		ZOMBIEINFO*			m_ZombieInfo;
		CDecorate*			m_pDecorate;
		CAnimMesh*			m_pAnimation[SKIN_ZNUM][MOTION_ZNUM];
		CWaterSlider*		m_pWaterSlider[NumZombies];
		//スコアの値を収める変数
		int value;
public:
	CZombie();
	CZombie(LPDIRECT3DDEVICE9);
	~CZombie();
	BOOL Initialize();
	void PositionCheck(int num);
	void ZombieInfoInitialize();
	void Draw(DWORD);
	void AnimationPlay(DWORD);
	void ZombieWalk(D3DXVECTOR3, DWORD);
	float Random(float min, float max);
	int Random(int min, int max);
	void Update(D3DXVECTOR3 playerPos, DWORD num);
	BOOL AllZombiCheckCollider(D3DXVECTOR3 pos, int num);
	BOOL IsDeadCheck();
	void ResetZombieInfo();
	
	void ExcuteState(int num, D3DXVECTOR3 playerPos);
	/*ゾンビのステート関数*/
	void ZombieDeathState(int num);
	void ZombieFlowState(int num);
	void ZombieWalowState(int num);
	void ZombieWalkUp(int num);
	void Zombierevival(int num);
	void HitStop(int num);
	void STAIRS_WalkState(int num);
	/*ここまで*/

	void SetAniamtionNum(int num, int anim)
	{
		m_ZombieInfo[num].animNum = anim;
	}
	void SetState(int num, int state)
	{
		m_ZombieInfo[num].state = state;
	}
	void SetScale(int num, D3DXVECTOR3 scale)
	{
		m_ZombieInfo[num].vScale = scale;
	}


	int          GetAnimationNum_Z(int num) { return m_ZombieInfo[num].animNum; }
	D3DXVECTOR3  GetZombiePosition(int num) { return m_ZombieInfo[num].vPosition; }
	INT			 GetState(int num) { return m_ZombieInfo[num].state; }
	LPD3DXEFFECT GetEffect() { return m_pEffect; }



};
