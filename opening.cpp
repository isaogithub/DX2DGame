//=============================================================================
//
// BG処理 [bg.cpp]
// Author : 
//
//=============================================================================

#include "input.h"
#include "fade.h"
#include "main.h"
#include "opening.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define TEXTURE_WIDTH				(1920)			// (SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(1080)			// (SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(OPENING_MAX + 2)				// テクスチャの数
#define FADE_RATE					(0.02f);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報

//static ID3D11ShaderResourceView* g_Tutorial[TEXTURE_TUTORIAL_MAX] = { NULL };	// テクスチャ情報

static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

//static char* g_TutorialName[TEXTURE_TUTORIAL_MAX] = {
//	"data/TEXTURE/BG_mori001.png",
//};


static char *g_TexturName[] = {
	"data/Opening/opening001.PNG",
	"data/Opening/opening002.PNG",
	"data/Opening/opening003.PNG",
	"data/Opening/opening004.PNG",
	"data/Opening/opening_white.png",
	"data/TEXTURE/PRESS.png",

};


static BOOL	 g_Load = FALSE;		// 初期化を行ったかのフラグ
static int   g_OpeningNum;
static int   g_OpFade;
static float g_OpAlpha;
static float g_keyAlpha;
static BOOL	 g_Clear;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitOpening(void)
{
	//グローバル変数初期化
	g_OpeningNum = OPENING_NUM001;
	g_OpFade = FADE_IN;
	g_OpAlpha = 0.0f;
	g_keyAlpha = 0.0f;
	g_Clear = FALSE;
	//
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成


		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			g_Texture[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_TexturName[i],
				NULL,
				NULL,
				&g_Texture[i],
				NULL);
		}

		

		// 変数の初期化
		

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitOpening(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateOpening(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{// Enter押したら、ステージを切り替える

		PlaySound(SOUND_LABEL_SE_openingselected);
		if (g_OpFade == FADE_NONE)
		{

			g_OpeningNum++;
			g_OpAlpha = 0.0f;
			//g_OpFade = FADE_IN;
			if (g_OpeningNum == OPENING_MAX)
			{
				SetFade(FADE_OUT, MODE_TUTORIAL);

			}
			else SetOpening(g_OpeningNum);		

			g_keyAlpha = 0.0f;
			g_Clear = FALSE;
		}
	}

	if (g_OpFade != FADE_NONE)
	{// フェード処理中
		if (g_OpFade == FADE_OUT)
		{// フェードアウト処理
			g_OpAlpha -= FADE_RATE;		// α値を加算して画面を消していく

			
			if (g_OpAlpha <= 0.0f)
				{
					// フェード処理終了
					g_OpAlpha = 0.0f;
				SetOpFade(FADE_IN);
				//SetOpening(g_OpeningNum);
				

			}

		}
		else if (g_OpFade == FADE_IN)
		{// フェードイン処理
			g_OpAlpha += FADE_RATE;		// α値を減算して画面を浮き上がらせる
		
			if (g_OpAlpha >= 1.0f)
			{
				// 鳴っている曲を全部止める
				// フェードイン処理に切り替え
				g_OpAlpha = 1.0f;
				SetOpFade(FADE_NONE);
				
			}

		}
	}
	if (g_OpFade == FADE_NONE)
	{
		//エンターキー
		int temp = 0.01f;
		if (g_keyAlpha > 1.0f)		g_Clear = TRUE;
		else if (g_keyAlpha < 0.0f)	g_Clear = FALSE;
		
		if (g_Clear)
		{
			g_keyAlpha -= 0.01f;
		}
		else g_keyAlpha += 0.01f;
		
	}
	//float speed = g_Enemy[i].move.x;
				//g_Enemy[i].pos.x += speed;

				//MAPの右端まで来た？
				//if (g_Enemy[i].pos.x > bg->w)
				//{
				//	g_Enemy[i].pos.x = bg->w;
				//	g_Enemy[i].move.x = 0 - g_Enemy[i].move.x;
				//}


				////MAPの左端まで来た？
				//if (g_Enemy[i].pos.x  < 0.0f)
				//{
				//	g_Enemy[i].pos.x = 0.0f;
				//	g_Enemy[i].move.x *= (-1);

				//}



	/*switch (GetMode())
	{
	case MODE_TUTORIAL:

		g_BG.scrl -= 0.005f;
		break;
	case MODE_GAME:
		break;

	default:
		break;
	}*/


	//g_BG.scrl -= 0.0f;		// 0.005f;		// スクロール


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawOpening(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	{//背景
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_MAX]);
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			0,
			0,
			TEXTURE_WIDTH,
			TEXTURE_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	{
		float tw = 500.0f,th;
		switch (g_OpeningNum)
		{// テクスチャ設定
		case OPENING_NUM001:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM001]);
			tw = 540.0f;
			break;

		case OPENING_NUM002:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM002]);
			tw = 991.0f;
			break;

		case OPENING_NUM003:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM003]);
			tw = 591.0f;
			break;

		case OPENING_NUM004:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM004]);
			tw = 700.0f;
			break;

		}
		th = 54.0f;

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer,
			960,
			540,
			tw,
			th,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, g_OpAlpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
	if (g_OpFade == FADE_NONE)
	{
		{//エンターキー
		// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				SCREEN_CENTER_X + 300,
				900,
				600,
				80,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(0.0f, 0.0f, 0.0f, g_keyAlpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	

}


//=============================================================================
// オープンニング番号設定
//=============================================================================
void SetOpening(int num)
{
	g_OpeningNum = num;
	g_OpFade = FADE_OUT;

}

void SetOpFade(int fade)
{
	g_OpFade = fade;
}




