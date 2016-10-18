#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Graphic/DSystem.h"

//======define�l======//
#define TEXTURE_NUM		27
//=====�N���X�錾=====//
class CTextureManager
{
	//private�����o�ϐ�
private:
	//�V���O���g��
	static CTextureManager* m_pTextureInstance;
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9		m_pDevice;
	//�e�N�X�`��
	CDSystem*				m_pDSystem[TEXTURE_NUM];

	TCHAR TextureFileName[TEXTURE_NUM][MAX_PATH] =
	{
		"CG/zero.png",				//0
		"CG/one.png",				//1
		"CG/two.png",				//2
		"CG/three.png",				//3
		"CG/four.png",				//4
		"CG/five.png",				//5
		"CG/six.png",				//6
		"CG/seven.png",				//7
		"CG/eight.png",				//8
		"CG/nine.png",				//9
		"CG/title.png",				//10
		"CG/sousa.png",				//11
		"CG/Main_Cap.PNG",			//12
		"CG/gameover.png",			//13
		"CG/clear.png",				//14
		"CG/credit2.png",			//15
		"CG/enjoy.png",				//16
		"CG/t_rogo.png",			//17
		"CG/result.jpg"	,			//18
		"CG/count1.png",			//19
		"CG/count2.png",			//20
		"CG/o_yajirusi.png"	,		//21
		"CG/Fade.png",				//22
		"CG/enjoy_z.png",			//23
		"CG/aka.png",				//24
		"CG/glow.jpg",				//25
		"CG/ranking.png"			//26
									//"CG/emmi.png"
									//"CG/yellow.png"				//25

	};
	//public�����o�ϐ�
public:

	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextureManager(LPDIRECT3DDEVICE9);
	~CTextureManager();

	void CreateResource();
	BOOL AllLoadTexture();

	static void Create(LPDIRECT3DDEVICE9);
	static void Destroy();
	
	void Delete();
	void Draw(DWORD num,D3DXVECTOR3 vPos, UINT tNum, FLOAT fAlpha, D3DXVECTOR3 posSize);

	//======�Q�b�g�֐�=====//
	static CTextureManager* GetInstance()
	{
		return m_pTextureInstance;
	}
	CHAR* GetTextureName(DWORD);

};