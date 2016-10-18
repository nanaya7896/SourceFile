#include "Xfile.h"

//====================================
//�֐���	:CXfile
//����		:
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CXfile::CXfile(LPDIRECT3DDEVICE9 pDevice9)
{
	m_pDevice9 = pDevice9;
	m_pMesh = NULL;
	m_pMaterials = NULL;
	m_pTextures = NULL;
	m_NumMaterials = 0;
	value_Light = 1;
	m_pEffect = NULL;
	vDirectionalLight = D3DXVECTOR4(0, 0, 0, 0);
	m_pOriginalMaterials = NULL;
	TextureUV = D3DXVECTOR2(0.0f, 0.0f);
	slider = FALSE;

	m_Math = new CMathLib();
}

//====================================
//�֐���	:~CXfile
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CXfile::~CXfile()
{
	//�e�N�X�`���̉��
	for (DWORD i = 0; i < m_NumMaterials; i++)
	{
		SAFE_RELEASE(m_pTextures[i]);
	}
	//�}�e���A���̔j��
	SAFE_DELETE_ARRAY(m_pMaterials);
	SAFE_DELETE_ARRAY(m_pOriginalMaterials);
	//�e�N�X�`���̔j��
	SAFE_DELETE_ARRAY(m_pTextures);
	//���b�V���̉��
	SAFE_RELEASE(m_pMesh);
	//�G�t�F�N�g�̉��
	SAFE_RELEASE(m_pEffect);

	SAFE_DELETE(m_Math);
}

//====================================
//�֐���	:LoatXfile
//����		:filename: �t�@�C���̖��O
//�߂�l	:TRUE:�ǂݍ��ݐ��� FALSE:���s
//���e		:Xfile�̓ǂݍ���
//====================================
BOOL CXfile::LoatXfile(const TCHAR* pfilename)
{
	HRESULT hr=E_FAIL;
	//�אږˏ��
	LPD3DXBUFFER pAdjacency;
	//�}�e���A���f�[�^
	LPD3DXBUFFER pMaterials;
	//�G�t�F�N�g�C���X�^���X
	LPD3DXBUFFER pEffectInstance;

	//�t�@�C���̏ꏊ���t���p�X�Ō�������
	char FullPathName[MAX_PATH];
	char FullPath[MAX_PATH];
	//���݂̃v���Z�X�̃J�����g�f�B���N�g�����擾
	//1:�J�����g�f�B���N�g�����擾���邽�߂̃o�b�t�@�̒������w��(NULL�������܂�) 
	//2:�o�b�t�@�ւ̃|�C���^���w��(��΃p�X�����i�[�����)
	GetCurrentDirectory(MAX_PATH, FullPathName);
	//�������ǉ�����
	//1:�ǉ���̕�����
	//2:���̕�����
	strcat_s(FullPathName, "\\");
	strcat_s(FullPathName, pfilename);

	DWORD i;
	//FullPathName�̕�����̒������J��Ԃ�
	for (i = strlen(FullPathName); i > 0; i--)
	{
		//'\\'���������ꍇ
		if (FullPathName[i] == '\\')
		{
			//char�^��NULL�����Ɋi�[
			FullPathName[i + 1] = '\0';
			break;
		}
	}
	//�����񂪂O�ɂȂ����ꍇ
	if (i == 0)
	{
		//char�^NULL�𖖔��ɒǉ�
		FullPathName[0] = '\0';
	}
	//FullPath��FullPathName�̃��������R�s�[
	memcpy(FullPath, FullPathName, strlen(FullPathName) + 1);

	hr = D3DXLoadMeshFromX(
		pfilename,			//�t�@�C�������w��
		D3DXMESH_MANAGED,	//�I�v�V�������w��
		m_pDevice9,			//�f�o�C�X���w��
		&pAdjacency,		//�אږˏ����w��
		&pMaterials,		//�}�e���A�������w��
		&pEffectInstance,	//�G�t�F�N�g�C���X�^���X���w��
		&m_NumMaterials,	//�}�e���A���̐�
		&m_pMesh			//���b�V��
		);

	//�ǂݍ��݂Ɏ��s���Ă�����
	if (hr != D3D_OK)
	{
		return FALSE;
	}

	//�}�e���A�����&�e�N�X�`�������}�e���A���o�b�t�@����擾
	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
	//�I�u�W�F�N�g�̐������A�}�e���A���ƃe�N�X�`�����������m��
	m_pMaterials = new D3DMATERIAL9[m_NumMaterials];
	m_pOriginalMaterials = new D3DMATERIAL9[m_NumMaterials];
	m_pTextures = new LPDIRECT3DTEXTURE9[m_NumMaterials];

	for (DWORD l = 0; l < m_NumMaterials; l++)
	{
		//�}�e���A���̃R�s�[
		memcpy(&m_pMaterials[l], &pD3DXMaterial[l ].MatD3D, sizeof(D3DMATERIAL9));
		memcpy(&m_pOriginalMaterials[l], &pD3DXMaterial[l].MatD3D, sizeof(D3DMATERIAL9));

		//�e�N�X�`���̃R�s�[
		if (pD3DXMaterial[l].pTextureFilename)
		{
			//�p�X��������
			ZeroMemory(FullPathName, sizeof(FullPathName));
			//FullPath�̒��g���R�s�[
			memcpy(FullPathName, FullPath, strlen(FullPath));
			//�e�N�X�`������ǉ�s
			strcat_s(FullPathName, pD3DXMaterial[l].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFile(
				m_pDevice9,		//�`��f�o�C�X���w��
				FullPathName,	//�t�@�C�������w��
				&m_pTextures[l]	//�e�N�X�`���A�h���X���w��
				)))
			{
				//�e�N�X�`����`��f�o�C�X�Ɋi�[�����̂ŏ�����
				m_pTextures[l] = NULL;
			}
			
		}
		//�e�N�X�`���̏�񂪂Ȃ����
		else
		{
			//�e�N�X�`����������
			m_pTextures[l] = NULL;
		}
	}
	//�}�e���A���A�G�t�F�N�g�C���X�^���X�A�אږʏ������
	SAFE_RELEASE(pMaterials);
	SAFE_RELEASE(pEffectInstance);
	SAFE_RELEASE(pAdjacency);

	LPD3DXBUFFER pErrorMsg;
	//======�V�F�[�_�[�̍쐬=====//
	hr = D3DXCreateEffectFromFile(
		m_pDevice9,
		"Effect\\XFile.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErrorMsg
		);

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsg->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsg);
		return FALSE;
	}
	return TRUE;
}

