//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : 
//
//=============================================================================
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "collision.h"
#include "bg.h"
#include "effect.h"
#include "sound.h"
#include "functionSet.h"
#include "field.h"
#include "hitscore.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// キャラサイズ
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(3)		// テクスチャの数

#define TEXTURE_NUMA_WIDTH			(120)	// キャラサイズ
#define TEXTURE_NUMA_HEIGHT			(82.5)	// 

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数


#define SKILL01_WAIT_MAX							(5)
#define SKILL01_PATTERN_DIVIDE_X					(5)
#define SKILL01_PATTERN_DIVIDE_Y					(3)
#define SKILL01_ANIM_PATTERN_NUM	(SKILL01_PATTERN_DIVIDE_X * SKILL01_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define BULLETHP_WAIT_MAX							(5)
#define BULLETHP_PATTERN_DIVIDE_X					(5)
#define BULLETHP_PATTERN_DIVIDE_Y					(2)
#define BULLETH_ANIM_PATTERN_NUM	(BULLETHP_PATTERN_DIVIDE_X * BULLETHP_PATTERN_DIVIDE_Y)	// アニメーションパタ


#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値

#define NUMA_WAIT_MAX				(8)

#define NUMA_PATTERN_DIVIDE_X		(1)		// アニメパターンのテクスチャ内分割数（X)
#define NUMA_PATTERN_DIVIDE_Y		(11)		// アニメパターンのテクスチャ内分割数（Y)
#define NUMA_PATTERN_NUM			(NUMA_PATTERN_DIVIDE_X*NUMA_PATTERN_DIVIDE_Y)	// アニメーションパターン数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[BULLET_MODE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[BULLET_MODE_MAX] = {
	"data/EFFECT/bullet01.png",
	"data/EFFECT/enemy_skill01.png",
	"data/EFFECT/numa.png",
	"data/EFFECT/bullet_hp.png",
	"data/EFFECT/player_bullet2.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static BULLET	g_Bullet[BULLET_MAX];	// バレット構造体
static NUMA		g_Numa[NUMA_MAX];

