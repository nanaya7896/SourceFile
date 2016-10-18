// ----- 共通部分 ------
float4x4 matWorld;			// ワールド座標
float4x4 matWorldInverse;	// ワールド座標の逆行列
float4x4 matScale;			// スケーリング
float4x4 matVP;				// ローカルから射影空間への座標変換
float4x4 matLVP;			// ライトから見た射影空間への座標変換
float4x4 matCombinedTrans;	// 合成済み行列
texture	Texture;			// テクスチャー
texture	ShadowBuffer;		// シャドウバッファー
float4 vViewFrom;			// カメラの位置
int ObjectNumber;			// オブジェクトの番号
// ----- アニメーション関連 ------
float4 MaterialDiffuse;		// 色
float4 MaterialAmbient;		// 色
float4 MaterialSpecular;	// 色
float MaterialSpecularPower = 32;	// Power
int NumBones;				// ボーン数

// 光の強さ
float4 Light_Diffuse = { 1.0f,1.0f,1.0f,0.0f };	// diffuse
float4 Light_Ambient = { 1.0f,1.0f,1.0f,0.0f };	// ambient
float4 Light_Specular ={ 0.4f,0.4f,0.4f,0.0f }; // specular
// 半球ライティング
float4 SkyBlue	= { 0.53f,0.81f,1.0f,0.0f };
float4 Sienna	= { 0.63f,0.32f,0.18f,0.0f };
float4 DirectionalLight;		// 環境光源

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 26
#endif
const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x3 amPalette[ MATRIX_PALETTE_SIZE_DEFAULT ]: WORLDMATRIXARRAY;

sampler TextureSampler =
sampler_state
{
	Texture = <Texture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler ShadowBufferSampler = 
sampler_state
{
	Texture = <ShadowBuffer>;
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = BORDER;
	AddressV = BORDER;
};

// ----- 構造体の定義 -----
struct VS_OUTPUT
{
	float4	Position	:	POSITION;	// 頂点の位置
	float4	Color		:	COLOR0;		// 頂点の色
	float4	Specular	:	COLOR1;		// スペキュラー色
	float2	TextureUV	:	TEXCOORD0;	// テクスチャーUV値
	float4	Light		:	TEXCOORD1;	// ライトの位置
};

struct SHADOWMAP_VS_OUTPUT
{
	float4	Position	:	POSITION;	// 頂点の位置
	float2	Depth		:	TEXCOORD0;	// ライトからの距離
};

VS_OUTPUT VS_Skin(
	float4 vPosition	:POSITION, 
	float3 vNormal		:NORMAL,
	float2 TextureUV	:TEXCOORD0,
	float3 BlendWeights	:BLENDWEIGHT,
	float4 BlendIndices	:BLENDINDICES,uniform int NumBones)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// 頂点と法線のブレンド計算	
	float3 Position = 0;
	float3 Normal = 0;
	float LastWeight = 0.0f;
	float Weight;
	float afBlendWeights[ 3 ] = (float[ 3 ]) BlendWeights;
	int aIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( BlendIndices );

    for ( int Bone = 0; (Bone < 3) && (Bone < NumBones - 1) ; ++Bone )
    {
        LastWeight += afBlendWeights[Bone];
        Position += mul(vPosition, amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
        Normal += mul(vNormal, amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
    }
	LastWeight = 1.0f - LastWeight;
	Position += mul(vPosition, amPalette[aIndices[NumBones - 1]]) * LastWeight;
	Normal += mul(vNormal, amPalette[aIndices[NumBones - 1]]) * LastWeight;

	// ワールド座標系
	float4 WorldPosition = mul(float4(Position,1.0f),matWorld);
	float4 WorldNormal = float4(Normal,0.0f);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matVP);

	// テクスチャーUV値を出力
	Output.TextureUV = TextureUV;

	// 頂点の色の決定
	// ライト方向ベクトル
	float4 DirectionToLight = normalize(mul(normalize(-DirectionalLight),matWorldInverse));
	// カメラ方向ベクトル
	float4 DirectionToViewFrom = normalize(vViewFrom - WorldPosition);

	float4 Diffuse = Light_Diffuse * MaterialDiffuse * max(0,dot(
		DirectionToLight.xyz,normalize(WorldNormal.xyz)));		 // 拡散反射光
	float4 Ambient = Light_Ambient * MaterialAmbient;// 環境光
	Output.Specular = Light_Specular * pow(max(0,dot(DirectionToLight.xyz,normalize(WorldNormal.xyz))),10);
	float HemiSphereValue = (dot(normalize(WorldNormal), float4(0.0f,1.0f,0.0f,0.0f)) + 1.0f) * 0.5f;
	float4 HemiSphere = lerp( Sienna, SkyBlue, HemiSphereValue ) * 0.2f;

	Output.Color = saturate(Diffuse + Ambient + HemiSphere);
	Output.Color.a = 1.0f;

	// 光源からみた頂点の位置の計算
	Output.Light = mul(WorldPosition,matLVP);

	return Output;
}


SHADOWMAP_VS_OUTPUT ShadowMap_VS_Skin(
	float4 vPosition	:POSITION, 
	float3 vNormal		:NORMAL,
	float2 TextureUV	:TEXCOORD0,
	float3 BlendWeights	:BLENDWEIGHT,
	float4 BlendIndices	:BLENDINDICES,uniform int NumBones)
{
	SHADOWMAP_VS_OUTPUT Output = (SHADOWMAP_VS_OUTPUT)0;

	// 頂点のブレンド計算	
	float3 Position = 0;
	float LastWeight = 0.0f;
	float Weight;
	float afBlendWeights[ 3 ] = (float[ 3 ]) BlendWeights;
	int aIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( BlendIndices );

	for ( int Bone = 0; (Bone < 3) && (Bone < NumBones - 1) ; ++Bone )
	{
		LastWeight += afBlendWeights[Bone];
		Position += mul(vPosition, amPalette[aIndices[Bone]]) * afBlendWeights[Bone];
	}
	LastWeight = 1.0f - LastWeight;
	Position += mul(vPosition, amPalette[aIndices[NumBones - 1]]) * LastWeight;

	// ワールド座標系
	float4 WorldPosition = mul(float4(Position,1.0f),matWorld);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matLVP);
	// 位置情報の出力
	Output.Depth.xy = Output.Position.zw;

	return Output;
}

