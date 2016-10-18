#include "beach_ball.h"

namespace Scene
{
	namespace GameScene
	{
		BeachBall::BeachBall(LPDIRECT3DDEVICE9 pd3dDevice) : kAccelerationGravity(0.01f) {
			pd3dDevice_ = pd3dDevice;
			state_ = kAir;
			position_ = { 0.0f, 80.0f, 0.0f };
			direction_ = { 0.0f, 0.0f, 0.0f };
			old_1f_position_ = position_;
			rotation_ = { 0.0f, 0.0f, 0.0f };
			gravity_ = 0.0f;
			ball_speed_ = 1.0f;
		}

		BeachBall::~BeachBall() {

		}

		// 定義内容：更新
		void BeachBall::Update()
		{
			if (state_ == kLand)
				Land();

			if (state_ == kHold)
				Hold();

			if (state_ == kThrow)
				Throw();

			if (state_ == kAir)
				Air();

			D3DXVec3Normalize(&direction_, &direction_);
			direction_ *= 1.0f;
			old_1f_position_ = position_; // 今の座標を保存
			position_ += direction_; // 座標移動
		}

		void BeachBall::Draw()
		{
			if (state_ != kHold)
				CModelManager::GetInstance()->Draw(8, position_, rotation_, { 0.1f, 0.1f, 0.1f }, 1.0f);
		}

		BOOL BeachBall::HitCheck(D3DXVECTOR3 check) {

			if (Collider::GetInstance()->EasyCheck(position_, check, 5.0f)) {
				return TRUE;
			}
			return FALSE;
		}
		void Scene::GameScene::BeachBall::ResetBeachBall()
		{
			state_ = kAir;
			position_ = { 0.0f, 80.0f, 0.0f };
			direction_ = { 0.0f, 0.0f, 0.0f };
			old_1f_position_ = position_;
			rotation_ = { 0.0f, 0.0f, 0.0f };
			gravity_ = 0.0f;
			ball_speed_ = 1.0f;
		}
		// 定義内容：着地中の処理
		void BeachBall::Land()
		{
			direction_ = { 0.0f, 0.0f, 0.0f };
			gravity_ = 0.0f;
		}

		void BeachBall::Hold()
		{
			direction_ = { 0.0f, 0.0f, 0.0f };
			ball_speed_ = 1.0f;
		}

		void BeachBall::Throw()
		{
			position_.y += 10.0f;
			direction_ = { 0.0f, 1.0f, 1.0f };
			state_ = kAir;
			D3DXMATRIX rotate_matrix;
			D3DXMatrixRotationY(&rotate_matrix, rotation_.y-D3DX_PI);
			D3DXVec3TransformCoord(&direction_, &direction_, &rotate_matrix);
		}

		// 定義内容：空中の処理
		void BeachBall::Air()
		{
			gravity_ += kAccelerationGravity;

			if (ball_speed_ > 0.0f)
				ball_speed_ -= 0.001f;
			else
				ball_speed_ = 0.0f;

			direction_.y -= gravity_;

			// 地形との衝突判定
			//地面
			RayPick(CModelManager::GetInstance()->GetMesh(0), CModelManager::GetInstance()->GetMatWorld(0));
			//
			RayPick(CModelManager::GetInstance()->GetMesh(13), CModelManager::GetInstance()->GetMatWorld(13));
			//橋
			RayPick(CModelManager::GetInstance()->GetMesh(12), CModelManager::GetInstance()->GetMatWorld(12));
		}

		// 定義内容：レイピッキングでの衝突判定
		void BeachBall::RayPick(const LPD3DXMESH& target_mesh, const D3DXMATRIX& target_world_matrix)
		{
			// 判定に必要なものを用意
			D3DXMATRIX world_matrix_inverse= target_world_matrix;
			D3DXMatrixInverse(&world_matrix_inverse, NULL, &target_world_matrix);
			D3DXVECTOR3 ray_start = old_1f_position_;
			D3DXVECTOR3 ray_end = position_ + direction_;
			D3DXVec3TransformCoord(&ray_start, &ray_start, &world_matrix_inverse);
			D3DXVec3TransformCoord(&ray_end, &ray_end, &world_matrix_inverse);
			BOOL hit = FALSE;
			DWORD face_index = 0;
			FLOAT u = 0.0f, v = 0.0f, dist = 0.0f;

			// レイピッキング
			D3DXIntersect(target_mesh, &ray_start, &ray_end, &hit, &face_index, &u, &v, &dist, NULL, NULL);
			if (hit)
			{
				WORD* index[3];
				FLOAT* index_buffer = 0;

				target_mesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&index_buffer);
				for (DWORD i = 0; i < 3; i++)
					index[i] = (WORD*)&index_buffer[face_index * 3 + i];

				target_mesh->UnlockIndexBuffer();

				D3DXVECTOR3* vertex[3];
				FLOAT* vertex_buffer = 0;

				target_mesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**)&vertex_buffer);
				for (DWORD i = 0; i < 3; i++)
					vertex[i] = (D3DXVECTOR3*)&vertex_buffer[*index[i]];
				target_mesh->UnlockVertexBuffer();

				D3DXVECTOR3 p12(*vertex[1] - *vertex[0]);
				D3DXVECTOR3 p23(*vertex[2] - *vertex[1]);

				D3DXVECTOR3 normal;
				D3DXVec3Cross(&normal, &p12, &p23);
				D3DXVec3Normalize(&normal, &normal);

				D3DXVECTOR3 reflect = (direction_ - 2.0f * D3DXVec3Dot(&direction_, &normal) * normal);

				if (direction_.z <= 0.1f)
				{
					//position_.y = dist - ray_end.y;
					position_.y += gravity_ + 2.5f;
					state_ = kLand;
				}
				else
				{
					D3DXVec3Normalize(&reflect, &reflect);
					reflect *= 0.5f;
					gravity_ *= 0.5f;
					direction_ = reflect;
				}
			}
		}
	}
}