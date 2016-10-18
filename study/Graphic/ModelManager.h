#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/Xfile.h"
//======defien�l=====//
#define	MAXFILE_NUM		14
//=====�N���X�錾=====//
class CModelManager
{
	//private�����o�ϐ�
private:
	//�V���O���g��
	static CModelManager*	m_pModelInstance;
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice9;	
	//XFIle
	CXfile*				m_pXFile[MAXFILE_NUM];

	TCHAR XFileName[MAXFILE_NUM][MAX_PATH] =
	{
		//MainField�`��p
		"Model/yuka.X",				//0
		"Model/pool.X",				//1	
		"Model/suraida.X",			//2
		"Model/suraida_kaidan.X",	//3
		"Model/sora.X",				//4
		"Model/baburu02.X",			//5
		"Model/yasi.X",			//6
		"Model/suraida_water.X",	//7
		"Model/bool.X",				//8
		"Model/baburu.X",			//9
		"Model/watergan.X",			//10
		"Model/mizu.X",				//11
		"Model/hasi.X",				//12
		"Model/zonbi_hokou.X"		//13
									//"",
									//"",
									//"",
									//"",
									//""
	};
	//public�����o�ϐ�
public:
	D3DXVECTOR3       rotation;
	D3DXMATRIX		  matRot;
	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CModelManager(LPDIRECT3DDEVICE9 pDevice9);
	~CModelManager();

	static void Create(LPDIRECT3DDEVICE9 pDevice9);
	static void Destory();

	//�f���[�g�p
	void Delete();
	//�ꊇ�ǂݍ���
	BOOL AllLoadXfile();
	
	//=====�Q�b�g�֐�======//
	CXfile* GetXFileData(DWORD num)
	{
		return m_pXFile[num];
	}

	//Get�֐�
	static CModelManager* GetInstance()
	{
		return m_pModelInstance;
	}
	//XFile�̖��O�̎擾
	CHAR* GetXFileName(DWORD);

	

	//�`��
	void Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3, float alpha);
	//void Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXCOLOR color, int t_Num);



	LPD3DXMESH GetMesh(DWORD num);

	D3DXVECTOR2 GetUV(DWORD num)
	{
		return m_pXFile[num]->GetSliderUV();
	};
	D3DXMATRIX GetMatRot(DWORD num)
	{
		return m_pXFile[num]->GetMatRot();
	}
	D3DXMATRIX GetMatWorld(DWORD num)
	{
		return m_pXFile[num]->GetMatWorld();
	}
};