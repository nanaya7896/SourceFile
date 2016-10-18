#pragma once
#include"../Global/Global.h"
#include"../Lib/DirectX/DInput.h"
#include"../Lib/Collider.h"
#include"../Zombie/Zombie.h"

#define GROUND	0
#define	HAVE	1
#define SHOT	2


#define	BULLET_MAX 64
#define BULLET_SPEED 5.0f
#define RECAST_TIME 0.2f

class WaterGun {

	//privateメンバ変数
private:

	static WaterGun* m_pWaterGunInstance;	//インスタンス変数

	CDInput*				w_pInput;//インプット系列
									 //水鉄砲の位置
	D3DXVECTOR3 gunpos;

	//弾の構造体
	struct Bullet
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rotate;
		int			deg;
		bool		used;
	};
	Bullet bullet[BULLET_MAX];

	int start = 0;
	int end = 0;

	//弾の保存した数
	int bulletnum = 0;
	//水鉄砲の状態
	int state;
	//回転率
	D3DXVECTOR3 rotate;

	//おいているかどうか
	BOOL floor;

	//発射中か
	BOOL schott;

	D3DXMATRIX				m_bullet;

public:
	WaterGun();
	~WaterGun();

	static void Create();//インスタンスを生成する
	static void Destory();//インスタンスを削除

						  //インスタンスを取得する
	static WaterGun* GetInstance() {
		return m_pWaterGunInstance;
	}

	void Init(CDInput* m_pInput);
	BOOL GetGun(D3DXVECTOR3 tmpPos);
	void DisGetGun();
	void Update(D3DXVECTOR3 tmpPos, D3DXVECTOR3 tmpRotate);
	void Draw();

	BOOL HitCheck(D3DXVECTOR3 checkPos);

	int wGetState()
	{
		return state;
	}

	void ResetWaterGun();
};
