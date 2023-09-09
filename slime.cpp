//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "collision.h"
#include "score.h"
#include "field.h"
#include "slime.h" 

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(128)	// キャラサイズ
#define TEXTURE_HEIGHT				(128)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X * TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(8)		// アニメーションの切り替わるWait値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {

	"data/CHARA/test_slime.png",
};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static ENEMY	g_Slime[SLIME_MAX];	// エネミーの構造体
static float g_gravityCnt = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSlime(void)
{
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

	

	

	// エネミー構造体の初期化
	for (int i = 0; i < SLIME_MAX; i++)
	{

		g_Slime[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);			// ポリゴンの座標
		g_Slime[i].opos = g_Slime[i].pos;
		g_Slime[i].nextpos = XMFLOAT3(0.0f, 0.0f, 0.0f);	//プレイヤーの位置

		g_Slime[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);;			// ポリゴンの回転量
		g_Slime[i].scl = XMFLOAT3(0.0f, 0.0f, 0.0f);;			// ポリゴンの拡大縮小
		g_Slime[i].use = TRUE;			// true:使っている  false:未使用
		g_Slime[i].w = TEXTURE_WIDTH;			// 幅と高さ
		g_Slime[i].h = TEXTURE_HEIGHT;			// 幅と高さ

		g_Slime[i].countAnim  = 0;		// アニメーションカウント
		g_Slime[i].patternAnim = 0;	// アニメーションパターンナンバー
		g_Slime[i].texNo = 0;			// テクスチャ番号

		g_Slime[i].hp = 100;				// エネミーのHP
		g_Slime[i].hitCnt = 0;			// 当たり判定カウンタ
		g_Slime[i].hit = FALSE;			// 当てられた
		g_Slime[i].hitting = FALSE;		// 当てられた途中
		g_Slime[i].clear = FALSE;			// 透明フラグ
		g_Slime[i].onGround = FALSE;

		g_Slime[i].str = 5;			// 攻撃力
		g_Slime[i].dir = ENEMY_DIR_RIGHT;			//エネミー方向
		g_Slime[i].move = XMFLOAT3( 3.0 , 0.0 , 0.0 );			// 移動速度
		g_Slime[i].amove = XMFLOAT3(3.0 , 0.0 , 0.0);			// プレイヤーを見つけた速度

		g_Slime[i].time = 0;			// 線形補間用
		g_Slime[i].tblNo = 0;			// 行動データのテーブル番号
		g_Slime[i].tblMax = 0;			// そのテーブルのデータ数

	}
	
	g_Slime[0].pos = XMFLOAT3( 600.0f, 400.0f, 0.0f);
	g_Slime[1].pos = XMFLOAT3(1200.0f, 400.0f, 0.0f);
	g_Slime[2].pos = XMFLOAT3(1800.0f, 400.0f, 0.0f);
	g_Slime[3].pos = XMFLOAT3(2400.0f, 400.0f, 0.0f);
	g_Slime[4].pos = XMFLOAT3(3000.0f, 400.0f, 0.0f);

	

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSlime(void)
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
void UpdateSlime(void)
{
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// 生きてるエネミーのだけ処理をする
		if (g_Slime[i].use == TRUE)
		{
			// 地形との当たり判定用に座標のバックアップを取っておく
			g_Slime[i].opos = g_Slime[i].pos;


			// アニメーション  
			g_Slime[i].countAnim += 1.0f;
			if (g_Slime[i].countAnim > ANIM_WAIT)
			{
				g_Slime[i].countAnim = 0.0f;
				// パターンの切り替え
				g_Slime[i].patternAnim = (g_Slime[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			g_Slime[i].rot.z -= 0.0f;
			
			float speed;


			BG* bg = GetBG();
			{
				float speed = g_Slime[i].move.x;
				//g_Slime[i].pos.x += speed;

				//MAPの右端まで来た？
				if (g_Slime[i].pos.x > MAP_WIDTH)
				{
					g_Slime[i].pos.x = MAP_WIDTH;
					g_Slime[i].move.x = 0 - g_Slime[i].move.x;
				}


				//MAPの左端まで来た？
				if (g_Slime[i].pos.x  < 0.0f)
				{
					g_Slime[i].pos.x = 0.0f;
					g_Slime[i].move.x *= (-1);

				}
			}

			{
		
				// MAP外チェック
				//BG* bg = GetBG();

				if (g_Slime[i].pos.x < 0.0f)
				{
					g_Slime[i].pos.x = 0.0f;
				}

				if (g_Slime[i].pos.x > MAP_WIDTH)
				{
					g_Slime[i].pos.x = MAP_WIDTH;
				}

				if (g_Slime[i].pos.y < 0.0f)
				{
					g_Slime[i].pos.y = 0.0f;
				}

				if (g_Slime[i].pos.y > MAP_HEIGHT)
				{
					g_Slime[i].pos.y = MAP_HEIGHT;
				}

				// 移動が終わったらバレットとの当たり判定
				
				PLAYER* player = GetPlayer();
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					if (player[j].use == TRUE)
					{//生きているエネミーとあたりはんていをおこなう

						BOOL ans = CollisionBC(
							g_Slime[i].pos,
							player[j].pos,
							g_Slime[i].w / 2,
							player[j].w / 2);
						if (ans == TRUE)
						{
							g_Slime[i].use = FALSE;
							
							break;
						}
					}
				}
				//重力
				if (g_Slime[i].onGround == FALSE)
				{
					g_gravityCnt += GRAVITY;
					float acc = g_gravityCnt * 5;

					g_Slime[i].pos.y += g_gravityCnt * (acc * acc);
				}
				else
				{
					g_gravityCnt = 0;
				}

			}
		}
	}

#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSlime(void)
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

	for (int i = 0; i < SLIME_MAX; i++)
	{
		if (g_Slime[i].use == TRUE)		// このエネミーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//エネミーの位置やテクスチャー座標を反映
			float px = g_Slime[i].pos.x - bg->pos.x;	// エネミーの表示位置X
			float py = g_Slime[i].pos.y - bg->pos.y;	// エネミーの表示位置Y
			float pw = g_Slime[i].w;		// エネミーの表示幅
			float ph = g_Slime[i].h;		// エネミーの表示高さ

			 //アニメーション用
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Slime[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Slime[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Slime[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}


}


//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetSlime(void)
{
	return &g_Slime[0];
}




