// ワールドビュープロジェクション変換行列
float4x4 mWVP : WORLDVIEWPROJECTION;
float4 matDiffuse : MATERIALDIFFUSE;
//頂点シェーダー出力
struct VS_OUTPUT
{
    float4 Pos  :   POSITION;
    float4 Col : COLOR0;
};


// 頂点シェーダー
VS_OUTPUT VS_main(float4 pos : POSITION)
{
    //VS_OUTPUTを初期化
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    //座標変換
    Out.Pos = mul(pos, mWVP);
    Out.Col = matDiffuse;
	// 行列にベクトルを掛け合わせて頂点を変換
    return Out;
}

// ピクセルシェーダー
float4 PS_main(float4 Col : COLOR0) : COLOR0
{

	// 常に白(1, 0, 0, 1)を返す
    return Col;
}

// テクニック
technique TShader
{
    pass P0
    {
        ZFUNC = LESS;
        CULLMODE = NONE;
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE;
        ALPHABLENDENABLE = FALSE;
        SHADEMODE = FLAT;

        VertexShader = compile vs_2_0 VS_main();
        PixelShader = compile ps_2_0 PS_main();
    }
}