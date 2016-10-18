#pragma once
// -------------------- IncludeFile -----------------
#include "../../Global/Global.h"				// 共通ヘッダー
// ------------------- クラス宣言 -------------------
class CMeshContainer;
// ------------------------------------------------------------------------
//	クラス	:CAllocateHierarchy		フレームとメッシュコンテナの読み込み
//	ID3DXAllocateHierarchyからの派生クラス
// ------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy {
private:
	STDMETHOD(CreateFrame)(	THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(	THIS_ LPCSTR Name,
									CONST D3DXMESHDATA* pMeshData,
									CONST D3DXMATERIAL* pMaterials,
									CONST D3DXEFFECTINSTANCE* pEffectInstances,
									DWORD NumMaterials, 
									CONST DWORD* pAdjacency,
									LPD3DXSKININFO pSkinInfo, 
									LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// 描画デバイス
	TCHAR					m_FullPath[MAX_PATH];	// XFile迄のフルパス
	LPD3DXEFFECT			m_pEffect;				// エフェクト
public:
	CAllocateHierarchy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXEFFECT pEffect);
	void setFullPath(LPCSTR pFullPath) { memcpy(m_FullPath,pFullPath,strlen(pFullPath) + 1); }
};
// ------------------------------------------------------------------------
//	クラス	:CFrame					トランスフォームフレームのカプセル化
//	D3DXFRAMEからの派生クラス
// ------------------------------------------------------------------------
class CFrame : public D3DXFRAME {
private:
	CMeshContainer*			MeshContainer;			// メッシュコンテナ
	CFrame*					FrameSibling;			// 兄弟フレーム
	CFrame*					FrameChild;				// 子フレーム
	D3DXMATRIXA16			CombinedTransMatrix;	// 合成済み変形行列
public:
	// コンストラクタ，デストラクタ
	CFrame(LPCTSTR name);
	~CFrame();
	// 行列の取得
	LPD3DXMATRIXA16 GetCombinedTransMatrix() {
		return &CombinedTransMatrix;
	}
	// フレームの検索
	CFrame* Find(LPCTSTR name);
	// ボーン変形行列の初期化
	void InitBoneTransMatrices(CFrame* root);
	// 変形行列の更新
	void UpdateMatrices(LPD3DXMATRIX pParentMatrix);
	// 描画
	void Draw();
};
// ------------------------------------------------------------------------
//	クラス	:CMeshContainer			メッシュコンテナ格納用
//	D3DXMESHCONTAINERからの派生クラス
// ------------------------------------------------------------------------
class CMeshContainer : public D3DXMESHCONTAINER {
private:
	LPD3DXMESH				pMesh;					// メッシュ
	LPD3DXMESH				pOrigMesh;				// メッシュ(オリジナル)
	DWORD					NumMaterials;			// マテリアルの数
	LPD3DXMATERIAL			pMaterials;				// マテリアル
	LPDIRECT3DTEXTURE9*		ppTextures;				// テクスチャ
	LPD3DXSKININFO			SkinInfo;				// スキン情報
	DWORD					NumBones;				// ボーンの数
	LPD3DXMATRIX			pBoneOffsetMatrices;	// オフセット行列
	LPD3DXMATRIX*			ppBoneMatrixPtrs;		// トランスフォーム行列
    DWORD					NumPaletteEntries;		// ボーン行列パレットの数(Indexed用)
	DWORD					NumMaxInfls;			// 影響を受けるボーンの1面あたりの最大数
	DWORD					NumBoneCombinations;	// ボーンの組み合わせテーブルに含まれるボーンの数
	LPD3DXBUFFER			pBoneCombinationTable;	// ボーンの組み合わせテーブルへのポインタ
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// 描画デバイス
	D3DCAPS9				m_d3dCaps;				// DX環境
	LPD3DXEFFECT			m_pEffect;				// エフェクト
	DWORD					m_WorkingPaletteSize;	// 合成結果格納用行列の最大数
	D3DXMATRIX*				m_amxWorkingPalette;	// 合成結果格納用行列
public:
	// コンストラクタ
	CMeshContainer(
		THIS_ LPCTSTR Name,
		CONST D3DXMESHDATA* meshdata,
		CONST D3DXMATERIAL* pMaterials,
		CONST DWORD numMaterials,
		CONST DWORD* adjacency,
		LPD3DXSKININFO skin_info,
		LPDIRECT3DDEVICE9,
		LPCSTR,
		LPD3DXEFFECT
	);
	// デストラクタ
	~CMeshContainer();
	// 次のメッシュコンテナを取得
	CMeshContainer* Next() {
		return (CMeshContainer*)pNextMeshContainer;
	}
	// ボーン変形行列の初期化
	void InitBoneTransMatrices(CFrame* root);
	// 描画
	void Draw(CFrame* frame);


};
// ------------------------------------------------------------------------
//	クラス	:CAnimMesh				アニメーション対応メッシュ
// ------------------------------------------------------------------------
class CAnimMesh {
private:
	CAllocateHierarchy*		AllocateHierarchy;		// ファイル読み込み用のオブジェクト
	CFrame*					FrameRoot;				// ルートのフレーム
	LPD3DXANIMATIONCONTROLLER AnimController;		// アニメーションコントローラ
	DWORD					CurrentAnimSetID;		// 実行中のアニメーション番号
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// 描画デバイス
	DWORD					m_dwTimer;				// ゲームスピード


	/*池原追加*/
	FLOAT m_AnimFinishTime=0.0f;
public:
	// コンストラクタ，デストラクタ
	CAnimMesh(LPDIRECT3DDEVICE9, DWORD, LPD3DXEFFECT);
	~CAnimMesh();
	// ファイルの読み込み
	BOOL LoadFromFile(LPCSTR);
	// アニメーション
	DOUBLE GetTime();
	void SetTime(DOUBLE Time);
	void SetAnimSet(DWORD Track,DWORD AnimationSet);
	void ChangeAnimation(DWORD AnimSetID);

	/*池原追加*/
	void SetAnimFinishTime(FLOAT time)
	{
		m_AnimFinishTime = time;
	};
	FLOAT GetAnimFinishTime()
	{
		return m_AnimFinishTime;
	}
	void ResetTime();
	/*ここまで*/
	// フレームの取得
	CFrame* GetFrameRoot() {return FrameRoot;}
	// 描画
	void Draw();

	LPD3DXANIMATIONCONTROLLER GetAnimController();
};