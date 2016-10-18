#pragma once
#include"../Global/Global.h"
#include"../Graphic/ModelManager.h"
#include"../Lib/DirectX/DInput.h"

//Collider.h
#define PLAYER_HEIGHT 6

class Collider {

	//private�����o�ϐ�
private:

	static Collider* m_pColiderInstance;	//�C���X�^���X�ϐ�

	struct MESH_STATE
	{
		LPD3DXMESH mesh;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rotate;
	};
	MESH_STATE pMesh[MAXFILE_NUM];

	int num = 0;								//�o�^������
public:

private:


	//public�����o�֐�
public:
	Collider();	//�R���X�g���N�^
	~Collider();//�f�X�g���N�^
	static void Create();//�C���X�^���X�𐶐�����
	static void Destory();//�C���X�^���X���폜

						  //�C���X�^���X���擾����
	static Collider* GetInstance() {
		return m_pColiderInstance;
	}

	//�ȈՓI�ȓ����蔻��
	BOOL EasyCheck(D3DXVECTOR3 pos, D3DXVECTOR3 posCheck, float large);
	BOOL Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large);
	BOOL Distance(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, float large, float *get, D3DXVECTOR3 *r_pos);

	//�I�u�W�F�N�g��ǂݍ��ݒ��_���擾�i�[
	bool SetObj(D3DXVECTOR3 pos, LPD3DXMESH pmesh);

	//���݂̈ʒu��y���̓����蔻������߂�
	BOOL GroundCol(D3DXVECTOR3 pos, D3DXVECTOR3 rotatepos, int *Hitnum, float *numreturn);
	BOOL RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate);
	BOOL RayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, float * HitNum);
	BOOL DirectRayCast(D3DXVECTOR3 pos, D3DXVECTOR3 rotate, int objnum, float * HitNum);


};