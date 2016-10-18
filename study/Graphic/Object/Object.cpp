#include "Object.h"

CWall::CWall(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_Scaling = 1.0f;
}

CWall::~CWall()
{
	SAFE_DELETE_ARRAY(m_pMaterials);
	for (DWORD i = 0; i < m_NumMaterials; i++)
	{
		SAFE_RELEASE(m_pTextures[i]);
	}
	SAFE_DELETE_ARRAY(m_pTextures);

	for (DWORD i = 0; i < NumObject; i++)
	{
		SAFE_RELEASE(m_ObjectInfo[i].pMesh);
	}

	SAFE_DELETE_ARRAY(m_ObjectInfo);
	SAFE_RELEASE(m_pVertexDecl);
	SAFE_RELEASE(m_pEffect);
}

BOOL CWall::Initialize(FLOAT Scaling, char* pFileName)
{
	m_Scaling = Scaling;

	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	// �V�F�[�_�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// �f�o�C�X
		"Effect/XFile.fx",		// �t�@�C����
		NULL,			// �v���v���Z�b�T��`�ւ̃|�C���^
		NULL,			// �I�v�V�����̃C���^�[�t�F�C�X�|�C���^ 
		0,				// �R���p�C���I�v�V����
		NULL,			// �G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&m_pEffect,		// �G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^
		&pErrorMsgs);	// �G���[����ьx���̃��X�g���܂ރo�b�t�@
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	// ���_�̃t�H�[�}�b�g�錾
	D3DVERTEXELEMENT9 VertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	// ���_�錾�̐���
	m_pDevice->CreateVertexDeclaration(VertexElements, &m_pVertexDecl);

	char FullPathName[MAX_PATH];
	char FullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, FullPathName);
	strcat_s(FullPathName, "\\");
	strcat_s(FullPathName, pFileName);
	DWORD i;
	for (i = strlen(FullPathName); i > 0; i--)
	{
		if (FullPathName[i] == '\\')
		{
			FullPathName[i + 1] = '\0';
			break;
		}
	}
	if (i == 0)
		FullPathName[0] = '\0';
	memcpy(FullPath, FullPathName, strlen(FullPathName) + 1);

	m_ObjectInfo = new OBJECTINFO[NumObject];
	for (DWORD j = 0; j < NumObject; j++)
	{
		// �I�u�W�F�N�g�̍��W�ƃA���O���̏�����
		// ���ɃI�[��0.0f
		m_ObjectInfo[j].vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_ObjectInfo[j].vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		HRESULT hr;
		LPD3DXBUFFER pAdjacency = NULL;
		LPD3DXBUFFER pEffectInstances = NULL;
		LPD3DXBUFFER pMaterials = NULL;

		// X�t�@�C�����烁�b�V�������[�h
		hr = D3DXLoadMeshFromX(
			pFileName,			// �t�@�C�����ւ̃A�h���X
			D3DXMESH_MANAGED,	// ���b�V���̍쐬�I�v�V�������w��
			m_pDevice,		// �f�o�C�X
			&pAdjacency,		// �אږʏ��ւ̃A�h���X
			&pMaterials,		// �}�e���A�����ւ̃A�h���X
			&pEffectInstances,	// �G�t�F�N�g���ւ̃A�h���X
			&m_NumMaterials,	// �}�e���A�����ւ̃A�h���X
			&m_ObjectInfo[j].pMesh);			// ���b�V���ւ̃A�h���X
		if (hr != D3D_OK)
			return FALSE;

		// �}�e���A�����̎擾
		m_pMaterials = new D3DMATERIAL9[m_NumMaterials];
		m_pTextures = new LPDIRECT3DTEXTURE9[m_NumMaterials];

		// �}�e���A�������}�e���A���o�b�t�@����擾
		D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
		for (DWORD i = 0; i < m_NumMaterials; i++)
		{
			m_pMaterials[i] = pD3DXMaterial[i].MatD3D;
			FLOAT Length;
			D3DXVECTOR4 vLength;
			// ambient�ɒl���Ȃ��ꍇ�́AEmissive�ɒl���邩�m�F
			// �Ȃ��ꍇ�́Adiffuse�𗘗p����B50%�̒l�Ƃ���B
			memcpy(&vLength, &m_pMaterials[i].Ambient, sizeof(D3DXVECTOR4));
			vLength.w = 0.0f;
			Length = D3DXVec4Length(&vLength);
			if (!(Length > 0.0f))
			{
				memcpy(&vLength, &m_pMaterials[i].Emissive, sizeof(D3DXVECTOR4));
				vLength.w = 0.0f;
				Length = D3DXVec4Length(&vLength);
				if (Length > 0.0f)
					memcpy(&vLength, &m_pMaterials[i].Emissive, sizeof(D3DXVECTOR4));
				else
					memcpy(&vLength, &m_pMaterials[i].Diffuse, sizeof(D3DXVECTOR4));
				vLength *= 0.5f;
				memcpy(&m_pMaterials[i].Ambient, &vLength, sizeof(D3DXVECTOR4));
			}

			// specular�ɒl�����邩�m�F����B
			// �Ȃ��ꍇ�́A10%�̊D�F���w�肷��B
			memcpy(&vLength, &m_pMaterials[i].Specular, sizeof(D3DXVECTOR4));
			vLength.w = 0.0f;
			Length = D3DXVec4Length(&vLength);
			if (Length > 0.0f)
				m_pMaterials[i].Specular.a = m_pMaterials[i].Power;
			else
			{
				if (m_pMaterials[i].Power > 0.0f)
				{
					vLength = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 0.0f);
					memcpy(&m_pMaterials[i].Specular, &vLength, sizeof(D3DXVECTOR4));
					m_pMaterials[i].Specular.a = m_pMaterials[i].Power;
				}
			}
			if (m_pMaterials[i].Power < 1.0f)
				m_pMaterials[i].Specular.a = 5.0f;

			m_pTextures[i] = NULL;
			if (pD3DXMaterial[i].pTextureFilename)
			{
				strcpy_s(FullPathName, FullPath);
				strcat_s(FullPathName, pD3DXMaterial[i].pTextureFilename);
				D3DXCreateTextureFromFile(
					m_pDevice,		// �f�o�C�X���w��
					FullPathName,		// �t�@�C�������w��
					&m_pTextures[i]);	// �e�N�X�`���[�A�h���X���w��
			}
		}

		SAFE_RELEASE(pAdjacency);
		SAFE_RELEASE(pEffectInstances);
		SAFE_RELEASE(pMaterials);

		// �傫���̒���
		// ���_�̐����擾
		DWORD NumVertices = m_ObjectInfo[j].pMesh->GetNumVertices();
		// ���_�̃X���C�h���擾
		DWORD NumBytesPerVertex = m_ObjectInfo[j].pMesh->GetNumBytesPerVertex();

		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		FLOAT* pVertex;

		// ���_�o�b�t�@���̃f�[�^���擾
		m_ObjectInfo[j].pMesh->GetVertexBuffer(&pVB);
		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�������ւ̃|�C���^���擾
		pVB->Lock(
			0, // ���b�N���钸�_�f�[�^�ւ̃I�t�Z�b�g(�o�C�g�P��)(�S�����b�N)
			0, // ���b�N���钸�_�f�[�^�̃T�C�Y(�o�C�g�P��)(�S�����b�N)
			(void**)&pVertex,	// �Ԃ���钸�_�f�[�^���i�[����Ă��郁�����o�b�t�@�ւ�VOID*�|�C���^
			D3DLOCK_DISCARD);	// ���s���郍�b�N�̎��
		for (DWORD i = 0; i < NumVertices; i++)
		{
			D3DXVECTOR3* pPostion =
				(D3DXVECTOR3*)&pVertex[i * (NumBytesPerVertex / sizeof(FLOAT))];
			*pPostion *= m_Scaling;
		}
		pVB->Unlock();		// ���b�N����
		SAFE_RELEASE(pVB);	// ���_�o�b�t�@�̉��
	}

	return TRUE;
}

