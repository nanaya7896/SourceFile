#include "DSystem.h"

//柔軟な頂点構造体宣言
#define SPRITE_FVF  (D3DFVF_XYZ | D3DFVF_TEX1 |D3DFVF_DIFFUSE)



//====================================
//関数名	:CDSystem
//引数		:pDevice:描画デバイス
//			 hWnd	:ウィンドウハンドル
//			 FileName:ファイル名
//戻り値	:
//内容		:コンストラクタ
//====================================
CDSystem::CDSystem(LPDIRECT3DDEVICE9 pDevice)
{
	//ウィンドウの大きさを取得
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
//関数名	:~CDSystem
//引数		:
//戻り値	:
//内容		:デストラクタ
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
//関数名	:CreateResource
//引数		:hWnd:ウィンドウハンドル
//戻り値	:
//内容		:リソースの作成
//====================================
BOOL CDSystem::CreateResource()
{


	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	//テクスチャ用シェーダーの読み込み
	hr = D3DXCreateEffectFromFile(
		m_pDevice3D,			//デバイス
		"Effect\\sprite.fx",	//ファイル名
		NULL,					//プリプロセッサ定義へのポインタ
		NULL,					//オプションのインターフェイスポインタ
		0,						//コンパイルオプション
		NULL,					//エフェクトプールオブジェクトへのポインタ
		&m_pEffect,				//エフェクトオブジェクトへのポインタ
		&pErrorMsgs				//エラー及び警告リストを含むバッファ
		);

	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->GetBufferPointer(), "エフェクトファイルの読み込みに失敗しました", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	return TRUE;
}

BOOL CDSystem::CreateSoundManager(HWND hWnd)
{
	
	m_pSoundManager = new CSoundManager(hWnd);

	//サウンドマネージャーの作成
	if (!m_pSoundManager->GetDirectSound()->Create(hWnd))
	{
		return FALSE;
	}
	//サウンドの読み込み
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
//関数名	:LoadTexture
//引数		:FileName:ファイル名
//戻り値	:
//内容		:テクスチャを読み込む関数
//====================================
BOOL CDSystem::LoadTexture(const TCHAR* FileName)
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
		if (D3DXCreateTextureFromFile(
			m_pDevice3D,	//描画デバイス
			FileName,		//ファイル名
			&m_pTexture		//テクスチャへのポインタ
			))
		{
			//読み込みに失敗
			return FALSE;
		}
	

		////シェーダに頂点のフォーマット情報を伝えるためにD3DVERTEXELEMENT9を作成
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
			D3DDECL_END()				//終端記号(必須)
		};
		//頂点シェーダーの作成
		m_pDevice3D->CreateVertexDeclaration(elems, &decl);

	return TRUE;
}

