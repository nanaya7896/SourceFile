#ifndef INCLUDE_YUKI_ANIMATION_H
#define INCLUDE_YUKI_ANIMATION_H
#pragma once

#include "../Global/Global.h"
#include "../Graphic/DirectXAnimation/AllocateHierarchy.h"
#include "../Camera/Camera.h"

class YukiAnimation
{
private:
	LPDIRECT3DDEVICE9 d3d_device9_;	// �`��f�o�C�X
	D3DXVECTOR3 position_;	// ���W
	D3DXVECTOR3 angle_;		// �A���O��
	D3DXVECTOR3 scale_;
	FLOAT animation_time_; // �A�j���[�V�����Đ����x
	CAnimMesh* animation_;	// �A�j���[�V�����N���X
	LPD3DXEFFECT effect_; // LPD3DXEFFECT�N���X

	FLOAT directionRadian;

public:
	YukiAnimation(LPDIRECT3DDEVICE9 pd3dDevice, char* pFileName);							// �R���X�g���N�^
	~YukiAnimation();							// �f�X�g���N�^

	void Update();						// �X�V
	void Draw();						// �`��
	BOOL Initialize(char* pFileName); // ����������

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
	LPD3DXEFFECT effect() { return effect_; } // �G�t�F�N�g�̂������[
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