static ANIMATION g_bulletAni[] =
{//divideX divideY animewait  w		 h
	{5,		2,		10,		192,	192,},
	{5,		3,		3,		80,		80,},
	{1,		11,		5,		100,	75,},
	{5,		2,		5,		 60,	60,},
	{5,		5,		3,		 192,	192,},
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < BULLET_MODE_MAX; i++)
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


	// バレット構造体の初期化
	for (int i = 0; i < BULLET_MODE_MAX; i++)
	{
		g_Bullet[i].use   = FALSE;			// 未使用（発射されていない弾）
		g_Bullet[i].w     = 0;
		g_Bullet[i].h     = 0;
		g_Bullet[i].pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;

		g_Bullet[i].dir = BULLET_DIR_RIGHT;
		g_Bullet[i].countAnim = 0;
		g_Bullet[i].patternAnim = 0;

		//g_Bullet[i].move = XMFLOAT3(0.0f, -BULLET_SPEED, 0.0f);	// 移動量を初期化
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
		g_Bullet[i].damage = 0;
		g_Bullet[i].timer = 0.0f;	
		g_Bullet[i].frameCnt = 0;	
		g_Bullet[i].mode = BULLET_1;
		g_Bullet[i].gravityCnt = 0.0f;

	}

	for (int i = 0; i < NUMA_MAX; i++)
	{
		g_Numa[i].use = FALSE;			// 未使用（発射されていない弾）
		g_Numa[i].w = TEXTURE_NUMA_WIDTH;
		g_Numa[i].h = TEXTURE_NUMA_HEIGHT;
		g_Numa[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Numa[i].texNo = 2;
		g_Numa[i].timer = 0.0f;	// 移動量を初期化
		g_Numa[i].countAnim = 0;	// 移動量を初期化
		g_Numa[i].patternAnim = 0;	// 移動量を初期化

	}
	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
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

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	int bulletCount = 0;				// 処理したバレットの数
	PLAYER* player = GetPlayer();
	ENEMY * enemy  =  GetEnemy();
	BG* bg = GetBG();
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)continue;	// このバレットが使われている？

		// Yes
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float waitMax = 0.5f;//second


		// アニメーション  
		if ((g_Bullet[i].countAnim++ > g_bulletAni[g_Bullet[i].mode].animewait))
		{
			g_Bullet[i].countAnim = 0;
			// パターンの切り替え
			int divideX = g_bulletAni[g_Bullet[i].mode].divideX;
			int divideY = g_bulletAni[g_Bullet[i].mode].divideY;
			g_Bullet[i].patternAnim = (g_Bullet[i].patternAnim + 1) % (divideX * divideY);
		}

		//時間カウンター
		switch (g_Bullet[i].mode)
		{
		case	BLACKHOLE:
			if (g_Bullet[i].frameCnt++ >= 280)
			{
				g_Bullet[i].frameCnt = 0;
				g_Bullet[i].use = FALSE;
				continue;
			}
			break;
		}

		// バレットの移動処理
		switch (g_Bullet[i].mode)
		{
		case BLACKHOLE://重力弾
			
			{//移動
				XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				XMVECTOR vopos = XMLoadFloat3(&g_Bullet[i].pos);//前の座標を記憶する
				XMVECTOR vpos = XMLoadFloat3(&g_Bullet[i].pos);

				if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
				{
					move = XMFLOAT3(0.1, 0.0f, 0.0f);
				}
				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					move = XMFLOAT3(-0.1, 0.0f, 0.0f);

				}
				g_Bullet[i].move = move;

				XMVECTOR vmove = XMLoadFloat3(&g_Bullet[i].move);
				vpos += vmove;
				XMStoreFloat3(&g_Bullet[i].pos, vpos);
			}
			
			// 当たり判定処理
			if (GetMode() == MODE_BOSS)
			{//ボスに当たったら消える

				ENEMY* boss = GetBoss();
				for (int j = 0; j < BOSS_MAX; j++)
				{
					if (boss[j].use == FALSE)continue;
					int ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h,
						boss[j].pos, boss[j].w, boss[j].h);

					if (ans == TRUE)
					{
						g_Bullet[i].use = FALSE;
						boss[j].hit = TRUE;

						SetEffect3(g_Bullet[i].pos, HIT3);
						SetBDamagedType(DAMAGED_BULLET);
					}
				}
			}
			else
			{//ボス以外は吸い込まれる
				ENEMY* enemy = GetEnemy();
				for (int j = 0; j < ENEMY_MAX; j++)
				{// エネミーの数分当たり判定を行う
					if (enemy[j].use == TRUE)
					{// 生きてるエネミーと当たり判定をする

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, enemy[j].pos, enemy[j].w, enemy[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							enemy[j].hit = TRUE;

							SetEDamagedType(j, DAMAGED_BULLET);
							XMFLOAT3 epos = XMFLOAT3((g_Bullet[i].pos.x + enemy[j].pos.x) * 0.5,
								(g_Bullet[i].pos.y + enemy[j].pos.y) * 0.5, 0.0f);
							SetEffect3(g_Bullet[i].pos, HIT2);

						}
					}
				}
			}
			
		break;
		case BULLET_1:

		{//移動
			XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMVECTOR vopos = XMLoadFloat3(&g_Bullet[i].pos);//前の座標を記憶する
			XMVECTOR vpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				move = XMFLOAT3(15.0f, 0.0f, 0.0f);
			}
			if (g_Bullet[i].dir == BULLET_DIR_LEFT)
			{
				move = XMFLOAT3(-15.0f, 0.0f, 0.0f);

			}
			g_Bullet[i].move = move;

			XMVECTOR vmove = XMLoadFloat3(&g_Bullet[i].move);
			vpos += vmove;
			XMStoreFloat3(&g_Bullet[i].pos, vpos);
		}


		{// 当たり判定処理
			if (GetMode() == MODE_BOSS)
			{

				ENEMY* boss = GetBoss();
				for (int j = 0; j < BOSS_MAX; j++)
				{
					if (boss[j].use == FALSE)continue;

					int ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h,
						boss[j].pos, boss[j].w, boss[j].h);

					if (ans == TRUE)
					{
						g_Bullet[i].use = FALSE;
						boss[j].hit = TRUE;
						SetEffect3(g_Bullet[i].pos, HIT2);
						SetBDamagedType(DAMAGED_BULLET2);
					}
				}
			}
			else
			{
				ENEMY* enemy = GetEnemy();
				for (int j = 0; j < ENEMY_MAX; j++)
				{// エネミーの数分当たり判定を行う
					if (enemy[j].use == TRUE)
					{// 生きてるエネミーと当たり判定をする

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, enemy[j].pos, enemy[j].w, enemy[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							enemy[j].hit = TRUE;
							SetEffect3(g_Bullet[i].pos, HIT3);
							SetEDamagedType(j, DAMAGED_BULLET2);
							g_Bullet[i].use = FALSE;
						}
					}
				}
			}
			//
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w/2, g_Bullet[i].h / 2))
			{
				g_Bullet[i].use = FALSE;
			}

			//マップ外に出たら
			if (GetMode() == MODE_BOSS)
			{
				if (g_Bullet[i].pos.x >= SCREEN_WIDTH || g_Bullet[i].pos.x <= 0.0f)g_Bullet[i].use = 0;
				else if (g_Bullet[i].pos.y >= SCREEN_HEIGHT || g_Bullet[i].pos.y <= 0.0f)g_Bullet[i].use = 0;
			}
			else
			{
				if (g_Bullet[i].pos.x >= SCREEN_WIDTH * 2 || g_Bullet[i].pos.x <= 0.0f)g_Bullet[i].use = 0;
				else if (g_Bullet[i].pos.y >= SCREEN_HEIGHT * 3 || g_Bullet[i].pos.y <= 0.0f)g_Bullet[i].use = 0;

			}
		}


		break;
		case BULLET_MODE_ENEMY:

		{

			XMVECTOR vM = XMLoadFloat3(&g_Bullet[i].initMove);
			XMVECTOR vBpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				g_Bullet[i].initMove.x -= 0.2f;
				g_Bullet[i].initMove.y += 0.2f;
				if (g_Bullet[i].initMove.x <= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			else
			{
				g_Bullet[i].initMove.x += 0.2f;
				g_Bullet[i].initMove.y += 0.2f;

				if (g_Bullet[i].initMove.x >= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			XMStoreFloat3(&g_Bullet[i].pos, vBpos + vM);

			{
				g_Bullet[i].gravityCnt += GRAVITY;
				XMFLOAT3 nextpos = GravityPosCal(g_Bullet[i].pos, g_Bullet[i].gravityCnt);

				g_Bullet[i].pos = nextpos;
			}
			if (g_Bullet[i].pos.y >= MAP_HEIGHT)
			{
				g_Bullet[i].use = FALSE;
			}

			// 当たり判定処理
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{// エネミーの数分当たり判定を行う
					if (player[j].use == TRUE)
					{// 生きてるエネミーと当たり判定をする

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							player[j].hit = TRUE;
							int str = 10 + rand() % 10;
#ifndef _DEBUG	// リリース番だけHPを減る
							AddPlayerHP(j,-str);
#endif
							
							g_Bullet[i].use = FALSE;
						}
					}
				}
			}
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h))
			{
				SetNuma(g_Bullet[i].pos);
				g_Bullet[i].use = FALSE;
			}
			break;

		}
				
		break;
		case BULLET_MODE_HP:

			XMVECTOR vM = XMLoadFloat3(&g_Bullet[i].initMove);
			XMVECTOR vBpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				g_Bullet[i].initMove.x -= 0.2f;
				g_Bullet[i].initMove.y += 0.2f;
				if (g_Bullet[i].initMove.x <= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			else
			{
				g_Bullet[i].initMove.x += 0.2f;
				g_Bullet[i].initMove.y += 0.2f;

				if (g_Bullet[i].initMove.x >= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			XMStoreFloat3(&g_Bullet[i].pos, vBpos + vM);

			{
				g_Bullet[i].gravityCnt += GRAVITY;
				XMFLOAT3 nextpos = GravityPosCal(g_Bullet[i].pos, g_Bullet[i].gravityCnt);

				g_Bullet[i].pos = nextpos;
			}
			if (g_Bullet[i].pos.y >= MAP_HEIGHT)
			{
				g_Bullet[i].use = FALSE;
			}

			// 当たり判定処理
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{// エネミーの数分当たり判定を行う
					if (player[j].use == TRUE)
					{// 生きてるエネミーと当たり判定をする

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							AddPlayerHP(j,20);
							SetHealEffect(player[j].pos);
							SetHitScore(20, player[j].pos, HEAL);

							g_Bullet[i].use = FALSE;

							PlaySound(SOUND_LABEL_SE_heal001);
						}
					}
				}
			}
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h))
			{
				g_Bullet[i].use = FALSE;
			}


		break;

		
		}

		bulletCount++;
		
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
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
	PLAYER* player = GetPlayer();
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)continue;	
		// このバレットが使われている？
		
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bullet[i].mode]);
			
		//バレットの位置やテクスチャー座標を反映
		float px = g_Bullet[i].pos.x - bg->pos.x;	// バレットの表示位置X
		float py = g_Bullet[i].pos.y - bg->pos.y;	// バレットの表示位置Y
		float pw = g_bulletAni[g_Bullet[i].mode].w;		// バレットの表示幅
		float ph = g_bulletAni[g_Bullet[i].mode].h;		// バレットの表示高さ

		int divideX = g_bulletAni[g_Bullet[i].mode].divideX;
		int divideY = g_bulletAni[g_Bullet[i].mode].divideY;

		float tw = 1.0f / (float)divideX;	// テクスチャの幅
		float th = 1.0f / (float)divideY;	// テクスチャの高さ
		float tx = (float)(g_Bullet[i].patternAnim % divideX) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Bullet[i].patternAnim / divideX) * th;	// テクスチャの左上Y座標

		if (g_Bullet[i].dir == BULLET_DIR_LEFT)
		{
			tw *= (-1);
		}
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, 
			px, py, pw, ph, 
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			g_Bullet[i].rot.z);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		
	}

}


