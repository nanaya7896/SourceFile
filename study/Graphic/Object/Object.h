#pragma once

#include "../../Global/Global.h"
#include "../../Camera/Camera.h"

#define NumObject 10// 仮に20にしています

typedef struct _tagObjectInfo_
{
	D3DXVECTOR3		vPosition;
	D3DXVECTOR3		vAngle;
	LPD3DXMESH		pMesh;
}OBJECTINFO;

class CWall {
private:
	//描画デバイス
	LPDIRECT3DDEVICE9   m_pDevice;
	DWORD				m_NumMaterials;	// マテリアルの数
	D3DMATERIAL9*		m_pMaterials;	// マテリアル
	LPDIRECT3DTEXTURE9*	m_pTextures;	// テクスチャ
	//LPD3DXMESH			m_pMesh;		// メッシュ
	D3DXVECTOR3			m_vPosition;	// 座標
	D3DXVECTOR3			m_vAngle;		// アングル
	FLOAT				m_Scaling;		// スケーリング値

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;	// 頂点宣言
	LPD3DXEFFECT		m_pEffect;				// エフェクト

	OBJECTINFO*			m_ObjectInfo;
public:
	CWall(LPDIRECT3DDEVICE9);
	~CWall();
	BOOL Initialize(FLOAT, char*);
	void Draw();
	OBJECTINFO* GetObjectInfo() { return m_ObjectInfo; }
};