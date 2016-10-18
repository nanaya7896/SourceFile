#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/DirectX/DInput.h"

#include"../Lib/Collider.h"

//#define GROUND		0
//#define HAVE		1

//=====クラス宣言=====//
class CWaterBalloon
{
	//privateメンバ変数
private:
	static CWaterBalloon* m_pCWaterBalloonInstance;

	LPDIRECT3DDEVICE9	m_pDevice9;
	CDInput*			m_pDInput;

	//発射されたか
	BOOL m_isShot;

	//座標
	D3DXVECTOR3 m_waterPos;
	//プレイヤーの座標・アングル
	D3DXVECTOR3 m_playerPos;
	D3DXVECTOR3 m_playerRot;
	//
	D3DXVECTOR3 gunPos;

	float value;

	int state;
	//publicメンバ変数
public:
	//

	//privateメンバ関数
private:


	//publicメンバ関数
public:
	CWaterBalloon();
	~CWaterBalloon();

	static void Create();//インスタンスを生成する
	static void Destory();//インスタンスを削除

						  //インスタンスを取得する
	static CWaterBalloon* GetInstance() {
		return m_pCWaterBalloonInstance;
	}

	void CreateResource(LPDIRECT3DDEVICE9, CDInput*);
	void UpdateWater();
	void Update(D3DXVECTOR3 vPos, D3DXVECTOR3 vRot);
	void Draw();

	BOOL GetWaterGun(D3DXVECTOR3 vPos, D3DXVECTOR3 vRot);
	void DisGun();
	BOOL HitCheck(D3DXVECTOR3 checkPos);

	FLOAT ChangeTF(FLOAT value, FLOAT rot, BOOL isSin);

	void ResetWaterBullon();
	//=====セット関数=====/
	void SetisShot(BOOL isBool)
	{
		m_isShot = isBool;
	}
	void SetGunPos(D3DXVECTOR3 pos)
	{
		gunPos = pos;
	}

	void SetPlayerPos(D3DXVECTOR3 pos)
	{
		m_playerPos = pos;
	}

	void SetPlayerRot(D3DXVECTOR3 rot)
	{
		if (!m_isShot)
			m_playerRot = rot;
	}

	void SetWaterPos(D3DXVECTOR3 pos)
	{
		if (!m_isShot)
			m_waterPos = pos;
	}

	void SetState(int st)
	{
		state = st;
	}
	//=====ゲット関数=====//
	int GetState()
	{
		return state;
	}
	BOOL Getishot()
	{
		return m_isShot;
	}
};