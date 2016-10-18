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

	// シェーダの読み込み
	hr = D3DXCreateEffectFromFile(
		m_pDevice,	// デバイス
		"Effect/XFile.fx",		// ファイル名
		NULL,			// プリプロセッサ定義へのポインタ
		NULL,			// オプションのインターフェイスポインタ 
		0,				// コンパイルオプション
		NULL,			// エフェクトプールオブジェクトへのポインタ
		&m_pEffect,		// エフェクトオブジェクトへのポインタ
		&pErrorMsgs);	// エラーおよび警告のリストを含むバッファ
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	// 頂点のフォーマット宣言
	D3DVERTEXELEMENT9 VertexElements[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	// 頂点宣言の生成
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
		// オブジェクトの座標とアングルの初期化
		// 仮にオール0.0f
		m_ObjectInfo[j].vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_ObjectInfo[j].vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		HRESULT hr;
		LPD3DXBUFFER pAdjacency = NULL;
		LPD3DXBUFFER pEffectInstances = NULL;
		LPD3DXBUFFER pMaterials = NULL;

		// Xファイルからメッシュをロード
		hr = D3DXLoadMeshFromX(
			pFileName,			// ファイル名へのアドレス
			D3DXMESH_MANAGED,	// メッシュの作成オプションを指定
			m_pDevice,		// デバイス
			&pAdjacency,		// 隣接面情報へのアドレス
			&pMaterials,		// マテリアル情報へのアドレス
			&pEffectInstances,	// エフェクト情報へのアドレス
			&m_NumMaterials,	// マテリアル数へのアドレス
			&m_ObjectInfo[j].pMesh);			// メッシュへのアドレス
		if (hr != D3D_OK)
			return FALSE;

		// マテリアル情報の取得
		m_pMaterials = new D3DMATERIAL9[m_NumMaterials];
		m_pTextures = new LPDIRECT3DTEXTURE9[m_NumMaterials];

		// マテリアル情報をマテリアルバッファから取得
		D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
		for (DWORD i = 0; i < m_NumMaterials; i++)
		{
			m_pMaterials[i] = pD3DXMaterial[i].MatD3D;
			FLOAT Length;
			D3DXVECTOR4 vLength;
			// ambientに値がない場合は、Emissiveに値あるか確認
			// ない場合は、diffuseを利用する。50%の値とする。
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

			// specularに値があるか確認する。
			// ない場合は、10%の灰色を指定する。
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
					m_pDevice,		// デバイスを指定
					FullPathName,		// ファイル名を指定
					&m_pTextures[i]);	// テクスチャーアドレスを指定
			}
		}

		SAFE_RELEASE(pAdjacency);
		SAFE_RELEASE(pEffectInstances);
		SAFE_RELEASE(pMaterials);

		// 大きさの調整
		// 頂点の数を取得
		DWORD NumVertices = m_ObjectInfo[j].pMesh->GetNumVertices();
		// 頂点のスライドを取得
		DWORD NumBytesPerVertex = m_ObjectInfo[j].pMesh->GetNumBytesPerVertex();

		LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
		FLOAT* pVertex;

		// 頂点バッファ内のデータを取得
		m_ObjectInfo[j].pMesh->GetVertexBuffer(&pVB);
		// 頂点データの範囲をロックし、頂点バッファメモリへのポインタを取得
		pVB->Lock(
			0, // ロックする頂点データへのオフセット(バイト単位)(全部ロック)
			0, // ロックする頂点データのサイズ(バイト単位)(全部ロック)
			(void**)&pVertex,	// 返される頂点データが格納されているメモリバッファへのVOID*ポインタ
			D3DLOCK_DISCARD);	// 実行するロックの種類
		for (DWORD i = 0; i < NumVertices; i++)
		{
			D3DXVECTOR3* pPostion =
				(D3DXVECTOR3*)&pVertex[i * (NumBytesPerVertex / sizeof(FLOAT))];
			*pPostion *= m_Scaling;
		}
		pVB->Unlock();		// ロック解除
		SAFE_RELEASE(pVB);	// 頂点バッファの解放
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

	// 頂点宣言を指定
	m_pDevice->SetVertexDeclaration(m_pVertexDecl);
	if (m_pEffect)
	{
		// テクニックの選択
		m_pEffect->SetTechnique("ShaderTechnique");

		// パスを指定
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);	// パスの番号を指定

		// 平行移動行列を作成
		D3DXMatrixTranslation(
			&matPosition,		// 演算結果の行列へのアドレス
			m_vPosition.x,		// X座標のオフセット
			m_vPosition.y,		// X座標のオフセット
			m_vPosition.z);		// X座標のオフセット
								// ヨー、ピッチ、ロールを指定して行列を作成
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// 演算結果の行列へのアドレス
			m_vAngle.y,		// Y軸を中心とするヨー(ラジアン単位)
			m_vAngle.x,		// X軸を中心とするピッチ(ラジアン単位) 
			m_vAngle.z);	// Z軸を中心とするロール(ラジアン単位)
		matWorld = matRotation * matPosition;

		// ワールド行列を指定
		m_pEffect->SetMatrix("matWorld", &matWorld);
		// ビュー行列を指定
		m_pEffect->SetMatrix("matView", &matView);
		// プロジェクション行列を指定
		m_pEffect->SetMatrix("matProjection", &matProjection);

		// カメラの位置を指定
		m_pEffect->SetVector("ViewPosition", &vViewPosition);

		// ライト方向を指定
		m_pEffect->SetVector("DirectionalLight", &vDirectionalLight);

		for (DWORD j = 0; j < NumObject; j++)
		{

			for (DWORD i = 0; i < m_NumMaterials; i++)
			{
				// マテリアルを指定
				D3DXVECTOR4 vColor;
				memcpy(&vColor, &m_pMaterials[i].Diffuse, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialDiffuse", &vColor);
				memcpy(&vColor, &m_pMaterials[i].Ambient, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialAmbient", &vColor);
				memcpy(&vColor, &m_pMaterials[i].Specular, sizeof(D3DXVECTOR4));
				m_pEffect->SetVector("MaterialSpecular", &vColor);

				// テクスチャーを指定
				if (m_pTextures[i])
					m_pEffect->SetTexture("Texture", m_pTextures[i]);
				// テクスチャの有無を設定
				if (m_pTextures[i] == NULL)
					m_pEffect->SetBool("TextureNone", TRUE);
				else
					m_pEffect->SetBool("TextureNone", FALSE);

				// シェーダ設定をグラボに更新
				m_pEffect->CommitChanges();
				// 描画
				m_ObjectInfo[j].pMesh->DrawSubset(i);
			}
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}
}