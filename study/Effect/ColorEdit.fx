//頂点シェーダー出力
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float4 Col : COLOR0;
};

// 頂点シェーダー
VS_OUTPUT VS_MainOut()
{

	return;
}

// ピクセルシェーダー
float4 PS_main(float4 Col : COLOR0) : COLOR0
{

	// 常に白(1, 0, 0, 1)を返す
	return Col;
}

technique _ColorEdit
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS_MainOut();
		PixelShader = compile vs_2_0 PS_main();

	}
}