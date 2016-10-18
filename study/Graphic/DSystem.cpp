#include "DSystem.h"

//�_��Ȓ��_�\���̐錾
#define SPRITE_FVF  (D3DFVF_XYZ | D3DFVF_TEX1 |D3DFVF_DIFFUSE)



//====================================
//�֐���	:CDSystem
//����		:pDevice:�`��f�o�C�X
//			 hWnd	:�E�B���h�E�n���h��
//			 FileName:�t�@�C����
//�߂�l	:
//���e		:�R���X�g���N�^
//====================================
CDSystem::CDSystem(LPDIRECT3DDEVICE9 pDevice)
{
	//�E�B���h�E�̑傫�����擾
	w = WINDOW_WIDTH;
	h = WINDOW_HEIGHT;
	m_pSoundManager = NULL;
	m_pTexture = NULL;
	m_pVB = NULL;
	m_pEffect = NULL;
	m_dwLoadIWaveFileNum = SOUNDMAX;
	m_pDevice3D = pDevice;
	
	m_pDevice3D->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//LoadTexture();
	CreateResource();
//	CreateBorad();
	
}

//====================================
//�֐���	:~CDSystem
//����		:
//�߂�l	:
//���e		:�f�X�g���N�^
//====================================
CDSystem::~CDSystem()
{

		SAFE_RELEASE(m_pTexture);
		
		SAFE_RELEASE(m_pDevice3D);
		SAFE_DELETE(m_pSoundManager);
		//SAFE_DELETE(m_pVB);
		m_pSoundManager = NULL;
		m_pEffect = NULL;
}

//====================================


//====================================
//�֐���	:CreateResource
//����		:hWnd:�E�B���h�E�n���h��
//�߂�l	:
//���e		:���\�[�X�̍쐬
//====================================
BOOL CDSystem::CreateResource()
{


	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	//�e�N�X�`���p�V�F�[�_�[�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		m_pDevice3D,			//�f�o�C�X
		"Effect\\sprite.fx",	//�t�@�C����
		NULL,					//�v���v���Z�b�T��`�ւ̃|�C���^
		NULL,					//�I�v�V�����̃C���^�[�t�F�C�X�|�C���^
		0,						//�R���p�C���I�v�V����
		NULL,					//�G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&m_pEffect,				//�G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^
		&pErrorMsgs				//�G���[�y�ьx�����X�g���܂ރo�b�t�@
		);

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->GetBufferPointer(), "�G�t�F�N�g�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	return TRUE;
}

