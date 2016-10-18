#ifndef ___Class_Camera___
#define ___Class_Camera___

#pragma once
//=====Include File=====//
#include"..\Global\Global.h"

//=====�N���X�錾=====//
class CCamera
{
	//private�����o�ϐ�
private:
	//�V���O���g��
	static CCamera*			m_pInstance;
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;
	//�J�����̊�ʒu
	D3DXVECTOR3				m_vViewPosition;
	//�{��(�J�������猴�_�܂ł̋���)
	FLOAT					m_fDistance;
	//�J�����̃A���O��
	D3DXVECTOR3				m_vViewAngle;
	D3DXMATRIX				m_View;
	D3DXMATRIX				m_Proj;
	D3DXMATRIX				m_Screen;
	//������
	D3DXVECTOR3				m_vDirectionalLight;
	//�������̏ꏊ
	D3DXVECTOR3				m_vLightPosition;
	D3DXVECTOR3				m_vEyePt2;
	//�����_
	D3DXVECTOR3				m_vLookatPt;
	
	D3DXVECTOR3 vViewBasePosition;	// �J�����̊�ʒu
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CCamera(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CCamera();

	static void Create(LPDIRECT3DDEVICE9, HWND);
	static void Destroy();

	void SetCamera(D3DXVECTOR3 pos);

	//=======================
	//
	//�Z�b�g�֐�
	//
	//=======================s
	void SetViewAngle(D3DXVECTOR3 Value)
	{
		m_vViewAngle = Value;
	}
	void SetDistance(FLOAT dis)
	{
		m_fDistance = dis;
	}
	//=======================
	//
	//�Q�b�g�֐�
	//
	//=======================

	//�J�����̃C���X�^���X�擾
	static CCamera* GetInstance()
	{
		return m_pInstance;
	}
	//View�s��̎擾
	D3DXMATRIX getView() 
	{
		return m_View; 
	}
	//
	D3DXMATRIX getProj()
	{
		return m_Proj;
	}
	//�X�N���[���s��̎擾
	D3DXMATRIX getScreen() 
	{
		return m_Screen;
	}
	//�J�����̈ʒu�̎擾
	D3DXVECTOR3 GetViewPosition() 
	{
		return m_vViewPosition; 
	}
	//�������̎擾
	D3DXVECTOR3 GetDirectionalLight()
	{
		return m_vDirectionalLight;
	}
	//�������̏ꏊ���擾
	D3DXVECTOR3 GetLightPosition() 
	{
		return m_vLightPosition; 
	}
	//�����_�̎擾
	D3DXVECTOR3 GetLookatPt() 
	{
		return m_vLookatPt;
	}
	//
	D3DXVECTOR3 GetViewAngle()
	{
		return m_vViewAngle;
	}
	D3DXVECTOR3 GetViewPos()
	{
		return vViewBasePosition;
	}

};

#endif