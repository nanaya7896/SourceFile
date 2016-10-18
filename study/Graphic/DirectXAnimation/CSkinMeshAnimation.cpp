#include"CSkinMeshAnimation.h"


//�t���[�����쐬����
HRESULT MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	//
	HRESULT hr = S_OK;
	//
	MYFRAME *pFrame;
	//�V�����t���[���A�h���X�i�[�p�ϐ���������
	*ppNewFrame = NULL;
	//�t���[���̗̈���m��
	pFrame = new MYFRAME;
	//�̈�m�ێ��s���̏���
	if (pFrame == NULL)
	{
		//
		return E_OUTOFMEMORY;
	}

	//�t���[�����i�[�p�̈�m��
	pFrame->Name = new TCHAR[lstrlen(Name) + 1];
	//�̈�m�ێ��s���̏���
	if (!pFrame->Name)
	{
		return E_FAIL;
	}

	//strcpy(pFrame->Name,Name);
	//�t���[�����i�[
	strcpy_s(pFrame->Name,lstrlen(Name)+1,Name);
	//�s��̏�����
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	//�I�t�Z�b�g�֌W������
	pFrame->OffsetID = 0xFFFFFFFF;
	D3DXMatrixIdentity(&(pFrame->OffsetMatrix));
	//�V�K�t���[���̃��b�V���R���e�i������
	pFrame->pMeshContainer = NULL;
	//�V�K�t���[���̌Z��t���[���A�h���X�i�[�p�ϐ�������
	pFrame->pFrameSibling = NULL;
	//�V�K�t���[���̎q�t���[���A�h���X�i�[�p�ϐ�������
	pFrame->pFrameFirstChild = NULL;
	//�O���̐V�K�t���[���A�h���X�i�[�p�ϐ��ɁA�쐬�����t���[���̃A�h���X���i�[
	*ppNewFrame = pFrame;

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::CreateMeshContainer
//���b�V���R���e�i�[���쐬����
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	
	//���[�J�������p
	MYMESHCONTAINER *pMeshContainer = NULL;
	//���b�V���̖ʂ̐����i�[
	int iFacesAmount;
	//for���[�v�ŉ�
	int iMaterial;
	//�ꎞ�I�ȃf�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = NULL;
	//�ꎞ�I�ȃ��b�V���f�[�^�i�[�p
	LPD3DXMESH pMesh = NULL;
	//���b�V���R���e�i�i�[�p�ϐ�������
	*ppMeshContainer = NULL;
	//�{�[���̐��i�[�p�ϐ�������
	DWORD dwBoneNum = 0;
	//pMesh��"�O��������"���b�V���A�h���X���i�[
	pMesh = pMeshData->pMesh;
	//���b�V���R���e�i�̈�̓��I�m��
	pMeshContainer = new MYMESHCONTAINER;
	//�̈�m�ێ��s���̏���
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	//���b�V���R���e�i�̏�����
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	//���b�V���R���e�i�̖��O�i�[�p�̈�𓮓I�m��
	pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];
	//�̈�m�ێ��s���̏���
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	//strcpy(pMeshContainer->Name, Name);
	//�m�ۂ����̈�Ƀ��b�V���R���e�i�����i�[
	strcpy_s(pMeshContainer->Name, lstrlen(Name) + 1, Name);
	//�f�o�C�X�̎擾
	pMesh->GetDevice(&pDevice);
	//���b�V���ʂ̐����擾
	iFacesAmount = pMesh->GetNumFaces();

	pMeshContainer->MeshData.pMesh = pMesh;
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();//�ʏ탁�b�V���̏ꍇ�͂��ꂪ�K�v�B�X�L���̏ꍇ�A���������ƂȂ������������[�N�ɂȂ�B
	}

	//=====���b�V���̃}�e���A���̐ݒ�=====//
	//���b�V���̃}�e���A�������i�[(�ő�ň��)
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	//���b�V���R���e�i�̃}�e���A���f�[�^�i�[�̈�𓮓I�m��
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//���b�V���R���e�i�̃e�N�X�`���f�[�^�i�[�̈�𓮓I�m��
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	//���b�V���R���e�i�́A�ʂ��ƂɎ���3�̗אڏ�񂪊i�[���ꂽDWORD�^�̃A�h���X�i�[�p(�|�C���^)�ϐ�
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	//�̈�m�ێ��s���̏���
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	//�O�������̗אڐ��������b�V���R���e�i�Ɋi�[
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	//�e�N�X�`���f�[�^�i�[�p�̈��������(memset���g�p����0�Œ��g�𖄂߂�)
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	//�����̃}�e���A������0����Ȃ��ꍇ
	if (NumMaterials > 0)
	{
		//�O�������̃}�e���A���f�[�^�A�h���X�����b�V���R���e�i�Ɋi�[
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
		//�}�e���A���������[�v������
		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			//�e�N�X�`���̃t�@�C������NULL�łȂ����(�e�N�X�`���f�[�^�������)
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//�e�N�X�`���̃t�@�C���p�X�ۑ��p�ϐ�
				TCHAR strTexturePath[MAX_PATH];
				//�e�N�X�`���̃t�@�C���p�X��ۑ�(�ēǂݍ��ݎ��ɕK�v)
				strcpy_s(strTexturePath,lstrlen(pMeshContainer->pMaterials[iMaterial].pTextureFilename)+1,pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				//strcpy(strTexturePath,pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				//�e�N�X�`���̏��ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(
					pDevice,
					strTexturePath,
					&pMeshContainer->ppTextures[iMaterial]
					))) 
				{
					//���s���̏���
					//�e�N�X�`���t�@�C�����i�[�p
					CHAR TexMeshPass[255];
					//�����Ȃ���΁AGraph�t�H���_�𒲂ׂ�
					//���j�t�@�C�����̌������ɁA�K�������Ƀt�@�C���������鎖���m�F���Ă���
					//  strcpy_s��strcat_s���g���悤�ɂ���(���̏ꍇ�́A��ɂ��� 
					//    �e�N�X�`���̃t�@�C��������A����ɂ��̃t�@�C�����̒�����0�łȂ���� �̏���if��)�B
					//  TexMeshPass�ɁAX�t�@�C��������ꏊ�Ɠ����f�B���N�g���ƁA�e�N�X�`���̃t�@�C������
					//  �����������̂��i�[
					// strcpy_s( TexMeshPass, sizeof( TexMeshPass ) , "./../Source/Graph/" );
					strcpy_s(TexMeshPass, sizeof(TexMeshPass), "./Graph/");
					strcat_s(TexMeshPass, sizeof(TexMeshPass) - strlen(TexMeshPass) - strlen(strTexturePath) - 1, strTexturePath);
					//�e�N�X�`�����̓ǂݍ���
					if (FAILED(D3DXCreateTextureFromFile(
						pDevice,									//
						strTexturePath,								//
						&pMeshContainer->ppTextures[iMaterial])))	//
					{
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}
					//�e�N�X�`���̃t�@�C���p�X��NULL�ɂ���
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;

				}
			}
		}
	}
	//
	else
	{
		//=====�}�e���A���Ȃ��̏ꍇ======//
		//�e�N�X�`���t�@�C������NULL�ɂ���@
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		//�}�e���A���f�[�^�̏�����(memset���g�p���Ē��g���O�Ŗ��߂�)
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		//�}�e���A���J���[��0.5�ɐݒ�
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		//�X�y�L������0.5�ɐݒ�(��Őݒ肵���}�e���A���J���[��0.5��ݒ�)
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
	if (pSkinInfo != NULL)
	{
		//���b�V�������i�[
		pMeshContainer->pSkinInfo = pSkinInfo;
		//�Q�ƃJ�E���^
		pSkinInfo->AddRef();
		//�{�[���̐����擾
		dwBoneNum = pSkinInfo->GetNumBones();
		//�t���[���P�ʂł̃��[���h�s��i�[�p�̈�̓��I�m��
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];
		//�{�[���̐��������[�v
		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			//�p�t���[���i�{�[���j�̃I�t�Z�b�g�s����擾���Ċi�[
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}

		if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMesh,							//���̃��b�V���f�[�^�A�h���X
			NULL,							//�I�v�V����(���݂͎g���Ă��Ȃ�����NULL�ł���)
			pMeshContainer->pAdjacency,		//���̃��b�V���̗אڐ����
			NULL,							//�o�̓��b�V���̗אڐ����
			NULL,							//�e�ʂ̐V�����C���f�b�N�X�l�i�[�p�ϐ��A�h���X
			NULL,							//�e���_�̐V�����C���f�b�N�X�l�i�[�p�ϐ��̃A�h���X
			&pMeshContainer->dwWeight,		//�{�[���̉e���̈�ʓ�����̍ő吔�i�[�p�ϐ��̃A�h���X
			&pMeshContainer->dwBoneNum,		//�{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[�����i�[�p�ϐ��̃A�h���X
			&pMeshContainer->pBoneBuffer,	//�{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
			&pMeshContainer->MeshData.pMesh)//�o�͂���郁�b�V���A�h���X�i�[�p�ϐ��̃A�h���X(�Œ�p�C�v���C���p)
			))
		{
			return E_FAIL;
		}
	}
	//���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����
	*ppMeshContainer = pMeshContainer;
	//pMeshContainer = NULL;
	//SAFE_DELETE(pMeshContainer);
	//�Q�ƃJ�E���^�𑝂₵���̂Ō��炷
	SAFE_RELEASE(pDevice);
	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//�t���[����j������
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{

	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//���b�V���R���e�i�[��j������
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			//�e�N�X�`���̉��
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pOriMesh);
	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}
	
	SAFE_DELETE(pMeshContainer);
	pMeshContainerBase = NULL;
	//pMeshContainer = NULL;
	return S_OK;
}



