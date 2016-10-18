// -------------------- IncludeFile -----------------
#include "AllocateHierarchy.h"
// ------------------------------------------------------------------------
//	クラス	:CAllocateHierarchy		フレームとメッシュコンテナの生成と破棄
//	ID3DXAllocateHierarchyからの派生クラス
// ------------------------------------------------------------------------
// フレームの生成時に呼び出される関数
HRESULT CAllocateHierarchy::CreateFrame(
	THIS_ LPCTSTR Name,LPD3DXFRAME* ppNewFrame)
{
	*ppNewFrame = new CFrame(Name);
	return S_OK;
}
// メッシュコンテナの生成時に呼び出される関数
HRESULT CAllocateHierarchy::CreateMeshContainer(
	THIS_ LPCTSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* /*pEffectInstances*/,
	DWORD NumMaterials, 
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	*ppNewMeshContainer = new CMeshContainer(
		Name,pMeshData,pMaterials,NumMaterials,
		pAdjacency,pSkinInfo,m_pd3dDevice,m_FullPath, m_pEffect);
	return S_OK;
}
// フレームの破棄時に呼び出される関数
HRESULT CAllocateHierarchy::DestroyFrame(
	THIS_ LPD3DXFRAME pFrameToFree)
{
	delete (CFrame*)pFrameToFree;
	return S_OK;
}
// メッシュコンテナの破棄時に呼び出される関数
HRESULT CAllocateHierarchy::DestroyMeshContainer(
	THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	delete (CMeshContainer*)pMeshContainerToFree;
	return S_OK;
}
// コンストラクタ
CAllocateHierarchy::CAllocateHierarchy(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXEFFECT pEffect)
{
	m_pd3dDevice = pd3dDevice;
	m_pEffect = pEffect;
}
// ------------------------------------------------------------------------
//	クラス	:CFrame					フレーム情報とメッシュコンテナ格納用
//	D3DXFRAMEからの派生クラス
// ------------------------------------------------------------------------
// フレームのコンストラクタ
CFrame::CFrame(LPCTSTR name)
{
	// 基本クラスの初期化
	ZeroMemory(this,sizeof(CFrame));
	// フレーム名の設定
	if(name) {
		UINT n = lstrlen(name) + 1;
		Name = new TCHAR[n];
		memcpy(Name,name,n * sizeof(TCHAR));
	}
	else Name = NULL;
	// 行列の単位化
	D3DXMatrixIdentity(&TransformationMatrix);
	D3DXMatrixIdentity(&CombinedTransMatrix);
}
// フレームのデストラクタ
CFrame::~CFrame()
{
	delete [] Name;
}
// フレームの描画
void CFrame::Draw()
{
	// フレームに属する全てのメッシュコンテナを描画
	for(CMeshContainer* mc = MeshContainer;mc;mc = mc->Next())
		MeshContainer->Draw(this);
	// 兄弟フレーム、子フレームの描画
	if(FrameSibling) FrameSibling->Draw();
	if(FrameChild) FrameChild->Draw();
}
// フレームの検索
CFrame* CFrame::Find(LPCTSTR name)
{
	// ルートフレームの子フレームの検索
	return (CFrame*)D3DXFrameFind(	this		// ルートフレームへのポインタ
									,name);		// 検索する子フレームの名前
}
// ボーン変形行列の初期化（フレーム）
void CFrame::InitBoneTransMatrices(CFrame* root)
{
	// メッシュコンテナ、兄弟フレーム、子フレームへのポインタの初期化
	MeshContainer = (CMeshContainer*)pMeshContainer;
	FrameSibling = (CFrame*)pFrameSibling;
	FrameChild = (CFrame*)pFrameFirstChild;
	// メッシュコンテナ、兄弟フレーム、子フレームに関するボーン変形行列の初期化
	if(MeshContainer) MeshContainer->InitBoneTransMatrices(root);
	if(FrameSibling) FrameSibling->InitBoneTransMatrices(root);
	if(FrameChild) FrameChild->InitBoneTransMatrices(root);
}
// 変形行列の更新
void CFrame::UpdateMatrices(LPD3DXMATRIX pParentMatrix)
{
	// このフレームに関する変形行列の更新
	if(pParentMatrix) {
		D3DXMatrixMultiply(
			&CombinedTransMatrix, 
			&TransformationMatrix,
			pParentMatrix);
	}
	else
		CombinedTransMatrix = TransformationMatrix;

	// 兄弟フレーム、子フレームに関する変形行列の更新
	if(FrameSibling) FrameSibling->UpdateMatrices(pParentMatrix);
	if(FrameChild) FrameChild->UpdateMatrices(&CombinedTransMatrix);
}
// ------------------------------------------------------------------------
//	クラス	:CMeshContainer			メッシュコンテナ格納用
//	D3DXMESHCONTAINERからの派生クラス
// ------------------------------------------------------------------------
// メッシュコンテナのコンストラクタ
CMeshContainer::CMeshContainer(
		THIS_ LPCTSTR name,
		CONST D3DXMESHDATA* meshdata,
		CONST D3DXMATERIAL* materials,
		CONST DWORD numMaterials,
		CONST DWORD* adjacency,
		LPD3DXSKININFO skin_info,
		LPDIRECT3DDEVICE9 pd3dDevice,
		LPCSTR pFullPath,
		LPD3DXEFFECT pEffect)
