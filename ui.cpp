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
#define TEXTURE_MAX					(UI_MAX + 3)
#define GAUGE_WIDTH			(220)
#define GAUGE_HEIGHT		(30)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[TEXTURE_MAX] = {

		"data/TEXTURE/blade1.png",
		"data/TEXTURE/bullet1.png",
		"data/TEXTURE/bullet2.png",
		"data/TEXTURE/guard.png",
		"data/TEXTURE/ult1.png",
		"data/TEXTURE/jump1.png",
		"data/TEXTURE/heroicon.png",
		"data/TEXTURE/bar_white.png",
		"data/TEXTURE/state.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static UI		g_SkillUI[UI_MAX];
static PLAYERICON	g_PlayerIcon;
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

	for (int i = 0; i < TEXTURE_MAX; i++)
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

	
	InitSkillUI();
	InitPlayerIconUI();

	g_Load = TRUE;
	return S_OK;
}

void InitSkillUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		g_SkillUI[i].use = TRUE;			// true:使っている  false:未使用
		g_SkillUI[i].texNo = i;			// true:使っている  false:未使用
		g_SkillUI[i].w = 200;			// true:使っている  false:未使用
		g_SkillUI[i].h = 200;			// true:使っている  false:未使用
		g_SkillUI[i].texNo = i;			// true:使っている  false:未使用
		g_SkillUI[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_SkillUI[i].time = 0;
		g_SkillUI[i].r = g_SkillUI[i].w * 0.3;
	}

	g_SkillUI[UI_BLADE].pos = XMFLOAT3(1310.0f, 880.0f, 0.0f);
	g_SkillUI[UI_BULLET].pos = XMFLOAT3(1500.0f, 775.0f, 0.0f);
	g_SkillUI[UI_BLACKHOLE].pos = XMFLOAT3(1680.0f, 920.0f, 0.0f);
	g_SkillUI[UI_GUARD].pos = XMFLOAT3(1680.0f, 650.0f, 0.0f);
	g_SkillUI[UI_ULT].pos = XMFLOAT3(1140.0f, 920.0f, 0.0f);
	g_SkillUI[UI_JUMP].pos = XMFLOAT3(1500.0f, 620.0f, 0.0f);

	g_SkillUI[UI_BULLET].w = 320;
	g_SkillUI[UI_BULLET].h = 320;
	g_SkillUI[UI_BULLET].r = 320 * 0.2;

	g_SkillUI[UI_JUMP].w = 150;
	g_SkillUI[UI_JUMP].h = 150;
	g_SkillUI[UI_JUMP].r = 150 * 0.3;
}
void InitPlayerIconUI(void)
{
	g_PlayerIcon.use = FALSE;
	g_PlayerIcon.pos = XMFLOAT3(175.0f, 180.0f, 0.0f);
	g_PlayerIcon.w = 180.0f;
	g_PlayerIcon.h = 180.0f;
	g_PlayerIcon.texNo = 6;
	g_PlayerIcon.theta = 0.0f;

	return;
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
	UpdateSkillUI();
	UpdatePlayerIconUI();
}