//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
CSKIN_MESH::CSKIN_MESH()
{
	//�}�e���A���ύX�t���O
	m_MaterialFlag = FALSE;
	//�}�e���A���f�[�^
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	//�P�ʍs�񏉊���
	D3DXMatrixIdentity(&(this->m_World));
	//�A�j���[�V�������ԏ�����
	m_AnimTime = 0;
	//�A�j���[�V�����X�s�[�h������
	m_AnimSpeed=60.0f/4800.0f;
	//���݂̃A�j���[�V�����g���b�N������
	m_CurrentTrack = 0;
	//�A�j���[�V�����f�[�^�g���b�N������
	//�L���ɂ���
	m_CurrentTrackDesc.Enable = TRUE;
	//�e���x100%
	m_CurrentTrackDesc.Weight = 1;
	//�J�n�ʒu������
	m_CurrentTrackDesc.Position = 0;
	//���x
	m_CurrentTrackDesc.Speed = 1;
}

//====================================
//�֐���	:
//����		:
//�߂�l	:
//���e		:
//====================================
CSKIN_MESH::~CSKIN_MESH()
{
	Release();
}

HRESULT CSKIN_MESH::InitThing(LPDIRECT3DDEVICE9 pDevice, LPSTR pMeshPass)
{
	CHAR TmpMeshPass[255];
	strcpy_s(TmpMeshPass, sizeof(TmpMeshPass) - 1, pMeshPass);
	//X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(
			TmpMeshPass,
			D3DXMESH_MANAGED,
			pDevice, 
			&m_cHierarchy,
			NULL,
			&m_pFrameRoot,
			&m_pAnimController)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", TmpMeshPass, MB_OK);
		return E_FAIL;
	}
	//�{�[���s��̏�����
	AllocateAllBoneMatrices(m_pFrameRoot, m_pFrameRoot);
	//�A�j���[�V�����g���b�N�̎擾
	for (DWORD i = 0; i < m_pAnimController->GetNumAnimationSets(); i++)
	{
		//�A�j���[�V�����擾
		m_pAnimController->GetAnimationSet(i, &(m_pAnimSet[i]));
	}

	//�S�Ẵt���[���Q�ƕϐ��̐���
	m_FrameArray.clear();
	m_IntoMeshFrameArray.clear();
	CreateFrameArray(m_pFrameRoot);
	//�t���[���z��ɃI�t�Z�b�g���쐬
	for (DWORD i = 0; i < m_IntoMeshFrameArray.size(); i++)
	{
		MYMESHCONTAINER* pMyMeshContainer = (MYMESHCONTAINER*)m_IntoMeshFrameArray[i]->pMeshContainer;
		while (pMyMeshContainer)
		{
			//�X�L�����
			if (pMyMeshContainer->pSkinInfo)
			{
				DWORD cBones = pMyMeshContainer->pSkinInfo->GetNumBones();
				for (DWORD iBone = 0; iBone<cBones; iBone++) 
				{
					//�t���[�������瓯�����O�̃t���[��������
					for (DWORD Idx = 0; Idx<m_FrameArray.size(); Idx++)
					{
						if (strcmp(pMyMeshContainer->pSkinInfo->GetBoneName(iBone), m_FrameArray[Idx]->Name) == 0) 
						{
							pMyMeshContainer->BoneFrameArray.push_back(m_FrameArray[Idx]);
							//Offset�s��
							m_FrameArray[Idx]->OffsetMatrix = *(pMyMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
							m_FrameArray[Idx]->OffsetID = Idx;
							//pMyMeshContainer->BoneFrameArray.clear();
							break;
						}
					}
				}
			}
			//����
			pMyMeshContainer = (MYMESHCONTAINER *)pMyMeshContainer->pNextMeshContainer;
		}
		
	}
	
	return S_OK;
}

