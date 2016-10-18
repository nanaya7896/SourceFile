float4x4 matWorld : WORLD; // ワールド座標
float4x4 matWorldInverse; // ワールド座標の逆行列
float4x4 matVP; // 射影空間への座標変換
float4x4 matLVP; // ライトから見た射影空間への座標変換
bool titleLight;
float4 LightPosition;//=float4(-500000.0f, -5000.0f, 0.0f, 0.0f); // 光源座標

texture Texture; // テクスチャー
//マテリアル色
float4 MaterialDiffuse : MATERIALDIFFUSE;
float4 MaterialAmbient : MATERIALAMBIENT;
float4 ambient : AMBIENT;

//:LIGHTDIRECTION;

sampler TextureSample = sampler_state
{
    Texture = <Texture>;
    MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//頂点入力構造体
struct VS_INPUT
{
    //座標
    float4 Pos : POSITION;
    //法線
    float3 Normal : NORMAL;
    //UV座標
    float2 TexUV : TEXCOORD0;    
};

//頂点出力構造体
struct VS_OUTPUT
{
    //頂点
    float4 Pos : POSITION;
    //
    float4 Col : COLOR0;
    //UV座標
    float2 TexUV : TEXCOORD0;
    //法線
    float3 Normal : TEXCOORD1;
    //光方向
    float3 Light : TEXCOORD2;
};

//正しい法線を計算し、ライティングを求める
float CalcLightingDot(float4 nor)
{
    //法線をワールド座標に変換し、平行移動分との差を求めて
    //正規化、ライトとの法線の内積を求める
    float3 N = mul(nor, matWorld).xyz;
    float3 BaseN = mul(float4(0, 0, 0, 1), matWorld).xyz;
    N = normalize(N - BaseN);
    return dot(-LightPosition.xyz, N);
}

//頂点情報の設定
VS_OUTPUT VS_VERTEX(VS_INPUT In)
{
    //VS_OUTPUTの初期化
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    //
    //float4 LightDirection = LightPosition;
    // ワールド座標系への変換
    float4 WorldPosition = mul(In.Pos, matWorld);
    //法線
    float4 WorldNormal = float4(In.Normal,0.0);
    // 頂点座標変換
    Out.Pos = mul(WorldPosition, matVP);

    //Out.Pos = mul(In.Pos, Out.Pos);
    //法線の計算
    Out.Normal = mul(float4(In.Normal,0), matWorld).xyz;
    //ワールド座標とライトの位置の差分
    Out.Light = LightPosition.xyz-WorldPosition.xyz;
    //UV座標
    Out.TexUV = In.TexUV;
    //ランバート反射
    Out.Col.rgb = MaterialDiffuse.rgb * CalcLightingDot(float4(In.Normal,0))+ MaterialAmbient.rgb * ambient.rgb;
   // Out.Col.a = MaterialDiffuse.a;
	
    return Out;
}

//頂点情報の設定
VS_OUTPUT VS_VERTEXLIGHT(VS_INPUT In)
{
    //VS_OUTPUTの初期化
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    //
    //float4 LightDirection = LightPosition;
    // ワールド座標系への変換
    float4 WorldPosition = mul(In.Pos, matWorld);
    //法線
    float4 WorldNormal = float4(In.Normal, 0.0);
    // 頂点座標変換
    Out.Pos = mul(WorldPosition, matVP);

    //Out.Pos = mul(In.Pos, Out.Pos);
    //法線の計算
    Out.Normal = mul(float4(In.Normal, 0), matWorld).xyz;
    //ワールド座標とライトの位置の差分
    Out.Light = LightPosition.xyz; // -WorldPosition.xyz;
    //UV座標
    Out.TexUV = In.TexUV;
    //ランバート反射
    Out.Col.rgb = MaterialDiffuse.rgb * CalcLightingDot(float4(In.Normal, 0)) + MaterialAmbient.rgb * ambient.rgb;
   // Out.Col.a = MaterialDiffuse.a;
	
    return Out;
}

VS_OUTPUT VS_VERTEXWORLD(VS_INPUT In)
{
	 VS_OUTPUT Out = (VS_OUTPUT) 0;
    // ワールド座標系への変換s
    float4 WorldPosition = mul(In.Pos, matWorld);
    //
    float4 WorldNormal = float4(In.Normal,0.0);
    // 頂点座標変換
    Out.Pos = mul(WorldPosition, matVP);	

    return Out;    
}

//ピクセルシェーダー
float4 PS_VERTEX(VS_OUTPUT In) : COLOR
{
    //法線・ライト正規化
    float3 N= normalize(In.Normal);
    float3 L = normalize(In.Light);
float3 diffuse;
if(titleLight){
    //ディフューズ色の決定
diffuse = (max(dot(N, L), 0.0f) * 10.8f + 0.5f);
}else{
diffuse = (max(dot(N, L), 0.0f) * 0.8f + 0.5f);
}
    
    //
    float4 output = float4(diffuse, 1.0f) * tex2D(TextureSample, In.TexUV);
    
    //透明度を設定
    output.a = MaterialDiffuse.a;
    //output.a = 1.0f;
    return output; //* tex2D(TextureSample, In.TexUV);
}

//ピクセルシェーダー
float4 PS_COLORVERTEX(VS_OUTPUT In) : COLOR
{
    //法線・ライト正規化
    float3 N = normalize(In.Normal);
    float3 L = normalize(In.Light);
    //ディフューズ色の決定
    float3 diffuse = (max(dot(N, L), 0.0f) * 0.8f + 0.5f);  
    //
    float4 output = float4(diffuse, 1.0f) * tex2D(TextureSample, In.TexUV);
    output.r = MaterialDiffuse.r;
    output.g = MaterialDiffuse.g;
    output.b = MaterialDiffuse.b;
    //透明度を設定
    output.a = MaterialDiffuse.a;
    //output.a = 0.5f;
    return output; //* tex2D(TextureSample, In.TexUV);
}



technique ShaderTechnique
{
    //透過なし
    Pass P0
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = false;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
    //透過処理あり
    Pass P1
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
    //床の色の表現
    Pass P2
    {
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_COLORVERTEX();

    }
    //Lightを動かす
    Pass P3
    {    
	    ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = InvSrcAlpha;
        CullMode = None;
        VertexShader = compile vs_2_0 VS_VERTEXLIGHT();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
}