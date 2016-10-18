// -------------------- IncludeFile -----------------
#include "AllocateHierarchy.h"
// ------------------------------------------------------------------------
//	�N���X	:CAllocateHierarchy		�t���[���ƃ��b�V���R���e�i�̐����Ɣj��
//	ID3DXAllocateHierarchy����̔h���N���X
// ------------------------------------------------------------------------
// �t���[���̐������ɌĂяo�����֐�
HRESULT CAllocateHierarchy::CreateFrame(
	THIS_ LPCTSTR Name,LPD3DXFRAME* ppNewFrame)
{
	*ppNewFrame = new CFrame(Name);
	return S_OK;
}
// ���b�V���R���e�i�̐������ɌĂяo�����֐�
HRESULT CAllocateHierarchy::CreateMeshContainer(
	THIS_ LPCTSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* /*pEffectInstances*/,
	DWORD NumMaterials, 
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	*ppNewMeshContainer = new CMeshContainer(
		Name,pMeshData,pMaterials,NumMaterials,
		pAdjacency,pSkinInfo,m_pd3dDevice,m_FullPath, m_pEffect);
	return S_OK;
}
// �t���[���̔j�����ɌĂяo�����֐�
HRESULT CAllocateHierarchy::DestroyFrame(
	THIS_ LPD3DXFRAME pFrameToFree)
{
	delete (CFrame*)pFrameToFree;
	return S_OK;
}
// ���b�V���R���e�i�̔j�����ɌĂяo�����֐�
HRESULT CAllocateHierarchy::DestroyMeshContainer(
	THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	delete (CMeshContainer*)pMeshContainerToFree;
	return S_OK;
}
// �R���X�g���N�^
CAllocateHierarchy::CAllocateHierarchy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXEFFECT pEffect)
{
	m_pd3dDevice = pd3dDevice;
	m_pEffect = pEffect;
}
// ------------------------------------------------------------------------
//	�N���X	:CFrame					�t���[�����ƃ��b�V���R���e�i�i�[�p
//	D3DXFRAME����̔h���N���X
// ------------------------------------------------------------------------
// �t���[���̃R���X�g���N�^
CFrame::CFrame(LPCTSTR name)
{
	// ��{�N���X�̏�����
	ZeroMemory(this,sizeof(CFrame));
	// �t���[�����̐ݒ�
	if(name) {
		UINT n = lstrlen(name) + 1;
		Name = new TCHAR[n];
		memcpy(Name,name,n * sizeof(TCHAR));
	}
	else Name = NULL;
	// �s��̒P�ʉ�
	D3DXMatrixIdentity(&TransformationMatrix);
	D3DXMatrixIdentity(&CombinedTransMatrix);
}
// �t���[���̃f�X�g���N�^
CFrame::~CFrame()
{
	delete [] Name;
}
// �t���[���̕`��
void CFrame::Draw()
{
	// �t���[���ɑ�����S�Ẵ��b�V���R���e�i��`��
	for(CMeshContainer* mc = MeshContainer;mc;mc = mc->Next())
		MeshContainer->Draw(this);
	// �Z��t���[���A�q�t���[���̕`��
	if(FrameSibling) FrameSibling->Draw();
	if(FrameChild) FrameChild->Draw();
}
// �t���[���̌���
CFrame* CFrame::Find(LPCTSTR name)
{
	// ���[�g�t���[���̎q�t���[���̌���
	return (CFrame*)D3DXFrameFind(	this		// ���[�g�t���[���ւ̃|�C���^
									,name);		// ��������q�t���[���̖��O
}
// �{�[���ό`�s��̏������i�t���[���j
void CFrame::InitBoneTransMatrices(CFrame* root)
{
	// ���b�V���R���e�i�A�Z��t���[���A�q�t���[���ւ̃|�C���^�̏�����
	MeshContainer = (CMeshContainer*)pMeshContainer;
	FrameSibling = (CFrame*)pFrameSibling;
	FrameChild = (CFrame*)pFrameFirstChild;
	// ���b�V���R���e�i�A�Z��t���[���A�q�t���[���Ɋւ���{�[���ό`�s��̏�����
	if(MeshContainer) MeshContainer->InitBoneTransMatrices(root);
	if(FrameSibling) FrameSibling->InitBoneTransMatrices(root);
	if(FrameChild) FrameChild->InitBoneTransMatrices(root);
}
// �ό`�s��̍X�V
void CFrame::UpdateMatrices(LPD3DXMATRIX pParentMatrix)
{
	// ���̃t���[���Ɋւ���ό`�s��̍X�V
	if(pParentMatrix) {
		D3DXMatrixMultiply(
			&CombinedTransMatrix, 
			&TransformationMatrix,
			pParentMatrix);
	}
	else
		CombinedTransMatrix = TransformationMatrix;

	// �Z��t���[���A�q�t���[���Ɋւ���ό`�s��̍X�V
	if(FrameSibling) FrameSibling->UpdateMatrices(pParentMatrix);
	if(FrameChild) FrameChild->UpdateMatrices(&CombinedTransMatrix);
}
// ------------------------------------------------------------------------
//	�N���X	:CMeshContainer			���b�V���R���e�i�i�[�p
//	D3DXMESHCONTAINER����̔h���N���X
// ------------------------------------------------------------------------
// ���b�V���R���e�i�̃R���X�g���N�^
CMeshContainer::CMeshContainer(
		THIS_ LPCTSTR name,
		CONST D3DXMESHDATA* meshdata,
		CONST D3DXMATERIAL* materials,
		CONST DWORD numMaterials,
		CONST DWORD* adjacency,
		LPD3DXSKININFO skin_info,
		LPDIRECT3DDEVICE9 pd3dDevice,
		LPCSTR pFullPath,
		LPD3DXEFFECT pEffect)
