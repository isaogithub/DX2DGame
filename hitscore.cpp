//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "enemy.h"
#include "player.h"
#include "hitscore.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(60)	// キャラサイズ
#define TEXTURE_HEIGHT				(60)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define SCORE_MAX					(20)

#define NUM_PATTERN_DIVIDE_X					(12)
#define NUM_PATTERN_DIVIDE_Y					(1)
#define NUM_ANIM_PATTERN_NUM	( NUM_PATTERN_DIVIDE_X *  NUM_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define WAIT_FRAME_MAX				(40)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/number.png",
	"data/TEXTURE/heal_number.png",
};


static SCORE	g_HitScore[SCORE_MAX];
static float	g_Scoretimer;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHitScore(void)
{
	g_Scoretimer = 0.0f;


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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	for (int i = 0; i < SCORE_MAX; i++)
	{
		g_HitScore[i].use = FALSE;
		g_HitScore[i].w = TEXTURE_WIDTH;
		g_HitScore[i].h = TEXTURE_HEIGHT;
		g_HitScore[i].pos = { 0.0f, 0.0f, 0.0f };
		g_HitScore[i].scl = { 1.0f, 1.0f, 1.0f };
		g_HitScore[i].texNo = 0;
		g_HitScore[i].score = 0;	// スコアの初期化
		g_HitScore[i].g_ScoreframeCnt = 0;	// スコアの初期化
		g_HitScore[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// スコアの初期化
	}



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHitScore(void)
{
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

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHitScore(void)
{


	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (g_HitScore[i].use)
		{
			if (g_HitScore[i].g_ScoreframeCnt++ >= WAIT_FRAME_MAX)
			{
				g_HitScore[i].g_ScoreframeCnt = 0;
				g_HitScore[i].use = FALSE;
			}
			float Alpha = 1.0f - (float)g_HitScore[i].g_ScoreframeCnt / WAIT_FRAME_MAX;
			g_HitScore[i].color.w = Alpha;
			

		}
	}

#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHitScore(void)
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

	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (g_HitScore[i].use)
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			ENEMY* enemy = GetEnemy();
			PLAYER* player = GetPlayer();
			BG* bg = GetBG();
			// 桁数分処理する

			int number = g_HitScore[i].score;

			{
				// 今回表示する桁の数字

				// スコアの位置やテクスチャー座標を反映
				float px = g_HitScore[i].pos.x - bg->pos.x - g_HitScore[i].w * 3;	// スコアの表示位置X
				float py = g_HitScore[i].pos.y - bg->pos.y - g_HitScore[i].g_ScoreframeCnt * 5;			// スコアの表示位置Y
				float pw = g_HitScore[i].w * g_HitScore[i].scl.x;				// スコアの表示幅
				float ph = g_HitScore[i].h * g_HitScore[i].scl.y;				// スコアの表示高さ

				float tw = 1.0f / NUM_PATTERN_DIVIDE_X;		// テクスチャの幅
				float th = 1.0f / NUM_PATTERN_DIVIDE_Y;		// テクスチャの高さ
				float tx;
				if (number <= 0) tx = tw * 11;			// テクスチャの左上X座標
				else tx = tw * 10;
				float ty = 0.0f;			// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
					
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_HitScore[i].color);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

				// 次の桁へ
			}


			number = abs(number);

			for (int j = 0; j < HITSCORE_DIGIT; j++)
			{

				// 今回表示する桁の数字
				float x = (float)(number % 10);

				// スコアの位置やテクスチャー座標を反映
				float px = g_HitScore[i].pos.x - bg->pos.x - g_HitScore[i].w * j;	// スコアの表示位置X
				float py = g_HitScore[i].pos.y - bg->pos.y - g_HitScore[i].g_ScoreframeCnt * 5;			// スコアの表示位置Y
				float pw = g_HitScore[i].w * g_HitScore[i].scl.x;				// スコアの表示幅
				float ph = g_HitScore[i].h * g_HitScore[i].scl.y;				// スコアの表示高さ

				float tw = 1.0f / NUM_PATTERN_DIVIDE_X;		// テクスチャの幅
				float th = 1.0f / NUM_PATTERN_DIVIDE_Y;		// テクスチャの高さ
				float tx = x * tw;			// テクスチャの左上X座標
				float ty = 0.0f;			// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				float Alpha = 1.0f - (float)g_HitScore[i].g_ScoreframeCnt / WAIT_FRAME_MAX;
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_HitScore[i].color);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

				// 次の桁へ
				number /= 10;
			}

			
		}
		
	}
	

		
	
	
}


void SetHitScore(int score,XMFLOAT3 pos,int type)
{
	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (!g_HitScore[i].use)
		{
			g_HitScore[i].use = TRUE;
			g_HitScore[i].score = score;
			g_HitScore[i].pos = pos;
			g_HitScore[i].type = type;

			if (g_HitScore[i].type == HEAL)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);


			}
			else if (g_HitScore[i].type == MP1)
			{
				g_HitScore[i].scl = XMFLOAT3(0.7f, 0.7f, 0.7f);
				g_HitScore[i].color = XMFLOAT4(0.1f, 0.8f, 1.0f, 1.0f);
			}
			else if (g_HitScore[i].type == DAMAGE_ENEMY)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.66f, 0.35f, 0.66f, 1.0f);
			}
				

			else if (g_HitScore[i].type == DAMAGE_PLAYER)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.95f, 0.1f, 0.0f, 1.0f);
			}

			else if (g_HitScore[i].type == DAMAGE_CRITICAL)
			{
				g_HitScore[i].scl = XMFLOAT3(1.5f, 1.5f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

			}

			return;
		}
	}

}