//=============================================================================
// バレット構造体の先頭アドレスを取得
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// バレットの発射設定
//=============================================================================
//void SetBullet(XMFLOAT3 pos)
//{
//	PLAYER* player = GetPlayer();
//	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
//	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
//	PlaySound(SOUND_LABEL_SE_skill001);
//	for (int i = 0; i < BULLET_MAX; i++)
//	{
//		if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
//		{
//			g_Bullet[i].use = TRUE;			// 使用状態へ変更する
//			g_Bullet[i].pos = pos;			// 座標をセット
//			g_Bullet[i].mode = BULLET_1;			// 座標をセット
//			g_Bullet[i].texNo = 0;
//			g_Bullet[i].w = 150;
//			g_Bullet[i].h = 150;
//			g_Bullet[i].countAnim = 0;
//			g_Bullet[i].patternAnim = 0;
//
//			if (player[0].dir == CHAR_DIR_LEFT)
//			{
//				g_Bullet[i].dir = BULLET_DIR_LEFT;
//			}
//			if (player[0].dir == CHAR_DIR_RIGHT)
//			{
//				g_Bullet[i].dir = BULLET_DIR_RIGHT;
//			}
//			player[0].bullet = FALSE;
//
//			return;							// 1発セットしたので終了する
//		}
//	}
//}