//====================================
//�֐���	:Draw
//����		:pos:���̂̍��W
//			 rota:���̂̉�]

//			 alphaValue:�����x

//�߂�l	:
//���e		:�`��(�����x�E���C�g�̂ݕύX�\)
//====================================
void CXfile::Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rota,D3DXVECTOR3 scale, float alphaValue)
{
	// �E�H�[�^�[�X���C�_�[�̏���
	if (slider)
	{
		TextureUV.y -= 0.1f;
		if (TextureUV.y < 0.0f)
			TextureUV.y = 1.0f;
	}
	else if (pool)
	{
		TextureUV.x -= 0.001f;
		if (TextureUV.x > 1.0f)
		{
			TextureUV.x = 0.0f;
		}

	}
	//�����x���͈͓������ׂ�
	if (alphaValue < 0)
	{
		alphaValue = 0;
	}
	else if (alphaValue>1.0f)
	{
		alphaValue = 1.0f;
	}
	D3DXMATRIX matWorld, matPosition, matRotation,matScaling;
	//�C���X�^���X�̎擾
	CCamera* pCamera = CCamera::GetInstance();
	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);

	D3DXMatrixTranslation(
		&matPosition,	// ���Z���ʂ̍s��ւ̃A�h���X
		pos.x,	// X���W�̃I�t�Z�b�g
		pos.y,	// X���W�̃I�t�Z�b�g
		pos.z);	// X���W�̃I�t�Z�b�g
	

						// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
		rota.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
		rota.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
		rota.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)
	matWorld = matRotation * matPosition;
	

	if (m_pEffect)
	{
		//�e�N�j�b�N�̑I��
		m_pEffect->SetTechnique("ShaderTechnique");
		//�p�X���w��
		m_pEffect->Begin(0, 0);
		//�p�X�̔ԍ����w��
		m_pEffect->BeginPass(0);

		//�X�P�[���ϊ�
		D3DXMatrixScaling(&matScaling,scale.x,scale.y,scale.z);
		// ���s�ړ��s����쐬
		D3DXMatrixTranslation(&matPosition, pos.x, pos.y, pos.z);
		// ���[�A�s�b�`�A���[�����w�肵�čs����쐬
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// ���Z���ʂ̍s��ւ̃A�h���X
			rota.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
			rota.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��) 
			rota.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)
		matRotCop = matRotation;

		//�r���ǉ�
		matWorld =matScaling* matRotation * matPosition;
		matWorldCopy = matWorld;
		// ���[���h�s����w��
		// �������͐�΃V�F�[�_�[�̂ق��Ő錾������ƈꏏ�ɂ���
		// ���Ȃ��Ⴋ��񂫂��ς�[
		m_pEffect->SetMatrix("matWorld", &matWorld);

		// �r���[�s����w��
		m_pEffect->SetMatrix("matView", &matView);

		// �v���W�F�N�V�����s����w��
		m_pEffect->SetMatrix("matProjection", &matProjection);

		// �J�����̈ʒu���w��
		m_pEffect->SetVector("ViewPosition", &vViewPosition);

		// ���C�g�������w��
		m_pEffect->SetVector("DirectionalLight", &vDirectionalLight);

		if (slider || pool)
		{
			m_pEffect->SetBool("slider", true);
			m_pEffect->SetBool("pool", pool);
			m_pEffect->SetFloat("uv_left", TextureUV.x);
			m_pEffect->SetFloat("uv_top", TextureUV.y);
			m_pEffect->SetFloat("uv_width", 1.0f);
			m_pEffect->SetFloat("uv_height", 1.0f);
		}
		else
		{
			m_pEffect->SetBool("slider", false);
		}
		if (pool)
		{
			m_pEffect->SetFloat("uv_width", 1.0f);
			m_pEffect->SetFloat("uv_height", 1.0f);

		}

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
					//�e�N�X�`�����w��
					m_pEffect->SetTexture("Texture", m_pTextures[i]);
					// �e�N�X�`���̗L����ݒ�
					if (m_pTextures[i] == NULL)
					{
						m_pEffect->SetBool("TextureNone", TRUE);
					}
					else
					{
						m_pEffect->SetBool("TextureNone", FALSE);
					}
					m_pEffect->SetFloat("alpha", alphaValue);

					//�V�F�[�_�ݒ���O���{�ɍX�V
					m_pEffect->CommitChanges();

					//���b�V���̕`��
					m_pMesh->DrawSubset(i);
				}

		m_pEffect->EndPass();
	}
}

