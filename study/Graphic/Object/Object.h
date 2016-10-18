#pragma once

#include "../../Global/Global.h"
#include "../../Camera/Camera.h"

#define NumObject 10// ����20�ɂ��Ă��܂�

typedef struct _tagObjectInfo_
{
	D3DXVECTOR3		vPosition;
	D3DXVECTOR3		vAngle;
	LPD3DXMESH		pMesh;
}OBJECTINFO;

class CWall {
private:
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9   m_pDevice;
	DWORD				m_NumMaterials;	// �}�e���A���̐�
	D3DMATERIAL9*		m_pMaterials;	// �}�e���A��
	LPDIRECT3DTEXTURE9*	m_pTextures;	// �e�N�X�`��
	//LPD3DXMESH			m_pMesh;		// ���b�V��
	D3DXVECTOR3			m_vPosition;	// ���W
	D3DXVECTOR3			m_vAngle;		// �A���O��
	FLOAT				m_Scaling;		// �X�P�[�����O�l

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;	// ���_�錾
	LPD3DXEFFECT		m_pEffect;				// �G�t�F�N�g

	OBJECTINFO*			m_ObjectInfo;
public:
	CWall(LPDIRECT3DDEVICE9);
	~CWall();
	BOOL Initialize(FLOAT, char*);
	void Draw();
	OBJECTINFO* GetObjectInfo() { return m_ObjectInfo; }
};