//
//HRESULT AllocateBoneMatrix( THING* pThing,LPD3DXMESHCONTAINER pMeshContainerBase )
//
HRESULT CSKIN_MESH::AllocateBoneMatrix(LPD3DXFRAME pFrameRoot,LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;
	//���b�V���R���e�i�^���I���W�i���̌^�Ƃ��Ĉ���
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	//�X�L�����b�V���ł����
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	//�{�[���̐����擾
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	//�e�{�[���̃��[���h�s��i�[�p�̈���m��
	SAFE_DELETE(pMeshContainer->ppBoneMatrix);
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];
	//�{�[���̐��������[�v
	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		//�q�t���[���̃A�h���X����������pFrame�Ɋi�[
		pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		//�q�t���[�����Ȃ����
		if (pFrame == NULL)
		{
			//�������I������
			return E_FAIL;
		}
		//�e�{�[���̃��[���h�s��i�[�p�ϐ��ɍŏI�s��i�[
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}


HRESULT CSKIN_MESH::AllocateAllBoneMatrices(LPD3DXFRAME pFrameRoot, LPD3DXFRAME pFrame)
{
	//�K�w�̑���(���������m�ۂ������b�V���R���e�i�̈��T������)
	if (pFrame->pMeshContainer != NULL)
	{
		//�{�[���s��̏���������
		if (FAILED(AllocateBoneMatrix(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	//�ċA���f����
	//�Z��t���[��
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	//�q�t���[��
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//VOID RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice,MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
//�t���[�����̂��ꂼ��̃��b�V���������_�����O����
VOID CSKIN_MESH::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
{

	DWORD i, k;
	DWORD dwBoneIndex, dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	//�X�L�����b�V���̏ꍇ
	if (pMeshContainer->pSkinInfo != NULL)
	{
		pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneBuffer->GetBufferPointer());
		//pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();
		//dwPrevBoneID��UINT_MAX�̒l(0xffffffff)���i�[
		dwPrevBoneID = UINT_MAX;
		//�X�L�j���O�v�Z
		for (i = 0; i < pMeshContainer->dwBoneNum; i++)
		{

			dwBlendMatrixNum = 0;
			//�e�����Ă���s�񐔎擾
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				//UINT_MAX(-1)
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					//���݉e�����󂯂Ă���{�[���̐�
					dwBlendMatrixNum = k;
				}
			}
			//�W�I���g���u�����f�B���O���g�p���邽�߂̍s��̌����w��
			pDevice->SetRenderState(
				D3DRS_VERTEXBLEND,
				dwBlendMatrixNum
				);
			//�e�����Ă���s��̌���
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				//iMatrixIndex��1�x�̌Ăяo���ŕ`��o����e�{�[�������ʂ���l���i�[
				//( ����BoneID�z��̒����̓��b�V���̎�ނɂ���ĈقȂ�
				//( �C���f�b�N�X�Ȃ��ł���΁@=�@���_���Ƃ̏d�� �ł���
				// �C���f�b�N�X����ł���΁@=�@�{�[���s��p���b�g�̃G���g����)
				//���݂̃{�[��(i�Ԗ�)����݂�k�Ԗڂ̃{�[��id
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				//�s��̏�񂪂����
				if (iMatrixIndex != UINT_MAX)
				{
					//mStack�ɃI�t�Z�b�g�s��*�{�[���s����i�[
					mStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					//�s��X�^�b�N�Ɋi�[
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			D3DMATERIAL9 TmpMat = pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D;
			//�A���r�G���g�̓����F��1.0�ɌŒ�
			TmpMat.Ambient.a = 1.0f;

			pDevice->SetMaterial(&TmpMat);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			//dwPrevBoneID�ɑ����e�[�u���̎��ʎq���i�[
			dwPrevBoneID = pBoneCombination[i].AttribId;
			//���b�V���̕`��
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
	//�ʏ탁�b�V���̏ꍇ
	else
	{
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
		for (i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}
//
//VOID DrawFrame(LPDIRECT3DDEVICE9 pDevice,LPD3DXFRAME pFrameBase)
//�t���[���������_�����O����B
VOID CSKIN_MESH::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		//SHADER_KIND a = GetpShader()->GetShaderKind();
		////�V�F�[�_���g�p���Ă���̂Ȃ��p�̕`��֐��ɔ�΂�
		//if( GetpShader() != NULL && GetpShader()->GetShaderKind() == SHADER_KIND_LAMBERT ){
		// ShaderDraw( pDevice, ControlNum, pMeshContainer, pFrame ); 
		//}else{}


		RenderMeshContainer(pDevice, pMeshContainer, pFrame);
		//���̃��b�V���R���e�i�ւ̃A�N�e�B�u���΂�
		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameFirstChild);
	}
}
VOID CSKIN_MESH::FreeAnim(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
		pFrame->pMeshContainer = NULL;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		FreeAnim(pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		FreeAnim(pFrame->pFrameFirstChild);
	}
}
VOID CSKIN_MESH::Release()
{
	if (m_pFrameRoot != NULL)
	{
		//�{�[���t���[���֌W���
		FreeAnim(m_pFrameRoot);
		//���̑��̏��(�e�N�X�`���̎Q�ƃf�[�^�Ȃ�)�̉��
		m_cHierarchy.DestroyFrame(m_pFrameRoot);
		m_pFrameRoot = NULL;
	}
	//�A�j���[�V�����R���g���[���[�̉��
	SAFE_RELEASE(m_pAnimController);
	//�S�Ẵt���[���Q�ƕϐ��̗v�f���폜
	m_FrameArray.clear();
	//���b�V���R���e�i����̃t���[���Q�ƕϐ��̗v�f���폜
	m_IntoMeshFrameArray.clear();


}

VOID CSKIN_MESH::CreateFrameArray(LPD3DXFRAME _pFrame)
{
	if (_pFrame == NULL)
	{
		return;
	}
	//�t���[���A�h���X�i�[
	MYFRAME* pMyFrame = (MYFRAME*)_pFrame;
	m_FrameArray.push_back(pMyFrame);
	//���b�V���R���e�i������ꍇ��IntoMeshFrameArray�ɃA�h���X�i�[
	if (pMyFrame->pMeshContainer != NULL)
	{
		m_IntoMeshFrameArray.push_back(pMyFrame);
	}
	//�q�t���[��������
	if (pMyFrame->pFrameFirstChild != NULL)
	{
		CreateFrameArray(pMyFrame->pFrameFirstChild);
	}
	//�Z��t���[������
	if (pMyFrame->pFrameSibling != NULL)
	{
		CreateFrameArray(pMyFrame->pFrameSibling);
	}
}

MYFRAME* CSKIN_MESH::SearchBoneFrame(LPSTR _BoneName, D3DXFRAME * _pFrame)
{
	MYFRAME* pFrame = (MYFRAME*)_pFrame;
	if (strcmp(pFrame->Name, _BoneName) == 0) {
		return pFrame;
	}
	if (_pFrame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameSibling);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	if (_pFrame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameFirstChild);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	return NULL;
}

VOID CSKIN_MESH::Update(D3DXMATRIX _World)
{
	//�}�g���b�N�X�s�񔽉f
	m_World = _World;
	//�A�j���[�V�������Ԃ��X�V
	m_AnimTime++;	
}
VOID CSKIN_MESH::Draw(LPDIRECT3DDEVICE9 pDevice)
{
	//���݂̃A�j���[�V�����ԍ���K��
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimSet[m_CurrentTrack]);
	//0(�Đ�����)�g���b�N����g���b�N�f�X�N���Z�b�g����
	m_pAnimController->SetTrackDesc(0, &(m_CurrentTrackDesc));
	//�A�j���[�V�������ԃf�[�^�̍X�V
	m_pAnimController->AdvanceTime(m_AnimSpeed, NULL);
	//�A�j���[�V�����f�[�^���X�V
	UpdateFrameMatrices(m_pFrameRoot, &m_World);
	//�A�j���[�V�����`��
	DrawFrame(pDevice, m_pFrameRoot);
	//0(�Đ�����)�g���b�N����X�V�����g���b�N�f�X�N���擾����
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
}
VOID CSKIN_MESH::ChangeAnim(DWORD NewAnimNum)
{
	//�V�K�A�j���[�V�����ɕύX
	m_CurrentTrack = NewAnimNum;
	//�A�j���[�V�����^�C����������
	m_AnimTime = 0;
	//�A�j���[�V�������ŏ��̈ʒu����Đ�������
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
	m_CurrentTrackDesc.Position = 0;
	m_pAnimController->SetTrackDesc(0, &m_CurrentTrackDesc);
}
D3DXMATRIX CSKIN_MESH::GetBoneMatrix(LPSTR _BoneName)
{
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//�{�[�����������
	if (pFrame != NULL) {
		//�{�[���s���Ԃ�
		return pFrame->CombinedTransformationMatrix;
	}
	//�{�[����������Ȃ����
	else {
		//�P�ʍs���Ԃ�
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}
D3DXMATRIX *CSKIN_MESH::GetPointBoneMatrix(LPSTR _BoneName)
{
		/////////////////////////////////////
	//���ӁjRokDeBone�p�ɐݒ�(�Ώۃ{�[���̈��̍s����Ƃ��Ă���)
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//�{�[�����������
	if (pFrame != NULL) {
		return &pFrame->CombinedTransformationMatrix;
	}
	//�{�[����������Ȃ����
	else {
		//NULL��Ԃ�
		return NULL;
	}
}
//
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
VOID CSKIN_MESH::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		//CombinedTransformationMatrix�ɍŏI�s����i�[
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		//CombinedTransformationMatrix�ɍŏI�s����i�[
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


