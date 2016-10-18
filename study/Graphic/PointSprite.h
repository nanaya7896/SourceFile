#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"DSystem.h"
#include"../Camera/Camera.h"
//=====�\����=====//
struct Vertex3 
{
	D3DXVECTOR3 pos;	//�ʒu	
	float       size;	//�T�C�Y
	DWORD		color;	//�F
};

//=====�N���X�錾=====//
class CPointSprite
{
	//private�����o�ϐ�
private:
	LPDIRECT3DVERTEXBUFFER9 m_pPointBuffer;
	LPDIRECT3DTEXTURE9		m_pTexture;
	D3DXMATRIX				m_World;
	LPDIRECT3DDEVICE9       m_pDevice9;
	//�V�F�[�_
	LPD3DXEFFECT		m_pEffect;


	//public�����o�ϐ�
public:
	float ColorChange;
	float Value;
	float Speed;
	D3DXVECTOR3 m_Pos[9];
	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CPointSprite(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CPointSprite();
	//
	BOOL PointLoadTexture(const TCHAR *FileName);
	//������
	BOOL InitPointSprite(LPDIRECT3DDEVICE9);
	//�|�C���g�X�v���C�g�`��
	void DrawPointSprite(Vertex3[],int, bool);
	void Update();

	//=====�Z�b�g�֐�====//
	void SetD3DTexture(LPDIRECT3DTEXTURE9 pTexture)
	{
		m_pTexture = pTexture;
	}

	void SetPosition(D3DXVECTOR3 pos,int num)
	{
		m_Pos[num] = pos;
	}

	void SetSpeed(float speed)
	{
		Speed = speed;
	}

};