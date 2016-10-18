#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Camera/Camera.h"
#include"../Math/MyMathLib.h"
//=====�N���X�錾=====//
class CXfile
{
	//private�����o�ϐ�
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9	m_pDevice9;
	//���b�V��
	LPD3DXMESH			m_pMesh;
	// ����
	D3DMATERIAL9*		m_pMaterials;
	// ����
	D3DMATERIAL9*		m_pOriginalMaterials;
	// �����̐�
	DWORD				m_NumMaterials;
	//�e�N�X�`��
	LPDIRECT3DTEXTURE9*	m_pTextures;
	//���S
	D3DXVECTOR3			m_vCenter;
	//���a
	FLOAT				m_Radius;
	//�V�F�[�_
	LPD3DXEFFECT		m_pEffect;

	// �X���C�_�[�̃e�N�X�`��UV
	D3DXVECTOR2 TextureUV;
	//D3DXVECTOR2 TextureUVpool;
	BOOL slider;
	BOOL pool;
	CMathLib*			m_Math;


	D3DXMATRIX	matRotCop;
	D3DXMATRIX  matWorldCopy;
	//���������w��
	D3DXVECTOR4 vDirectionalLight;
	//public�����o�ϐ�
public:
	int value_Light;
	//private�����o�֐�
private:

	//public�����o�֐�
public:
	//�R���X�g���N�^
	CXfile(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CXfile();

	//Xfile�̓ǂݍ���
	BOOL LoatXfile(const TCHAR*);
	
	//�`��p
	void Draw(D3DXVECTOR3, D3DXVECTOR3,D3DXVECTOR3, float);
	//�`��p
	void Draw(D3DXVECTOR3*, D3DXVECTOR3*, D3DXVECTOR3*, D3DXCOLOR, int);

	LPD3DXMESH CXfile::GetMesh()
	{
		return m_pMesh;
	}
	void SetSlider(BOOL Value) { slider = Value; }
	void Setpool(BOOL Value) { pool = Value; }


	//UV�l�̃Q�b�^�[
	D3DXVECTOR2 GetSliderUV()
	{
		return TextureUV;
	}
	D3DXMATRIX GetMatRot()
	{
		return matRotCop;
	}
	D3DXMATRIX GetMatWorld()
	{
		return matWorldCopy;
	}
};