VS_OUTPUT VS_XFILE(
	float4 Position		:POSITION, 
	float3 Normal		:NORMAL,
	float2 TextureUV	:TEXCOORD0)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ワールド座標系
	float4 WorldPosition = mul(Position,matCombinedTrans);
	WorldPosition = mul(WorldPosition,matWorld);
	float4 WorldNormal = mul(float4(Normal,0.0f),matCombinedTrans);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matVP);

	// テクスチャーUV値を出力
	Output.TextureUV = TextureUV;

	// 頂点の色の決定
	// ライト方向ベクトル
	float4 DirectionToLight = normalize(mul(normalize(-DirectionalLight),matWorldInverse));
	// カメラ方向ベクトル
	float4 DirectionToViewFrom = normalize(vViewFrom - WorldPosition);

	float4 Diffuse = Light_Diffuse * MaterialDiffuse * max(0,dot(
		DirectionToLight.xyz,WorldNormal.xyz));		 // 拡散反射光
	float4 Ambient = Light_Ambient * MaterialAmbient;// 環境光
	Output.Specular = Light_Specular * pow(max(0,dot(DirectionToLight.xyz,normalize(WorldNormal.xyz))),10);
	float HemiSphereValue = (dot(normalize(WorldNormal), float4(0.0f,1.0f,0.0f,0.0f)) + 1.0f) * 0.5f;
	float4 HemiSphere = lerp( Sienna, SkyBlue, HemiSphereValue ) * 0.2f;

	Output.Color = saturate(Diffuse + Ambient + HemiSphere);
	Output.Color.a = 1.0f;

	// 光源からみた頂点の位置の計算
	Output.Light = mul(WorldPosition,matLVP);

	return Output;
}

