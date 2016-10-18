#pragma once
//=====Include File=====//
#include"../../Global/Global.h"

//=====�N���X�錾=====//
//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	//�I�t�Z�b�g�s��
	D3DXMATRIX OffsetMatrix;
	//�s��e�[�u���̃C���f�b�N�X�ԍ�
	DWORD	   OffsetID;
};
//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;								//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;							//�{�[���̐�
	LPD3DXBUFFER pBoneBuffer;					//�{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;					//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;			//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
	LPD3DXMESH	pOriMesh;						//�I���W�i�����b�V���p
	DWORD		NumPaletteEntries;				//�p���b�g�T�C�Y

	//Work�p
	//std::vector<D3DXMATRIX>		m_WorkBoneMatArray;
	//�e������t���[���ւ̎Q�Ɣz��B�`�掞�ɂ��̃t���[���̍s����g��
	std::vector<MYFRAME*>		BoneFrameArray;

	MYMESHCONTAINER()
	{
		ppBoneMatrix = NULL;
		pBoneOffsetMatrices = NULL;
	}
};


//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	//�R�s�[�R���X�g���N�^
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//�X�L�����b�V���N���X
class CSKIN_MESH
{
private:
	//���ׂẴt���[���|�C���^�i�[�����֐�
	VOID CreateFrameArray(LPD3DXFRAME _pFrame);
	//�Ώۂ̃{�[��������
	MYFRAME* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);

	//�t���[���Q�Ɣz��
	//�S�t���[���Q�Ɣz��
	std::vector<MYFRAME*>		m_FrameArray;
	//���b�V���R���e�i����̃t���[���Q�Ɣz��
	std::vector<MYFRAME*>		m_IntoMeshFrameArray;
	//�{�[�����
	LPD3DXFRAME					m_pFrameRoot;
	//�A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	//�q�G�����L�[�N���X�ϐ�
	MY_HIERARCHY				m_cHierarchy;
	//�A�j���[�V�����f�[�^�i�[�p�ϐ�
	LPD3DXANIMATIONSET			m_pAnimSet[20];
	//���݂̃A�j���[�V�������J�n����Ă���̎���(1/60)
	DWORD						m_AnimTime;
	//�A�j���[�V�����X�s�[�h
	FLOAT						m_AnimSpeed;
	//���݂̃A�j���[�V�����g���b�N
	DWORD						m_CurrentTrack;
	//���݂̃A�j���[�V�����f�[�^�g���b�N
	D3DXTRACK_DESC				m_CurrentTrackDesc;
	//���[���h���W
	D3DXMATRIX					m_World;
	//���b�V���̃}�e���A���֌W
	//�}�e���A���ύX�t���O
	BOOL						m_MaterialFlag;
	//�}�e���A���f�[�^
	D3DMATERIAL9				m_Material;

public:
	//�R���X�g���N�^
	CSKIN_MESH();
	//�f�X�g���N�^
	~CSKIN_MESH();

	//�X�L�����b�V����������
	HRESULT InitThing(LPDIRECT3DDEVICE9, LPSTR);
	HRESULT AllocateBoneMatrix(LPD3DXFRAME, LPD3DXMESHCONTAINER);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME,LPD3DXFRAME);
	VOID RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER*, MYFRAME*);
	VOID UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);
	VOID DrawFrame(LPDIRECT3DDEVICE9, LPD3DXFRAME);
	//�t���[�����
	VOID FreeAnim(LPD3DXFRAME);
	//�������
	VOID Release();
	//�X�V����
	VOID Update(D3DXMATRIX);
	//�`�揈��
	VOID Draw(LPDIRECT3DDEVICE9);
	//�I�u�W�F�N�g�̃A�j���[�V�����ύX�i���b�V���I�u�W�F�N�g�̑���p�ԍ��A�ύX����A�j���[�V�����ԍ��j
	VOID ChangeAnim(DWORD NewAnimnum);


	//======�Z�b�g�֐�======//
	//�A�j���[�V�����̑��x��ݒ�
	VOID SetAnimSpeed(FLOAT _AnimSpeed)
	{
		m_AnimSpeed = _AnimSpeed;
	}


	//======�Q�b�g�֐�=====//
	DWORD GetAnimTrack()
	{
		return m_AnimTime;
	}
	FLOAT GetAnimSpeed()
	{
		return m_AnimSpeed;
	}
	//�{�[���̃}�g���b�N�X�擾�i�{�[���̖��O�j
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);
	//�{�[���̃}�g���b�N�X�|�C���^���擾(�{�[���̖��O)
	D3DXMATRIX* GetPointBoneMatrix(LPSTR _BoneName);
};


