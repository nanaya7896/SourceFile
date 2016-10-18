float4x4 matWorld;	//ワールド座標
float4x4 matVP;		//射影変換への座標変換
texture m_pTexture;	//テクスチャ
float4 Diffuse : COLOR0;//頂点のディフューズ色


//テクスチャからどうやって色を取り出すか設定
sampler smp =sampler_state{
	texture=<m_pTexture>;
	MipFilter=NONE;
	MinFilter=LINEAR;
	MagFilter=LINEAR;
	AddressU=WRAP;
	AddressV=WRAP;
};

//=====構造体の定義=====//
//入力の構造体
struct VS_INPUT{
	float4 pos 	: POSITION;	//頂点の位置
	float4 Diffuse 	: COLOR0;	//頂点のデフューズ色
	float2 uv 	:TEXCOORD0; 	//UV値
};

//出力の構造体
struct VS_OUTPUT{
	float4 pos 	: POSITION;	//頂点の位置
	float4 Diffuse 	: COLOR0;	//頂点のデフューズ色
	float2 uv 	: TEXCOORD0;	//UV値
};

//頂点shader
VS_OUTPUT VS_VERTEX(VS_INPUT Input){
	//使用する構造体VS_OUTを初期化している
	VS_OUTPUT Output=(VS_OUTPUT)0;

	//ワールド座標系
	float4 WorldPosition =mul(Input.pos,matWorld);

	//頂点座標変換
	Output.pos=mul(WorldPosition,matVP);

	//頂点の色
	Output.Diffuse=Input.Diffuse;

	//テクスチャUV値を出力
	Output.uv=Input.uv;

	return Output;
}


//ピクセルシェーダー
float4 PS_VERTEX(VS_OUTPUT Input) : COLOR0{
	
	float4 color=Input.Diffuse*tex2D(smp,Input.uv);
	//color.a*=alpha;
	return color;
}


//Mainとなる箇所
technique Tech{
	//パスによりテクニックの種類を分ける
	pass P0{
		//透明度あり
		//エフェクトステート
		//カリングの設定
		CULLMODE=NONE;
		//アルファブレンドの設定
		ALPHABLENDENABLE=TRUE;
		//
		SRCBLEND=SRCALPHA;
		//
		DESTBLEND=ONE;
		//
		ZENABLE=TRUE;
		//
		ZWRITEENABLE=TRUE;

		VertexShader=compile vs_2_0 VS_VERTEX();
		PixelShader=compile ps_2_0 PS_VERTEX();
	}
	pass P1{
		//通常合成
		CULLMODE=NONE;
		ALPHABLENDENABLE=TRUE;
		SRCBLEND=SRCALPHA;
		DESTBLEND=INVSRCALPHA;
		ZENABLE=FALSE;
		ZWRITEENABLE=FALSE;
		VertexShader=compile vs_2_0 VS_VERTEX();
		PixelShader=compile ps_2_0 PS_VERTEX();
	}
    pass P2
    {
        CULLMODE = NONE;
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = INVDESTCOLOR;
        DESTBLEND = ONE;
        ZENABLE = TRUE;
        ZWRITEENABLE = TRUE
;
        VertexShader = compile vs_2_0 VS_VERTEX();
        PixelShader = compile ps_2_0 PS_VERTEX();
    }

}