//====================================
//関数名	:Draw
//引数		:vPos	:画像の位置を指定
//			 num	:使用するテクニックを指定
//					 0:そのまま
//					 1:乗算
//			 fAlpha	:透明度
//			 posSize:画像のサイズを指定
//戻り値	:
//内容		:描画
//====================================
void CDSystem::Draw(D3DXVECTOR3 vPos,UINT num,FLOAT fAlpha, D3DXVECTOR3 posSize)
{
	BOARDVERTEX vertex[4];

	//画像の透明度の設定が規定値以内か判別
	if (fAlpha < 0.0f) {
		fAlpha = 0.0f;
	}
	if (fAlpha > 1.0f) {
		fAlpha = 1.0f;
	}
	
	//カラーとポジションの初期化
	for (DWORD i = 0; i < 4; i++)
	{
		//カラーの初期化
		vertex[i].vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, fAlpha);
		//Z軸を０にする
		vertex[i].vPosition.z =0;
		//値wを1.0に指定
		vertex[i].vPosition.w = 1.0f;
	}

	
	//頂点の設定
	vertex[0].vPosition.x = vertex[3].vPosition.x = vPos.x;
	vertex[1].vPosition.x = vertex[2].vPosition.x = vPos.x + posSize.x;//+テクスチャのサイズ;
	vertex[0].vPosition.y = vertex[1].vPosition.y = vPos.y;
	vertex[2].vPosition.y = vertex[3].vPosition.y = vPos.y + posSize.y;	
	//UV値の設定
	vertex[0].tu = vertex[3].tu = 0.0f;
	vertex[1].tu = vertex[2].tu = 1.0f;
	vertex[0].tv = vertex[1].tv = 0.0f;
	vertex[2].tv = vertex[3].tv = 1.0f;


	D3DVIEWPORT9 vp;
	//画面サイズを取得
	m_pDevice3D->GetViewport(&vp);

	//情報をシェーダーで利用するために変換
	for (DWORD i = 0; i < 4; i++)
	{
		vertex[i].vPosition.x -= (FLOAT)vp.Width / 2.0f;
		vertex[i].vPosition.y = (FLOAT)vp.Height - vertex[i].vPosition.y;
		vertex[i].vPosition.y -= (FLOAT)vp.Height / 2.0f;
	}


	D3DXMATRIX matWorld,matProjection;
	//行列を初期化する
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matProjection);

	//左手座標系正射影行列を作成
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
	//射形変換
	//matProjection._11 = 2 / h;
	//matProjection._22 = -2 / w;
	////射形変換
	////D3DXMATRIX proj(
	////	2/h, 0.0f, 0.0f, 0.0f,
	////	0.0f, -2/w, 0.0f, 0.0f,
	////	0.0f, 0.0f, 1.0f, 0.0f,
	////	0.0f, 0.0f, 0.0f, 1.0f
	////	);
	//
	////=====移動=====
	////ｘ軸が横にずれる
	//matWorld._41 = vPos.x;
	////y軸がずれる
	//matWorld._42 = vPos.y;
	//=====移動終了=====
	//変換移動行列を行うための行列(原点を左上へ)
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
	////合成射影変換行列
	////初期位置を左上にする
	//matProjection *= offset;
	//m_pDevice3D->SetTransform(D3DTS_PROJECTION, &matProjection);

	if (m_pEffect) 
	{
		//頂点フォーマット宣言を設定
		m_pDevice3D->SetVertexDeclaration(decl);
		//複数のテクニックを使う場合があるのでどのテクニックを使うか指定
		m_pEffect->SetTechnique("Tech");
		//第一引数に指定のtechnique内に含まれるパスの数が返ってくる
		//シェーダはパスを連続で呼び出すことで複数回点を穿つことができるためパスの番号を取得する必要がある
		//
		m_pEffect->Begin(0, 0);
		if (num == 0) {
			//P0を使用
			m_pEffect->BeginPass(0);
		}
		else if(num==1){
			//P1を使用
			m_pEffect->BeginPass(1);
		}
		
		//行列の設定
		m_pEffect->SetMatrix("matWorld", &matWorld);
		m_pEffect->SetMatrix("matVP", &matVP);
		//テクスチャの設定
		m_pEffect->SetTexture("m_pTexture",m_pTexture);
		//シェーダーに送る値(今回はUV値)
		//m_pEffect->SetFloat("uv_left", 0);
		//m_pEffect->SetFloat("uv_top", 0);
		//m_pEffect->SetFloat("uv_width", 0.55f);
		//m_pEffect->SetFloat("uv_height", 0.55f);
		//m_pDevice3D->SetVertexDeclaration(decl);

		//グラボの性能をチェック
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
	//画面サイズを取得
	m_pDevice3D->GetViewport(&vp);

	D3DXMATRIX matWorld, matProjection;
	//行列を初期化する
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixIdentity(&matProjection);

	//左手座標系正射影行列を作成
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
		//頂点フォーマット宣言を設定
		m_pDevice3D->SetVertexDeclaration(decl);
		//複数のテクニックを使う場合があるのでどのテクニックを使うか指定
		m_pEffect->SetTechnique("Tech");
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);
		//行列の設定
		m_pEffect->SetMatrix("matWorld", &matWorld);
		m_pEffect->SetMatrix("matVP", &matVP);
		//テクスチャの設定
		m_pEffect->SetTexture("m_pTexture", m_pTexture);

		m_pDevice3D->SetFVF(SPRITE_FVF);


		m_pDevice3D->SetStreamSource(0, m_pVB, 0, sizeof(BOARDVERTEX));
		m_pDevice3D->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		m_pEffect->EndPass();
		m_pEffect->End();
	}


}