:	pOrigMesh(meshdata->pMesh),NumMaterials(numMaterials),
	pMaterials(NULL), ppTextures(NULL),
	SkinInfo(skin_info), NumBones(0), pBoneOffsetMatrices(0)
	, m_amxWorkingPalette(NULL), m_WorkingPaletteSize(0)
{
	// 基本クラスの初期化
	ZeroMemory(this,sizeof(D3DXMESHCONTAINER));
	m_pd3dDevice = pd3dDevice;
	m_pEffect = pEffect;
	m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);			// DX環境取得

	pBoneOffsetMatrices = NULL;			// オフセット行列
	ppBoneMatrixPtrs = NULL;			// トランスフォーム行列
	pBoneCombinationTable = NULL;		// ボーンの組み合わせテーブル

	// メッシュコンテナ名の設定
	if(name) {
		UINT n = lstrlen(name) + 1;
		Name = new TCHAR[n];
		memcpy(Name,name,n * sizeof(TCHAR));
	}
	else Name = NULL;

	// メッシュに法線ベクトルが存在するか確認
	if(!(pOrigMesh->GetFVF() & D3DFVF_NORMAL)) {
		// (FVF) コードを使ってメッシュのコピー
		pOrigMesh->CloneMeshFVF(pOrigMesh->GetOptions(),			// 作成オプションを指定(元メッシュのオプションを指定)
							pOrigMesh->GetFVF() | D3DFVF_NORMAL,	// FVFコードの組み合わせを指定(元メッシュのFVFに法線ベクトルを追加)
							m_pd3dDevice,							// デバイスを指定
							&pOrigMesh);							// コピーされたメッシュを指定
		// メッシュに含まれる各頂点の法線を計算
		D3DXComputeNormals(	pOrigMesh,	// メッシュへのポインタを指定
							NULL);		// 隣接面を指定(設定なし)
	}
	else pOrigMesh->AddRef();	// 参照カウントアップ

	// オブジェクトの数だけ、マテリアルとテクスチャー構造体メモリ確保
	pMaterials = new D3DXMATERIAL[NumMaterials];
	ppTextures = new LPDIRECT3DTEXTURE9[NumMaterials];
	for(DWORD i = 0;i < NumMaterials;i++) {	
		// マテリアル情報のコピー
		memcpy(&pMaterials[i],&materials[i],sizeof(D3DXMATERIAL));
		pMaterials[i].MatD3D = materials[i].MatD3D;
		FLOAT Length;
		D3DXVECTOR4 vLength;
		// ambientに値がない場合は、Emissiveに値あるか確認
		// ない場合は、diffuseを利用する。50%の値とする。
		memcpy(&vLength, &pMaterials[i].MatD3D.Ambient, sizeof(D3DXVECTOR4));
		vLength.w = 0.0f;
		Length = D3DXVec4Length(&vLength);
		if (!(Length > 0.0f))
		{
			memcpy(&vLength, &pMaterials[i].MatD3D.Emissive, sizeof(D3DXVECTOR4));
			vLength.w = 0.0f;
			Length = D3DXVec4Length(&vLength);
			if (Length > 0.0f)
				memcpy(&vLength, &pMaterials[i].MatD3D.Emissive, sizeof(D3DXVECTOR4));
			else
				memcpy(&vLength, &pMaterials[i].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			vLength *= 0.5f;
			memcpy(&pMaterials[i].MatD3D.Ambient, &vLength, sizeof(D3DXVECTOR4));
		}

		// specularに値があるか確認する。
		// ない場合は、10%の灰色を指定する。
		memcpy(&vLength, &pMaterials[i].MatD3D.Specular, sizeof(D3DXVECTOR4));
		vLength.w = 0.0f;
		Length = D3DXVec4Length(&vLength);
		if (Length > 0.0f)
			pMaterials[i].MatD3D.Specular.a = pMaterials[i].MatD3D.Power;
		else
		{
			if (pMaterials[i].MatD3D.Power > 0.0f)
			{
				vLength = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 0.0f);
				memcpy(&pMaterials[i].MatD3D.Specular, &vLength, sizeof(D3DXVECTOR4));
				pMaterials[i].MatD3D.Specular.a = pMaterials[i].MatD3D.Power;
			}
		}
		if (pMaterials[i].MatD3D.Power < 1.0f)
			pMaterials[i].MatD3D.Specular.a = 5.0f;

		// フォルダー確認
		if(materials[i].pTextureFilename == NULL) {
			ppTextures[i] = NULL;
			continue;
		}

		if(materials[i].pTextureFilename) {
			TCHAR FullPathName[MAX_PATH];
			if(strlen(pFullPath))
				strncpy_s(FullPathName,pFullPath,strlen(pFullPath) + 1);
			strcat_s(FullPathName,materials[i].pTextureFilename);
			// ファイルを基にしてテクスチャを作成
			if(FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,			// デバイスを指定
												FullPathName,			// ファイル名を指定
												&ppTextures[i])))		// IDirect3DTexture9インターフェイスへのポインタのアドレスを指定
				ppTextures[i] = NULL;
		}
		else
			ppTextures[i] = NULL;
	}

	// スキニング情報を読み込む
	if(SkinInfo) {
		SkinInfo->AddRef();	// 参照カウントアップ
		// ボーン変形行列、ボーンオフセット行列の作成
		NumBones = SkinInfo->GetNumBones();
		ppBoneMatrixPtrs = new D3DXMATRIX * [NumBones];
		pBoneOffsetMatrices = new D3DXMATRIX[NumBones];
		for(DWORD i = 0;i < NumBones;i++)
			pBoneOffsetMatrices[i] = *SkinInfo->GetBoneOffsetMatrix(i);

		//----- 0420追加 -----//
		UINT PaletteSize = 0;
		m_pEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&PaletteSize);
		NumPaletteEntries = min(PaletteSize, NumBones);

		// スキン対応のメッシュを作成する
		SkinInfo->ConvertToIndexedBlendedMesh(
			pOrigMesh,					// 入力メッシュ
			0,							// 現在は使われていない
			NumPaletteEntries,			// 行列パレットスキニングに使用可能なボーン行列の数
			adjacency,					// 入力メッシュの隣接性情報
			NULL,						// 出力メッシュの隣接性情報
			NULL,						// 各面の新インデックスを格納している転送先バッファへのポインタ
			NULL,						// 各頂点の新インデックスを格納する、ID3DXBufferインターフェイスへのポインタ
			&NumMaxInfls,				// 影響を受けるボーンの1面あたりの最大数を格納するDWORDへのポインタ
			&NumBoneCombinations,		// ボーンの組み合わせテーブルに含まれるボーンの数へのポインタ
			&pBoneCombinationTable,		// ボーンの組み合わせテーブルへのポインタ
			&pMesh);					// 新しいメッシュへのポインタ

		// ウエイト計算用行列を作成
		m_WorkingPaletteSize = 0;
		if (m_WorkingPaletteSize < NumPaletteEntries)
		{
			SAFE_DELETE_ARRAY(m_amxWorkingPalette);
			m_WorkingPaletteSize = NumPaletteEntries;
			m_amxWorkingPalette = new D3DXMATRIX[m_WorkingPaletteSize];
		}
		// D3DCOLORtoUBYTE4変換
		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9 * pDeclCur;
		pMesh->GetDeclaration(pDecl);
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}
		pMesh->UpdateSemantics(pDecl);
	}
	else {
		// (FVF)コードを使ってメッシュのコピー
		pOrigMesh->CloneMeshFVF(pOrigMesh->GetOptions(),// 作成オプションを指定(元メッシュのオプションを指定)
							pOrigMesh->GetFVF(),		// FVFコードの組み合わせを指定(元メッシュのFVFに法線ベクトルを追加)
							m_pd3dDevice,				// デバイスを指定
							&pMesh);					// コピーされたメッシュを指定
	}
	if(pOrigMesh) pOrigMesh->Release();
}
// メッシュコンテナのデストラクタ
CMeshContainer::~CMeshContainer()
{
	if(Name) delete [] Name;
	if(pMesh) pMesh->Release();
	if(pMaterials) delete [] pMaterials;
	if(ppTextures) {
		for(DWORD i = 0;i < NumMaterials;i++) {
			if(ppTextures[i]) ppTextures[i]->Release();
		}
		delete [] ppTextures;
	}
	if(SkinInfo) SkinInfo->Release();
	if(ppBoneMatrixPtrs) delete [] ppBoneMatrixPtrs;
	if(pBoneOffsetMatrices) delete [] pBoneOffsetMatrices;
	if(pBoneCombinationTable) pBoneCombinationTable->Release();
	if (m_amxWorkingPalette) delete[] m_amxWorkingPalette;
}
// メッシュコンテナの描画
void CMeshContainer::Draw(CFrame* frame)
{
	// スキニングを行う場合の描画
	if(SkinInfo) {
		UINT iAttrib;
		LPD3DXBONECOMBINATION pBoneComb;
		UINT iMatrixIndex;

		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pBoneCombinationTable->GetBufferPointer());
		for(iAttrib = 0;iAttrib < NumBoneCombinations;iAttrib++) {
			// ボーン行列パレットの設定
			for(DWORD i = 0;i < NumPaletteEntries;++i) {
				iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
				if(iMatrixIndex != UINT_MAX) {
					D3DXMatrixMultiply(&m_amxWorkingPalette[i],
						&pBoneOffsetMatrices[iMatrixIndex],
						ppBoneMatrixPtrs[iMatrixIndex]);
				}
			}

			// ボーンの数
			m_pEffect->SetInt("NumBones", NumMaxInfls - 1);
			// ボーンパレットを指定
			m_pEffect->SetMatrixArray("amPalette", m_amxWorkingPalette, NumPaletteEntries);

			// マテリアルを指定
			D3DXVECTOR4 vColor;
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialDiffuse", &vColor);
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialAmbient", &vColor);
			memcpy(&vColor, &pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Specular, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialSpecular", &vColor);

			// テクスチャーを指定
			if (ppTextures[pBoneComb[iAttrib].AttribId])
				m_pEffect->SetTexture("Texture", ppTextures[pBoneComb[iAttrib].AttribId]);
			// テクスチャの有無を設定
			if (ppTextures[pBoneComb[iAttrib].AttribId] == NULL)
				m_pEffect->SetBool("TextureNone", TRUE);
			else
				m_pEffect->SetBool("TextureNone", FALSE);

			m_pEffect->BeginPass(0);	// パスの番号を指定

			// シェーダ設定をグラボに更新
			m_pEffect->CommitChanges();
			// ポリゴンの描画
			pMesh->DrawSubset(iAttrib);

			m_pEffect->EndPass();
		}
	}
	else {	// スキニングを行わない場合の描画

		// 行列の設定
		D3DXMATRIX* pCombinedTrans = frame->GetCombinedTransMatrix();

		// マテリアル、テクスチャが一致するポリゴンでループ
		for (DWORD i = 0; i < NumMaterials; i++) {

			// 
			m_pEffect->SetMatrix("matCombinedTrans", pCombinedTrans);

			// マテリアルを指定
			D3DXVECTOR4 vColor;
			memcpy(&vColor, &pMaterials[i].MatD3D.Diffuse, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialDiffuse", &vColor);
			memcpy(&vColor, &pMaterials[i].MatD3D.Ambient, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialAmbient", &vColor);
			memcpy(&vColor, &pMaterials[i].MatD3D.Specular, sizeof(D3DXVECTOR4));
			m_pEffect->SetVector("MaterialSpecular", &vColor);

			// テクスチャーを指定
			if (ppTextures[i]!=NULL)
				m_pEffect->SetTexture("Texture", ppTextures[i]);
			// テクスチャの有無を設定
			if (ppTextures[i] == NULL)
				m_pEffect->SetBool("TextureNone", TRUE);
			else
				m_pEffect->SetBool("TextureNone", FALSE);

			m_pEffect->BeginPass(1);	// パスの番号を指定

			// シェーダ設定をグラボに更新
			m_pEffect->CommitChanges();

			pMesh->DrawSubset(i);

			m_pEffect->EndPass();
		}

	}
}
// ボーン変形行列の初期化（メッシュコンテナ）
void CMeshContainer::InitBoneTransMatrices(CFrame* root)
{
	if(SkinInfo) {
		// 全てのボーンについて、対応するフレームを探す
		for(DWORD i = 0;i < NumBones;i++) {
			// ボーンインデックスからボーンの名前を取得
			CFrame* pFrame = root->Find(SkinInfo->GetBoneName(i));
			// 対応するフレームの合成済み変形行列をボーンから参照する
			if(pFrame)
				ppBoneMatrixPtrs[i] = pFrame->GetCombinedTransMatrix();
		}
	}
}
// ------------------------------------------------------------------------
//	クラス	:CAnimMesh			アニメーション対応メッシュ
// ------------------------------------------------------------------------
// コンストラクタ
CAnimMesh::CAnimMesh(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwTimer, LPD3DXEFFECT pEffect)
{
	m_pd3dDevice = pd3dDevice;
	m_dwTimer = dwTimer;
	AllocateHierarchy = new CAllocateHierarchy(pd3dDevice, pEffect);
	AnimController = NULL;
	FrameRoot = NULL;
}
// デストラクタ
CAnimMesh::~CAnimMesh()
{
	// アニメーションコントローラの開放
	if(AnimController)
		AnimController->Release();
	if(FrameRoot) {
		// フレームの開放
		D3DXFrameDestroy(FrameRoot,AllocateHierarchy);
		FrameRoot = NULL;
	}
	delete AllocateHierarchy;
}
// ファイルのロード
BOOL CAnimMesh::LoadFromFile(LPCSTR pLoadFile)
{
	// フルパスを指定
	TCHAR FullPathName[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,FullPathName);
	strcat_s(FullPathName,"\\");
	strcat_s(FullPathName,pLoadFile);
	DWORD i;
	for(i = (DWORD)strlen(FullPathName);i > 0;i--) {
		if(FullPathName[i] == '\\'){
			FullPathName[i + 1] = '\0';
			break;
		}
	}
	if(i == 0)
		FullPathName[0] = '\0';
	strncpy_s(FullPath,FullPathName,strlen(FullPathName) + 1);
	AllocateHierarchy->setFullPath(FullPath);
	// 仮アニメーションコントローラ
	LPD3DXANIMATIONCONTROLLER pAnimController;
	// メッシュのロード
	LPD3DXFRAME frame_root;
	// 最初のフレーム階層を.xファイルからロード
	if(FAILED(D3DXLoadMeshHierarchyFromX(
		pLoadFile,			// ファイル名を指定する文字列へのポインタ
		D3DXMESH_MANAGED,	// メッシュの作成オプションを指定(頂点/インデックス バッファをシステムメモリ管理下に指定)
		m_pd3dDevice,		// デバイスを指定
		AllocateHierarchy,	// ID3DXAllocateHierarchyインターフェイスへのポインタ
		NULL,				// ユーザーデータのロードを許可するインターフェイス
		&frame_root,		// ロードされるフレームルートへのポインタを指定
		&pAnimController))) return FALSE;	// アニメーションコントローラへのポインタを指定
	// アニメーションセット数を取得
	DWORD NumAnimSet = pAnimController->GetNumAnimationSets();
	// ミキサのトラックの最大数を取得
	DWORD MaxNumTracks = pAnimController->GetMaxNumTracks();
	if(MaxNumTracks < NumAnimSet) MaxNumTracks = NumAnimSet;
	// アニメーションコントローラのコピーを作成
	if(FAILED(pAnimController->CloneAnimationController(
		pAnimController->GetMaxNumAnimationOutputs(),	// サポート可能なアニメーション出力の最大数を指定
		pAnimController->GetMaxNumAnimationSets(),	// サポート可能なアニメーションセットの最大数を指定
		MaxNumTracks,								// サポート可能なトラックの最大数を指定
		pAnimController->GetMaxNumEvents(),			// サポート可能なイベントの最大数を指定
		&AnimController))) return FALSE;			// コピーされたアニメーションコントローラへのポインタを指定
	LPD3DXANIMATIONSET pAnimSet = NULL;
	// アニメーションセットを取得
	AnimController->GetAnimationSet(0				// アニメーションセットのインデックスを指定
									,&pAnimSet);	// 返されたアニメーションセットへのポインタを指定
	// Xファイルのフレーム数をAnimTicksPerSecond(１秒間の分割係数)で割った値
	double PeriodTime = pAnimSet->GetPeriod();
	SetAnimFinishTime((float)PeriodTime);
	float LoopTime = 1.0f;	// 1秒ループ
	// トラック速度を設定
	AnimController->SetTrackSpeed(0,(float)PeriodTime / LoopTime);

	// アニメーションセット開放
	if(pAnimSet)
		pAnimSet->Release();
	// 仮アニメーションコントローラの開放
	if(pAnimController)
		pAnimController->Release();
	// アニメーションセットが複数ある場合
	if(NumAnimSet > 1) {
		// トラックに全てのアニメーションセットを読み込む
		for(DWORD i = 1; i < NumAnimSet;i++) {
			D3DXTRACK_DESC Desc = {D3DXPRIORITY_HIGH,1.0f,1.0f,0,FALSE};
			// {未使用,重みの値,速度の値,ローカルアニメーションタイム,トラック(ミキサ利用)の有効/無効}
			// トラックの記述を設定
			AnimController->SetTrackDesc(	i,		// 変更するトラック
											&Desc);	// トラックの記述
			LPD3DXANIMATIONSET pAnimSet = NULL;
			// アニメーションセットを取得
			AnimController->GetAnimationSet(i				// アニメーションセットのインデックスを指定
											,&pAnimSet);	// 返されたアニメーションセットへのポインタを指定
			PeriodTime = pAnimSet->GetPeriod();
			
			// トラック速度を設定
			AnimController->SetTrackSpeed(i,(float)PeriodTime / LoopTime);
			// 指定したトラックにアニメーション セット
			AnimController->SetTrackAnimationSet(	i,			// アニメーションセットを追加しているトラックを指定
													pAnimSet);	// トラックに追加するアニメーションセットを指定
			// アニメーションセット開放
			if(pAnimSet)
				pAnimSet->Release();
		}
	}
	CurrentAnimSetID = 0;				// 実行、アニメーション番号
	FrameRoot = (CFrame*)frame_root;	// フレームルートを設定
	// ボーン変形行列の設定
	FrameRoot->InitBoneTransMatrices(FrameRoot);

	return TRUE;
}
// アニメーション変更
void CAnimMesh::ChangeAnimation(DWORD AnimSetID) {
	// 変更するアニメーション番号がアニメーションセット数を超えない
	if(AnimSetID >= AnimController->GetNumAnimationSets())
		return;
	// 今、実行されているアニメーションか確認
	if(CurrentAnimSetID == AnimSetID)
		return;

	AnimController->SetTrackPosition(CurrentAnimSetID,0.0f);
	AnimController->SetTrackEnable(CurrentAnimSetID,FALSE);
	CurrentAnimSetID = AnimSetID;
	AnimController->SetTrackPosition(CurrentAnimSetID,0.0f);
	AnimController->SetTrackEnable(CurrentAnimSetID,TRUE);
	AnimController->ResetTime();
}
// 時間の取得
double CAnimMesh::GetTime()
{
	if (AnimController)
	{
		return AnimController->GetTime();
	}
	return 0.0;
}

// 時間の設定
void CAnimMesh::SetTime(DOUBLE Time)
{
	if(!AnimController)
		return;
	AnimController->AdvanceTime(Time,NULL);
}
void CAnimMesh::ResetTime()
{
	AnimController->ResetTime();
}
// 変換行列を指定して描画
void CAnimMesh::Draw()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	FrameRoot->UpdateMatrices(&matWorld);
	FrameRoot->Draw();
}


LPD3DXANIMATIONCONTROLLER CAnimMesh::GetAnimController() {

	LPD3DXANIMATIONCONTROLLER tmp;

	//アニメーションコントローラーのヘルパー関数　ディープコピーを行う
	if (S_OK == AnimController->CloneAnimationController(AnimController->GetMaxNumAnimationOutputs(),
		AnimController->GetMaxNumAnimationSets(),
		AnimController->GetMaxNumTracks(),
		AnimController->GetMaxNumEvents(),
		&tmp))
		return tmp;

	return nullptr;

}