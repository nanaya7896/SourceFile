#include "YukiAnimation.h"

// コンストラクタ
YukiAnimation::YukiAnimation(LPDIRECT3DDEVICE9 pd3dDevice, char* pFileName)
{
	d3d_device9_ = pd3dDevice;

	animation_ = nullptr;
	effect_ = nullptr;
	position_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	angle_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(1.2f, 1.2f, 1.2f);
	animation_time_ = 0.025f;

	Initialize(pFileName);
}
// デストラクタ
YukiAnimation::~YukiAnimation()
{
	SAFE_DELETE(animation_);
	SAFE_RELEASE(effect_);
}

// 実行
void YukiAnimation::Update()
{
	animation_->SetTime(animation_time_);
}
// 描画
void YukiAnimation::Draw()
{
	D3DXMATRIX matWorld, matPosition, matRotation, matView, matProjection, matScaling;

	CCamera* pCamera = CCamera::GetInstance();
	matView = pCamera->getView();
	matProjection = pCamera->getProj();
	D3DXVECTOR4 vViewPosition(pCamera->GetViewPosition(), 1.0f);
	D3DXVECTOR4 vDirectionalLight(pCamera->GetDirectionalLight(), 1.0f);

	D3DXMatrixTranslation(
		&matPosition,	// 演算結果の行列へのアドレス
		position_.x,	// X座標のオフセット
		position_.y,	// X座標のオフセット
		position_.z);	// X座標のオフセット
						// ヨー、ピッチ、ロールを指定して行列を作成
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,	// 演算結果の行列へのアドレス
		angle_.y,		// Y軸を中心とするヨー(ラジアン単位)
		angle_.x,		// X軸を中心とするピッチ(ラジアン単位) 
		angle_.z);	// Z軸を中心とするロール(ラジアン単位)


	matWorld = matRotation * matPosition;

	if (effect_)
	{
		// テクニックの選択
		effect_->SetTechnique("ShaderTechnique");

		// びぎん
		effect_->Begin(0, 0); // パスを指定
	
							  //スケール変換
		D3DXMatrixScaling(&matScaling, scale_.x, scale_.y, scale_.z);

								// 平行移動行列を作成
		D3DXMatrixTranslation(&matPosition, position_.x, position_.y, position_.z);
		// ヨー、ピッチ、ロールを指定して行列を作成
		D3DXMatrixRotationYawPitchRoll(
			&matRotation,	// 演算結果の行列へのアドレス
			angle_.y,		// Y軸を中心とするヨー(ラジアン単位)
			angle_.x,		// X軸を中心とするピッチ(ラジアン単位) 
			angle_.z);	// Z軸を中心とするロール(ラジアン単位)

		matWorld = matScaling * matRotation * matPosition;

		// ワールド行列を指定
		// 第一引数は絶対シェーダーのほうで宣言したやつと一緒にする
		// やらなきゃきゅんきゅんぱわー
		effect_->SetMatrix("matWorld", &matWorld);

		// ビュー行列を指定
		effect_->SetMatrix("matView", &matView);

		// プロジェクション行列を指定
		effect_->SetMatrix("matProjection", &matProjection);

		// カメラの位置を指定
		effect_->SetVector("ViewPosition", &vViewPosition);

		// ライト方向を指定
		effect_->SetVector("DirectionalLight", &vDirectionalLight);

		// シェーダ設定をグラボに更新
		effect_->CommitChanges();

		animation_->Draw();

		effect_->EndPass();
	}
}

// 初期化処理
BOOL YukiAnimation::Initialize(char* pFileName)
{
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;

	// シェーダの読み込み
	hr = D3DXCreateEffectFromFile(
		d3d_device9_,	// デバイス
		"Effect\\YukiAnimation.fx",	// ファイル名
		NULL,			// プリプロセッサ定義へのポインタ
		NULL,			// オプションのインターフェイスポインタ 
		0,				// コンパイルオプション
		NULL,			// エフェクトプールオブジェクトへのポインタ
		&effect_,		// エフェクトオブジェクトへのポインタ (デバイス死んだらともに死ぬお)
		&pErrorMsgs);	// エラーおよび警告のリストを含むバッファ
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE(pErrorMsgs);
		return FALSE;
	}

	DWORD Time = 20; // 20ミリ秒
	animation_ = new CAnimMesh(d3d_device9_, Time, effect_);

	animation_->LoadFromFile(pFileName);

	return TRUE;
}