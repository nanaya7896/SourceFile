#include "Xfile.h"

//====================================
//関数名	:CXfile
//引数		:
//戻り値	:
//内容		:コンストラクタ
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
//関数名	:~CXfile
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CXfile::~CXfile()
{
	//テクスチャの解放
	for (DWORD i = 0; i < m_NumMaterials; i++)
	{
		SAFE_RELEASE(m_pTextures[i]);
	}
	//マテリアルの破棄
	SAFE_DELETE_ARRAY(m_pMaterials);
	SAFE_DELETE_ARRAY(m_pOriginalMaterials);
	//テクスチャの破棄
	SAFE_DELETE_ARRAY(m_pTextures);
	//メッシュの解放
	SAFE_RELEASE(m_pMesh);
	//エフェクトの解放
	SAFE_RELEASE(m_pEffect);

	SAFE_DELETE(m_Math);
}

//====================================
//関数名	:LoatXfile
//引数		:filename: ファイルの名前
//戻り値	:TRUE:読み込み成功 FALSE:失敗
//内容		:Xfileの読み込み
//====================================
BOOL CXfile::LoatXfile(const TCHAR* pfilename)
{
	HRESULT hr=E_FAIL;
	//隣接麺情報
	LPD3DXBUFFER pAdjacency;
	//マテリアルデータ
	LPD3DXBUFFER pMaterials;
	//エフェクトインスタンス
	LPD3DXBUFFER pEffectInstance;

	//ファイルの場所をフルパスで検索する
	char FullPathName[MAX_PATH];
	char FullPath[MAX_PATH];
	//現在のプロセスのカレントディレクトリを取得
	//1:カレントディレクトリを取得するためのバッファの長さを指定(NULL文字を含む) 
	//2:バッファへのポインタを指定(絶対パス名が格納される)
	GetCurrentDirectory(MAX_PATH, FullPathName);
	//文字列を追加する
	//1:追加先の文字列
	//2:元の文字列
	strcat_s(FullPathName, "\\");
	strcat_s(FullPathName, pfilename);

	DWORD i;
	//FullPathNameの文字列の長さ分繰り返す
	for (i = strlen(FullPathName); i > 0; i--)
	{
		//'\\'を見つけた場合
		if (FullPathName[i] == '\\')
		{
			//char型のNULLを一つ先に格納
			FullPathName[i + 1] = '\0';
			break;
		}
	}
	//文字列が０になった場合
	if (i == 0)
	{
		//char型NULLを末尾に追加
		FullPathName[0] = '\0';
	}
	//FullPathにFullPathNameのメモリをコピー
	memcpy(FullPath, FullPathName, strlen(FullPathName) + 1);

	hr = D3DXLoadMeshFromX(
		pfilename,			//ファイル名を指定
		D3DXMESH_MANAGED,	//オプションを指定
		m_pDevice9,			//デバイスを指定
		&pAdjacency,		//隣接麺情報を指定
		&pMaterials,		//マテリアル情報を指定
		&pEffectInstance,	//エフェクトインスタンスを指定
		&m_NumMaterials,	//マテリアルの数
		&m_pMesh			//メッシュ
		);

	//読み込みに失敗していたら
	if (hr != D3D_OK)
	{
		return FALSE;
	}

	//マテリアル情報&テクスチャ情報をマテリアルバッファから取得
	D3DXMATERIAL* pD3DXMaterial = (D3DXMATERIAL*)pMaterials->GetBufferPointer();
	//オブジェクトの数だけ、マテリアルとテクスチャメモリを確保
	m_pMaterials = new D3DMATERIAL9[m_NumMaterials];
	m_pOriginalMaterials = new D3DMATERIAL9[m_NumMaterials];
	m_pTextures = new LPDIRECT3DTEXTURE9[m_NumMaterials];

	for (DWORD l = 0; l < m_NumMaterials; l++)
	{
		//マテリアルのコピー
		memcpy(&m_pMaterials[l], &pD3DXMaterial[l ].MatD3D, sizeof(D3DMATERIAL9));
		memcpy(&m_pOriginalMaterials[l], &pD3DXMaterial[l].MatD3D, sizeof(D3DMATERIAL9));

		//テクスチャのコピー
		if (pD3DXMaterial[l].pTextureFilename)
		{
			//パスを初期化
			ZeroMemory(FullPathName, sizeof(FullPathName));
			//FullPathの中身をコピー
			memcpy(FullPathName, FullPath, strlen(FullPath));
			//テクスチャ名を追加s
			strcat_s(FullPathName, pD3DXMaterial[l].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFile(
				m_pDevice9,		//描画デバイスを指定
				FullPathName,	//ファイル名を指定
				&m_pTextures[l]	//テクスチャアドレスを指定
				)))
			{
				//テクスチャを描画デバイスに格納したので初期化
				m_pTextures[l] = NULL;
			}
			
		}
		//テクスチャの情報がなければ
		else
		{
			//テクスチャを初期化
			m_pTextures[l] = NULL;
		}
	}
	//マテリアル、エフェクトインスタンス、隣接面情報を解放
	SAFE_RELEASE(pMaterials);
	SAFE_RELEASE(pEffectInstance);
	SAFE_RELEASE(pAdjacency);

	LPD3DXBUFFER pErrorMsg;
	//======シェーダーの作成=====//
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
//関数名	:Draw
//引数		:pos:物体の座標
//			 rota:物体の回転