void CWall::Draw()
{
	D3DXMATRIX matView, matProjection;
	CCamera* pCamera = CCamera::GetInstance();
	//pCamera->SetCamera();
	matView = pCamera->getView();
	matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);
	D3DXMATRIX matWorld, matPosition, matRotation;

	// ���_�錾���w��
	m_pDevice->SetVertexDeclaration(m_pVertexDecl);
	if (m_pEffect)
	{
		// �e�N�j�b�N�̑I��
		m_pEffect->SetTechnique("ShaderTechnique");

		// �p�X���w��
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);	// �p�X�̔ԍ����w��

		// ���s�ړ��s����쐬
		D3DXMatrixTranslation(
			&matPosition,		// ���Z���ʂ̍s��ւ̃A�h���X
			m_vPosition.x,		// X���W�̃I�t�Z�b�g
			m_vPosition.y,		// X���W�̃I�t�Z�b�g
			m_vPosition.z);		// X���W�̃I�t�Z�b�g
								// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
			m_vAngle.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
			m_vAngle.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
			m_vAngle.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)
		matWorld = matRotation * matPosition;

		// ���[���h�s����w��
		m_pEffect->SetMatrix("matWorld", &matWorld);
		// �r���[�s����w��
		m_pEffect->SetMatrix("matView", &matView);
		// �v���W�F�N�V�����s����w��
		m_pEffect->SetMatrix("matProjection", &matProjection);

		// �J�����̈ʒu���w��
		m_pEffect->SetVector("ViewPosition", &vViewPosition);

		// ���C�g�������w��
		m_pEffect->SetVector("DirectionalLight", &vDirectionalLight);

		for (DWORD j = 0; j < NumObject; j++)
		{

			for (DWORD i = 0; i < m_NumMaterials; i++)
			{
				// �}�e���A�����w��
				D3DXVECTOR4 vColor;
				memcpy(&vColor, &m_pMaterials[i].Diffuse, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialDiffuse", &vColor);
				memcpy(&vColor, &m_pMaterials[i].Ambient, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialAmbient", &vColor);
				memcpy(&vColor, &m_pMaterials[i].Specular, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialSpecular", &vColor);

				// �e�N�X�`���[���w��
				if (m_pTextures[i])
					m_pEffect->SetTexture("Texture", m_pTextures[i]);
				// �e�N�X�`���̗L����ݒ�
				if (m_pTextures[i] == NULL)
					m_pEffect->SetBool("TextureNone", TRUE);
				else
					m_pEffect->SetBool("TextureNone", FALSE);

				// �V�F�[�_�ݒ���O���{�ɍX�V
				m_pEffect->CommitChanges();
				// �`��
				m_ObjectInfo[j].pMesh->DrawSubset(i);
			}
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}
}