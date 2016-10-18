// GLOBALS //
float4x4		matWorld;			// ワールド座標行列
float4x4		matView;			// ビュー行列
float4x4		matProjection;		// プロジェクション行列

float4			MaterialDiffuse;	// diffuse
float4			MaterialAmbient;	// ambient
float4			MaterialSpecular;	// specular

texture			Texture;			// テクスチャ
bool			TextureNone = true;	// テクスチャの有無(true:なし)

// スライダー用変数
bool slider = false;
float uv_left;
float uv_top;
float uv_width;
float uv_height;
//透明度
float           alpha;              // 透過
float4			DirectionalLight;	// 環境光源
float4			ViewPosition;		// カメラの位置

// Light //
float4 LightDiffuse  = { 1.0, 1.0, 1.0, 0.0 }; // diffuse
float4 LightAmbient  = { 1.0, 1.0, 1.0, 0.0 }; // ambient
float4 LightSpecular = { 1.0, 1.0, 1.0, 0.0 }; // specular

// TYPEDEFS //
sampler TextureSampler =
sampler_state
{
	Texture = < Texture > ;			// <>内の名前は上でtextureで定義した名前と同じにする
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT
{
	float4 Position			: POSITION;		// 頂点
	float3 Normal			: NORMAL;		// 法線
	float2 TextureUV		: TEXCOORD0;	// テクスチャUV値
};
struct VS_OUTPUT
{
	float4 Position			: POSITION;		// 頂点	
	float2 TextureUV		: TEXCOORD0;	// テクスチャUV値
	float3 DirectionToView	: TEXCOORD1;	// 視点座標系におけるカメラ方向ベクトル
	float3 DirectionToLight	: TEXCOORD2;	// 視点座標系におけるライト方向ベクトル
	float3 Normal			: TEXCOORD3;	// 視点座標系における法線ベクトル
	float3 ReflectVector	: TEXCOORD4;	// 視点座標系における反射ベクトル
};

VS_OUTPUT VS_XFILE(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ワールド座標系に変換
	float4 WorldPosition = mul(Input.Position, matWorld);
	// ビュー座標系に変換
	Output.Position = mul(WorldPosition, matView);
	// プロジェクション座標系に変換
	Output.Position = mul(Output.Position, matProjection);
	// 視点座標系における法線ベクトル
	float3 WorldNormal = mul(Input.Normal, (float3x3)matWorld);
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
	if(slider)
		Output.TextureUV = Input.TextureUV * float2(uv_width, uv_height) + float2(uv_left, uv_top);

	return Output;
}

float4 PS_XFILE(VS_OUTPUT Input) : COLOR0
{
	float4 Color;

	// 拡散反射光(ハーフランバート)
	//float DiffuseAngle = dot(Input.Normal, Input.DirectionToLight);
	// ハーフランバートに変更
	//float HalfLambert = DiffuseAngle * 0.5 + 0.5;
	//HalfLambert = clamp(HalfLambert * HalfLambert, 0.0, 1.0);
	//float4 Diffuse = LightDiffuse * MaterialDiffuse * HalfLambert;
	
	// 拡散反射光
	float DiffuseAngle = dot(Input.Normal, Input.DirectionToLight);

	float4 Diffuse = LightDiffuse * MaterialDiffuse;
	// 環境光
	float4 Ambient = LightAmbient * MaterialAmbient;

	// 反射光
	float3 HalfVector = normalize(Input.DirectionToView + Input.DirectionToLight );
	float3 Specular = LightSpecular.rbg * saturate(pow(max(0.0, dot(HalfVector, Input.Normal)), MaterialSpecular.a) * MaterialSpecular.rbg);

	Color = Diffuse + Ambient;
	if (!TextureNone)
		Color *= tex2D(TextureSampler, Input.TextureUV);
	Color += float4(Specular, 0.0);

	 //MaterialDiffuse.a;
	Color = saturate( Color );
	Color.a = alpha;
	return Color;
}

technique ShaderTechnique
{
	pass P0
	{
		//アルファブレンドの設定
		ALPHABLENDENABLE = TRUE;
		CULLMODE = NONE;
		VertexShader = compile vs_2_0 VS_XFILE();
		PixelShader  = compile ps_2_0 PS_XFILE();
	}
}