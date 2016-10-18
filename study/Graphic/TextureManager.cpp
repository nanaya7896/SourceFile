#include "TextureManager.h"

//唯一のインスタンスの生成
CTextureManager* CTextureManager::m_pTextureInstance = NULL;


CTextureManager::CTextureManager(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	for (int i = 0; i < TEXTURE_NUM; i++)
	{
		m_pDSystem[i] = NULL;
	}

	CreateResource();
}

CTextureManager::~CTextureManager()
{

}

void CTextureManager::CreateResource()
{
	for (int i = 0; i < TEXTURE_NUM; i++)
	{
		m_pDSystem[i] = new CDSystem(m_pDevice);
	}
}

void CTextureManager::Create(LPDIRECT3DDEVICE9 pDevice)
{
	//Textureインスタンスが存在しないなら
	if (!m_pTextureInstance)
	{
		m_pTextureInstance = new CTextureManager(pDevice);
	}
}

void CTextureManager::Destroy()
{
	SAFE_DELETE(m_pTextureInstance);
}

void CTextureManager::Delete()
{
	for (int i = 0; i < TEXTURE_NUM; i++)
	{
		SAFE_DELETE(m_pDSystem[i]);
	}
}

BOOL CTextureManager::AllLoadTexture()
{
	for (int i = 0; i < TEXTURE_NUM; i++)
	{
		if (FAILED(m_pDSystem[i]->LoadTexture(TextureFileName[i])))
		{
			OutputDebugString("Texture Load Error");
			return FALSE;
		}
	}
	return TRUE;
}



void CTextureManager::Draw(DWORD num, D3DXVECTOR3 vPos, UINT tNum, FLOAT fAlpha, D3DXVECTOR3 posSize)
{
	m_pDSystem[num]->Draw(vPos, tNum, fAlpha, posSize);
}

CHAR * CTextureManager::GetTextureName(DWORD num)
{
	return TextureFileName[num];
}
