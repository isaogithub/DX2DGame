//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"
#include "bg.h"
#include "bg.h"
#include "player.h"
#include "field.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define EFFECT_TEXTURE_SIZE_X	(100)		// テクスチャサイズ
#define EFFECT_TEXTURE_SIZE_Y	(100)		// 同上

#define EFFECT_TEXTURE_PATTERN_DIVIDE_X	(4)	// アニメパターンのテクスチャ内分割数（X)
#define EFFECT_TEXTURE_PATTERN_DIVIDE_Y	(4)	// アニメパターンのテクスチャ内分割数（Y)
#define EFFECT_ANIM_PATTERN_NUM			(EFFECT_TEXTURE_PATTERN_DIVIDE_X*EFFECT_TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define EFFECT_TIME_ANIMATION			(1)	// アニメーションの切り替わるカウント

#define EMISSION_FULL 0		//パーティクル全生成フラグ
#define EMISSION_RATE 5		//パーティクルの生成間隔(duration/EMISSION_RATEの数分エフェクトが出る)

#define EMISSION_WIDTH  50	//パーティクル生成範囲（横幅）
#define EMISSION_HEIGHT 50	//パーティクル生成範囲（高さ）


#define TEXTURE_MAX					(2)		// テクスチャの数

//

#define HEAL_WAIT_MAX							(5)
#define HEAL_PATTERN_DIVIDE_X					(10)
#define HEAL_PATTERN_DIVIDE_Y					(1)
#define HEAL_ANIM_PATTERN_NUM	( HEAL_PATTERN_DIVIDE_X *  HEAL_PATTERN_DIVIDE_Y)	// アニメーションパターン数



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ResetParticle(int i, int n);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[EFFECT_TYPE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[] = {
	"data/TEXTURE/bomb.png",
	"data/EFFECT/effect_heal.png",
	"data/EFFECT/effect_jump.png",
	"data/EFFECT/effect_hit.png",
	"data/EFFECT/effect_attack002.png",
	"data/EFFECT/effect_guard.png",
	"data/EFFECT/ult01.png",
	"data/EFFECT/effect_ult2.png",
	"data/EFFECT/skill05_dash.png",
	"data/EFFECT/effect_bomb.png",
	"data/EFFECT/effect_hit96.png",
	"data/EFFECT/effect_hit192.png",
	"data/EFFECT/defend.png",
	"data/EFFECT/deadeffect.png",
	"data/EFFECT/effect_mpUp.png",
};


static BOOL		g_Load = FALSE;					// 初期化を行ったかのフラグ
static EFFECT	effectWk[EFFECT_NUM_EFFECTS];	// エフェクト構造体
static EFFECT2	g_healeffect;
static float	g_healtimeCnt;


static EFFECT3	g_Effect[EFFECT_MAX];
static ANIMATION g_effectAni[EFFECT_TYPE_MAX] =
{//divideX divideY animewait  w		 h
	{7,		1,		5,		120,	120,},//BOMB
	{7,		1,		5,		120,	120,},//HEAL1
	{7,		1,		5,		120,	120,},//JUMP
	{1,		1,		20,		 96,	96,},//HIT
	{5,		2,		1,		 384,	384,},//BLADE
	{5,		4,		1,		 192,	192,},//GUARD
	{1,		5,		20,		 2560,	1920,},//ULT1
	{1,		18,		10,		 640,	480,},//ULT2
	{1,		1,		1,		 500,	500,},//SKILL05
	{3,		3,		3,		 342,	342,},//BOMB2
	{5,		1,		2,		 96,	96,},//HIT2
	{5,		1,		2,		 192,	192,},//HIT3
	{1,		1,		20,		 500,	500,},//BOSS_DEFEND
	{15,	1,		3,		800,	800,},//BOSS_DeadEffect
	{5,		4,		1,		192,	192,},//BOSS_DeadEffect

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
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


	// 初期化処理
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		effectWk[i].use = false;
		effectWk[i].elapsed = 0;	//経過時間

		for (int n = 0; n < EFFECT_NUM_PARTS; n++)
		{
			ResetParticle(i, n);
		}
	}
	InitHealEffect();
	InitEffect3();

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// パーティクルのリセット
//=============================================================================
void ResetParticle(int i, int n) 
{
	effectWk[i].pParticle[n].pos = XMFLOAT3(effectWk[i].pos.x + rand() % EMISSION_WIDTH - (EMISSION_WIDTH/2), effectWk[i].pos.y + rand() % EMISSION_HEIGHT - (EMISSION_HEIGHT/2), 0.0f);	// 座標データを初期化
	effectWk[i].pParticle[n].move = XMFLOAT3(0.0f, 0.0f, 0.0f);					// 移動量

	effectWk[i].pParticle[n].PatternAnim = 0;									// アニメパターン番号をランダムで初期化
	effectWk[i].pParticle[n].CountAnim = 0;										// アニメカウントを初期化
	effectWk[i].pParticle[n].liveTime = 2000;
	effectWk[i].pParticle[n].isFinish = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
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
void UpdateEffect(void)
{
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use)
		{

			if (effectWk[i].isEnding){
				effectWk[i].use = FALSE;
				continue;
			}

			//エミットが有効であればエフェクト作成処理を行う
			if (effectWk[i].isRemoveOnFinish == FALSE)
			{

				//エフェクト作成レートの増加処理
				if (effectWk[i].effectCount < EFFECT_NUM_PARTS)
					effectWk[i].emitCounter++;

				//バッファに空きがあり、追加タイミングが来ていれば新たなエフェクトを追加する
				while ((effectWk[i].effectCount < EFFECT_NUM_PARTS) && (effectWk[i].emitCounter > EMISSION_RATE))
				{
					//全体追加フラグがONであれば空き領域全てに新たなエフェクトを追加する
					if (EMISSION_FULL)
						effectWk[i].effectCount = EFFECT_NUM_PARTS;
					else
						effectWk[i].effectCount++;

					//エフェクト作成レートの初期化
					effectWk[i].emitCounter = 0;
				}

				effectWk[i].elapsed++;

				//時間超過
				if ((effectWk[i].duration != -1) && (effectWk[i].duration < effectWk[i].elapsed)){
					effectWk[i].isRemoveOnFinish = TRUE;
				}
			}

			int effectIndex = 0;

			//エフェクトの更新処理
			while (effectIndex < effectWk[i].effectCount)
			{
				if (effectWk[i].pParticle[effectIndex].liveTime > 0)
				{
					//生存フレーム減少
					effectWk[i].pParticle[effectIndex].liveTime--;

					//アニメパターン進行
					if (++effectWk[i].pParticle[effectIndex].CountAnim > EFFECT_TIME_ANIMATION) {
						//アニメパターンが最大値に達した場合でも終了
						if (++effectWk[i].pParticle[effectIndex].PatternAnim >= (EFFECT_ANIM_PATTERN_NUM-1))
						{
							effectWk[i].pParticle[effectIndex].PatternAnim = EFFECT_ANIM_PATTERN_NUM - 1;
							effectWk[i].pParticle[effectIndex].liveTime = 0;
						}

						effectWk[i].pParticle[effectIndex].CountAnim = 0;
					}

					effectIndex++;
				}
				else{
					if (effectWk[i].isRemoveOnFinish){
						if (effectWk[i].pParticle[effectIndex].isFinish == 0)
						{
							effectWk[i].pParticle[effectIndex].isFinish = 1;
							effectWk[i].numFinish++;
						}
						//終了処理
						if (effectWk[i].numFinish == effectWk[i].effectCount)
						{
							effectWk[i].isEnding = TRUE;
							break;
						}
						effectIndex++;
					}
					else{
						//バッファを初期化する
						ResetParticle(i, effectIndex);

						//末尾でなければインデックスを詰める
						if (effectIndex != (effectWk[i].effectCount - 1))
						{
							effectWk[i].pParticle[effectIndex] = effectWk[i].pParticle[effectWk[i].effectCount - 1];
							ResetParticle(i, (effectWk[i].effectCount - 1));
						}
						effectWk[i].effectCount--;
					}
				}
			}
		}
	}

	UpdateHealEffect();
	UpdateEffect3();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
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

	BG* bg = GetBG();

	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == TRUE)		// このエフェクトが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			for (int n = 0; n < effectWk[i].effectCount; n++)
			{
				if (effectWk[i].pParticle[n].isFinish == 0) {
					//バレットの位置やテクスチャー座標を反映
					float px = effectWk[i].pParticle[n].pos.x - bg->pos.x;	// エフェクトの表示位置X
					float py = effectWk[i].pParticle[n].pos.y - bg->pos.y;	// エフェクトの表示位置Y
					float pw = EFFECT_TEXTURE_SIZE_X;			// エフェクトの表示幅
					float ph = EFFECT_TEXTURE_SIZE_Y;			// エフェクトの表示高さ

					px -= EFFECT_TEXTURE_SIZE_X / 4;
					py -= EFFECT_TEXTURE_SIZE_Y / 4;

					float tw = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
					float th = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
					float tx = (float)(effectWk[i].pParticle[n].PatternAnim % EFFECT_TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
					float ty = (float)(effectWk[i].pParticle[n].PatternAnim / EFFECT_TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteColorRotation(g_VertexBuffer,
						px, py, pw, ph,
						tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						0.0f);

					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}
	}

	DrawHealEffect();
	DrawEffect3();
}


//=============================================================================
// エフェクトのセット
// 
// int duration		エフェクト発生源の生存時間
//=============================================================================
void SetEffect(float x, float y, int duration)
{
	// もし未使用のエフェクトが無かったら実行しない( =これ以上表示できないって事 )
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == false)		// 未使用状態のエフェクトを見つける
		{
			effectWk[i].use = true;
			effectWk[i].isEnding = false;
			effectWk[i].isRemoveOnFinish = FALSE;

			effectWk[i].duration = duration;
			effectWk[i].pos.x = x;			// 座標をセット
			effectWk[i].pos.y = y;			// 座標をセット

			effectWk[i].effectCount = 0;
			effectWk[i].elapsed = 0;
			effectWk[i].emitCounter = EMISSION_RATE;
			effectWk[i].numFinish = 0;

			//パーティクルの初期化
			for (int n = 0; n < EFFECT_NUM_PARTS; n++)
			{
				ResetParticle(i, n);
			}

			return;							// 1個セットしたので終了する
		}
	}
}


