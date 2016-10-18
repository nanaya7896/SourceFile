#pragma once
//=====Include File=====//
#include"../../Global/Global.h"

//=====クラス宣言=====//
//派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	//オフセット行列
	D3DXMATRIX OffsetMatrix;
	//行列テーブルのインデックス番号
	DWORD	   OffsetID;
};
//派生メッシュコンテナー構造体(
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;								//重みの個数（重みとは頂点への影響。）
	DWORD dwBoneNum;							//ボーンの数
	LPD3DXBUFFER pBoneBuffer;					//ボーン・テーブル
	D3DXMATRIX** ppBoneMatrix;					//全てのボーンのワールド行列の先頭ポインター
	D3DXMATRIX* pBoneOffsetMatrices;			//フレームとしてのボーンのワールド行列のポインター
	LPD3DXMESH	pOriMesh;						//オリジナルメッシュ用
	DWORD		NumPaletteEntries;				//パレットサイズ

	//Work用
	//std::vector<D3DXMATRIX>		m_WorkBoneMatArray;
	//影響するフレームへの参照配列。描画時にこのフレームの行列を使う
	std::vector<MYFRAME*>		BoneFrameArray;

	MYMESHCONTAINER()
	{
		ppBoneMatrix = NULL;
		pBoneOffsetMatrices = NULL;
	}
};


//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	//コピーコンストラクタ
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//スキンメッシュクラス
class CSKIN_MESH
{
private:
	//すべてのフレームポインタ格納処理関数
	VOID CreateFrameArray(LPD3DXFRAME _pFrame);
	//対象のボーンを検索
	MYFRAME* SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame);

	//フレーム参照配列
	//全フレーム参照配列
	std::vector<MYFRAME*>		m_FrameArray;
	//メッシュコンテナありのフレーム参照配列
	std::vector<MYFRAME*>		m_IntoMeshFrameArray;
	//ボーン情報
	LPD3DXFRAME					m_pFrameRoot;
	//アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	//ヒエラルキークラス変数
	MY_HIERARCHY				m_cHierarchy;
	//アニメーションデータ格納用変数
	LPD3DXANIMATIONSET			m_pAnimSet[20];
	//現在のアニメーションが開始されてからの時間(1/60)
	DWORD						m_AnimTime;
	//アニメーションスピード
	FLOAT						m_AnimSpeed;
	//現在のアニメーショントラック
	DWORD						m_CurrentTrack;
	//現在のアニメーションデータトラック
	D3DXTRACK_DESC				m_CurrentTrackDesc;
	//ワールド座標
	D3DXMATRIX					m_World;
	//メッシュのマテリアル関係
	//マテリアル変更フラグ
	BOOL						m_MaterialFlag;
	//マテリアルデータ
	D3DMATERIAL9				m_Material;

public:
	//コンストラクタ
	CSKIN_MESH();
	//デストラクタ
	~CSKIN_MESH();

	//スキンメッシュ内部処理
	HRESULT InitThing(LPDIRECT3DDEVICE9, LPSTR);
	HRESULT AllocateBoneMatrix(LPD3DXFRAME, LPD3DXMESHCONTAINER);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME,LPD3DXFRAME);
	VOID RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER*, MYFRAME*);
	VOID UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);
	VOID DrawFrame(LPDIRECT3DDEVICE9, LPD3DXFRAME);
	//フレーム解放
	VOID FreeAnim(LPD3DXFRAME);
	//解放処理
	VOID Release();
	//更新処理
	VOID Update(D3DXMATRIX);
	//描画処理
	VOID Draw(LPDIRECT3DDEVICE9);
	//オブジェクトのアニメーション変更（メッシュオブジェクトの操作用番号、変更するアニメーション番号）
	VOID ChangeAnim(DWORD NewAnimnum);


	//======セット関数======//
	//アニメーションの速度を設定
	VOID SetAnimSpeed(FLOAT _AnimSpeed)
	{
		m_AnimSpeed = _AnimSpeed;
	}


	//======ゲット関数=====//
	DWORD GetAnimTrack()
	{
		return m_AnimTime;
	}
	FLOAT GetAnimSpeed()
	{
		return m_AnimSpeed;
	}
	//ボーンのマトリックス取得（ボーンの名前）
	D3DXMATRIX GetBoneMatrix(LPSTR _BoneName);
	//ボーンのマトリックスポインタを取得(ボーンの名前)
	D3DXMATRIX* GetPointBoneMatrix(LPSTR _BoneName);
};


