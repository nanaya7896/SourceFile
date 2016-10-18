#include "PointSprite.h"


DWORD FloatToDWORD(float value)
{
	return *((DWORD*)&value);
}

//====================================
//関数名	:CPointSprite
//引数		:
//戻り値	:
//内容		:
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
//関数名	:~CPointSprite
//引数		:
//戻り値	:
//内容		:
//====================================
CPointSprite::~CPointSprite()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pPointBuffer);
	//SAFE_RELEASE(m_pTexture);
}

BOOL CPointSprite::PointLoadTexture(const TCHAR *FileName)
{
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
	strcat_s(FullPathName, FileName);


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

	//テクスチャの読み込み関数
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
	//======シェーダーの作成=====//
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
//関数名	:InitPointSprite
//引数		:
//戻り値	:
//内容		:
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
		//バッファの初期化に失敗
		return FALSE;
	}
	D3DXMatrixIdentity(&m_World);
	
	return TRUE;
}

//====================================
//関数名	:DrawPointSprite
//引数		:
//戻り値	:
//内容		:
//====================================
void CPointSprite::DrawPointSprite(Vertex3 point[], int numPointSprite, bool /*alpha*/)
{
	Vertex3 *vertex;

	//インスタンスの取得
	CCamera* pCamera = CCamera::GetInstance();

	D3DXMATRIX matView = pCamera->getView();
	D3DXMATRIX matProjection = pCamera->getProj();

	m_pPointBuffer->Lock(0, 0, (LPVOID*)&vertex, 0);
	for (int i = 0; i < numPointSprite; i++)
	{
		// ポイントスプライトの位置の計算
		vertex[i].pos = point[i].pos;

		// ポイントスプライトのサイズを設定
		vertex[i].size = point[i].size;

		// ポイントスプライトの色を設定
		vertex[i].color = point[i].color;
	}
	m_pPointBuffer->Unlock();

	if (m_pEffect)
	{
		m_pEffect->SetTechnique("tec");
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);

		//色をセット
		D3DXCOLOR v;
		v.r = 1.0f;/*ColorChange;*/
		v.g = 1.0f;
		v.b = 1.0f;
		v.a = 0.5f;
		//頂点の色を指定
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