void InitHealEffect(void)
{
	g_healeffect.use = FALSE;
	g_healeffect.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_healeffect.countAnim = 0;
	g_healeffect.patternAnim = 0;
}


void UpdateHealEffect(void)
{
	if (g_healeffect.use)
	{
		PLAYER* player = GetPlayer();
		g_healeffect.pos = player[0].pos;

		float waitMax = 1.0f;
		g_healtimeCnt += GetDeltatime();
		if (g_healtimeCnt >= waitMax * 1000.0f)
		{
			g_healtimeCnt = 0.0f;
			g_healeffect.use = FALSE;
		}


		if (g_healeffect.countAnim++ > HEAL_WAIT_MAX)
		{
			g_healeffect.countAnim = 0;
			// パターンの切り替え
			g_healeffect.patternAnim = (g_healeffect.patternAnim + 1) % HEAL_ANIM_PATTERN_NUM;
		}

	}
}
void DrawHealEffect(void)
{

	BG* bg = GetBG();

	if (g_healeffect.use == TRUE)		// このプレイヤーが使われている？
	{									// Yes
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_healeffect.pos.x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_healeffect.pos.y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = 150;		// プレイヤーの表示幅
		float ph = 150;		// プレイヤーの表示高さ

		// アニメーション用
		//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		float tw = 1.0f / HEAL_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
		float th = 1.0f / HEAL_PATTERN_DIVIDE_Y;								// テクスチャの高さ
		float tx = tw * (g_healeffect.patternAnim % HEAL_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
		float ty = th * (g_healeffect.patternAnim / HEAL_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

void SetHealEffect(XMFLOAT3 pos)
{
	if (g_healeffect.use == FALSE)
	{
		g_healeffect.use = TRUE;
		g_healeffect.pos = pos;

	}
	return;
}





void InitEffect3(void)
{

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].use = FALSE;
		g_Effect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Effect[i].countAnim = 0;
		g_Effect[i].patternAnim = 0;
		g_Effect[i].type = 0;
		g_Effect[i].effectCnt = 0;
		g_Effect[i].dir = RIGHT;
		g_Effect[i].w = 0.0f;
		g_Effect[i].h = 0.0f;
	}
	
}


void UpdateEffect3(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use)
		{
			int patternMax = g_effectAni[g_Effect[i].type].divideX * g_effectAni[g_Effect[i].type].divideY;
			int waitMax = g_effectAni[g_Effect[i].type].animewait;

			// && g_Effect[i].type != GUARD
			if ((g_Effect[i].effectCnt++ > patternMax * waitMax))
			{
				g_Effect[i].effectCnt = 0;
				g_Effect[i].use = FALSE;
			}
			
			if (g_Effect[i].countAnim++ > waitMax)
			{
				g_Effect[i].countAnim = 0;
				// パターンの切り替え
				g_Effect[i].patternAnim = (g_Effect[i].patternAnim + 1) % patternMax;
			}

		}
	}

}
void DrawEffect3(void)
{

	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect[i].type]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Effect[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Effect[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_effectAni[g_Effect[i].type].w;		// プレイヤーの表示幅
			float ph = g_effectAni[g_Effect[i].type].h;		// プレイヤーの表示高さ

			float tw = 1.0f / g_effectAni[g_Effect[i].type].divideX;							// テクスチャの幅	8パターン
			float th = 1.0f / g_effectAni[g_Effect[i].type].divideY;								// テクスチャの高さ
			float tx = tw * (g_Effect[i].patternAnim % g_effectAni[g_Effect[i].type].divideX);		// テクスチャの左上X座標
			float ty = th * (g_Effect[i].patternAnim / g_effectAni[g_Effect[i].type].divideX);								// テクスチャの左上Y座標

			float Alpha = 1.0f;
			if (g_Effect[i].dir == LEFT)
			{
				tw *= (-1);
			}
			if (g_Effect[i].type == ULT1 ||
				g_Effect[i].type == SKILL05||
				g_Effect[i].type == BOSS_DEFEND||
				g_Effect[i].type == MPUP)
			{
				Alpha = 0.3f;
			}
			if (g_Effect[i].type == ULT2 || g_Effect[i].type == ULT1)
			{
				if (GetMode() != MODE_BOSS)
				{
					px = g_Effect[i].pos.x;
					py = g_Effect[i].pos.y;
					pw = 1920.0f;
					ph = 1080.0f;
				}
				else
				{
					px = 1920.0f / 2;
					py = 1080.0f / 2;
					pw = 1920.0f;
					ph = 1080.0f;
				}
			}
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

	}

}

EFFECT3* GetEffect3(void)
{
	return &g_Effect[0];
}

void SetEffect3(XMFLOAT3 pos,int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)
		{
			g_Effect[i].use = TRUE;

			g_Effect[i].pos = pos;
			g_Effect[i].type = type;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
			g_Effect[i].w = g_effectAni[type].w;
			g_Effect[i].h = g_effectAni[type].h;

			return;

		}
	}
}

void SetEffect3(XMFLOAT3 pos, int type,int dir)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)
		{
			g_Effect[i].use = TRUE;

			g_Effect[i].pos = pos;
			g_Effect[i].type = type;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
			g_Effect[i].dir = dir;
			g_Effect[i].w = g_effectAni[type].w;
			g_Effect[i].h = g_effectAni[type].h;

			return;

		}
	}
}

void ResetEffect3(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].type == type)
		{
			g_Effect[i].use = FALSE;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
		}
	}
}

XMFLOAT3 GetEffect3Pos(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)continue;

		if (g_Effect[i].type == type)
		{
			return g_Effect[i].pos;
		}
	}
}

int GetEffect3Dir(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)continue;

		if (g_Effect[i].type == type)
		{
			return g_Effect[i].dir;
		}
	}
}