void SetBullet(XMFLOAT3 pos, BOOL mode)
{

	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	PLAYER* player = GetPlayer();

	switch (mode)
	{
	case BLACKHOLE:
		PlaySound(SOUND_LABEL_SE_gravityBall);

		// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
			{
				g_Bullet[i].use = TRUE;			// 使用状態へ変更する
				g_Bullet[i].pos = pos;			// 座標をセット
				g_Bullet[i].mode = mode;			// 座標をセット
				g_Bullet[i].texNo = mode;
				g_Bullet[i].w = g_bulletAni[mode].w;
				g_Bullet[i].h = g_bulletAni[mode].h;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				if (player[0].dir == CHAR_DIR_LEFT)
				{
					g_Bullet[i].dir = BULLET_DIR_LEFT;
				}
				if (player[0].dir == CHAR_DIR_RIGHT)
				{
					g_Bullet[i].dir = BULLET_DIR_RIGHT;
				}

				return;							// 1発セットしたので終了する
			}
		}

		break;
	case BULLET_1:

		PlaySound(SOUND_LABEL_SE_bullethp);

		// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
			{
				g_Bullet[i].use = TRUE;			// 使用状態へ変更する
				g_Bullet[i].pos = pos;			// 座標をセット
				g_Bullet[i].mode = mode;			// 座標をセット
				g_Bullet[i].texNo = mode;
				g_Bullet[i].w = g_bulletAni[mode].w;
				g_Bullet[i].h = g_bulletAni[mode].h;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;

				if (player[0].dir == CHAR_DIR_LEFT)
				{
					g_Bullet[i].dir = BULLET_DIR_LEFT;
				}
				if (player[0].dir == CHAR_DIR_RIGHT)
				{
					g_Bullet[i].dir = BULLET_DIR_RIGHT;
				}

				return;							// 1発セットしたので終了する
			}
		}
		break;
	}

	




}

