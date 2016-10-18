//float4 color = (1,1,1, 1);
//ディフューズ色
float4 matDiffuse : MATERIALDIFFUSE;
//アンビエント色
float4 matAmbient : MATERIALAMBIENT;
float4 ambient : AMBIENT;
float4 emmisive : EMMISIVE;
//行列
float4x4 world;
float4x4 view;
float4x4 proj;
//ライト
float3 dir : LIGHTDIRECTION;

//テクスチャ
texture tex;

//テクスチャの設定
sampler smp = sampler_state
{
    texture = <tex>;
    //テクスチャ座標U,Vの範囲外の位置にある座標は0.0または1.0にする
    AddressU = CLAMP;
    AddressV = CLAMP;
    //線形補間
    //ミップレベル
    MipFilter = LINEAR;
    //縮小のサンプリング
    MinFilter = LINEAR;
    //拡大のサンプリング
    MagFilter = LINEAR;
};


// 頂点シェーダー入力
struct VS_INPUT
{
    //座標
    float3 pos : POSITION;
    //ポインタサイズ
    float size : PSIZE;
    //色
    float4 color : COLOR0;
    float4 unused : COLOR1;
    //法線
    float4 normal : NORMAL;
    float2 tex : TEXCOORD0;
   
};

// 頂点シェーダー出力
struct VS_OUTPUT
{
    float4 pos : POSITION;
    float size : PSIZE;
    float4 color : COLOR0;
    float2 tex : TEXCOORD0;

};

// ピクセルシェーダー入力
struct PS_INPUT
{
    float4 Col : COLOR0;
    float2 TexUV : TEXCOORD0;
};

VS_OUTPUT VS_VERTEX(VS_INPUT In)
{
    // VS_OUTPUTを初期化
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    //座標変換
    Out.pos = float4(In.pos, 1);
    Out.pos = mul(Out.pos, world);
    Out.pos = mul(Out.pos, view);

    float d = length(Out.pos.xyz);
    Out.pos = mul(Out.pos, proj);
    
     //法線の変換・正規化
    float3 N = mul(In.normal, Out.pos);
    N = normalize(N);
    //法線とライトの内積を計算し、反射率としてディヒューズ色に掛け合わせる
    Out.color.rgb = matDiffuse.rgb * max(dot(-dir, N), 0);
    //+matAmbient.rgb * ambient.rgb;
    //アルファ成分とテクスチャのUV座標はそのまま出力
    Out.color.a = matDiffuse.a;
    Out.tex = In.tex;
    Out.size = In.size;

    Out.color = matDiffuse;

    return Out;
}

float4 PS_VERTEX(PS_INPUT In) : COLOR0
{
    return tex2D(smp, In.TexUV) * In.Col+emmisive; //* emmisive;
    //out_color =tex2D(smp, in_tex)*in_color; //* color;
}

technique tec
{

    Pass P0
    {
  // サイズは自分で計算するのでスプライトの生成だけする
        ALPHABLENDENABLE = true;
        SPECULARENABLE = true;
        PointSpriteEnable = true;
        Ditherenable = true;
        ZENABLE = true;
        ZWRITEENABLE = false;
        SrcBlend = SRCALPHA;
        DestBlend = ONE;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
      
    }
    Pass P1
    {
        ALPHABLENDENABLE = true;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }
}