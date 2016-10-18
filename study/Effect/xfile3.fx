float4x4	matWorld;				// ワールド座標
float4x4	matWorldInverse;		// ワールド座標の逆行列
float4x4	matVP;					// 射影空間への座標変換
float4x4	matLVP;					// ライトから見た射影空間への座標変換
texture		Texture;				// テクスチャー
texture		ShadowBuffer;			// シャドウバッファー
bool		TextureNone = true;		// テクスチャーの有無(true:無し)	
int			ObjectNumber;			// オブジェクトの番号

float4		MaterialDiffuse;		// 色

// 光の強さ
float4 Light_Diffuse = { 1.0f,1.0f,1.0f,0.0f };	// diffuse
float4 Light_Ambient = { 1.0f,1.0f,1.0f,0.0f };	// ambient
float4 Light_Specular= { 0.4f,0.4f,0.4f,0.0f }; // specular
// 半球ライティング
float4 SkyBlue	= { 0.53f,0.81f,1.0f,0.0f };
float4 Sienna	= { 0.63f,0.32f,0.18f,0.0f };
float4		DirectionalLight;		// 環境光源
float4		ViewFrom;				// カメラの位置

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

struct VS_INPUT_XFILE
{
	float4	Position	:	POSITION;	// 頂点の位置
	float3  Normal		:	NORMAL;		// 頂点の法線
	float2	TextureUV	:	TEXCOORD0;	// テクスチャーUV値
};

struct VS_OUTPUT_XFILE
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

VS_OUTPUT_XFILE VS_XFILE( VS_INPUT_XFILE Input )
{
    VS_OUTPUT_XFILE Output = (VS_OUTPUT_XFILE)0;
	
	// ワールド座標系
	float4 WorldPosition = mul(Input.Position,matWorld);
	float4 WorldNormal = float4(Input.Normal,0.0);
    // 頂点座標変換
	Output.Position = mul(WorldPosition,matVP);

	// テクスチャーUV値を出力
	Output.TextureUV = Input.TextureUV;

	// 頂点の色の決定
	// ライト方向ベクトル
	float4 DirectionToLight = normalize(mul(normalize(-DirectionalLight),matWorldInverse));
	// カメラ方向ベクトル
	float4 DirectionToViewFrom = normalize(ViewFrom - WorldPosition);

	float4 Diffuse = Light_Diffuse * MaterialDiffuse * max(0,dot(
		DirectionToLight.xyz,WorldNormal.xyz));		 // 拡散反射光
	float4 Ambient = Light_Ambient * MaterialDiffuse;// 環境光
	Output.Specular = Light_Specular * pow(max(0,dot(DirectionToLight.xyz,normalize(WorldNormal.xyz))),10);
	float HemiSphereValue = (dot(normalize(WorldNormal), float4(0.0f,1.0f,0.0f,0.0f)) + 1.0f) * 0.5f;
	float4 HemiSphere = lerp( Sienna, SkyBlue, HemiSphereValue ) * 0.2f;

	Output.Color = saturate(Diffuse + Ambient + HemiSphere);

	Output.Color.a = 1.0f;

	// 光源からみた頂点の位置の計算
	Output.Light = mul(mul(Input.Position,matWorld),matLVP);

    return Output;
}

float4 PS_XFILE( VS_OUTPUT_XFILE Input ) : COLOR0
{
    float4 Color;

    if (TextureNone)
    {
        Color = Input.Color + Input.Specular;
    }
    else
    {
        Color = Input.Color * tex2D(TextureSampler, Input.TextureUV) + Input.Specular;
    }

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
    Color.a = 1.0f;
    return Color;
}

float4 PS_XFILE2( VS_OUTPUT_XFILE Input ) : COLOR0
{
    float4 Color;

    if (TextureNone)
    {
        Color = Input.Color + Input.Specular;
    }
    else
    {
        Color = Input.Color * tex2D(TextureSampler, Input.TextureUV) + Input.Specular;
    }

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
	//Color = float4(Color.xyz+Light_Ambient.xyz * Light_Ambient.w, //Color.w);
	Color.a=0.7f;
    return Color;
}

// ----- シャドウマップ関連 -----
SHADOWMAP_VS_OUTPUT ShadowMapVS(SHADOWMAP_VS_OUTPUT Input)
{
	SHADOWMAP_VS_OUTPUT Output = (SHADOWMAP_VS_OUTPUT)0;	// 出力データ

	// 座標変換
	float4 WorldPosition = mul(Input.Position,matWorld);
	Output.Position = mul(Output.Position,matLVP);
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

technique ShaderTechnique
{
	pass P0
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;
		CULLMODE=NONE;
		VertexShader = compile vs_2_0 VS_XFILE();
		pixelShader  = compile ps_2_0 PS_XFILE();
	}
	pass P1
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = FALSE;

		VertexShader = compile vs_2_0 ShadowMapVS();
		pixelShader  = compile ps_2_0 ShadowMapPS();
	}
	pass P2
	{
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		ALPHABLENDENABLE = TRUE;
		
		VertexShader = compile vs_2_0 VS_XFILE();
		pixelShader  = compile ps_2_0 PS_XFILE2();	
	}
}