SHADOWMAP_VS_OUTPUT ShadowMap_VS_XFILE(
	float4 Position		:POSITION, 
	float3 Normal		:NORMAL,
	float2 TextureUV	:TEXCOORD0)
{
	SHADOWMAP_VS_OUTPUT Output = (SHADOWMAP_VS_OUTPUT)0;

	// ワールド座標系
	float4 WorldPosition = mul(Position,matCombinedTrans);
	WorldPosition = mul(WorldPosition,matWorld);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matLVP);
	// 位置情報の出力
	Output.Depth.xy = Output.Position.zw;

	return Output;
}

float4 PS(VS_OUTPUT Input,uniform bool Tex)	:	COLOR0
{
    float4 Color;

	if (!Tex)
		Color = Input.Color + Input.Specular;
	else
		Color = Input.Color * tex2D(TextureSampler,Input.TextureUV) + Input.Specular;

	// シャドウマップから値の取得
	float2 ShadowUV = 0.5f * Input.Light.xy 
				/ Input.Light.w + float2(0.5f,0.5f);
	ShadowUV.y = 1.0f - ShadowUV.y;
	ShadowUV.x = ShadowUV.x + 0.5f / 1280.0f;
	ShadowUV.y = ShadowUV.y + 0.5f / 720.0f;

	float ShadowValue = tex2D(ShadowBufferSampler, ShadowUV).r;

	// 光源から頂点までの距離を計算
	float Depth = Input.Light.z / Input.Light.w;

	// シャドウマップの深度の値と比較
	// 影になっている
	if(Depth - ShadowValue > 0.000065f)
	{
		// 影になっている
		if (ObjectNumber == int(tex2D(ShadowBufferSampler, ShadowUV).g) && int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
		{
			Color = Color * 0.95f;
			Color.a = 1.0f;
		}
		else
		{
			if (int(tex2D(ShadowBufferSampler, ShadowUV).g) != 0)
			{
				Color = Color * 0.5f;
				Color.a = 1.0f;
			}
		}
	}

    return Color;
}

SHADOWMAP_VS_OUTPUT ShadowMapVS(SHADOWMAP_VS_OUTPUT Input)
{
	SHADOWMAP_VS_OUTPUT Output = (SHADOWMAP_VS_OUTPUT)0;	// 出力データ

	// ワールド座標系
	float4 WorldPosition = mul(Input.Position,matCombinedTrans);
	WorldPosition = mul(WorldPosition,matWorld);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matLVP);

	// 位置情報の出力
	Output.Depth.xy = Output.Position.zw;

    return Output;
}

float4 ShadowMapPS(SHADOWMAP_VS_OUTPUT Input) :	COLOR
{
	// ピクセルの深度情報を計算して出力(0～1に正規化)
	float4 Color = float4(0,0,0,0);
	Color.r = Input.Depth.x / Input.Depth.y;
	Color.g = float(ObjectNumber);

	return Color;
}

VertexShader vsArray[ 4 ] =	
{	compile vs_2_0 VS_Skin( 1 ),
	compile vs_2_0 VS_Skin( 2 ),
	compile vs_2_0 VS_Skin( 3 ),
	compile vs_2_0 VS_Skin( 4 )
};

VertexShader ShadowMapvsArray[ 4 ] =
{	compile vs_2_0 ShadowMap_VS_Skin( 1 ),
	compile vs_2_0 ShadowMap_VS_Skin( 2 ),
	compile vs_2_0 ShadowMap_VS_Skin( 3 ),
	compile vs_2_0 ShadowMap_VS_Skin( 4 )
};

technique ShaderTechnique
{
	pass P0
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = ( vsArray[ NumBones ] );
		PixelShader  = compile ps_2_0 PS(true);
	}
	pass P1
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = ( vsArray[ NumBones ] );
		PixelShader  = compile ps_2_0 PS(false);
	}
	pass P2
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader  = compile ps_2_0 PS(true);
	}
	pass P3
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader  = compile ps_2_0 PS(false);
	}
	pass P4
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		CULLMODE = NONE;

		VertexShader = ( ShadowMapvsArray[ NumBones ] );
		pixelShader  = compile ps_2_0 ShadowMapPS();
	}
	pass P5
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		CULLMODE = NONE;

		VertexShader = compile vs_2_0 ShadowMapVS();
		pixelShader  = compile ps_2_0 ShadowMapPS();
	}
}