:	pOrigMesh(meshdata->pMesh),NumMaterials(numMaterials),
	pMaterials(NULL), ppTextures(NULL),
	SkinInfo(skin_info), NumBones(0), pBoneOffsetMatrices(0)
	, m_amxWorkingPalette(NULL), m_WorkingPaletteSize(0)
{
	// ��{�N���X�̏�����
	ZeroMemory(this,sizeof(D3DXMESHCONTAINER));
	m_pd3dDevice = pd3dDevice;
	m_pEffect = pEffect;
	m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);			// DX���擾

	pBoneOffsetMatrices = NULL;			// �I�t�Z�b�g�s��
	ppBoneMatrixPtrs = NULL;			// �g�����X�t�H�[���s��
	pBoneCombinationTable = NULL;		// �{�[���̑g�ݍ��킹�e�[�u��

	// ���b�V���R���e�i���̐ݒ�
	if(name) {
		UINT n = lstrlen(name) + 1;
		Name = new TCHAR[n];
		memcpy(Name,name,n * sizeof(TCHAR));
	}
	else Name = NULL;

	// ���b�V���ɖ@���x�N�g�������݂��邩�m�F
	if(!(pOrigMesh->GetFVF() & D3DFVF_NORMAL)) {
		// (FVF) �R�[�h���g���ă��b�V���̃R�s�[
		pOrigMesh->CloneMeshFVF(pOrigMesh->GetOptions(),			// �쐬�I�v�V�������w��(�����b�V���̃I�v�V�������w��)
							pOrigMesh->GetFVF() | D3DFVF_NORMAL,	// FVF�R�[�h�̑g�ݍ��킹���w��(�����b�V����FVF�ɖ@���x�N�g����ǉ�)
							m_pd3dDevice,							// �f�o�C�X���w��
							&pOrigMesh);							// �R�s�[���ꂽ���b�V�����w��
		// ���b�V���Ɋ܂܂��e���_�̖@�����v�Z
		D3DXComputeNormals(	pOrigMesh,	// ���b�V���ւ̃|�C���^���w��
							NULL);		// �אږʂ��w��(�ݒ�Ȃ�)
	}
	else pOrigMesh->AddRef();	// �Q�ƃJ�E���g�A�b�v

	// �I�u�W�F�N�g�̐������A�}�e���A���ƃe�N�X�`���[�\���̃������m��
	pMaterials = new D3DXMATERIAL[NumMaterials];
	ppTextures = new LPDIRECT3DTEXTURE9[NumMaterials];
	for(DWORD i = 0;i < NumMaterials;i++) {	
		// �}�e���A�����̃R�s�[
		memcpy(&pMaterials[i],&materials[i],sizeof(D3DXMATERIAL));
		pMaterials[i].MatD3D = materials[i].MatD3D;
		FLOAT Length;
		D3DXVECTOR4 vLength;
		// ambient�ɒl���Ȃ��ꍇ�́AEmissive�ɒl���邩�m�F
		// �Ȃ��ꍇ�́Adiffuse�𗘗p����B50%�̒l�Ƃ���B
		memcpy(&vLength, &pMaterials[i].MatD3D.Ambient, sizeof(D3DXVECTOR4));
		vLength.w = 0.0f;
		Length = D3DXVec4Length(&vLength);
		if (!(Length > 0.0f))
		{
			memcpy(&vLength, &pMaterials[i].MatD3D.Emissive, sizeof(D3DXVECTOR4));
			vLength.w = 0.0f;
			Length = D3DXVec4Length(&vLength);
			if (Length > 0.0f)
				memcpy(&vLength, &pMaterials[i].MatD3D.Emissive, sizeof(D3DXVECTOR4));
			else
				memcpy(&vLength, &pMaterials[i].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			vLength *= 0.5f;
			memcpy(&pMaterials[i].MatD3D.Ambient, &vLength, sizeof(D3DXVECTOR4));
		}

		// specular�ɒl�����邩�m�F����B
		// �Ȃ��ꍇ�́A10%�̊D�F���w�肷��B
		memcpy(&vLength, &pMaterials[i].MatD3D.Specular, sizeof(D3DXVECTOR4));
		vLength.w = 0.0f;
		Length = D3DXVec4Length(&vLength);
		if (Length > 0.0f)
			pMaterials[i].MatD3D.Specular.a = pMaterials[i].MatD3D.Power;
		else
		{
			if (pMaterials[i].MatD3D.Power > 0.0f)
			{
				vLength = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 0.0f);
				memcpy(&pMaterials[i].MatD3D.Specular, &vLength, sizeof(D3DXVECTOR4));
				pMaterials[i].MatD3D.Specular.a = pMaterials[i].MatD3D.Power;
			}
		}
		if (pMaterials[i].MatD3D.Power < 1.0f)
			pMaterials[i].MatD3D.Specular.a = 5.0f;

		// �t�H���_�[�m�F
		if(materials[i].pTextureFilename == NULL) {
			ppTextures[i] = NULL;
			continue;
		}

		if(materials[i].pTextureFilename) {
			TCHAR FullPathName[MAX_PATH];
			if(strlen(pFullPath))
				strncpy_s(FullPathName,pFullPath,strlen(pFullPath) + 1);
			strcat_s(FullPathName,materials[i].pTextureFilename);
			// �t�@�C������ɂ��ăe�N�X�`�����쐬
			if(FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,			// �f�o�C�X���w��
												FullPathName,			// �t�@�C�������w��
												&ppTextures[i])))		// IDirect3DTexture9�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X���w��
				ppTextures[i] = NULL;
		}
		else
			ppTextures[i] = NULL;
	}

	// �X�L�j���O����ǂݍ���
	if(SkinInfo) {
		SkinInfo->AddRef();	// �Q�ƃJ�E���g�A�b�v
		// �{�[���ό`�s��A�{�[���I�t�Z�b�g�s��̍쐬
		NumBones = SkinInfo->GetNumBones();
		ppBoneMatrixPtrs = new D3DXMATRIX * [NumBones];
		pBoneOffsetMatrices = new D3DXMATRIX[NumBones];
		for(DWORD i = 0;i < NumBones;i++)
			pBoneOffsetMatrices[i] = *SkinInfo->GetBoneOffsetMatrix(i);

		//----- 0420�ǉ� -----//
		UINT PaletteSize = 0;
		m_pEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&PaletteSize);
		NumPaletteEntries = min(PaletteSize, NumBones);

		// �X�L���Ή��̃��b�V�����쐬����
		SkinInfo->ConvertToIndexedBlendedMesh(
			pOrigMesh,					// ���̓��b�V��
			0,							// ���݂͎g���Ă��Ȃ�
			NumPaletteEntries,			// �s��p���b�g�X�L�j���O�Ɏg�p�\�ȃ{�[���s��̐�
			adjacency,					// ���̓��b�V���̗אڐ����
			NULL,						// �o�̓��b�V���̗אڐ����
			NULL,						// �e�ʂ̐V�C���f�b�N�X���i�[���Ă���]����o�b�t�@�ւ̃|�C���^
			NULL,						// �e���_�̐V�C���f�b�N�X���i�[����AID3DXBuffer�C���^�[�t�F�C�X�ւ̃|�C���^
			&NumMaxInfls,				// �e�����󂯂�{�[����1�ʂ�����̍ő吔���i�[����DWORD�ւ̃|�C���^
			&NumBoneCombinations,		// �{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[���̐��ւ̃|�C���^
			&pBoneCombinationTable,		// �{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
			&pMesh);					// �V�������b�V���ւ̃|�C���^

		// �E�G�C�g�v�Z�p�s����쐬
		m_WorkingPaletteSize = 0;
		if (m_WorkingPaletteSize < NumPaletteEntries)
		{
			SAFE_DELETE_ARRAY(m_amxWorkingPalette);
			m_WorkingPaletteSize = NumPaletteEntries;
			m_amxWorkingPalette = new D3DXMATRIX[m_WorkingPaletteSize];
		}
		// D3DCOLORtoUBYTE4�ϊ�
		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9 * pDeclCur;
		pMesh->GetDeclaration(pDecl);
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}
		pMesh->UpdateSemantics(pDecl);
	}
	else {
		// (FVF)�R�[�h���g���ă��b�V���̃R�s�[
		pOrigMesh->CloneMeshFVF(pOrigMesh->GetOptions(),// �쐬�I�v�V�������w��(�����b�V���̃I�v�V�������w��)
							pOrigMesh->GetFVF(),		// FVF�R�[�h�̑g�ݍ��킹���w��(�����b�V����FVF�ɖ@���x�N�g����ǉ�)
							m_pd3dDevice,				// �f�o�C�X���w��
							&pMesh);					// �R�s�[���ꂽ���b�V�����w��
	}
	if(pOrigMesh) pOrigMesh->Release();
}
// ���b�V���R���e�i�̃f�X�g���N�^
CMeshContainer::~CMeshContainer()
{
	if(Name) delete [] Name;
	if(pMesh) pMesh->Release();
	if(pMaterials) delete [] pMaterials;
	if(ppTextures) {
		for(DWORD i = 0;i < NumMaterials;i++) {
			if(ppTextures[i]) ppTextures[i]->Release();
		}
		delete [] ppTextures;
	}
	if(SkinInfo) SkinInfo->Release();
	if(ppBoneMatrixPtrs) delete [] ppBoneMatrixPtrs;
	if(pBoneOffsetMatrices) delete [] pBoneOffsetMatrices;
	if(pBoneCombinationTable) pBoneCombinationTable->Release();
	if (m_amxWorkingPalette) delete[] m_amxWorkingPalette;
}
// ���b�V���R���e�i�̕`��
void CMeshContainer::Draw(CFrame* frame)
{
	// �X�L�j���O���s���ꍇ�̕`��
	if(SkinInfo) {
		UINT iAttrib;
		LPD3DXBONECOMBINATION pBoneComb;
		UINT iMatrixIndex;

		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pBoneCombinationTable->GetBufferPointer());
		for(iAttrib = 0;iAttrib < NumBoneCombinations;iAttrib++) {
			// �{�[���s��p���b�g�̐ݒ�
			for(DWORD i = 0;i < NumPaletteEntries;++i) {
				iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
				if(iMatrixIndex != UINT_MAX) {
					D3DXMatrixMultiply(&m_amxWorkingPalette[i],
						&pBoneOffsetMatrices[iMatrixIndex],
						ppBoneMatrixPtrs[iMatrixIndex]);
				}
			}

			// �{�[���̐�
			m_pEffect->SetInt("NumBones", NumMaxInfls - 1);
			// �{�[���p���b�g���w��
			m_pEffect->SetMatrixArray("amPalette", m_amxWorkingPalette, NumPaletteEntries);

			// �}�e���A�����w��
			D3DXVECTOR4 vColor;
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialDiffuse", &vColor);
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialAmbient", &vColor);
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Specular, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialSpecular", &vColor);

			// �e�N�X�`���[���w��
			if (ppTextures[pBoneComb[iAttrib].AttribId])
				m_pEffect->SetTexture("Texture", ppTextures[pBoneComb[iAttrib].AttribId]);
			// �e�N�X�`���̗L����ݒ�
			if (ppTextures[pBoneComb[iAttrib].AttribId] == NULL)
				m_pEffect->SetBool("TextureNone", TRUE);
			else
				m_pEffect->SetBool("TextureNone", FALSE);

			m_pEffect->BeginPass(0);	// �p�X�̔ԍ����w��

			// �V�F�[�_�ݒ���O���{�ɍX�V
			m_pEffect->CommitChanges();
			// �|���S���̕`��
			pMesh->DrawSubset(iAttrib);

			m_pEffect->EndPass();
		}
	}
	else {	// �X�L�j���O���s��Ȃ��ꍇ�̕`��

		// �s��̐ݒ�
		D3DXMATRIX* pCombinedTrans = frame->GetCombinedTransMatrix();

		// �}�e���A���A�e�N�X�`������v����|���S���Ń��[�v
		for (DWORD i = 0; i < NumMaterials; i++) {

			// 
			m_pEffect->SetMatrix("matCombinedTrans", pCombinedTrans);

			// �}�e���A�����w��
			D3DXVECTOR4 vColor;
			memcpy(&vColor, &pMaterials[i].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialDiffuse", &vColor);
			memcpy(&vColor, &pMaterials[i].MatD3D.Ambient, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialAmbient", &vColor);
			memcpy(&vColor, &pMaterials[i].MatD3D.Specular, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialSpecular", &vColor);

			// �e�N�X�`���[���w��
			if (ppTextures[i]!=NULL)
				m_pEffect->SetTexture("Texture", ppTextures[i]);
			// �e�N�X�`���̗L����ݒ�
			if (ppTextures[i] == NULL)
				m_pEffect->SetBool("TextureNone", TRUE);
			else
				m_pEffect->SetBool("TextureNone", FALSE);

			m_pEffect->BeginPass(1);	// �p�X�̔ԍ����w��

			// �V�F�[�_�ݒ���O���{�ɍX�V
			m_pEffect->CommitChanges();

			pMesh->DrawSubset(i);

			m_pEffect->EndPass();
		}

	}
}
// �{�[���ό`�s��̏������i���b�V���R���e�i�j
void CMeshContainer::InitBoneTransMatrices(CFrame* root)
{
	if(SkinInfo) {
		// �S�Ẵ{�[���ɂ��āA�Ή�����t���[����T��
		for(DWORD i = 0;i < NumBones;i++) {
			// �{�[���C���f�b�N�X����{�[���̖��O���擾
			CFrame* pFrame = root->Find(SkinInfo->GetBoneName(i));
			// �Ή�����t���[���̍����ςݕό`�s����{�[������Q�Ƃ���
			if(pFrame)
				ppBoneMatrixPtrs[i] = pFrame->GetCombinedTransMatrix();
		}
	}
}
// ------------------------------------------------------------------------
//	�N���X	:CAnimMesh			�A�j���[�V�����Ή����b�V��
// ------------------------------------------------------------------------
// �R���X�g���N�^
CAnimMesh::CAnimMesh(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwTimer, LPD3DXEFFECT pEffect)
{
	m_pd3dDevice = pd3dDevice;
	m_dwTimer = dwTimer;
	AllocateHierarchy = new CAllocateHierarchy(pd3dDevice, pEffect);
	AnimController = NULL;
	FrameRoot = NULL;
}
// �f�X�g���N�^
CAnimMesh::~CAnimMesh()
{
	// �A�j���[�V�����R���g���[���̊J��
	if(AnimController)
		AnimController->Release();
	if(FrameRoot) {
		// �t���[���̊J��
		D3DXFrameDestroy(FrameRoot,AllocateHierarchy);
		FrameRoot = NULL;
	}
	delete AllocateHierarchy;
}
// �t�@�C���̃��[�h
BOOL CAnimMesh::LoadFromFile(LPCSTR pLoadFile)
{
	// �t���p�X���w��
	TCHAR FullPathName[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,FullPathName);
	strcat_s(FullPathName,"\\");
	strcat_s(FullPathName,pLoadFile);
	DWORD i;
	for(i = (DWORD)strlen(FullPathName);i > 0;i--) {
		if(FullPathName[i] == '\\'){
			FullPathName[i + 1] = '\0';
			break;
		}
	}
	if(i == 0)
		FullPathName[0] = '\0';
	strncpy_s(FullPath,FullPathName,strlen(FullPathName) + 1);
	AllocateHierarchy->setFullPath(FullPath);
	// ���A�j���[�V�����R���g���[��
	LPD3DXANIMATIONCONTROLLER pAnimController;
	// ���b�V���̃��[�h
	LPD3DXFRAME frame_root;
	// �ŏ��̃t���[���K�w��.x�t�@�C�����烍�[�h
	if(FAILED(D3DXLoadMeshHierarchyFromX(
		pLoadFile,			// �t�@�C�������w�肷�镶����ւ̃|�C���^
		D3DXMESH_MANAGED,	// ���b�V���̍쐬�I�v�V�������w��(���_/�C���f�b�N�X �o�b�t�@���V�X�e���������Ǘ����Ɏw��)
		m_pd3dDevice,		// �f�o�C�X���w��
		AllocateHierarchy,	// ID3DXAllocateHierarchy�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,				// ���[�U�[�f�[�^�̃��[�h��������C���^�[�t�F�C�X
		&frame_root,		// ���[�h�����t���[�����[�g�ւ̃|�C���^���w��
		&pAnimController))) return FALSE;	// �A�j���[�V�����R���g���[���ւ̃|�C���^���w��
	// �A�j���[�V�����Z�b�g�����擾
	DWORD NumAnimSet = pAnimController->GetNumAnimationSets();
	// �~�L�T�̃g���b�N�̍ő吔���擾
	DWORD MaxNumTracks = pAnimController->GetMaxNumTracks();
	if(MaxNumTracks < NumAnimSet) MaxNumTracks = NumAnimSet;
	// �A�j���[�V�����R���g���[���̃R�s�[���쐬
	if(FAILED(pAnimController->CloneAnimationController(
		pAnimController->GetMaxNumAnimationOutputs(),	// �T�|�[�g�\�ȃA�j���[�V�����o�͂̍ő吔���w��
		pAnimController->GetMaxNumAnimationSets(),	// �T�|�[�g�\�ȃA�j���[�V�����Z�b�g�̍ő吔���w��
		MaxNumTracks,								// �T�|�[�g�\�ȃg���b�N�̍ő吔���w��
		pAnimController->GetMaxNumEvents(),			// �T�|�[�g�\�ȃC�x���g�̍ő吔���w��
		&AnimController))) return FALSE;			// �R�s�[���ꂽ�A�j���[�V�����R���g���[���ւ̃|�C���^���w��
	LPD3DXANIMATIONSET pAnimSet = NULL;
	// �A�j���[�V�����Z�b�g���擾
	AnimController->GetAnimationSet(0				// �A�j���[�V�����Z�b�g�̃C���f�b�N�X���w��
									,&pAnimSet);	// �Ԃ��ꂽ�A�j���[�V�����Z�b�g�ւ̃|�C���^���w��
	// X�t�@�C���̃t���[������AnimTicksPerSecond(�P�b�Ԃ̕����W��)�Ŋ������l
	double PeriodTime = pAnimSet->GetPeriod();
	SetAnimFinishTime((float)PeriodTime);
	float LoopTime = 1.0f;	// 1�b���[�v
	// �g���b�N���x��ݒ�
	AnimController->SetTrackSpeed(0,(float)PeriodTime / LoopTime);

	// �A�j���[�V�����Z�b�g�J��
	if(pAnimSet)
		pAnimSet->Release();
	// ���A�j���[�V�����R���g���[���̊J��
	if(pAnimController)
		pAnimController->Release();
	// �A�j���[�V�����Z�b�g����������ꍇ
	if(NumAnimSet > 1) {
		// �g���b�N�ɑS�ẴA�j���[�V�����Z�b�g��ǂݍ���
		for(DWORD i = 1; i < NumAnimSet;i++) {
			D3DXTRACK_DESC Desc = {D3DXPRIORITY_HIGH,1.0f,1.0f,0,FALSE};
			// {���g�p,�d�݂̒l,���x�̒l,���[�J���A�j���[�V�����^�C��,�g���b�N(�~�L�T���p)�̗L��/����}
			// �g���b�N�̋L�q��ݒ�
			AnimController->SetTrackDesc(	i,		// �ύX����g���b�N
											&Desc);	// �g���b�N�̋L�q
			LPD3DXANIMATIONSET pAnimSet = NULL;
			// �A�j���[�V�����Z�b�g���擾
			AnimController->GetAnimationSet(i				// �A�j���[�V�����Z�b�g�̃C���f�b�N�X���w��
											,&pAnimSet);	// �Ԃ��ꂽ�A�j���[�V�����Z�b�g�ւ̃|�C���^���w��
			PeriodTime = pAnimSet->GetPeriod();
			
			// �g���b�N���x��ݒ�
			AnimController->SetTrackSpeed(i,(float)PeriodTime / LoopTime);
			// �w�肵���g���b�N�ɃA�j���[�V���� �Z�b�g
			AnimController->SetTrackAnimationSet(	i,			// �A�j���[�V�����Z�b�g��ǉ����Ă���g���b�N���w��
													pAnimSet);	// �g���b�N�ɒǉ�����A�j���[�V�����Z�b�g���w��
			// �A�j���[�V�����Z�b�g�J��
			if(pAnimSet)
				pAnimSet->Release();
		}
	}
	CurrentAnimSetID = 0;				// ���s�A�A�j���[�V�����ԍ�
	FrameRoot = (CFrame*)frame_root;	// �t���[�����[�g��ݒ�
	// �{�[���ό`�s��̐ݒ�
	FrameRoot->InitBoneTransMatrices(FrameRoot);

	return TRUE;
}
// �A�j���[�V�����ύX
void CAnimMesh::ChangeAnimation(DWORD AnimSetID) {
	// �ύX����A�j���[�V�����ԍ����A�j���[�V�����Z�b�g���𒴂��Ȃ�
	if(AnimSetID >= AnimController->GetNumAnimationSets())
		return;
	// ���A���s����Ă���A�j���[�V�������m�F
	if(CurrentAnimSetID == AnimSetID)
		return;

	AnimController->SetTrackPosition(CurrentAnimSetID,0.0f);
	AnimController->SetTrackEnable(CurrentAnimSetID,FALSE);
	CurrentAnimSetID = AnimSetID;
	AnimController->SetTrackPosition(CurrentAnimSetID,0.0f);
	AnimController->SetTrackEnable(CurrentAnimSetID,TRUE);
	AnimController->ResetTime();
}
// ���Ԃ̎擾
double CAnimMesh::GetTime()
{
	if (AnimController)
	{
		return AnimController->GetTime();
	}
	return 0.0;
}

// ���Ԃ̐ݒ�
void CAnimMesh::SetTime(DOUBLE Time)
{
	if(!AnimController)
		return;
	AnimController->AdvanceTime(Time,NULL);
}
void CAnimMesh::ResetTime()
{
	AnimController->ResetTime();
}
// �ϊ��s����w�肵�ĕ`��
void CAnimMesh::Draw()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	FrameRoot->UpdateMatrices(&matWorld);
	FrameRoot->Draw();
}


LPD3DXANIMATIONCONTROLLER CAnimMesh::GetAnimController() {

	LPD3DXANIMATIONCONTROLLER tmp;

	//�A�j���[�V�����R���g���[���[�̃w���p�[�֐��@�f�B�[�v�R�s�[���s��
	if (S_OK == AnimController->CloneAnimationController(AnimController->GetMaxNumAnimationOutputs(),
		AnimController->GetMaxNumAnimationSets(),
		AnimController->GetMaxNumTracks(),
		AnimController->GetMaxNumEvents(),
		&tmp))
		return tmp;

	return nullptr;

}