// GLOBALS //
#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 26
#endif
int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x3 amPalette[MATRIX_PALETTE_SIZE_DEFAULT]: WORLDMATRIXARRAY;

float4x4		matWorld;			// ワールド座標行列
float4x4		matView;			// ビュー行列
float4x4		matProjection;		// プロジェクション行列
float4x4		matCombinedTrans;	// 合成済み行列

float4			MaterialDiffuse;	// diffuse
float4			MaterialAmbient;	// ambient
float4			MaterialSpecular;	// specular

texture			Texture;			// テクスチャ
bool			TextureNone = true;	// テクスチャの有無(true:無し false:テクスチャ有り)

int				NumBones;			// ボーン数(影響を受けるボーンの数)(ループする数かな)

float4			DirectionalLight;	// 環境光源
float4			ViewPosition;		// カメラの位置

/*池原付け足し*/
// スライダー用変数
bool setSlider = false;
float uv_left;
float uv_top;
float uv_width;
float uv_height;
/*以上*/

// Light //
float4 LightDiffuse = { 1.0, 1.0, 1.0, 0.0 }; // diffuse
float4 LightAmbient = { 1.0, 1.0, 1.0, 0.0 }; // ambient
float4 LightSpecular = { 1.0, 1.0, 1.0, 0.0 }; // specular

