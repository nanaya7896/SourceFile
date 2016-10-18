#pragma once
// -------------------- IncludeFile -----------------
#include "../../Global/Global.h"				// ���ʃw�b�_�[
// ------------------- �N���X�錾 -------------------
class CMeshContainer;
// ------------------------------------------------------------------------
//	�N���X	:CAllocateHierarchy		�t���[���ƃ��b�V���R���e�i�̓ǂݍ���
//	ID3DXAllocateHierarchy����̔h���N���X
// ------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy {
private:
	STDMETHOD(CreateFrame)(	THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(	THIS_ LPCSTR Name,
									CONST D3DXMESHDATA* pMeshData,
									CONST D3DXMATERIAL* pMaterials,
									CONST D3DXEFFECTINSTANCE* pEffectInstances,
									DWORD NumMaterials, 
									CONST DWORD* pAdjacency,
									LPD3DXSKININFO pSkinInfo, 
									LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// �`��f�o�C�X
	TCHAR					m_FullPath[MAX_PATH];	// XFile���̃t���p�X
	LPD3DXEFFECT			m_pEffect;				// �G�t�F�N�g
public:
	CAllocateHierarchy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXEFFECT pEffect);
	void setFullPath(LPCSTR pFullPath) { memcpy(m_FullPath,pFullPath,strlen(pFullPath) + 1); }
};
// ------------------------------------------------------------------------
//	�N���X	:CFrame					�g�����X�t�H�[���t���[���̃J�v�Z����
//	D3DXFRAME����̔h���N���X
// ------------------------------------------------------------------------
class CFrame : public D3DXFRAME {
private:
	CMeshContainer*			MeshContainer;			// ���b�V���R���e�i
	CFrame*					FrameSibling;			// �Z��t���[��
	CFrame*					FrameChild;				// �q�t���[��
	D3DXMATRIXA16			CombinedTransMatrix;	// �����ςݕό`�s��
public:
	// �R���X�g���N�^�C�f�X�g���N�^
	CFrame(LPCTSTR name);
	~CFrame();
	// �s��̎擾
	LPD3DXMATRIXA16 GetCombinedTransMatrix() {
		return &CombinedTransMatrix;
	}
	// �t���[���̌���
	CFrame* Find(LPCTSTR name);
	// �{�[���ό`�s��̏�����
	void InitBoneTransMatrices(CFrame* root);
	// �ό`�s��̍X�V
	void UpdateMatrices(LPD3DXMATRIX pParentMatrix);
	// �`��
	void Draw();
};
// ------------------------------------------------------------------------
//	�N���X	:CMeshContainer			���b�V���R���e�i�i�[�p
//	D3DXMESHCONTAINER����̔h���N���X
// ------------------------------------------------------------------------
class CMeshContainer : public D3DXMESHCONTAINER {
private:
	LPD3DXMESH				pMesh;					// ���b�V��
	LPD3DXMESH				pOrigMesh;				// ���b�V��(�I���W�i��)
	DWORD					NumMaterials;			// �}�e���A���̐�
	LPD3DXMATERIAL			pMaterials;				// �}�e���A��
	LPDIRECT3DTEXTURE9*		ppTextures;				// �e�N�X�`��
	LPD3DXSKININFO			SkinInfo;				// �X�L�����
	DWORD					NumBones;				// �{�[���̐�
	LPD3DXMATRIX			pBoneOffsetMatrices;	// �I�t�Z�b�g�s��
	LPD3DXMATRIX*			ppBoneMatrixPtrs;		// �g�����X�t�H�[���s��
    DWORD					NumPaletteEntries;		// �{�[���s��p���b�g�̐�(Indexed�p)
	DWORD					NumMaxInfls;			// �e�����󂯂�{�[����1�ʂ�����̍ő吔
	DWORD					NumBoneCombinations;	// �{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[���̐�
	LPD3DXBUFFER			pBoneCombinationTable;	// �{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// �`��f�o�C�X
	D3DCAPS9				m_d3dCaps;				// DX��
	LPD3DXEFFECT			m_pEffect;				// �G�t�F�N�g
	DWORD					m_WorkingPaletteSize;	// �������ʊi�[�p�s��̍ő吔
	D3DXMATRIX*				m_amxWorkingPalette;	// �������ʊi�[�p�s��
public:
	// �R���X�g���N�^
	CMeshContainer(
		THIS_ LPCTSTR Name,
		CONST D3DXMESHDATA* meshdata,
		CONST D3DXMATERIAL* pMaterials,
		CONST DWORD numMaterials,
		CONST DWORD* adjacency,
		LPD3DXSKININFO skin_info,
		LPDIRECT3DDEVICE9,
		LPCSTR,
		LPD3DXEFFECT
	);
	// �f�X�g���N�^
	~CMeshContainer();
	// ���̃��b�V���R���e�i���擾
	CMeshContainer* Next() {
		return (CMeshContainer*)pNextMeshContainer;
	}
	// �{�[���ό`�s��̏�����
	void InitBoneTransMatrices(CFrame* root);
	// �`��
	void Draw(CFrame* frame);


};
// ------------------------------------------------------------------------
//	�N���X	:CAnimMesh				�A�j���[�V�����Ή����b�V��
// ------------------------------------------------------------------------
class CAnimMesh {
private:
	CAllocateHierarchy*		AllocateHierarchy;		// �t�@�C���ǂݍ��ݗp�̃I�u�W�F�N�g
	CFrame*					FrameRoot;				// ���[�g�̃t���[��
	LPD3DXANIMATIONCONTROLLER AnimController;		// �A�j���[�V�����R���g���[��
	DWORD					CurrentAnimSetID;		// ���s���̃A�j���[�V�����ԍ�
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// �`��f�o�C�X
	DWORD					m_dwTimer;				// �Q�[���X�s�[�h


	/*�r���ǉ�*/
	FLOAT m_AnimFinishTime=0.0f;
public:
	// �R���X�g���N�^�C�f�X�g���N�^
	CAnimMesh(LPDIRECT3DDEVICE9, DWORD, LPD3DXEFFECT);
	~CAnimMesh();
	// �t�@�C���̓ǂݍ���
	BOOL LoadFromFile(LPCSTR);
	// �A�j���[�V����
	DOUBLE GetTime();
	void SetTime(DOUBLE Time);
	void SetAnimSet(DWORD Track,DWORD AnimationSet);
	void ChangeAnimation(DWORD AnimSetID);

	/*�r���ǉ�*/
	void SetAnimFinishTime(FLOAT time)
	{
		m_AnimFinishTime = time;
	};
	FLOAT GetAnimFinishTime()
	{
		return m_AnimFinishTime;
	}
	void ResetTime();
	/*�����܂�*/
	// �t���[���̎擾
	CFrame* GetFrameRoot() {return FrameRoot;}
	// �`��
	void Draw();

	LPD3DXANIMATIONCONTROLLER GetAnimController();
};