void SetBullet(XMFLOAT3 pos,BOOL mode,int dir, XMFLOAT3 initMove)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();

	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	switch (mode)
	{
	case BULLET_MODE_ENEMY:
		PlaySound(SOUND_LABEL_SE_enemyskill01);
		break;
	case BULLET_MODE_HP:
		PlaySound(SOUND_LABEL_SE_bullethp);
		break;
	}

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			g_Bullet[i].use = TRUE;			// 使用状態へ変更する
			g_Bullet[i].mode = mode;

			switch (mode)
			{
			case BULLET_MODE_ENEMY:

				g_Bullet[i].texNo = 1;
				g_Bullet[i].mode = BULLET_MODE_ENEMY;
				g_Bullet[i].gravityCnt = 0;
				g_Bullet[i].w = 80;
				g_Bullet[i].h = 80;
				g_Bullet[i].pos = pos;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				//プレイヤーはエネミーの左側
				g_Bullet[i].dir = DisBulletDir();

				//if (player[0].pos.x <= enemy[ENEMY_MAX - 1].pos.x) g_Bullet[i].dir = BULLET_DIR_LEFT;
				////プレイヤーはエネミーの右側
				//else  if (player[0].pos.x > enemy[ENEMY_MAX - 1].pos.x)g_Bullet[i].dir = BULLET_DIR_RIGHT;

				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					XMFLOAT3 tempW = XMFLOAT3(-g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);

					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// 座標をセット
					g_Bullet[i].initMove = XMFLOAT3(-initMove.x, initMove.y, initMove.z);
				}
				else
				{
					XMFLOAT3 tempW = XMFLOAT3(g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);
					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// 座標をセット

					g_Bullet[i].initMove = XMFLOAT3(initMove.x, initMove.y, initMove.z);
				}
				
				break;
			case BULLET_MODE_HP:

				g_Bullet[i].texNo = 3;
				g_Bullet[i].mode = BULLET_MODE_HP;
				g_Bullet[i].gravityCnt = 0;
				g_Bullet[i].w = 60;
				g_Bullet[i].h = 60;
				g_Bullet[i].pos = pos;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				//プレイヤーはエネミーの左側
				g_Bullet[i].dir = DisBulletDir();

				//if (player[0].pos.x <= enemy[ENEMY_MAX - 1].pos.x) g_Bullet[i].dir = BULLET_DIR_LEFT;
				////プレイヤーはエネミーの右側
				//else  if (player[0].pos.x > enemy[ENEMY_MAX - 1].pos.x)g_Bullet[i].dir = BULLET_DIR_RIGHT;

				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					XMFLOAT3 tempW = XMFLOAT3(-g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);

					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// 座標をセット
					g_Bullet[i].initMove = XMFLOAT3(-initMove.x, initMove.y, initMove.z);
				}
				else
				{
					XMFLOAT3 tempW = XMFLOAT3(g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);
					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// 座標をセット

					g_Bullet[i].initMove = XMFLOAT3(initMove.x, initMove.y, initMove.z);
				}

				break;
			}
			return;							// 1発セットしたので終了する

		}
	}
}

