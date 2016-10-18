#pragma once
//=====Include File=====//
#include"../Global/Global.h"
#include"../Lib/Sound/SoundManager.h"

#define SOUNDMAX	14
//=====構造体宣言=====//
struct AnimationNum
{
	UINT numU;
	UINT numV;
};
//=====クラス宣言=====//
class CDSystem
{
	//privateメンバ変数
private:
	//板ポリ２D用
	struct Vertex {
		float x, y, z;// 3次元座標
		float rhw;	// 2D変換済みフラグ
		float u, v;	// UV座標
	};

	//
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	//テクスチャ
	LPDIRECT3DTEXTURE9	m_pTexture;
	//描画デバイス
	LPDIRECT3DDEVICE9	m_pDevice3D;
	//シェーダ
	LPD3DXEFFECT		m_pEffect;
	//頂点宣言作成
	LPDIRECT3DVERTEXDECLARATION9 decl;

	//サウンド関連
	CSoundManager*		m_pSoundManager;

	// 読み込むウェーブファイルの数
	DWORD				m_dwLoadIWaveFileNum;

	char SoundFileName[SOUNDMAX][MAX_PATH] =
	{
		//BGM
		"Music/BGM/title.wav",		//0 タイトルBGM
		"Music/BGM/game.wav",		//1 ゲームBGM
		"Music/BGM/gameover.wav",	//2 
		"Music/BGM/result.wav",		//3 
		//SE
		"Music/SE/y_kettei.wav",	//4 
		"Music/SE/y_sentaku.wav",	//5 
		"Music/SE/agaru.wav",		//6 水から上がる
		"Music/SE/baburu.wav",		//7 シャボン玉発射時
		"Music/SE/bomb.wav",		//8 シャボン玉破裂音
		"Music/SE/count.wav",		//9 カウントダウン用
		"Music/SE/nageru.wav",		//10 ボール投げる瞬間
		"Music/SE/oyogu.wav",		//11 泳ぎ中
		"Music/SE/slider.wav",		//12 スライダー滑り中
		"Music/SE/suberioti.wav"	//13 スライダー落ち際
	};
	//publicメンバ変数
public:
	//板ポリ関連
	typedef struct _BoardVertex_
	{
		//頂点の座標
		D3DXVECTOR4 vPosition;
		//ディフューズ
		D3DXVECTOR4 vColor;
		//テクスチャUV値
		FLOAT		tu, tv;
	}BOARDVERTEX;

	int width;
	int height;

	float w;
	float h;

	D3DXVECTOR2 pos;
	//UVの分割数
	UINT divU;
	UINT divV;
	//UVの番号
	UINT numU,numV;

	//privateメンバ関数
private:


	
	//publicメンバ関数
public:
	//コンストラクタ
	CDSystem(LPDIRECT3DDEVICE9);
	//デストラクタ
	~CDSystem();
	BOOL CreateBorad();
	BOOL CreateResource();
	BOOL CreateSoundManager(HWND);
	BOOL LoadTexture(const TCHAR*);
	//BOOL PointLoadTexture(const TCHAR*);
	void Draw(D3DXVECTOR3,UINT,FLOAT,D3DXVECTOR3);
	void Draw(bool);
	void Draw();
	//=====セット関数=====//
	void SetDivede(UINT DivU, UINT DivV)
	{
		if (DivU <= 0 || DivV <= 0)
		{
			return;
		}
		divU = DivU;
		divV = DivV;
	}

	void SetUVNum(UINT NumU,UINT NumV)
	{
		if (NumU >= divU)
			return;
		if (NumV >= divV)
			return;

		numU = NumU;
		numV = NumV;
	}
	void SetWidth(int Width, int Height)
	{
		width = Width;
		height = Height;
	}

	void Setpos(D3DXVECTOR2 vPos)
	{
		pos = vPos;
	}
	//=====ゲット関数=====//
	LPDIRECT3DTEXTURE9 getTexture() 
	{
		return m_pTexture;
	};

	CSoundManager* GetSoundManager() 
	{ 
		return m_pSoundManager; 
	}

	LPD3DXEFFECT GetEffect()
	{
		return m_pEffect; 
	}
};