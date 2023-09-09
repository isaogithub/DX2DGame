//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "ui.h"
#include "bg.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(300)	// キャラサイズ
#define TEXTURE_HEIGHT				(300)	// 

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報

static ID3D11ShaderResourceView* g_Texture[UI_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[UI_MAX] = {
		//"data/TEXTURE/bar_white.png",
		//"data/TEXTURE/state.png",
		"data/TEXTURE/blade1.png",
		"data/TEXTURE/bullet1.png",
		"data/TEXTURE/bullet2.png",
		"data/TEXTURE/guard.png",
		"data/TEXTURE/ult1.png",
		"data/TEXTURE/jump1.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static UI		g_UI[UI_MAX];
static XMFLOAT3 g_Clickpos;
static int		g_SkillNum;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

	//グローバル変数初期化
	g_Clickpos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_SkillNum = 0;
	//テクスチャ生成

	for (int i = 0; i < UI_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
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

	for (int i = 0; i < UI_MAX; i++)
	{
		g_UI[i].use = TRUE;			// true:使っている  false:未使用
		g_UI[i].texNo = i;			// true:使っている  false:未使用
		g_UI[i].w = 200;			// true:使っている  false:未使用
		g_UI[i].h = 200;			// true:使っている  false:未使用
		g_UI[i].texNo = i;			// true:使っている  false:未使用
		g_UI[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_UI[i].time = 0;
		g_UI[i].r = g_UI[i].w * 0.3;
	}

	g_UI[UI_BLADE].pos = XMFLOAT3(1310.0f, 880.0f, 0.0f);
	g_UI[UI_BULLET].pos = XMFLOAT3(1500.0f, 775.0f, 0.0f);
	g_UI[UI_BLACKHOLE].pos = XMFLOAT3(1680.0f, 920.0f, 0.0f);
	g_UI[UI_GUARD].pos = XMFLOAT3(1680.0f, 650.0f, 0.0f);
	g_UI[UI_ULT].pos = XMFLOAT3(1140.0f, 920.0f, 0.0f);
	g_UI[UI_JUMP].pos = XMFLOAT3(1500.0f, 620.0f, 0.0f);

	g_UI[UI_BULLET].w = 320;
	g_UI[UI_BULLET].h = 320;
	g_UI[UI_BULLET].r = 320 * 0.2 ;
	
	g_UI[UI_JUMP].w = 150;
	g_UI[UI_JUMP].h = 150;
	g_UI[UI_JUMP].r = 150 * 0.3;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUI(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < UI_MAX; i++)
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
void UpdateUI(void)
{
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();

	{//UIの更新処理
		if (player[0].state == PLAYER_STATE_BLADE)SetUIUSE(UI_BLADE, FALSE);
		else SetUIUSE(UI_BLADE, TRUE);

		if (player[0].state == PLAYER_STATE_GUARD)SetUIUSE(UI_GUARD, FALSE);
		else SetUIUSE(UI_GUARD, TRUE);

		if (player[0].jumptimes <= 1)SetUIUSE(UI_JUMP, FALSE);
		else SetUIUSE(UI_JUMP, TRUE);

		if (player[0].mp < 1.0f || player[0].state == PLAYER_STATE_BULLET)SetUIUSE(UI_BULLET, FALSE);
		else SetUIUSE(UI_BULLET, TRUE);

		if (player[0].mp < 35.0f || player[0].state == PLAYER_STATE_BLACKHOLE)SetUIUSE(UI_BLACKHOLE, FALSE);
		else SetUIUSE(UI_BLACKHOLE, TRUE);

		if (player[0].mp < PLAYER_MP_MAX || player[0].state == PLAYER_STATE_ULT)SetUIUSE(UI_ULT, FALSE);
		else SetUIUSE(UI_ULT, TRUE);

	}


	{//マウス操作

		g_Clickpos = XMFLOAT3((float)GetMouseX(), (float)GetMouseY(), 0.0f);
		BOOL ans = CollisionBC(g_UI[UI_GUARD].pos, g_Clickpos, g_UI[UI_GUARD].r, 1);
		if (IsMouseLeftPressed() && ans == TRUE)
		{
				g_SkillNum = UI_GUARD;
				return;

		}

		if (!IsMouseLeftPressed() && ans)
		{
			g_SkillNum = -1;
			return;
		}


		if (IsMouseLeftTriggered())
		{
			for (int i = 0; i < UI_MAX; i++)
			{
				if (CollisionBC(g_UI[i].pos, g_Clickpos, g_UI[i].r, 1) && i != UI_GUARD)
				{
					g_SkillNum = i;
					return;
				}
			}
		}

		g_SkillNum = -1;
		//if (IsMouseLeftPressed())
		//{
		//	g_Clickpos = XMFLOAT3((float)GetMouseX(), (float)GetMouseY(), 0.0f);
		//	
		//	if (CollisionBC(g_UI[UI_GUARD].pos, g_Clickpos, g_UI[UI_GUARD].r,0.1))
		//	{
		//		g_SkillNum = UI_GUARD;
		//		return;
		//	}
		//}
		
		

		
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUI(void)
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

	for (int i = 0; i < UI_MAX; i++)
	{
		{//プレイヤー表示
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_UI[i].texNo]);	//0 : STAND, 1:walk 2

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_UI[i].pos.x;	// プレイヤーの表示位置X
			float py = g_UI[i].pos.y;	// プレイヤーの表示位置Y
			float pw = g_UI[i].w;		// プレイヤーの表示幅
			float ph = g_UI[i].h;		// プレイヤーの表示高さ

			
			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			float Alpha = 1.0f;

			if (g_UI[i].use == FALSE) Alpha = 0.6;
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0, 1.0, 1.0, Alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
			
			//攻撃表示
			//DrawAttackEffect();
	}

	//DrawUISkill();
}


UI* GetUI(void)
{
	return &g_UI[0];
}

void SetUIUSE(int type,BOOL tf)
{
	g_UI[type].use = tf;
	return;
}

int	GetUIClicked()
{

	if(g_SkillNum != UI_GUARD)return g_SkillNum;
	/*if (type == g_SkillNum)return TRUE;
	return FALSE;*/
}

int GetUIPressed()
{
	return g_SkillNum;
}

BOOL isPressGuard(void)
{
	if(g_SkillNum == UI_GUARD)return TRUE;
	return FALSE;
}

////プレイヤーのHP表示
//void DrawUIHP(int num)
//{
//	// HPの表示
//	BG* bg = GetBG();
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 195.0f;		// ゲージの表示位置X
//		float py = 90.0f;		// ゲージの表示位置Y
//		float pw = 145;	// ゲージの表示幅
//		float ph = 15.0f;		// ゲージの表示高さ
//
//		float tw = 1.0f;	// テクスチャの幅
//		float th = 1.0f;	// テクスチャの高さ
//		float tx = 0.0f;	// テクスチャの左上X座標
//		float ty = 0.0f;	// テクスチャの左上Y座標
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//
//		// エネミーの数に従ってゲージの長さを表示してみる
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		pw = pw * ((float)g_UI[num].hp / PLAYER_HP_MAX);
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}

////プレイヤーのMP表示
////void DrawUIMP(int num)
//{
//	// HPの表示
//	BG* bg = GetBG();
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 195.0f;		// ゲージの表示位置X
//		float py = 135.0f;		// ゲージの表示位置Y
//		float pw = 145.0f;	// ゲージの表示幅
//		float ph = 15.0f;		// ゲージの表示高さ
//
//		float tw = 1.0f;	// テクスチャの幅
//		float th = 1.0f;	// テクスチャの高さ
//		float tx = 0.0f;	// テクスチャの左上X座標
//		float ty = 0.0f;	// テクスチャの左上Y座標
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		//SetSpriteLTColor(g_VertexBuffer,
//		//	px, py, pw, ph,
//		//	tx, ty, tw, th,
//		//	XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));
//
//		//// ポリゴン描画
//		//GetDeviceContext()->Draw(4, 0);
//
//
//		// エネミーの数に従ってゲージの長さを表示してみる
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		pw = pw * ((float)g_UI[num].mp / PLAYER_MP_MAX);
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}

////プレイヤー状態表示
//void DrawUIState(int num)
//{
//	DrawUIHP(num);
//	DrawUIMP(num);
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 60.0f;		// ゲージの表示位置X
//		float py = -10.0f;		// ゲージの表示位置Y
//		float pw = 320.0f;	// ゲージの表示幅
//		float ph = 270.0f;		// ゲージの表示高さ
//
//		float tw = 1.0f;	// テクスチャの幅
//		float th = 1.0f;	// テクスチャの高さ
//		float tx = 0.0f;	// テクスチャの左上X座標
//		float ty = 0.0f;	// テクスチャの左上Y座標
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//	}
//
//}

//プレイヤーSikll表示



