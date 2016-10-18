#include "BB.h"

Cbb::Cbb() : kAccelerationGravity(0.01f)
{
	m_BeachBallPos= { 0.0f, 80.0f, 0.0f };
	m_State = Air;

	direction_ = { 0.0f, 0.0f, 0.0f };
	old_1f_position_ = m_BeachBallPos;

	gravity_ = 0.0f;
	ball_speed_ = 1.0f;
}

Cbb::~Cbb()
{
	
}

void Cbb::BB_ResetState()
{
	m_BeachBallPos = { 0.0f, 80.0f, 0.0f };
	m_State = Air;
	direction_ = { 0.0f, 0.0f, 0.0f };
	old_1f_position_ = m_BeachBallPos;
	gravity_ = 0.0f;
	ball_speed_ = 1.0f;
}


BOOL Cbb::PutWeapon(D3DXVECTOR3 pos)
{
	if (m_State == Have)
	{
		m_State = Land;
		m_BeachBallPos = pos;
		return TRUE;
	}

	return FALSE;
}


BOOL Cbb::GetWeapon(D3DXVECTOR3 pPos)
{
	D3DXVECTOR3 dis = pPos - m_BeachBallPos;
	
	if (abs(D3DXVec3Length(&dis)) <= 3.0f)
	{
		m_State = Have;
		return TRUE;
	}

	return FALSE;
}

void Cbb::Ground()
{
	direction_ = { 0.0f, 0.0f, 0.0f };
	gravity_ = 0.0f;
}

void Cbb::BHave()
{
	direction_ = { 0.0f, 0.0f, 0.0f };
	ball_speed_ = 1.0f;
}

void Cbb::BThrow()
{
	m_BeachBallPos.y += 3.0f;
	direction_ = { 0.0f, 1.0f, 1.0f };
	m_State = Air;
	D3DXMATRIX rotate_matrix;
	D3DXMatrixRotationY(&rotate_matrix, m_BeachBallRot.y - D3DX_PI);
	D3DXVec3TransformCoord(&direction_, &direction_, &rotate_matrix);
}

void Cbb::BAir()
{
	gravity_ += kAccelerationGravity;

	if (ball_speed_ > 0.0f)
	{
		ball_speed_ -= 0.001f;
	}
	else
	{
		ball_speed_ = 0.0f;
	}

	direction_.y -= gravity_;

	// 地形との衝突判定
	//地面
	RayPick(CModelManager::GetInstance()->GetMesh(0), CModelManager::GetInstance()->GetMatWorld(0));
	//
	RayPick(CModelManager::GetInstance()->GetMesh(13), CModelManager::GetInstance()->GetMatWorld(13));
	//橋
	RayPick(CModelManager::GetInstance()->GetMesh(12), CModelManager::GetInstance()->GetMatWorld(12));
}

void Cbb::BUpdate(D3DXVECTOR3 pos, D3DXVECTOR3 rotate)
{
	if (m_State == Land)
	{
		Ground();
	}

	if (m_State == Have)
	{
		BHave();
	}

	if (m_State == Throw)
	{
		BThrow();
	}

	if (m_State == Air)
	{
		BAir();
	}

	D3DXVec3Normalize(&direction_, &direction_);
	direction_ *= 1.0f;
	old_1f_position_ = m_BeachBallPos; // 今の座標を保存
	m_BeachBallPos += direction_; // 座標移動
	//MyOutputDebugString("ビーチボール更新\n");
}

void Cbb::Attack()
{
	
	if (m_State == Have)
	{
		m_State = Throw;
	}
	//MyOutputDebugString("ビーチボールで攻撃！\n");
}

BOOL Cbb::HitCheck(D3DXVECTOR3 checkPos)
{
	
	if (Collider::GetInstance()->EasyCheck(m_BeachBallPos, checkPos, 5.0f))
	{
		return TRUE;
	}
	return FALSE;
}

void Cbb::Draw()
{
	if (m_State != Have)
	{
		CModelManager::GetInstance()->Draw(8, m_BeachBallPos, { 0.0f,0.0f,0.0f }, { 0.1f, 0.1f, 0.1f }, 1.0f);
	}
}

// 定義内容：レイピッキングでの衝突判定
void Cbb::RayPick(const LPD3DXMESH& target_mesh, const D3DXMATRIX& target_world_matrix)
{
	// 判定に必要なものを用意
	D3DXMATRIX world_matrix_inverse = target_world_matrix;
	D3DXMatrixInverse(&world_matrix_inverse, NULL, &target_world_matrix);
	D3DXVECTOR3 ray_start = old_1f_position_;
	D3DXVECTOR3 ray_end = m_BeachBallPos + direction_;
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
			m_BeachBallPos.y += gravity_ + 2.5f;
			m_State = Land;
			m_BeachBallPos.y = 14.0f;
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