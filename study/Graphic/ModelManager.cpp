#include "ModelManager.h"

//唯一のインスタンス生成
CModelManager*	CModelManager::m_pModelInstance = NULL;


//====================================
//関数名	:CXfileManager
//引数		:
//戻り値	:
//内容		:コンストラクタ
//====================================
CModelManager::CModelManager(LPDIRECT3DDEVICE9 pDevice9)
{
	//描画デバイスのコピー
	m_pDevice9 = pDevice9;
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		m_pXFile[i] = NULL;
	}
}

//====================================
//関数名	:~CXfileManager
//引数		:
//戻り値	:
//内容		:デストラクタ
//====================================
CModelManager::~CModelManager()
{

}

//====================================
//関数名	:Create
//引数		:pDevice:描画デバイス
//戻り値	:
//内容		:Managerのインスタンスが存在しないなら作る
//====================================
void CModelManager::Create(LPDIRECT3DDEVICE9 pDevice9)
{
	//XFileManagerのインスタンスが存在しないなら
	if (!m_pModelInstance)
	{
		m_pModelInstance = new CModelManager(pDevice9);
	}
}

//====================================
//関数名	:Destory
//引数		:
//戻り値	:
//内容		:インスタンスの消去
//====================================
void CModelManager::Destory()
{
	SAFE_DELETE(m_pModelInstance);
}

//====================================
//関数名	:Delete
//引数		:
//戻り値	:
//内容		:XFIleのデリート
//====================================
void CModelManager::Delete()
{
	//XFILEの削除
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		SAFE_DELETE(m_pXFile[i]);
	}
}

//====================================
//関数名	:AllLoadXfile
//引数		:
//戻り値	:
//内容		:全てのXFileの一括読み込み
//====================================
BOOL CModelManager::AllLoadXfile()
{
	for (int i = 0; i < MAXFILE_NUM; i++)
	{
		m_pXFile[i] = new CXfile(m_pDevice9);
	}

	//全てのXFileのロード
	 for (int i = 0; i < MAXFILE_NUM; i++)
	 {	 
		 if (FAILED(m_pXFile[i]->LoatXfile(XFileName[i])))
		 {
			 OutputDebugString("XFile Load Error");
			 return FALSE;
		 }
	 }
	return TRUE;
}

//====================================
//関数名	:GetXFileName
//引数		:num:モデル番号
//戻り値	:
//内容		:読み込んでいるXFILEの名前を取得する
//====================================
CHAR* CModelManager::GetXFileName(DWORD num)
{
	return XFileName[num];
}

//====================================
//関数名	:Draw
//引数		:	num		:XFile描画番号
//				pos		:位置
//				rot		:回転
//				sacle	:大きさ
//				t_Num	:テクニック番号
//				alpha	:透明度
//戻り値	:
//内容		:指定されたXFileの描画(透明度のみ変更可能)
//====================================
void CModelManager::Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scale, float alpha)
{
	if (num == 7)
	{
		m_pXFile[num]->SetSlider(TRUE);
	}
	else if (num == 1)
	{
		m_pXFile[num]->Setpool(TRUE);
	}
	else {
		m_pXFile[num]->SetSlider(FALSE);
		m_pXFile[num]->Setpool(FALSE);
	}
	m_pXFile[num]->Draw(pos, rot,scale, alpha);
}

//====================================
//関数名	:Draw
//引数		:	num		:XFile描画番号
//				pos		:位置
//				rot		:回転
//				sacle	:大きさ
//				color	:RGB色の指定
//				t_Num	:テクニック番号
//戻り値	:
//内容		:指定されたXFileの描画(color変更可)
//====================================
//void CModelManager::Draw(DWORD num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale,D3DXCOLOR color, int t_Num)
//{
//	m_pXFile[num]->Draw(&pos, &rot, &scale,color,t_Num);
//}

LPD3DXMESH CModelManager::GetMesh(DWORD num) 
{
	return m_pXFile[num]->GetMesh();
}

