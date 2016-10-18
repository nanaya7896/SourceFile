#pragma once
////=====Include File=====//
#include"../../Global/Global.h"
#include"CSkinMeshAnimation.h"
#include"../../Camera/Camera.h"
#include"../ModelManager.h"
//=====クラス宣言=====//
class CDirectXAnimation
{
	//privateメンバ変数
private:
	//描画デバイス
	LPDIRECT3DDEVICE9		m_pDevice9;
	//シェーダ
	LPD3DXEFFECT			m_pEffect;
	//アニメーショントラック
	D3DXTRACK_DESC			m_TrackDesc;
	//アニメーション
	MY_HIERARCHY			m_cHierarchy;
	CSKIN_MESH				m_SkinMesh;
	//publicメンバ変数
public:
	//ポジション
	D3DXVECTOR3 m_PlayerPos;
	//回転
	D3DXVECTOR3 m_PlayerRot;
	//スケール
	D3DXVECTOR3 m_PlayerScale;
	//カメラの座標
	D3DXVECTOR3 m_CameraPos;
	
	//privateメンバ関数
private:


	//publicメンバ関数
public:
	//コンストラクタ
	CDirectXAnimation(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CDirectXAnimation();

	void LoadAnimXFile();
	void FreeAnim(LPD3DXFRAME);
	void Render();
	void AnimationChange(DWORD);

	//=====ゲット関数=====//
	

	//=====セット関数====//
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetScale(D3DXVECTOR3 Scale);

	void SetTransformPos(D3DXVECTOR3 pos)
	{
		m_PlayerPos = pos;
	}
	void SetTransformRot(D3DXVECTOR3 rot)
	{
		m_PlayerRot = rot;
	}

};