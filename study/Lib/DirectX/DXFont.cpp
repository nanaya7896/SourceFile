#include "DXFont.h"


//====================================
//関数名	:CDXFont
//引数		:
//戻り値	:
//内容		:
//====================================
CDXFont::CDXFont(LPDIRECT3DDEVICE9 pDevice)
{
	m_pFont = NULL;
	CreateFont(pDevice);
}

//====================================
//関数名	:~CDXFont
//引数		:
//戻り値	:
//内容		:
//====================================
CDXFont::~CDXFont()
{
	SAFE_RELEASE(m_pFont);
}

//====================================
//関数名	:CreateFont
//引数		:pDevice:描画デバイス
//戻り値	:
//内容		:フォントの作成
//====================================
void CDXFont::CreateFont(LPDIRECT3DDEVICE9 pDevice)
{
	//フォントスタイルの構造体
	D3DXFONT_DESC font;
	//構造体の初期化
	ZeroMemory(&font, sizeof(D3DXFONT_DESC));
	//文字の高さ
	font.Height = 32;
	//文字の幅
	font.Width	= 32;
	//ウェイト値
	font.Weight = FW_NORMAL;
	//ミップマップレベル
	font.MipLevels = D3DX_DEFAULT;
	//斜体
	font.Italic = FALSE;
	//文字セット
	font.CharSet = SHIFTJIS_CHARSET;
	//出力の精度
	font.OutputPrecision = OUT_DEFAULT_PRECIS;
	//出力の品質
	font.Quality = PROOF_QUALITY;
	//ピッチとファミリ
	font.PitchAndFamily = VARIABLE_PITCH;
	//フォント名
	::lstrcpy(font.FaceName, "ＭＳ 明朝");
	
	//フォントの作成
	D3DXCreateFontIndirect(
		pDevice,	//描画デバイス
		&font,		//設定したスタイルの構造体のアドレス
		&m_pFont	//受け取るLPD3DXFONTのアドレス
		);


}

//====================================
//関数名	:DrawText
//引数		:x		:ｘ座標
//			 y		:ｙ座標
//			 pText	:文字列の最初のアドレス
//			 color	:色
//戻り値	:
//内容		:フォントの描画
//====================================
void CDXFont::DrawText(int x, int y, const char * pText, D3DCOLOR color,...)
{
	//矩形
	RECT rc;
	rc.left = x;
	rc.top = y;
	rc.right = CW_USEDEFAULT;
	rc.bottom = CW_USEDEFAULT;

	//テキストを矩形に描画
	m_pFont->DrawTextA(
		NULL,				//スプライトへのアドレス
		pText,				//文字列の先頭アドレス
		-1,					//描画する文字列を計算(-1で自動計算)
		&rc,				//描画位置情報の入った構造体
		DT_LEFT | DT_NOCLIP,//表示形式を指定(左よせ、クリッピングなし)
		color				//テキストの色を指定
		);
}
