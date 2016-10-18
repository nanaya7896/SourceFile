#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/DirectX/DInput.h"
#include"../Graphic/Object/Object.h"
#include"../Player/YukiAnimation.h"

//=====�N���X�錾=====//
class CFiled
{
	//private�����o�ϐ�
private:
	LPDIRECT3DDEVICE9		m_pDevice9;
	//�I�u�W�F�N�g�̓ǂݍ���
	CWall*					m_pWall;

	D3DXVECTOR3 m_vWatergunPos;
	D3DXVECTOR3 m_vBallPos;
	D3DXVECTOR3 m_vBaburuPos;

	//��
	//YukiAnimation*		m_pool;

	float m_WaterRot;
	float m_WaterUpDownValue;
	//public�����o�ϐ�
public:

private:


	//public�����o�֐�
public:
	CFiled(LPDIRECT3DDEVICE9);
	~CFiled();

	BOOL CreateResource(LPDIRECT3DDEVICE9);
	void Update();
	void Draw();

	

};