//			 alphaValue:透明度

//戻り値	:
//内容		:描画(透明度・ライトのみ変更可能)
//====================================
void CXfile::Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rota,D3DXVECTOR3 scale, float alphaValue)
{
	// ウォータースライダーの処理
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
	//透明度が範囲内か調べる
	if (alphaValue < 0)
	{
		alphaValue = 0;
	}
	else if (alphaValue>1.0f)
	{
		alphaValue = 1.0f;
	}
	D3DXMATRIX matWorld, matPosition, matRotation,matScaling;
	//インスタンスの取得
	CCamera* pCamera = CCamera::GetInstance();
	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);

	D3DXMatrixTranslation(
		&matPosition,	// 演算結果の行列へのアドレス
		pos.x,	// X座標のオフセット
		pos.y,	// X座標のオフセット
		pos.z);	// X座標のオフセット
	

						// ヨー、ピッチ、ロールを指定して行列を作成
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// 演算結果の行列へのアドレス
		rota.y,		// Y軸を中心とするヨー(ラジアン単位)
		rota.x,		// X軸を中心とするピッチ(ラジアン単位) 
		rota.z);	// Z軸を中心とするロール(ラジアン単位)
	matWorld = matRotation * matPosition;
	

	if (m_pEffect)
	{
		//テクニックの選択
		m_pEffect->SetTechnique("ShaderTechnique");
		//パスを指定
		m_pEffect->Begin(0, 0);
		//パスの番号を指定
		m_pEffect->BeginPass(0);

		//スケール変換
		D3DXMatrixScaling(&matScaling,scale.x,scale.y,scale.z);
		// 平行移動行列を作成
		D3DXMatrixTranslation(&matPosition, pos.x, pos.y, pos.z);
		// ヨー、ピッチ、ロールを指定して行列を作成
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// 演算結果の行列へのアドレス
			rota.y,		// Y軸を中心とするヨー(ラジアン単位)
			rota.x,		// X軸を中心とするピッチ(ラジアン単位) 
			rota.z);	// Z軸を中心とするロール(ラジアン単位)
		matRotCop = matRotation;

		//池原追加
		matWorld =matScaling* matRotation * matPosition;
		matWorldCopy = matWorld;
		// ワールド行列を指定
		// 第一引数は絶対シェーダーのほうで宣言したやつと一緒にする
		// やらなきゃきゅんきゅんぱわー
		m_pEffect->SetMatrix("matWorld", &matWorld);

		// ビュー行列を指定
		m_pEffect->SetMatrix("matView", &matView);

		// プロジェクション行列を指定
		m_pEffect->SetMatrix("matProjection", &matProjection);

		// カメラの位置を指定
		m_pEffect->SetVector("ViewPosition", &vViewPosition);

		// ライト方向を指定
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
					// マテリアルを指定
					D3DXVECTOR4 vColor;
					memcpy(&vColor, &m_pMaterials[i].Diffuse, sizeof(D3DXVECTOR4));
					m_pEffect->SetVector("MaterialDiffuse", &vColor);
					memcpy(&vColor, &m_pMaterials[i].Ambient, sizeof(D3DXVECTOR4));
					m_pEffect->SetVector("MaterialAmbient", &vColor);
					memcpy(&vColor, &m_pMaterials[i].Specular, sizeof(D3DXVECTOR4));
					m_pEffect->SetVector("MaterialSpecular", &vColor);
					//テクスチャを指定
					m_pEffect->SetTexture("Texture", m_pTextures[i]);
					// テクスチャの有無を設定
					if (m_pTextures[i] == NULL)
					{
						m_pEffect->SetBool("TextureNone", TRUE);
					}
					else
					{
						m_pEffect->SetBool("TextureNone", FALSE);
					}
					m_pEffect->SetFloat("alpha", alphaValue);

					//シェーダ設定をグラボに更新
					m_pEffect->CommitChanges();

					//メッシュの描画
					m_pMesh->DrawSubset(i);
				}

		m_pEffect->EndPass();
	}
}

