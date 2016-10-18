#ifndef ___BEACH_BALL_H_
#define ___BEACH_BALL_H_
#pragma once

#include "../Global/Global.h"
#include "../Camera/Camera.h"
#include "../Graphic/Xfile.h"
#include "../Graphic/ModelManager.h"
#include"../Lib/Collider.h"

namespace Scene
{
	namespace GameScene
	{
		class BeachBall
		{
		public:
			enum State
			{
				kLand,
				kHold,
				kThrow,
				kAir,
			};

			BeachBall(LPDIRECT3DDEVICE9);
			~BeachBall();

			void Update();
			void Draw();
			BOOL HitCheck(D3DXVECTOR3 check);

			D3DXVECTOR3 position() { return position_; }
			State state() { return state_; }
			void set_position(D3DXVECTOR3 position) { position_ = position; }
			void set_rotation(D3DXVECTOR3 rotation) { rotation_ = rotation; }
			void set_state(State state) { state_ = state; }
			void ResetBeachBall();
		private:
			const FLOAT kAccelerationGravity;  // 重力加速度

			void Land();  // 地面に着地中の処理
			void Hold(); // プレイヤーが持っている時の処理
			void Throw(); // プレイヤーが投げた時の処理
			void Air();  // 空中の処理

						 // レイピッキング
						 // 入力：対象のメッシュ
			void RayPick(const LPD3DXMESH&, const D3DXMATRIX&);

			LPDIRECT3DDEVICE9 pd3dDevice_;  // デバイス
			State state_; // ボールの状態
			D3DXVECTOR3 position_;  // 座標
			D3DXVECTOR3 direction_;  // 移動方向
			D3DXVECTOR3 old_1f_position_; // 1F前の座標
			D3DXVECTOR3 rotation_; // プレイヤーの向き
			FLOAT gravity_; // 重力
			FLOAT ball_speed_;  // ビーチボールの移動量
		};

	}
}

#endif
