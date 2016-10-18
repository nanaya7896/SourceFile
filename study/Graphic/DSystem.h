#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/Sound/SoundManager.h"

#define SOUNDMAX	14
//=====�\���̐錾=====//
struct AnimationNum
{
	UINT numU;
	UINT numV;
};
//=====�N���X�錾=====//
class CDSystem
{
	//private�����o�ϐ�
private:
	//�|���QD�p
	struct Vertex {
		float x, y, z;// 3�������W
		float rhw;	// 2D�ϊ��ς݃t���O
		float u, v;	// UV���W
	};

	//
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	//�e�N�X�`��
	LPDIRECT3DTEXTURE9	m_pTexture;
	//�`��f�o�C�X
	LPDIRECT3DDEVICE9	m_pDevice3D;
	//�V�F�[�_
	LPD3DXEFFECT		m_pEffect;
	//���_�錾�쐬
	LPDIRECT3DVERTEXDECLARATION9 decl;

	//�T�E���h�֘A
	CSoundManager*		m_pSoundManager;

	// �ǂݍ��ރE�F�[�u�t�@�C���̐�
	DWORD				m_dwLoadIWaveFileNum;

	char SoundFileName[SOUNDMAX][MAX_PATH] =
	{
		//BGM
		"Music/BGM/title.wav",		//0 �^�C�g��BGM
		"Music/BGM/game.wav",		//1 �Q�[��BGM
		"Music/BGM/gameover.wav",	//2 
		"Music/BGM/result.wav",		//3 
		//SE
		"Music/SE/y_kettei.wav",	//4 
		"Music/SE/y_sentaku.wav",	//5 
		"Music/SE/agaru.wav",		//6 ������オ��
		"Music/SE/baburu.wav",		//7 �V���{���ʔ��ˎ�
		"Music/SE/bomb.wav",		//8 �V���{���ʔj��
		"Music/SE/count.wav",		//9 �J�E���g�_�E���p
		"Music/SE/nageru.wav",		//10 �{�[��������u��
		"Music/SE/oyogu.wav",		//11 �j����
		"Music/SE/slider.wav",		//12 �X���C�_�[���蒆
		"Music/SE/suberioti.wav"	//13 �X���C�_�[������
	};
	//public�����o�ϐ�
public:
	//�|���֘A
	typedef struct _BoardVertex_
	{
		//���_�̍��W
		D3DXVECTOR4 vPosition;
		//�f�B�t���[�Y
		D3DXVECTOR4 vColor;
		//�e�N�X�`��UV�l
		FLOAT		tu, tv;
	}BOARDVERTEX;

	int width;
	int height;

	float w;
	float h;

	D3DXVECTOR2 pos;
	//UV�̕�����
	UINT divU;
	UINT divV;
	//UV�̔ԍ�
	UINT numU,numV;

	//private�����o�֐�
private:


	
	//public�����o�֐�
public:
	//�R���X�g���N�^
	CDSystem(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CDSystem();
	BOOL CreateBorad();
	BOOL CreateResource();
	BOOL CreateSoundManager(HWND);
	BOOL LoadTexture(const TCHAR*);
	//BOOL PointLoadTexture(const TCHAR*);
	void Draw(D3DXVECTOR3,UINT,FLOAT,D3DXVECTOR3);
	void Draw(bool);
	void Draw();
	//=====�Z�b�g�֐�=====//
	void SetDivede(UINT DivU, UINT DivV)
	{
		if (DivU <= 0 || DivV <= 0)
		{
			return;
		}
		divU = DivU;
		divV = DivV;
	}

	void SetUVNum(UINT NumU,UINT NumV)
	{
		if (NumU >= divU)
			return;
		if (NumV >= divV)
			return;

		numU = NumU;
		numV = NumV;
	}
	void SetWidth(int Width, int Height)
	{
		width = Width;
		height = Height;
	}

	void Setpos(D3DXVECTOR2 vPos)
	{
		pos = vPos;
	}
	//=====�Q�b�g�֐�=====//
	LPDIRECT3DTEXTURE9 getTexture() 
	{
		return m_pTexture;
	};

	CSoundManager* GetSoundManager() 
	{ 
		return m_pSoundManager; 
	}

	LPD3DXEFFECT GetEffect()
	{
		return m_pEffect; 
	}
};