void UpdateSkillUI(void)
{
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();

	{//UIの更新処理
		if (player[0].state == PLAYER_STATE_BLADE)SetSkillUIUSE(UI_BLADE, FALSE);
		else SetSkillUIUSE(UI_BLADE, TRUE);

		if (player[0].state == PLAYER_STATE_GUARD)SetSkillUIUSE(UI_GUARD, FALSE);
		else SetSkillUIUSE(UI_GUARD, TRUE);

		if (player[0].jumptimes <= 1)SetSkillUIUSE(UI_JUMP, FALSE);
		else SetSkillUIUSE(UI_JUMP, TRUE);

		if (player[0].mp < 1.0f || player[0].state == PLAYER_STATE_BULLET)SetSkillUIUSE(UI_BULLET, FALSE);
		else SetSkillUIUSE(UI_BULLET, TRUE);

		if (player[0].mp < 35.0f || player[0].state == PLAYER_STATE_BLACKHOLE)SetSkillUIUSE(UI_BLACKHOLE, FALSE);
		else SetSkillUIUSE(UI_BLACKHOLE, TRUE);

		if (player[0].mp < PLAYER_MP_MAX || player[0].state == PLAYER_STATE_ULT)SetSkillUIUSE(UI_ULT, FALSE);
		else SetSkillUIUSE(UI_ULT, TRUE);

	}


	{//マウス操作

		g_Clickpos = XMFLOAT3((float)GetMouseX(), (float)GetMouseY(), 0.0f);
		BOOL ans = CollisionBC(g_SkillUI[UI_GUARD].pos, g_Clickpos, g_SkillUI[UI_GUARD].r, 1);
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
				if (CollisionBC(g_SkillUI[i].pos, g_Clickpos, g_SkillUI[i].r, 1) && i != UI_GUARD)
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
void UpdatePlayerIconUI(void)
{
	PLAYER* player = GetPlayer();
	if (player[0].hitting == TRUE)g_PlayerIcon.use = TRUE;
	else g_PlayerIcon.use = FALSE;

	if (g_PlayerIcon.use == FALSE)
	{
		g_PlayerIcon.theta = 0.0f;
		return;
	}

	g_PlayerIcon.theta += 1.5f;


}


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


	DrawSkillUI();
	DrawPlayerState();
	DrawPlayerIconUI();

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSkillUI(void)
{

	for (int i = 0; i < UI_MAX; i++)
	{
		{//プレイヤー表示
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_SkillUI[i].texNo]);	//0 : STAND, 1:walk 2

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_SkillUI[i].pos.x;	// プレイヤーの表示位置X
			float py = g_SkillUI[i].pos.y;	// プレイヤーの表示位置Y
			float pw = g_SkillUI[i].w;		// プレイヤーの表示幅
			float ph = g_SkillUI[i].h;		// プレイヤーの表示高さ

			
			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			float Alpha = 1.0f;

			if (g_SkillUI[i].use == FALSE) Alpha = 0.6;
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

void  DrawPlayerIconUI(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerIcon.texNo]);

	float tx = g_PlayerIcon.pos.x;
	float ty = g_PlayerIcon.pos.y;
	float tw = g_PlayerIcon.w;
	float th = g_PlayerIcon.h;

	float u = 0.0f;
	float v = 0.0f;
	float uw = 1.0f;
	float vh = 1.0f;

	float R = 1.0f;
	float G = 1.0f;
	float B = 1.0f;
	float Alpha = 1.0f;

	if (g_PlayerIcon.use == TRUE)
	{
		G = 0.0f, B = 0.0f, Alpha = 0.7f;

		tx += 10 * sin(g_PlayerIcon.theta);
		/*	ty += 5 * cos(g_PlayerIcon.theta);*/
		tw = g_PlayerIcon.w * (1 + 0.025 * sin(g_PlayerIcon.theta));
		th = g_PlayerIcon.h * (1 + 0.025 * sin(
			g_PlayerIcon.theta));
	}
	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);

	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
	//// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);


}

//プレイヤー状態表示
void DrawPlayerState(void)
{
	DrawPlayerHP();
	DrawPlayerMP();
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

		//ゲージの位置やテクスチャー座標を反映
		float px = 60.0f;		// ゲージの表示位置X
		float py = -10.0f;		// ゲージの表示位置Y
		float pw = 480.0f;	// ゲージの表示幅
		float ph = 405.0f;		// ゲージの表示高さ

		float tw = 1.0f;	// テクスチャの幅
		float th = 1.0f;	// テクスチャの高さ
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}

//プレイヤーのHP表示
void DrawPlayerHP(void)
{
	// HPの表示
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//ゲージの位置やテクスチャー座標を反映
		float px = 263.0f;		// ゲージの表示位置X
		float py = 135.0f;		// ゲージの表示位置Y
		float pw = 220;	// ゲージの表示幅
		float ph = 30.0f;		// ゲージの表示高さ

		float tw = 1.0f;	// テクスチャの幅
		float th = 1.0f;	// テクスチャの高さ
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


		// エネミーの数に従ってゲージの長さを表示してみる
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//ゲージの位置やテクスチャー座標を反映
		pw = pw * ((float)player[0].hp / PLAYER_HP_MAX);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


	}

}

//プレイヤーのMP表示
void DrawPlayerMP(void)
{
	// HPの表示
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//ゲージの位置やテクスチャー座標を反映
		float px = 263.0f;		// ゲージの表示位置X
		float py = 200.0f;		// ゲージの表示位置Y
		float pw = GAUGE_WIDTH;	// ゲージの表示幅
		float ph = GAUGE_HEIGHT;		// ゲージの表示高さ

		float tw = 1.0f;	// テクスチャの幅
		float th = 1.0f;	// テクスチャの高さ
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;	// テクスチャの左上Y座標

		float R = 0.0f;
		float G = 0.3f;
		float B = 0.8f;
		float Alpha = 1.0f;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//ゲージの位置やテクスチャー座標を反映
		float temp = ((float)player[0].mp / PLAYER_MP_MAX);
		pw = pw * temp;
		if (pw >= GAUGE_WIDTH)
		{
			R = 0.11765f;
			G = 0.941f;
			B = 1.0f;
		}
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(R, G, B, Alpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


	}

}



UI* GetSkillUI(void)
{
	return &g_SkillUI[0];
}

void SetSkillUIUSE(int type,BOOL tf)
{
	g_SkillUI[type].use = tf;
	return;
}

int	GetSkillUIClicked()
{

	if(g_SkillNum != UI_GUARD)return g_SkillNum;
	/*if (type == g_SkillNum)return TRUE;
	return FALSE;*/
}

int GetSkillUIPressed()
{
	return g_SkillNum;
}

BOOL isPressGuard(void)
{
	if(g_SkillNum == UI_GUARD)return TRUE;
	return FALSE;
}





