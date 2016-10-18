#ifndef INCLUDE_YUKI_ANIMATION_H
#define INCLUDE_YUKI_ANIMATION_H
#pragma once

#include "../Global/Global.h"
#include "../Graphic/DirectXAnimation/AllocateHierarchy.h"
#include "../Camera/Camera.h"

class YukiAnimation
{
private:
	LPDIRECT3DDEVICE9 d3d_device9_;	// 描画デバイス
	D3DXVECTOR3 position_;	// 座標
	D3DXVECTOR3 angle_;		// アングル
	D3DXVECTOR3 scale_;
	FLOAT animation_time_; // アニメーション再生速度
	CAnimMesh* animation_;	// アニメーションクラス
	LPD3DXEFFECT effect_; // LPD3DXEFFECTクラス

	FLOAT directionRadian;

public:
	YukiAnimation(LPDIRECT3DDEVICE9 pd3dDevice, char* pFileName);							// コンストラクタ
	~YukiAnimation();							// デストラクタ

	void Update();						// 更新
	void Draw();						// 描画
	BOOL Initialize(char* pFileName); // 初期化処理

	D3DXVECTOR3 GetPosition()
	{
		return position_;
	}
	D3DXVECTOR3 GetAngle()
	{
		return angle_;
	}

	void SetPosition(D3DXVECTOR3 pos)
	{
		position_ = pos;
	}

	D3DXVECTOR3 angle() { return angle_; }
	void set_angle(D3DXVECTOR3 Value) { angle_ = Value; }
	void set_animation_time(FLOAT animation_time) { animation_time_ = animation_time; }
	void SetDRadian(FLOAT rad) { directionRadian = rad; }
	LPD3DXEFFECT effect() { return effect_; } // エフェクトのげったー
	void ResetTime()
	{
		animation_->ResetTime();
	}
	FLOAT getFinishTime()
	{
		return animation_->GetAnimFinishTime();
	}

	DOUBLE getTime()
	{
		return animation_->GetTime();
	}

};

#endif