BOOL CDSystem::CreateSoundManager(HWND hWnd)
{
	
	m_pSoundManager = new CSoundManager(hWnd);

	//�T�E���h�}�l�[�W���[�̍쐬
	if (!m_pSoundManager->GetDirectSound()->Create(hWnd))
	{
		return FALSE;
	}
	//�T�E���h�̓ǂݍ���
	for (DWORD i = 0; i < m_dwLoadIWaveFileNum; i++)
	{
		if (!m_pSoundManager->LoadWave(i, SoundFileName[i]))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//====================================
//�֐���	:LoadTexture
//����		:FileName:�t�@�C����
//�߂�l	:
//���e		:�e�N�X�`����ǂݍ��ފ֐�
//====================================
BOOL CDSystem::LoadTexture(const TCHAR* FileName)
{
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
	strcat_s(FullPathName, FileName);


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


		//�e�N�X�`���̓ǂݍ��݊֐�
		if (D3DXCreateTextureFromFile(
			m_pDevice3D,	//�`��f�o�C�X
			FileName,		//�t�@�C����
			&m_pTexture		//�e�N�X�`���ւ̃|�C���^
			))
		{
			//�ǂݍ��݂Ɏ��s
			return FALSE;
		}
	

		////�V�F�[�_�ɒ��_�̃t�H�[�}�b�g����`���邽�߂�D3DVERTEXELEMENT9���쐬
		D3DVERTEXELEMENT9 elems[] =
		{
			{
			0,
			0,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION,
			0
			},
			{
			0,
			16,
			D3DDECLTYPE_FLOAT4,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_COLOR,
			0
			},
			{
			0,
			32,
			D3DDECLTYPE_FLOAT2,
			D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD,
			0
			},
			D3DDECL_END()				//�I�[�L��(�K�{)
		};
		//���_�V�F�[�_�[�̍쐬
		m_pDevice3D->CreateVertexDeclaration(elems, &decl);

	return TRUE;
}

//====================================
//�֐���	:Draw
//����		:vPos	:�摜�̈ʒu���w��
//			 num	:�g�p����e�N�j�b�N���w��
//					 0:���̂܂�
//					 1:��Z
//			 fAlpha	:�����x
//			 posSize:�摜�̃T�C�Y���w��
//�߂�l	:
//���e		:�`��
//====================================
void CDSystem::Draw(D3DXVECTOR3 vPos,UINT num,FLOAT fAlpha, D3DXVECTOR3 posSize)
{
	BOARDVERTEX vertex[4];

	//�摜�̓����x�̐ݒ肪�K��l�ȓ�������
	if (fAlpha < 0.0f) {
		fAlpha = 0.0f;
	}
	if (fAlpha > 1.0f) {
		fAlpha = 1.0f;
	}
	
	//�J���[�ƃ|�W�V�����̏�����
	for (DWORD i = 0; i < 4; i++)
	{
		//�J���[�̏�����
		vertex[i].vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, fAlpha);
		//Z�����O�ɂ���
		vertex[i].vPosition.z =0;
		//�lw��1.0�Ɏw��
		vertex[i].vPosition.w = 1.0f;
	}

	
	//���_�̐ݒ�
	vertex[0].vPosition.x = vertex[3].vPosition.x = vPos.x;
	vertex[1].vPosition.x = vertex[2].vPosition.x = vPos.x + posSize.x;//+�e�N�X�`���̃T�C�Y;
	vertex[0].vPosition.y = vertex[1].vPosition.y = vPos.y;
	vertex[2].vPosition.y = vertex[3].vPosition.y = vPos.y + posSize.y;	
	//UV�l�̐ݒ�
	vertex[0].tu = vertex[3].tu = 0.0f;
	vertex[1].tu = vertex[2].tu = 1.0f;
	vertex[0].tv = vertex[1].tv = 0.0f;
	vertex[2].tv = vertex[3].tv = 1.0f;


	D3DVIEWPORT9 vp;
	//��ʃT�C�Y���擾
	m_pDevice3D->GetViewport(&vp);

	//�����V�F�[�_�[�ŗ��p���邽�߂ɕϊ�
	for (DWORD i = 0; i < 4; i++)
	{
		vertex[i].vPosition.x -= (FLOAT)vp.Width / 2.0f;
		vertex[i].vPosition.y = (FLOAT)vp.Height - vertex[i].vPosition.y;
		vertex[i].vPosition.y -= (FLOAT)vp.Height / 2.0f;
	}


	D3DXMATRIX matWorld,matProjection;
	//�s�������������
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matProjection);

	//������W�n���ˉe�s����쐬
	D3DXMatrixOrthoLH
		(
		&matProjection,
		(FLOAT)vp.Width,
		(FLOAT)vp.Height,
		0.0f, 
		1.0f
		);
	//matProjection._11 = 2 / h;
	//matProjection._22 = -2 / w;
	D3DXMATRIX matVP = matProjection;
	//�ˌ`�ϊ�
	//matProjection._11 = 2 / h;
	//matProjection._22 = -2 / w;
	////�ˌ`�ϊ�
	////D3DXMATRIX proj(
	////	2/h, 0.0f, 0.0f, 0.0f,
	////	0.0f, -2/w, 0.0f, 0.0f,
	////	0.0f, 0.0f, 1.0f, 0.0f,
	////	0.0f, 0.0f, 0.0f, 1.0f
	////	);
	//
	////=====�ړ�=====
	////���������ɂ����
	//matWorld._41 = vPos.x;
	////y���������
	//matWorld._42 = vPos.y;
	//=====�ړ��I��=====
	//�ϊ��ړ��s����s�����߂̍s��(���_�������)
	//x+=-1.0f
	//y+=1.0f
	//D3DXMATRIX offset(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	-1.0f, 1.0f, 0.0f, 1.0f
	//	);
	//VTX vtx[] = {
	//	{0.0f, 0.0f, 0.0f,0.0f,0.0f},		//0
	//	{150.0f,0.0f, 0.0f,1.0f,0.0f},		//2
	//	{0.0f, 150.0f, 0.0f,0.0f,1.0f},		//1
	//	{150.0f,150.0f,0.0f,1.0f,1.0f}		//3
	//};
	////�����ˉe�ϊ��s��
	////�����ʒu������ɂ���
	//matProjection *= offset;
	//m_pDevice3D->SetTransform(D3DTS_PROJECTION, &matProjection);

	if (m_pEffect) 
	{
		//���_�t�H�[�}�b�g�錾��ݒ�
		m_pDevice3D->SetVertexDeclaration(decl);
		//�����̃e�N�j�b�N���g���ꍇ������̂łǂ̃e�N�j�b�N���g�����w��
		m_pEffect->SetTechnique("Tech");
		//�������Ɏw���technique���Ɋ܂܂��p�X�̐����Ԃ��Ă���
		//�V�F�[�_�̓p�X��A���ŌĂяo�����Ƃŕ�����_������Ƃ��ł��邽�߃p�X�̔ԍ����擾����K�v������
		//
		m_pEffect->Begin(0, 0);
		if (num == 0) {
			//P0���g�p
			m_pEffect->BeginPass(0);
		}
		else if(num==1){
			//P1���g�p
			m_pEffect->BeginPass(1);
		}
		
		//�s��̐ݒ�
		m_pEffect->SetMatrix("matWorld", &matWorld);
		m_pEffect->SetMatrix("matVP", &matVP);
		//�e�N�X�`���̐ݒ�
		m_pEffect->SetTexture("m_pTexture",m_pTexture);
		//�V�F�[�_�[�ɑ���l(�����UV�l)
		//m_pEffect->SetFloat("uv_left", 0);
		//m_pEffect->SetFloat("uv_top", 0);
		//m_pEffect->SetFloat("uv_width", 0.55f);
		//m_pEffect->SetFloat("uv_height", 0.55f);
		//m_pDevice3D->SetVertexDeclaration(decl);

		//�O���{�̐��\���`�F�b�N
		m_pEffect->CommitChanges();

		m_pDevice3D->DrawPrimitiveUP
			(
			D3DPT_TRIANGLEFAN,
			2,
			&vertex,
			sizeof(BOARDVERTEX)
			);

		m_pEffect->EndPass();
		m_pEffect->End();
	}
	//m_pDevice3D->SetFVF(D3DFVF_XYZ);
	//SAFE_DELETE(vertex);
}