//====================================
//関数名	:Draw
//引数		:pos	:位置
//			 rota	:回転
//			 scale	:大きさ
//			 color	:RGB値
//			 num	:テクニック番号
//戻り値	:
//内容		:RGBの値を変更できるXFILEの描画関数
//====================================

void CXfile::Draw(D3DXVECTOR3* pos, D3DXVECTOR3* rota, D3DXVECTOR3 *scale, D3DXCOLOR color, int num)
{
	//透明度が範囲内か調べる
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


	//インスタンスの取得
	CCamera* pCamera = CCamera::GetInstance();

	//３Dファイルの座標を定義する
	D3DXMATRIX *m_World = new D3DXMATRIX();
	D3DXMATRIX *m_temp = new D3DXMATRIX();
	//行列の初期化
	D3DXMatrixIdentity(m_World);
	D3DXMatrixIdentity(m_temp);
	//=====以下３Dの座標・拡大・移動の計算=====//
	//拡大の処理
	D3DXMatrixScaling(m_temp, scale->x, scale->y, scale->z);
	*m_World *= *m_temp;
	//モデルの座標変換(ヨー、ピッチ、ロールを指定して行列を作成)
	D3DXMatrixRotationYawPitchRoll(m_temp, rota->y, rota->x, rota->z);
	*m_World *= *m_temp;
	//モデル移動の処理
	D3DXMatrixTranslation(m_temp, pos->x, pos->y, pos->z);
	//全ての行列の演算結果格納
	*m_World *= *m_temp;

	//=====ここまで=====//

	//ワールド座標変換
	D3DXMATRIX matWorldInverse;

	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();
	D3DXMATRIX matVP = matView*matProjection;

	//左手座標系ビュー行列を作成(ライトからみた映像)
	D3DXMatrixLookAtLH(
		&matView,									//演算結果であるD3DXMATRIX構造体へのポインタを指定
		(D3DXVECTOR3*)&pCamera->GetLightPosition(),	//視点を定義したD3DXVECTOR3構造体へのポインタを指定
		(D3DXVECTOR3*)&pCamera->GetLookatPt(),		//カメラの注視対象を定義したD3DXVECTOR3構造体へのポインタを指定
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)				//カレントワールドの上方、一般には[0,1,0]を定義したD3DXVECTOR3構造体へのポインタを指定
		);
	//左手座標系のViewProj行列
	D3DXMATRIX matLVP = matView*matProjection;

	if (m_pEffect)
	{
		//テクニックの選択
		m_pEffect->SetTechnique("ShaderTechnique");
		//パスを指定
		m_pEffect->Begin(0, 0);
		//パスの番号を指定
		m_pEffect->BeginPass(num);

		//ワールド座標
		m_pEffect->SetMatrix("matWorld", m_World);

		//ワールド座標の逆行列を指定
		D3DXMatrixInverse(&matWorldInverse, NULL, m_World);
		m_pEffect->SetMatrix("matWorldInverse", &matWorldInverse);

		//投影変換を指定
		m_pEffect->SetMatrix("matVP", &matVP);
		m_pEffect->SetMatrix("matLVP", &matLVP);
		m_pEffect->SetBool("titleLight", true);
		//オブジェクト番号を指定
		//m_pEffect->SetInt("ObjectNumber", ObjectNumber);

		//環境光源を指定
		D3DXVECTOR4 vDirectionalLight = D3DXVECTOR4(pCamera->GetDirectionalLight(), 0.0f);


		m_pEffect->SetVector("LightDirection ", &vDirectionalLight);

		//カメラの位置を指定
		//D3DXVECTOR4 vViewFrom(pCamera->GetViewPosition(), 0.0f);
		//m_pEffect->SetVector("ViewFrom", &vViewFrom);


		for (DWORD i = 0; i < m_NumMaterials; i++)
		{
			//色をセット

			D3DXCOLOR v;
			v.r = color.r;// m_pMaterials[i].Diffuse.r;
			v.g = color.g;
			v.b = color.b;
			v.a = color.a;

			//頂点の色を指定
			m_pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&v);

			//テクスチャの有無を指定
			//m_pEffect->SetBool("TextureNone", m_pTextures ? FALSE : TRUE);

			//テクスチャを指定
			m_pEffect->SetTexture("Texture", m_pTextures[i]);

			//シェーダ設定をグラボに更新
			m_pEffect->CommitChanges();

			//メッシュの描画
			m_pMesh->DrawSubset(i);
		}
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	SAFE_DELETE(m_World);
	SAFE_DELETE(m_temp);
}
