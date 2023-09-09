/*******************************************************************************
* タイトル:		ライトヘッダー
* プログラム名:	write.h
* 作成者:		GP11A132 30 少林文孝
* 作成日:		2023/06/14
* 
*******************************************************************************/

#pragma once	// インクルードガード（多重インクルード防止）


#pragma warning(push)
#pragma warning(disable:4005)

#include "main.h"

#include <d2d1.h>
#include <dwrite.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


/*******************************************************************************
* マクロ定義
*******************************************************************************/

//=============================================================================
//		フォントリスト
//=============================================================================
enum Font
{
	Meiryo,
	Arial,
	MeiryoUI,

	FONT_MAX
};



/*******************************************************************************
* 構造体定義
*******************************************************************************/
// フォントデータ構造体
struct FONT_DATA
{
	Font font;								// フォント名
	IDWriteFontCollection* fontCollection;	// フォントコレクション
	DWRITE_FONT_WEIGHT fontWeight;			// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;			// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;		// フォントの幅
	FLOAT fontSize;							// フォントサイズ
	WCHAR const* localeName;				// ロケール名
	DWRITE_TEXT_ALIGNMENT textAlignment;	// テキストの配置
	D2D1_COLOR_F Color;						// フォントの色

};

// DIRECT_WRITE構造体
struct DIRECT_WRITE
{
	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWriteFactory;
	IDWriteTextFormat* TextFormat;
	IDWriteTextLayout* TextLayout;
	ID2D1RenderTarget* RT;
	ID2D1SolidColorBrush* SolidBrush;
	IDXGISurface* BackBuffer;

	// フォントデータ
	FONT_DATA* FD;

};

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

// DIRECT_WRITE OBJ
void InitDirectWrite(void);		// DIRECT_WRITEの初期化処理
void UninitDirectWrite(void);	// DIRECT_WRITEの終了処理


// 文字描画処理
// WCHAR：文字列
// pos：描画ポジション
// options：テキストの整形
void DrawString(WCHAR* FontList, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options);

// 文字描画処理
// string：文字列
// rect：領域指定
// options：テキストの整形
void DrawStringRect(WCHAR* FontList, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options);


// フォント設定
// 第1引数：フォントデータ構造体
void SetFont(const WCHAR* FontName, float fontSize, const D2D1_COLOR_F color);


DIRECT_WRITE* GetDirectWrite(void);