//====================================
//�֐���	:Draw
//����		:pos	:�ʒu
//			 rota	:��]
//			 scale	:�傫��
//			 color	:RGB�l
//			 num	:�e�N�j�b�N�ԍ�
//�߂�l	:
//���e		:RGB�̒l��ύX�ł���XFILE�̕`��֐�
//====================================

void CXfile::Draw(D3DXVECTOR3* pos, D3DXVECTOR3* rota, D3DXVECTOR3 *scale, D3DXCOLOR color, int num)
{
	//�����x���͈͓������ׂ�
	if (color.a < 0)
	{
		color.a = 0;
	}
	else if (color.a>1.0f)
	{
		color.a = 1.0f;
	}
	if (color.r < 0)
	{
		color.r = 0;
	}
	else if (color.r>1.0f)
	{
		color.r = 1.0f;
	}
	if (color.g < 0)
	{
		color.g = 0;
	}
	else if (color.g>1.0f)
	{
		color.g = 1.0f;
	}
	if (color.b < 0)
	{
		color.b = 0;
	}
	else if (color.b>1.0f)
	{
		color.b = 1.0f;
	}


	//�C���X�^���X�̎擾
	CCamera* pCamera = CCamera::GetInstance();

	//�RD�t�@�C���̍��W���`����
	D3DXMATRIX *m_World = new D3DXMATRIX();
	D3DXMATRIX *m_temp = new D3DXMATRIX();
	//�s��̏�����
	D3DXMatrixIdentity(m_World);
	D3DXMatrixIdentity(m_temp);
	//=====�ȉ��RD�̍��W�E�g��E�ړ��̌v�Z=====//
	//�g��̏���
	D3DXMatrixScaling(m_temp, scale->x, scale->y, scale->z);
	*m_World *= *m_temp;
	//���f���̍��W�ϊ�(���[�A�s�b�`�A���[�����w�肵�čs����쐬)
	D3DXMatrixRotationYawPitchRoll(m_temp, rota->y, rota->x, rota->z);
	*m_World *= *m_temp;
	//���f���ړ��̏���
	D3DXMatrixTranslation(m_temp, pos->x, pos->y, pos->z);
	//�S�Ă̍s��̉��Z���ʊi�[
	*m_World *= *m_temp;

	//=====�����܂�=====//

	//���[���h���W�ϊ�
	D3DXMATRIX matWorldInverse;

	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();
	D3DXMATRIX matVP = matView*matProjection;

	//������W�n�r���[�s����쐬(���C�g����݂��f��)
	D3DXMatrixLookAtLH(
		&matView,									//���Z���ʂł���D3DXMATRIX�\���̂ւ̃|�C���^���w��
		(D3DXVECTOR3*)&pCamera->GetLightPosition(),	//���_���`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		(D3DXVECTOR3*)&pCamera->GetLookatPt(),		//�J�����̒����Ώۂ��`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)				//�J�����g���[���h�̏���A��ʂɂ�[0,1,0]���`����D3DXVECTOR3�\���̂ւ̃|�C���^���w��
		);
	//������W�n��ViewProj�s��
	D3DXMATRIX matLVP = matView*matProjection;

	if (m_pEffect)
	{
		//�e�N�j�b�N�̑I��
		m_pEffect->SetTechnique("ShaderTechnique");
		//�p�X���w��
		m_pEffect->Begin(0, 0);
		//�p�X�̔ԍ����w��
		m_pEffect->BeginPass(num);

		//���[���h���W
		m_pEffect->SetMatrix("matWorld", m_World);

		//���[���h���W�̋t�s����w��
		D3DXMatrixInverse(&matWorldInverse, NULL, m_World);
		m_pEffect->SetMatrix("matWorldInverse", &matWorldInverse);

		//���e�ϊ����w��
		m_pEffect->SetMatrix("matVP", &matVP);
		m_pEffect->SetMatrix("matLVP", &matLVP);
		m_pEffect->SetBool("titleLight", true);
		//�I�u�W�F�N�g�ԍ����w��
		//m_pEffect->SetInt("ObjectNumber", ObjectNumber);

		//���������w��
		D3DXVECTOR4 vDirectionalLight = D3DXVECTOR4(pCamera->GetDirectionalLight(), 0.0f);


		m_pEffect->SetVector("LightDirection ", &vDirectionalLight);

		//�J�����̈ʒu���w��
		//D3DXVECTOR4 vViewFrom(pCamera->GetViewPosition(), 0.0f);
		//m_pEffect->SetVector("ViewFrom", &vViewFrom);


		for (DWORD i = 0; i < m_NumMaterials; i++)
		{
			//�F���Z�b�g

			D3DXCOLOR v;
			v.r = color.r;// m_pMaterials[i].Diffuse.r;
			v.g = color.g;
			v.b = color.b;
			v.a = color.a;

			//���_�̐F���w��
			m_pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&v);

			//�e�N�X�`���̗L�����w��
			//m_pEffect->SetBool("TextureNone", m_pTextures ? FALSE : TRUE);

			//�e�N�X�`�����w��
			m_pEffect->SetTexture("Texture", m_pTextures[i]);

			//�V�F�[�_�ݒ���O���{�ɍX�V
			m_pEffect->CommitChanges();

			//���b�V���̕`��
			m_pMesh->DrawSubset(i);
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	SAFE_DELETE(m_World);
	SAFE_DELETE(m_temp);
}
