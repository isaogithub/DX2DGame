/*******************************************************************************
* タイトル:		文字表示プログラム
* プログラム名:	write.cpp
* 作成者:		GP11A132 29 林肇勲
* 作成日:		2023/08/08
*******************************************************************************/



/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "write.h"
#include "renderer.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define DIRECT_WRITE_MAX	(1)

//=============================================================================
//		フォント名
//=============================================================================

const WCHAR* FontList[FONT_MAX]
{
	L"メイリオ",
	L"Arial",
	L"Meiryo UI"
};

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
DIRECT_WRITE DirectWrite[DIRECT_WRITE_MAX];



void SetFont(const WCHAR* FontName, float fontSize, const D2D1_COLOR_F color)
{
	DirectWrite[0].DWriteFactory[0].CreateTextFormat(FontName, DirectWrite[0].FD[0].fontCollection,
		DirectWrite[0].FD[0].fontWeight, DirectWrite[0].FD[0].fontStyle, DirectWrite[0].FD[0].fontStretch, 
		fontSize, DirectWrite[0].FD[0].localeName, &DirectWrite[0].TextFormat);

	DirectWrite[0].TextFormat[0].SetTextAlignment(DirectWrite[0].FD[0].textAlignment);

	DirectWrite[0].RT[0].CreateSolidColorBrush(color, &DirectWrite[0].SolidBrush);

}





//=============================================================================
//     初期化
//=============================================================================
void InitDirectWrite(void)
{
	HRESULT hr = S_OK;

	DIRECT_WRITE* DW = GetDirectWrite();

	// フォントデータ
	DW[0].FD = new FONT_DATA();

	// デフォルト設定
	DW[0].FD->font = Font::Meiryo;
	DW[0].FD->fontCollection = nullptr;
	DW[0].FD->fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
	DW[0].FD->fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
	DW[0].FD->fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
	DW[0].FD->fontSize = 20;
	DW[0].FD->localeName = L"ja-jp";
	DW[0].FD->textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
	DW[0].FD->Color = D2D1::ColorF(D2D1::ColorF::White);

	// Direct2D,DirectWriteの初期化
	// ID2D1Factory インターフェイスの作成
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &DW[0].D2DFactory);

	if (SUCCEEDED(hr)) {
		// IDWriteFactoryの作成
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DW[0].DWriteFactory));
	}

	if (SUCCEEDED(hr)) {
		//関数CreateTextFormat()
		//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
		//第2引数：フォントコレクション（nullptr）
		//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
		//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
		//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
		//第6引数：フォントサイズ（20, 30等）
		//第7引数：ロケール名（L""）
		//第8引数：テキストフォーマット（&g_pTextFormat）
		hr = DW[0].DWriteFactory->CreateTextFormat(FontList[(int)DW[0].FD->font],
			DW[0].FD->fontCollection,
			DW[0].FD->fontWeight,
			DW[0].FD->fontStyle,
			DW[0].FD->fontStretch,
			DW[0].FD->fontSize,
			DW[0].FD->localeName,
			&DW[0].TextFormat);
	}

	if (SUCCEEDED(hr)) {
		//関数SetTextAlignment()
		//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
		//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
		hr = DW[0].TextFormat->SetTextAlignment(DW[0].FD->textAlignment);
	}


	// バックバッファの取得
	// 型：IDXGISwapChain
	GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&DW[0].BackBuffer));

	// dpiの設定
	FLOAT dpiX;
	FLOAT dpiY;
	DW[0].D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	// レンダーターゲットの作成
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,
		D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	if (!DW[0].RT) {
		// サーフェスに描画するレンダーターゲットを作成
		hr = DW[0].D2DFactory->CreateDxgiSurfaceRenderTarget(&DW[0].BackBuffer[0], &props, &DW[0].RT);

		// アンチエイリアシングモード
		DW[0].RT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		if (SUCCEEDED(hr))
		{
			//ブラシ作成関数
			//第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
			DW[0].RT->CreateSolidColorBrush(DW[0].FD->Color, &DW[0].SolidBrush);
		}
	}


}

//=============================================================================
//     終了処理
//=============================================================================
void UninitDirectWrite(void)
{
	DIRECT_WRITE* DW = GetDirectWrite();
	// 文字描画関連のリリース
	if (DW[0].FD) delete(DW[0].FD);
	if (DW[0].BackBuffer) DW[0].BackBuffer->Release();
	if (DW[0].SolidBrush) DW[0].SolidBrush->Release();
	if (DW[0].RT) DW[0].RT->Release();
	if (DW[0].TextFormat) DW[0].TextFormat->Release();
	if (DW[0].DWriteFactory) DW[0].DWriteFactory->Release();
	if (DW[0].D2DFactory) DW[0].D2DFactory->Release();
	if (DW[0].TextLayout) DW[0].TextLayout->Release();

}


//=============================================================================
// 文字描画
// string：文字列
// pos：描画ポジション
// rect：領域指定
// options：テキストの整形
//=============================================================================
void DrawString(WCHAR* str, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options)
{
	HRESULT hr = S_OK;
	DIRECT_WRITE* DR = GetDirectWrite();

	// ターゲットサイズの取得
	D2D1_SIZE_F TargetSize = DR[0].RT->GetSize();

	// テキストレイアウトを作成
	hr = DR[0].DWriteFactory->CreateTextLayout(str, (UINT32)wcslen(str), DR[0].TextFormat, TargetSize.width, TargetSize.height, &DR[0].TextLayout);

	if (SUCCEEDED(hr)) {
		// 描画位置の確定
		D2D1_POINT_2F points;
		points.x = pos.x;
		points.y = pos.y;

		// 描画の開始
		DR[0].RT->BeginDraw();

		// 描画処理
		DR[0].RT->DrawTextLayout(points, DR[0].TextLayout, DR[0].SolidBrush, options);

		// 描画の終了
		DR[0].RT->EndDraw();
	}
}

void DrawStringRect(WCHAR* str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options)
{
	DIRECT_WRITE* DW = GetDirectWrite();

	// 描画の開始
	DW[0].RT->BeginDraw();

	// 描画処理
	DW[0].RT->DrawText(str, (UINT32)wcslen(str), DW[0].TextFormat, rect, DW[0].SolidBrush, options);

	// 描画の終了
	DW[0].RT->EndDraw();
}

DIRECT_WRITE* GetDirectWrite(void)
{
	return &DirectWrite[0];
}