//弾の発射はプレイヤーかエネミー
void SetBulletMode(int num,BOOL	mode)
{
	g_Bullet[num].mode = mode;
}

//敵の攻撃目標を設定する
void SetTarget(XMFLOAT3 pos)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)		// 未使用状態のバレットを見つける
		{
			g_Bullet[i].initMove = pos;
			return;							// 1発セットしたので終了する
		}
	}
}

void SetNuma(XMFLOAT3 pos)
{
	for (int i = 0; i < NUMA_MAX; i++)
	{
		if (g_Numa[i].use == FALSE)
		{
			g_Numa[i].use = TRUE;
			g_Numa[i].pos = pos;
			return;
		}
	}
}
void UpdateNuma(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < NUMA_MAX; i++)
	{
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (g_Numa[i].use == TRUE)
			{

				if (g_Numa[i].countAnim++ > NUMA_WAIT_MAX)
				{
					g_Numa[i].countAnim = 0;
					// パターンの切り替え
					g_Numa[i].patternAnim = (g_Numa[i].patternAnim + 1) % NUMA_PATTERN_NUM;
				}

				g_Numa[i].timer += GetDeltatime();
				float sec = 5.0;
				if (g_Numa[i].timer >= sec * 1000)
				{

					g_Numa[i].timer = 0;
					g_Numa[i].use = FALSE;

				}
				if (CollisionBB(player[j].pos, player[j].w, player[j].h, g_Numa[i].pos, g_Numa[i].w / 2 - 10.0f, g_Numa[i].h))
				{
					if (player[j].hitting == TRUE)
					{
						break;
					}
					else
					{
						player[j].hit = TRUE;


						#ifndef _DEBUG	// デバッグ版の時だけFPSを表示する
						AddPlayerHP(j,-5);
						#endif
						
					}
				}
			}
		}
	
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawNuma(void)
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
	PLAYER* player = GetPlayer();
	for (int i = 0; i < NUMA_MAX; i++)
	{
		if (g_Numa[i].use == TRUE)		// このバレットが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Numa[i].texNo]);

			//バレットの位置やテクスチャー座標を反映
			float px = g_Numa[i].pos.x - bg->pos.x;	// バレットの表示位置X
			float py = g_Numa[i].pos.y - bg->pos.y;	// バレットの表示位置Y
			float pw = g_Numa[i].w;		// バレットの表示幅
			float ph = g_Numa[i].h;		// バレットの表示高さ

			

			float tw = 1.0f / NUMA_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / NUMA_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = tw * (g_Numa[i].patternAnim % NUMA_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
			float ty = th * (g_Numa[i].patternAnim / NUMA_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標

			
			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

}