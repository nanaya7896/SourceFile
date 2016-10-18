#pragma once
////=====Include File=====//
#include"../../Global/Global.h"
#include"CSkinMeshAnimation.h"
#include"../../Camera/Camera.h"
#include"../ModelManager.h"
//=====�N���X�錾=====//
class CDirectXAnimation
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;
	//�V�F�[�_
	LPD3DXEFFECT			m_pEffect;
	//�A�j���[�V�����g���b�N
	D3DXTRACK_DESC			m_TrackDesc;
	//�A�j���[�V����
	MY_HIERARCHY			m_cHierarchy;
	CSKIN_MESH				m_SkinMesh;
	//public�����o�ϐ�
public:
	//�|�W�V����
	D3DXVECTOR3 m_PlayerPos;
	//��]
	D3DXVECTOR3 m_PlayerRot;
	//�X�P�[��
	D3DXVECTOR3 m_PlayerScale;
	//�J�����̍��W
	D3DXVECTOR3 m_CameraPos;
	
	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CDirectXAnimation(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CDirectXAnimation();

	void LoadAnimXFile();
	void FreeAnim(LPD3DXFRAME);
	void Render();
	void AnimationChange(DWORD);

	//=====�Q�b�g�֐�=====//
	

	//=====�Z�b�g�֐�====//
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