#include "PointSprite.h"


DWORD FloatToDWORD(float value)
{
	return *((DWORD*)&value);
}

//====================================
//�֐���	:CPointSprite
//����		:
//�߂�l	:
//���e		:
//====================================
CPointSprite::CPointSprite(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice9 = pDevice;
	ColorChange = 0;

	for (int i = 0; i < 9; i++)
	{
		m_Pos[i] = D3DXVECTOR3(0,0,0);
	}

	Value = 0.01f;
	m_pTexture = NULL;
	m_pEffect = NULL;
	InitPointSprite(m_pDevice9);
}

//====================================
//�֐���	:~CPointSprite
//����		:
//�߂�l	:
//���e		:
//====================================
CPointSprite::~CPointSprite()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pPointBuffer);
	//SAFE_RELEASE(m_pTexture);
}

BOOL CPointSprite::PointLoadTexture(const TCHAR *FileName)
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
	if (FAILED(D3DXCreateTextureFromFileEx(
		m_pDevice9,
		FileName,
		0,
		0,
		0,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		D3DCOLOR_ARGB(255,255,255, 255),
		nullptr,
		nullptr,
		&m_pTexture)))
	{
		return FALSE;
	}
	HRESULT hr;
	LPD3DXBUFFER pErrorMsg;
	//======�V�F�[�_�[�̍쐬=====//
	hr = D3DXCreateEffectFromFile(
		m_pDevice9,
		"Effect/particle.fx",
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
//�֐���	:InitPointSprite
//����		:
//�߂�l	:
//���e		:
//====================================
BOOL CPointSprite::InitPointSprite(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		10000 * sizeof(Vertex3),
		D3DUSAGE_WRITEONLY,
		VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_pPointBuffer,
		nullptr
		)))
	{
		//�o�b�t�@�̏������Ɏ��s
		return FALSE;
	}
	D3DXMatrixIdentity(&m_World);
	
	return TRUE;
}

//====================================
//�֐���	:DrawPointSprite
//����		:
//�߂�l	:
//���e		:
//====================================
void CPointSprite::DrawPointSprite(Vertex3 point[], int numPointSprite, bool /*alpha*/)
{
	Vertex3 *vertex;

	//�C���X�^���X�̎擾
	CCamera* pCamera = CCamera::GetInstance();

	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();

	m_pPointBuffer->Lock(0, 0, (LPVOID*)&vertex, 0);
	for (int i = 0; i < numPointSprite; i++)
	{
		// �|�C���g�X�v���C�g�̈ʒu�̌v�Z
		vertex[i].pos = point[i].pos;

		// �|�C���g�X�v���C�g�̃T�C�Y��ݒ�
		vertex[i].size = point[i].size;

		// �|�C���g�X�v���C�g�̐F��ݒ�
		vertex[i].color = point[i].color;
	}
	m_pPointBuffer->Unlock();

	if (m_pEffect)
	{
		m_pEffect->SetTechnique("tec");
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);

		//�F���Z�b�g
		D3DXCOLOR v;
		v.r = 1.0f;/*ColorChange;*/
		v.g = 1.0f;
		v.b = 1.0f;
		v.a = 0.5f;
		//���_�̐F���w��
		m_pEffect->SetVector("matDiffuse", (D3DXVECTOR4*)&v);

		D3DXMATRIX m_world;
		D3DXMatrixIdentity(&m_world);
		m_pEffect->SetMatrix("world", &m_world);
		m_pEffect->SetMatrix("proj", &matProjection);
		m_pEffect->SetMatrix("view", &matView);
		m_pEffect->SetTexture("tex", m_pTexture);
		m_pDevice9->SetStreamSource(0, m_pPointBuffer, 0, sizeof(Vertex3));
		m_pDevice9->SetFVF(VERTEX_FVF);
		for (int i = 0; i < 5; i++)
		{
			m_pDevice9->DrawPrimitive(D3DPT_POINTLIST, 0, numPointSprite);
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}

}

void CPointSprite::Update()
{
	Speed += 0.07f;
}