void CDSystem::Draw()
{

	D3DVIEWPORT9 vp;
	//��ʃT�C�Y���擾
	m_pDevice3D->GetViewport(&vp);

	D3DXMATRIX matWorld, matProjection;
	//�s�������������
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixIdentity(&matProjection);

	//������W�n���ˉe�s����쐬
	D3DXMatrixOrthoLH
		(
			&matProjection,
			(FLOAT)vp.Width,
			(FLOAT)vp.Height,
			0.0f,
			1.0f
			);

	D3DXMATRIX matVP = matProjection;

	if (m_pEffect)
	{
		//���_�t�H�[�}�b�g�錾��ݒ�
		m_pDevice3D->SetVertexDeclaration(decl);
		//�����̃e�N�j�b�N���g���ꍇ������̂łǂ̃e�N�j�b�N���g�����w��
		m_pEffect->SetTechnique("Tech");
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);
		//�s��̐ݒ�
		m_pEffect->SetMatrix("matWorld", &matWorld);
		m_pEffect->SetMatrix("matVP", &matVP);
		//�e�N�X�`���̐ݒ�
		m_pEffect->SetTexture("m_pTexture", m_pTexture);

		m_pDevice3D->SetFVF(SPRITE_FVF);


		m_pDevice3D->SetStreamSource(0, m_pVB, 0, sizeof(BOARDVERTEX));
		m_pDevice3D->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		m_pEffect->EndPass();
		m_pEffect->End();
	}


}
