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
			const FLOAT kAccelerationGravity;  // �d�͉����x

			void Land();  // �n�ʂɒ��n���̏���
			void Hold(); // �v���C���[�������Ă��鎞�̏���
			void Throw(); // �v���C���[�����������̏���
			void Air();  // �󒆂̏���

						 // ���C�s�b�L���O
						 // ���́F�Ώۂ̃��b�V��
			void RayPick(const LPD3DXMESH&, const D3DXMATRIX&);

			LPDIRECT3DDEVICE9 pd3dDevice_;  // �f�o�C�X
			State state_; // �{�[���̏��
			D3DXVECTOR3 position_;  // ���W
			D3DXVECTOR3 direction_;  // �ړ�����
			D3DXVECTOR3 old_1f_position_; // 1F�O�̍��W
			D3DXVECTOR3 rotation_; // �v���C���[�̌���
			FLOAT gravity_; // �d��
			FLOAT ball_speed_;  // �r�[�`�{�[���̈ړ���
		};

	}
}

#endif