// TYPEDEFS //
sampler TextureSampler = sampler_state
{
	Texture = < Texture >; // <こ↑こ↓に入る変数>は上の変数と同じ
	MipFilter = NONE; // みんなのアイドルミミロップ(迫真)
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT_SKIN
{
	float4 Position			: POSITION;		// 頂点
	float3 Normal			: NORMAL;		// 法線
	float2 TextureUV		: TEXCOORD0;	// テクスチャUV値
	float3 BlendWeights		: BLENDWEIGHT;  // ウェイト
	float4 BlendIndices		: BLENDINDICES; // インデックス
};

struct VS_INPUT_XFILE
{
	float4 Position			: POSITION;		// 頂点
	float3 Normal			: NORMAL;		// 法線
	float2 TextureUV		: TEXCOORD0;	// テクスチャUV値
};
struct VS_OUTPUT
{
	float4 Position			: POSITION;		// 頂点
	float2 TextureUV		: TEXCOORD0;
	float3 DirectionToView	: TEXCOORD1;	// 視点座標系におけるカメラ方向ベクトル
	float3 DirectionToLight	: TEXCOORD2;	// 視点座標系におけるライト方向ベクトル
	float3 Normal			: TEXCOORD3;	// 視点座標系における法線ベクトル
	float3 ReflectVector	: TEXCOORD4;	// 視点座標系における反射ベクトル
};

//
// 第一引数:ストリーム cppから引数としてもらってくる
// 第二引数:シェーダー内で引数もらってくる
//
VS_OUTPUT VS_SKIN(VS_INPUT_SKIN Input, uniform int NumBones)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	//
	// 頂点と法線のブレンド計算
	//
	float3 BlendPosition = 0.0;
	float3 BlendNormal = 0.0;
	float3 Normal = normalize(Input.Normal);
	float LastWeight = 0.0;
	float Weight;

	float afBlendWeights[3] = (float[3]) Input.BlendWeights;
	int aIndices[4] = (int[4]) D3DCOLORtoUBYTE4(Input.BlendIndices);

	for (int Bone = 0; (Bone < 3) && (Bone < NumBones - 1); Bone++)
	{
		LastWeight += afBlendWeights[Bone];
		BlendPosition += mul(Input.Position, amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
		BlendNormal += mul(float4(Normal, 0.0), amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
	}
	LastWeight = 1.0 - LastWeight;
	BlendPosition += mul(Input.Position, amPalette[aIndices[NumBones - 1]]) * LastWeight;
	BlendNormal += mul(float4(Normal, 0.0), amPalette[aIndices[NumBones - 1]]) * LastWeight;
	
	//
	// ワールド座標系に変換
	//
	float4 WorldPosition = float4(BlendPosition, 1.0);
	WorldPosition = mul(WorldPosition, matWorld);
	
	//
	// ビュー座標系に変換
	//
	Output.Position = mul(WorldPosition, matView);
	// プロジェクション座標系に変換
	Output.Position = mul(Output.Position, matProjection);

	// 視点座標系における法線ベクトル
	float3 WorldNormal = mul(Input.Normal, (float3x3)matCombinedTrans);
	WorldNormal = mul(WorldNormal, (float3x3)matWorld);
	Output.Normal = normalize(mul(float4(WorldNormal, 0.0), matView).xyz);

	// ライト方向ベクトル
	float4 DirectionToLight = -DirectionalLight;
	// 視点座標系におけるライト方向ベクトル
	Output.DirectionToLight = normalize(mul(DirectionToLight, matView).xyz);

	// カメラ方向ベクトル
	float4 DirectionToViewPosition = ViewPosition - WorldPosition;
	// 視点座標系におけるカメラ方向ベクトル
	Output.DirectionToView = normalize(mul(DirectionToViewPosition, matView).xyz);

	/// 反射ベクトル
	float3 ReflectVector = -DirectionToViewPosition.xyz + 2.0
		* max(0.0, dot(WorldNormal, DirectionToViewPosition.xyz)) * WorldNormal;

	// テクスチャUV値
	Output.TextureUV = Input.TextureUV;
	return Output;
}

VS_OUTPUT VS_XFILE(VS_INPUT_XFILE Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ワールド座標系に変換
	float4 WorldPosition = mul(Input.Position, matCombinedTrans);
	WorldPosition = mul(WorldPosition, matWorld);
	// ビュー座標系に変換
	Output.Position = mul(WorldPosition, matView);
	// プロジェクション座標系に変換
	Output.Position = mul(Output.Position, matProjection);

	// 視点座標系における法線ベクトル
	float3 WorldNormal = mul(Input.Normal, (float3x3)matCombinedTrans);
	WorldNormal = mul(WorldNormal, (float3x3)matWorld);
	Output.Normal = normalize(mul(float4(WorldNormal, 0.0), matView).xyz);

	// ライト方向ベクトル
	float4 DirectionToLight = -DirectionalLight;
	// 視点座標系におけるライト方向ベクトル
	Output.DirectionToLight = normalize(mul(DirectionToLight, matView).xyz);

	// カメラ方向ベクトル
	float4 DirectionToViewPosition = ViewPosition - WorldPosition;
	// 視点座標系におけるカメラ方向ベクトル
	Output.DirectionToView = normalize(mul(DirectionToViewPosition, matView).xyz);

	/// 反射ベクトル
	float3 ReflectVector = -DirectionToViewPosition.xyz + 2.0
		* max(0.0, dot(WorldNormal, DirectionToViewPosition.xyz)) * WorldNormal;

	// テクスチャUV値
	Output.TextureUV = Input.TextureUV;

	return Output;
}

float4 PS_XFILE(VS_OUTPUT Input) : COLOR0
{
	float4 Color;

	// 拡散反射光(ハーフランバート)
	float DiffuseAngle = dot(Input.Normal, Input.DirectionToLight);
	// ハーフランバートに変更
	float HalfLambert = DiffuseAngle * 0.5 + 0.5;
	HalfLambert = clamp(HalfLambert * HalfLambert, 0.0, 1.0);
	float4 Diffuse;
	if (!TextureNone)
		Diffuse = LightDiffuse * MaterialDiffuse * HalfLambert;
	else
		Diffuse = LightDiffuse * MaterialDiffuse * DiffuseAngle;

		// 環境光
		float4 Ambient = LightAmbient * MaterialAmbient;

		// 反射光
		float3 HalfVector = normalize(Input.DirectionToView + Input.DirectionToLight);
		float3 Specular = LightSpecular.rbg * saturate(pow(max(0.0, dot(HalfVector, Input.Normal)), MaterialSpecular.a) * MaterialSpecular.rbg);

		Color = Diffuse + Ambient + float4(Specular, 0.0);
	if (!TextureNone)
		Color *= tex2D(TextureSampler, Input.TextureUV);
	Color += float4(Specular, 0.0);
	Color.a = MaterialDiffuse.a;
	Color = saturate(Color);

	return Color;
}

VertexShader vsArray[4] =
{
	compile vs_2_0 VS_SKIN(1),
	compile vs_2_0 VS_SKIN(2),
	compile vs_2_0 VS_SKIN(3),
	compile vs_2_0 VS_SKIN(4)
};

technique ShaderTechnique
{
	pass P0
	{
		VertexShader = ( vsArray[ NumBones ]);
		PixelShader = compile ps_2_0 PS_XFILE();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader = compile ps_2_0 PS_XFILE();
	}
}