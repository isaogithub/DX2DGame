/*******************************************************************************
* タイトル:		プログラム
* プログラム名:	.cpp
* 作成者:		GP11A132 29 林肇勳
* 作成日:		2022/04/18
*******************************************************************************/



/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "bg.h"
#include "field.h"
#include "main.h"
#include "player.h"

#include "enemy.h"
#include "slime.h"

#include "camera.h"
#include "collision.h"
#include "bg.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "tutorial.h"
#include "hitscore.h"
#include "effect.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/

//マップチップ
#define		C_MAX			(100500)
#define		MAPCHIPBLOCK_TEXTURE_WIDTH	(80)	//ブロックテクスチャ		
#define		MAPCHIPBLOCK_TEXTURE_HEIGHT	(80)


#define		TEXTURE_TUTORIAL_MAX	(4)
#define		TEXTURE_MAX		(17)		// テクスチャの数

#define		MAPCHIP_TUTORIAL_WIDTH	(10)		//マップチップ
#define		MAPCHIP_TUTORIAL_HEIGHT	(2)

#define		MAPCHIP_GAME_WIDTH	(10)		//マップチップ
#define		MAPCHIP_GAME_HEIGHT	(5)

#define		COLLUSION_BLOCK_ID	(14)	
#define		COLLUSION_BLOCK_ID_GAME	(19)	

//マップオブジェクト
#define		TRAP_MAX	(10)	
#define		ROPE_MAX	(1)	
#define		BUS_MAX		(2)
#define		TOGE_MAX	(5)
#define		PI			(3.1415926)

#define		TRAP_DAMAGE		(40.0f)

#define IOB_MAX									(2)

#define DROPITEM_MAX							(5)

#define DROPITEM_WAIT_MAX						(5)
#define DROPITEM_PATTERN_DIVIDE_X				(5)
#define DROPITEM_PATTERN_DIVIDE_Y				(2)
#define DROPITEM_PATTERN_MAX					(DROPITEM_PATTERN_DIVIDE_X * DROPITEM_PATTERN_DIVIDE_Y)

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報


static char* g_TexturName[TEXTURE_MAX] = {
	"data/MAPCHIP/mapchip80_game.png",
	"data/TEXTURE/[A]Grass1-Dirt3_pipo.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/trap.png",
	"data/TEXTURE/bus.png",
	"data/TEXTURE/rope.png",
	"data/TEXTURE/hint.png",
	"data/TEXTURE/key1.png",
	"data/TEXTURE/toge.png",
	"data/TEXTURE/faketrap.png",
	"data/EFFECT/bullet_hp.png",
	"data/MAPCHIP/mapchip80_tutorial.png",
	"data/TEXTURE/arrow.png",
	"data/TEXTURE/cave.png",
	"data/MAPCHIP/mapchip80_tutorial.png",
	"data/TEXTURE/door.png",//ヒロインの
	"data/CHARA/hiroine.png",//ヒロインの
};



int     g_y = 0;         // 行インデクス
int     g_x = 0;         // 列インデクス

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
												// * MAP_MAX
static int g_map_tutorial[SCREEN_HEIGHT / MAPCHIPBLOCK_TEXTURE_HEIGHT][MAP_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH];
static int g_map[42][MAP_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH];
static int g_map_boss[(SCREEN_HEIGHT / MAPCHIPBLOCK_TEXTURE_HEIGHT)][SCREEN_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH];

static BOOL g_GetKey;
static ITEM g_Arrow;
static ITEM g_Cave;
static ITEM g_Bus[BUS_MAX];
static ITEM2 g_Key;
static ROPE	g_Rope[ROPE_MAX];
static TRAP g_Trap[TRAP_MAX];
static HINT g_Hint;
static TOGE g_Toge[TOGE_MAX];

static DROP_ITEM g_Dropitem[DROPITEM_MAX];

INTERACT_OB g_IOBgame[IOB_MAX];


INTERACT_OB g_Door;
INTERACT_OB g_Heroine;
//

/*******************************************************************************
 関数名:
 引数　:
 戻り値:
 説明　:
*******************************************************************************/


HRESULT InitField(void)
{
	ID3D11Device* pDevice = GetDevice();
	g_GetKey = FALSE;

	LoadMap();
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


	switch (GetMode())
	{
	case MODE_TUTORIAL:



		{//矢印初期化
			g_Arrow.use = TRUE;
			g_Arrow.pos = XMFLOAT3(500.0f, 400.0f, 0.0f);
			g_Arrow.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_Arrow.w = 600.0f;
			g_Arrow.h = 600.0f;
			g_Arrow.texNo = TUTORIAL_ARROW;
		}
		{//洞窟初期化
			g_Cave.use = TRUE;
			g_Cave.pos = XMFLOAT3(3730.0f, 480.0f, 0.0f);
			g_Cave.scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_Cave.w = 750.0f;
			g_Cave.h = 450.0f;
			g_Cave.texNo = TUTORIAL_CAVE;
		}

		break;
	case MODE_GAME:

		

		//
		InitTrap();
		//バスの初期化処理
		InitBus();
		//ロープの初期化
		InitRope();
		InitKey();
		InitHint();
		InitIOB();
		InitToge();


		break;

	case MODE_BOSS:


		InitHint();
		InitIOB();

		break;
	}


	InitDropItem();




	g_Load = TRUE;
	return S_OK;
}

void UninitField(void)
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

void UpdateField(void)
{
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();

	switch (GetMode())
	{
	case MODE_TUTORIAL:
		//矢印アニメーション  更新処理
		g_Arrow.countAnim++;
		if (g_Arrow.countAnim >= 5)
		{
			g_Arrow.countAnim = 0;
			g_Arrow.patternAnim = (g_Arrow.patternAnim + 1) % 10;
		}

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_Cave.pos, g_Cave.w / 3, g_Cave.h))
			{
				SetFade(FADE_OUT, MODE_GAME);
			}

		}

		break;



	case MODE_GAME:

		//UpdateDropItem();

		UpdateTrap();
		UpdateIOB();
		UpdateBus();
		UpdateHint();
		UpdateKey();
		UpdateToge();
		break;

	case MODE_BOSS:

		UpdateIOB();

		//UpdateIOB();
		//UpdateHint();

		break;

	}

	UpdateDropItem();



#ifdef _DEBUG	// デバッグ情報を表示する


#endif
}

void DrawField(void)
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


	PLAYER* player = GetPlayer();
	// テクスチャ設定

	BG* bg = GetBG();

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);

		for (int y = 0; y < SCREEN_HEIGHT / MAPCHIPBLOCK_TEXTURE_HEIGHT + 3; y++)//1920*1080
		{//マップチップ表示
			for (int x = 0; x < SCREEN_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH + 2; x++)
			{
				{
					int chipID = g_map_tutorial[(int)(y + bg->pos.y / MAPCHIPBLOCK_TEXTURE_HEIGHT)][(int)(x + bg->pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH)];
					if (chipID == -1)continue;

					float mx = (float)(x * MAPCHIPBLOCK_TEXTURE_WIDTH) - fmodf(bg->pos.x, MAPCHIPBLOCK_TEXTURE_WIDTH);//ウィンドウの座標
					float my = (float)(y * MAPCHIPBLOCK_TEXTURE_HEIGHT) - fmodf(bg->pos.y, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					float mw = MAPCHIPBLOCK_TEXTURE_WIDTH;
					float mh = MAPCHIPBLOCK_TEXTURE_HEIGHT;


					float tw = 1.0f / (MAPCHIP_TUTORIAL_WIDTH);	// テクスチャの幅の比率
					float th = 1.0f / MAPCHIP_TUTORIAL_HEIGHT;	// テクスチャの高さの比率
					float tx = (float)(chipID % MAPCHIP_TUTORIAL_WIDTH * tw);	// テクスチャの左上X座標
					float ty = (float)(chipID / MAPCHIP_TUTORIAL_WIDTH * th);// テクスチャの左上Y座標_

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					float toumei = 1.0f;
					//if (tx == 0 && ty == 0) toumei = 0.0f;

					SetSpriteColor(g_VertexBuffer,
						mx, my, mw, mh,
						tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, toumei));
					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

				}
			}
		}

		{//矢印表示

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);

			float mx = (float)g_Arrow.pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Arrow.pos.y - bg->pos.y;
			float mw = g_Arrow.w;
			float mh = g_Arrow.h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f / 10;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = (float)(g_Arrow.patternAnim * th);// テクスチャの左上Y座標_

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float toumei = 1.0f;
			//if (tx == 0 && ty == 0) toumei = 0.0f;

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, toumei));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

		{//洞窟表示
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[13]);

			float mx = (float)g_Cave.pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Cave.pos.y - bg->pos.y;
			float mw = g_Cave.w;
			float mh = g_Cave.h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float toumei = 1.0f;
			//if (tx == 0 && ty == 0) toumei = 0.0f;

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, toumei));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		//DrawDropItem();

		break;

	case MODE_GAME:

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		for (int y = 0; y < SCREEN_HEIGHT / MAPCHIPBLOCK_TEXTURE_HEIGHT + 3; y++)//960*540
		{
			for (int x = 0; x < SCREEN_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH + 2; x++)
			{
				{

					int chipID = g_map[(int)(y + bg->pos.y / MAPCHIPBLOCK_TEXTURE_HEIGHT)][(int)(x + bg->pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH)];
					if (chipID == -1)continue;

					float mx = (float)(x * MAPCHIPBLOCK_TEXTURE_WIDTH) - fmodf(bg->pos.x, MAPCHIPBLOCK_TEXTURE_WIDTH);//ウィンドウの座標
					float my = (float)(y * MAPCHIPBLOCK_TEXTURE_HEIGHT) - fmodf(bg->pos.y, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					float mw = MAPCHIPBLOCK_TEXTURE_WIDTH;
					float mh = MAPCHIPBLOCK_TEXTURE_HEIGHT;



					float tw = 1.0f / (MAPCHIP_GAME_WIDTH);	// テクスチャの幅の比率
					float th = 1.0f / (MAPCHIP_GAME_HEIGHT);	// テクスチャの高さの比率
					float tx = (float)(chipID % MAPCHIP_GAME_WIDTH * tw);	// テクスチャの左上X座標
					float ty = (float)(chipID / MAPCHIP_GAME_WIDTH * th);// テクスチャの左上Y座標_


					// １枚のポリゴンの頂点とテクスチャ座標を設定
					float toumei = 1.0f;
					//if (tx == 0 && ty == 0) toumei = 0.0f;

					SetSpriteColor(g_VertexBuffer,
						mx, my, mw, mh,
						tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, toumei));
					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);

				}
			}
		}

		//DrawDropItem();
		DrawTrap();
		DrawFaketrap();
		DrawBus();
		DrawIOB();
		DrawHint();
		DrawKey();
		DrawToge();

		break;


	case MODE_BOSS:

		for (int y = 0; y < SCREEN_HEIGHT / MAPCHIPBLOCK_TEXTURE_HEIGHT + 3; y++)//960*540
		{
			for (int x = 0; x < SCREEN_WIDTH / MAPCHIPBLOCK_TEXTURE_WIDTH + 2; x++)
			{
				{
					/*GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[ g_mapChip [ g_map[y][x] ].texNo ]);*/
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

					//float mx = (float)( x * TEXTURE_WIDTH ) - fmodf(scrollx, TEXTURE_WIDTH);//ウィンドウの座標
					//float my = (float)( y * TEXTURE_HEIGHT) - fmodf(scrolly, TEXTURE_HEIGHT);
					//float mx = (float)(x * TEXTURE_WIDTH) + TEXTURE_WIDTH / 2 - bg->pos.x;//ウィンドウの座標
					int chipID = g_map_boss[(int)(y + bg->pos.y / MAPCHIPBLOCK_TEXTURE_HEIGHT)][(int)(x + bg->pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH)];
					if (chipID == -1)continue;

					float mx = (float)(x * MAPCHIPBLOCK_TEXTURE_WIDTH);//ウィンドウの座標
					float my = (float)(y * MAPCHIPBLOCK_TEXTURE_HEIGHT);
					float mw = MAPCHIPBLOCK_TEXTURE_WIDTH;
					float mh = MAPCHIPBLOCK_TEXTURE_HEIGHT;



					float tw = 1.0f / (MAPCHIP_GAME_WIDTH);	// テクスチャの幅の比率
					float th = 1.0f / (MAPCHIP_GAME_HEIGHT);	// テクスチャの高さの比率
					float tx = (float)(chipID % MAPCHIP_GAME_WIDTH * tw);	// テクスチャの左上X座標
					float ty = (float)(chipID / MAPCHIP_GAME_WIDTH * th);// テクスチャの左上Y座標_


					// １枚のポリゴンの頂点とテクスチャ座標を設定
					float toumei = 1.0f;
					//if (tx == 0 && ty == 0) toumei = 0.0f;

					SetSpriteColor(g_VertexBuffer,
						mx, my, mw, mh,
						tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, toumei));
					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}
		DrawIOB();
		DrawHint();
		break;
	}

	DrawDropItem();
}

//引数の位置を中心に全マップの座標を参照し、今画面の大きさによるXの初期位置を求める

float GetScrollX(XMFLOAT3 pos)
{
	float startx = pos.x - SCREEN_WIDTH / 2;

	if (startx < 0.0f)
	{
		startx = 0.0f;
	}
	if (startx > MAP_WIDTH - SCREEN_WIDTH)
	{
		startx = MAP_WIDTH - SCREEN_WIDTH;
	}

	return startx;

}

float GetScrollY(XMFLOAT3 pos)
{
	float starty = pos.y - SCREEN_HEIGHT / 2;

	if (starty < 0.0f)
	{
		starty = 0.0f;
	}
	if (starty > MAP_HEIGHT - SCREEN_HEIGHT)
	{
		starty = MAP_HEIGHT - SCREEN_HEIGHT;
	}

	return starty;
}

float GetStartCollusionX(XMFLOAT3 pos)
{

	float startx = pos.x - MAPCHIPBLOCK_TEXTURE_WIDTH;

	if (startx < 0.0f)
	{
		startx = 0.0f;
	}

	if (GetMode() != MODE_BOSS)
	{
		if (startx > MAP_WIDTH - MAPCHIPBLOCK_TEXTURE_WIDTH)
		{
			startx = MAP_WIDTH - MAPCHIPBLOCK_TEXTURE_WIDTH;
		}
	}
	else
	{
		if (startx > SCREEN_WIDTH - MAPCHIPBLOCK_TEXTURE_WIDTH)
		{
			startx = SCREEN_WIDTH - MAPCHIPBLOCK_TEXTURE_WIDTH;
		}
	}
	return startx;
}

//引数の位置を中心に全マップの座標を参照し、、今画面の大きさによるYの初期位置を求める
float GetStartCollusionY(XMFLOAT3 pos)
{
	float starty = pos.y - MAPCHIPBLOCK_TEXTURE_HEIGHT;

	if (starty < 0.0f)
	{
		starty = 0.0f;
	}
	if (GetMode() != MODE_BOSS)
	{
		if (starty > MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}
	else
	{
		if (starty > SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}

	return starty;
}


//引数の位置を中心に全マップの座標を参照し、、今画面の大きさによるYの初期位置を求める
float GetStartDownCollusionY(XMFLOAT3 pos)
{
	float starty = pos.y + MAPCHIPBLOCK_TEXTURE_HEIGHT / 2;

	if (starty < 0.0f)
	{
		starty = 0.0f;
	}

	if (GetMode() != MODE_BOSS)
	{
		if (starty > MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}
	else
	{
		if (starty > SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}

	return starty;
}
float GetStartWaistCollusionY(XMFLOAT3 pos)
{
	float starty = pos.y;

	if (starty < 0.0f)
	{
		starty = 0.0f;
	}

	if (GetMode() != MODE_BOSS)
	{
		if (starty > MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = MAP_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}
	else
	{
		if (starty > SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT)
		{
			starty = SCREEN_HEIGHT - MAPCHIPBLOCK_TEXTURE_HEIGHT;
		}
	}

	return starty;
}

//フィールドのアドレスを返す
int* GetFieldPtr(void)
{
	return &g_map[0][0];
}
//指定されたＸＹ座標のフィールドデータを返す
int GetFieldData(int y, int x)
{
	return g_map[y][x];
}

//posの座標はマップチップの当たり判定BLOCKと同じ？
BOOL FieldCollision(XMFLOAT3 pos, float width, float height)
{
	int y = (int)pos.y / MAPCHIPBLOCK_TEXTURE_WIDTH;
	int x = (int)pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH;

	float startx = GetStartCollusionX(pos);
	float starty = GetStartCollusionY(pos);

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_tutorial[y][x] >= COLLUSION_BLOCK_ID)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_GAME:
		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_BOSS:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_boss[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}


		break;
	default:
		break;
	}

	return FALSE;
}

BOOL DownFieldCollision(XMFLOAT3 pos, float width, float height)
{
	int y = (int)pos.y / MAPCHIPBLOCK_TEXTURE_WIDTH;
	int x = (int)pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH;

	float startx = GetStartCollusionX(pos);
	float starty = GetStartDownCollusionY(pos);;

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_tutorial[y][x] >= COLLUSION_BLOCK_ID)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_GAME:
		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_BOSS:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_boss[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}


		break;
	default:
		break;
	}

	return FALSE;
}

BOOL WaistFieldCollision(XMFLOAT3 pos, float width, float height)
{
	int y = (int)pos.y / MAPCHIPBLOCK_TEXTURE_WIDTH;
	int x = (int)pos.x / MAPCHIPBLOCK_TEXTURE_WIDTH;

	float startx = GetStartCollusionX(pos);
	float starty = GetStartWaistCollusionY(pos);;

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_tutorial[y][x] >= COLLUSION_BLOCK_ID)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_GAME:
		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}
		break;

	case MODE_BOSS:

		for (int y = (int)(starty / MAPCHIPBLOCK_TEXTURE_HEIGHT); y <= (int)((starty + SCREEN_HEIGHT) / MAPCHIPBLOCK_TEXTURE_HEIGHT); y++)
		{
			for (int x = (int)(startx / MAPCHIPBLOCK_TEXTURE_WIDTH); x <= (int)((startx + SCREEN_WIDTH) / MAPCHIPBLOCK_TEXTURE_HEIGHT); x++)
			{
				if (g_map_boss[y][x] >= COLLUSION_BLOCK_ID_GAME)
				{

					XMFLOAT3 block = XMFLOAT3((float)x * MAPCHIPBLOCK_TEXTURE_WIDTH, (float)y * MAPCHIPBLOCK_TEXTURE_HEIGHT, 0.0f);
					BOOL ans = CollisionBB(pos, width, height, block, MAPCHIPBLOCK_TEXTURE_WIDTH, MAPCHIPBLOCK_TEXTURE_HEIGHT);
					if (ans == TRUE)
					{
						return TRUE;
					}
				}
			}
		}


		break;
	default:
		break;
	}

	return FALSE;
}


//マップロード
void LoadMap(void)
{
	// char    * fnm = "test.csv";
	FILE* csv;
	FILE* fp = NULL;

	char    bff[C_MAX];   // 実数文字列
	char* c;        // 入力文字
	  // 格納先配列

	int		buff;
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		csv = fopen("data/MAPCHIP/map80_tutorial.csv", "r");

		if (csv == NULL)return;

		while (fgets(bff, C_MAX, csv) != NULL) {
			c = bff;
			//printf("%s", c);
			while ((c = strtok(c, ",\n")) != NULL) {

				buff = atoi(c);
				g_map_tutorial[g_y][g_x] = buff;
				//printf("%d ", db[i][j]);

				c = NULL;
				g_x++;
			}
			g_y++;
			g_x = 0;
		}
		g_y = 0;
		g_x = 0;
		fclose(csv);

		break;
	case MODE_GAME:

		csv = fopen("data/MAPCHIP/map80_game.csv", "r");

		if (csv == NULL)return;

		while (fgets(bff, C_MAX, csv) != NULL) {
			c = bff;
			//printf("%s", c);
			while ((c = strtok(c, ",\n")) != NULL) {

				buff = atoi(c);
				g_map[g_y][g_x] = buff;
				//printf("%d ", db[i][j]);

				c = NULL;
				g_x++;
			}
			g_y++;
			g_x = 0;
		}
		g_y = 0;
		g_x = 0;
		fclose(csv);

		break;

	case MODE_BOSS:

		csv = fopen("data/MAPCHIP/map80_boss.csv", "r");

		if (csv == NULL)return;

		while (fgets(bff, C_MAX, csv) != NULL) {
			c = bff;
			//printf("%s", c);
			while ((c = strtok(c, ",\n")) != NULL) {

				buff = atoi(c);
				g_map_boss[g_y][g_x] = buff;
				//printf("%d ", db[i][j]);

				c = NULL;
				g_x++;
			}
			g_y++;
			g_x = 0;
		}
		g_y = 0;
		g_x = 0;
		fclose(csv);

		break;


	default:
		break;
	}


	return;
}

//重力位置計算
XMFLOAT3 GravityPosCal(XMFLOAT3 pos, float gravityCnt)
{

	XMFLOAT3 nextpos = pos;

	float acc = gravityCnt * 5;
	float totalmove = gravityCnt * (acc * acc);
	nextpos.y += totalmove;
	return  nextpos;

}

void RopeUpdate(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player[i].use)
		{
			for (int j = 0; j < ROPE_MAX; j++)
			{
				if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_Rope[j].pos, g_Rope[j].w, g_Rope[j].h));
				{
					g_Rope[j].use = TRUE;

				}
			}
		}
	}
}

void Arcmove(void)
{
	for (int i = 0; i < ROPE_MAX; i++)
	{
		if (g_Rope[i].use)
		{
			g_Rope[i].theta += 0.03;
			// 指定された座標を中心に回転するプログラム
			//float  ropeRP = g_Rope[i].pos;

			float x0 = g_Rope[i].pos.x;
			float y0 = g_Rope[i].pos.y - g_Rope[i].h * 0.5f;
			float r = g_Rope[i].h;
			float theta = g_Rope[i].theta;
			float sin = cosf(theta * PI / 180.0f);
			float x1 = x0 + r * cosf(theta * PI / 180.0f);
			float y1 = y0 + r * sin;

			g_Rope[i].pos = XMFLOAT3(x1, y1, 0.0f);


			//float hw = g_Rope[i].w;
			//float hh = g_Rope[i].h;

			//float BaseAngle = atan2f(hh, hw);			// 中心点から頂点に対する角度
			//XMVECTOR temp = { hw, hh, 0.0f, 0.0f };
			//temp = XMVector2Length(temp);				// 中心点から頂点に対する距離
			//float Radius = hh;
			//XMStoreFloat(&Radius, temp);

			//// ここでアフィン変換（sincosのやつ）を使って4頂点を回転させる
			//float x = g_Rope[i].pos.x - cosf(BaseAngle + g_Rope[i].theta) * Radius;
			//float y = g_Rope[i].pos.y - sinf(BaseAngle + g_Rope[i].theta) * Radius;
			//g_Rope[i].pos = XMFLOAT3(x, y, 0.0f);

		}
	}
}

void InitTrap(void)
{

	for (int i = 0; i < TRAP_MAX; i++)
	{
		g_Trap[i].use = FALSE;
		g_Trap[i].trig = FALSE;
		g_Trap[i].pos = XMFLOAT3(100.0f + i * 400.0f, 161.0f, 0.0f);
		g_Trap[i].w = 40.0f;
		g_Trap[i].h = 60.0f;
		g_Trap[i].texNo = 3;
		g_Trap[i].gravityCnt = 0.0f;
	}
	for (int i = 0; i < 5; i++)
	{
		g_Trap[i].use = TRUE;
	}

	g_Trap[0].pos = XMFLOAT3( 940.0f,155.0f, 0.0f);
	g_Trap[1].pos = XMFLOAT3(1536.0f,155.0f, 0.0f);
	g_Trap[2].pos = XMFLOAT3(1700.0f,155.0f, 0.0f);
	g_Trap[3].pos = XMFLOAT3(1990.0f,155.0f, 0.0f);
	g_Trap[4].pos = XMFLOAT3(3700.0f,155.0f, 0.0f);

}

void UpdateTrap(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (g_Trap[j].use)
			{

				BOOL ans;
				//プレイヤーの位置がトラップの下？YES　トラップ発動
				XMFLOAT3 triggerpos = XMFLOAT3(g_Trap[j].pos.x, player[i].pos.y, 0.0f);
				if (CollisionBB(player[i].pos, player[i].w, player[i].h, triggerpos, g_Trap[j].w, g_Trap[j].h) &&
					g_Trap[j].trig == FALSE)
				{
					SetVolumeSound(SOUND_LABEL_SE_trapfall, 0.4f);
					PlaySound(SOUND_LABEL_SE_trapfall);
					g_Trap[j].trig = TRUE;
				}
				//トラップ発動したら
				if (g_Trap[j].trig)
				{
					g_Trap[j].gravityCnt += GRAVITY * 2;
					XMFLOAT3 nextpos = GravityPosCal(g_Trap[j].pos, g_Trap[j].gravityCnt);

					g_Trap[j].pos = nextpos;
				}
				if (g_Trap[j].pos.y >= SCREEN_HEIGHT)
				{
					g_Trap[j].use = FALSE;
				}
				//トラップがプレイヤーに当たったら
				if (CollisionBC(player[i].pos, g_Trap[j].pos, player[i].w / 2, g_Trap[j].w / 2))
				{
#ifndef _DEBUG	// リリース番だけHPを減る
					player[i].hit = TRUE;
					g_Trap[j].use = FALSE;
					AddPlayerHP(i, -50.0f);
					//player[i].hp -= TRAP_DAMAGE;
					//SetHitScore(-TRAP_DAMAGE, player[i].pos, DAMAGE_PLAYER);
					//SetEffect3(g_Trap[j].pos, HIT2);
#endif

				}
				else if (FieldCollision(g_Trap[j].pos, g_Trap[j].w, g_Trap[j].h))
				{
					g_Trap[j].use = FALSE;
				}

			}
			else
			{
				g_Trap[j].trig = FALSE;
			}
		}
	}
}

void DrawTrap(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < TRAP_MAX; i++)
	{
		//if (g_Trap[i].use)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Trap[i].texNo]);

			float mx = (float)g_Trap[i].pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Trap[i].pos.y - bg->pos.y;
			float mw = g_Trap[i].w;
			float mh = g_Trap[i].h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


void DrawFaketrap(void)
{
	BG* bg = GetBG();
	//とげとげを描画する
	for (int i = 0; i < 9; i++)
	{

		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

			float mx = (float)100.0f + i * 400 - bg->pos.x;//ウィンドウの座標
			float my = (float)70.0f - bg->pos.y;
			float mw = 400;
			float mh = 60;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


	}
}


void InitBus(void)
{
	for (int i = 0; i < BUS_MAX; i++)
	{
		g_Bus[i].use = TRUE;
		g_Bus[i].w = 130.0f;
		g_Bus[i].h = 60.0f;
		g_Bus[i].texNo = 4;
		g_Bus[i].move = XMFLOAT3(4.0f, 2.0f, 0.0f);
	}
	g_Bus[0].pos = XMFLOAT3(2700.0f, 1340.0f, 0.0f);
	g_Bus[0].type = LR_BUS;
	g_Bus[1].pos = XMFLOAT3(1130.0f, 2500.0f, 0.0f);
	g_Bus[1].type = UD_BUS;
}
void UpdateBus(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		for (int j = 0; j < BUS_MAX; j++)
		{
			{//バスの移動処理
				XMFLOAT3 tempPos = g_Bus[j].pos;

				switch (g_Bus[j].type)
				{
				case LR_BUS:

					tempPos.x += g_Bus[j].w / 2;
					if (FieldCollision(tempPos, g_Bus[j].w / 10, g_Bus[j].h))
					{
						g_Bus[j].move.x *= (-1);
					}

					tempPos = g_Bus[j].pos;
					tempPos.x -= g_Bus[j].w / 2;

					if (FieldCollision(tempPos, g_Bus[j].w / 10, g_Bus[j].h))
					{
						g_Bus[j].move.x *= (-1);
					}
					g_Bus[j].pos.x += g_Bus[j].move.x;

					{//バスとプレイヤーの当たり判定
						XMFLOAT3 PfootPos = XMFLOAT3(player[i].pos.x, player[i].pos.y + player[i].h / 2, 0.0f);
						if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, PfootPos, TEXTURE_COLLISION_FOOT_WIDTH, TEXTURE_COLLISION_FOOT_HEIGHT))
							//if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, player[i].pos, player[i].w, player[i].h))
						{
							player[i].pos.x += g_Bus[j].move.x;
							player[i].onGround = TRUE;
						}
					}

					break;
				case UD_BUS:

					//tempPos.x += g_Bus[j].w / 2;
					if (FieldCollision(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h) || g_Bus[j].pos.y == 2350.0f)
					{
						g_Bus[j].move.y *= (-1);
					}

					g_Bus[j].pos.y += g_Bus[j].move.y;

					{//バスとプレイヤーの当たり判定

						XMFLOAT3 headPos = XMFLOAT3(player[i].pos.x, player[i].pos.y - player[i].h / 2, 0.0f);//頭
						XMFLOAT3 footPos = XMFLOAT3(player[i].pos.x, player[i].pos.y + player[i].h / 2, 0.0f);//足元

						if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, headPos, TEXTURE_COLLISION_FOOT_WIDTH, 1.0f))
							//if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, player[i].pos, player[i].w, player[i].h))
						{
							JumpReset();
						}

						else if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, footPos, TEXTURE_COLLISION_FOOT_WIDTH, 1.0f))
							//if (CollisionBB(g_Bus[j].pos, g_Bus[j].w, g_Bus[j].h, player[i].pos, player[i].w, player[i].h))
						{

							player[i].pos.y += g_Bus[j].move.y;
							player[i].onGround = TRUE;
						}
					}

					break;
				}

			}

		}

	}

}

void DrawBus(void)
{
	BG* bg = GetBG();
	//バスを描画する
	for (int i = 0; i < BUS_MAX; i++)
	{
		if (g_Bus[i].use)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bus[i].texNo]);

			float mx = (float)g_Bus[i].pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Bus[i].pos.y - bg->pos.y;
			float mw = g_Bus[i].w;
			float mh = g_Bus[i].h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void InitRope(void)
{
	for (int i = 0; i < ROPE_MAX; i++)
	{
		g_Rope[i].use = TRUE;
		g_Rope[i].pos = XMFLOAT3(500, 450.0f, 0.0f);
		g_Rope[i].w = 50.0f;
		g_Rope[i].h = 250.0f;
		g_Rope[i].texNo = 5;
		g_Rope[i].theta = 270.0f;
		/*	g_Rope[i].x = 0.0f;
			g_Rope[i].y= 0.0f;*/

	}
}
void InitHint(void)
{
	g_Hint.use = FALSE;
	g_Hint.w = 80;
	g_Hint.h = 80;
	g_Hint.texNo = 6;
	g_Hint.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void UpdateHint(void)
{
	INTERACT_OB* iob = GetIOB();
	PLAYER* player = GetPlayer();
	for (int i = 0; i < IOB_MAX; i++)
	{
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (CollisionBB(player[j].pos, player[j].w, player[j].h, iob[i].pos, iob[i].w, iob[i].h))
			{
				g_Hint.use = TRUE;
				g_Hint.pos = iob[i].pos;
				return;

			}
			else
			{
				g_Hint.use = FALSE;
				g_Hint.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

			}
		}
	}
}

void DrawHint(void)
{
	BG* bg = GetBG();
	if (g_Hint.use)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Hint.texNo]);

		float mx = (float)g_Hint.pos.x - bg->pos.x;//ウィンドウの座標
		float my = (float)g_Hint.pos.y - bg->pos.y;
		float mw = g_Hint.w;
		float mh = g_Hint.h;

		float tw = 1.0f;	// テクスチャの幅の比率
		float th = 1.0f;		// テクスチャの高さの比率
		float tx = 0.0f;	// テクスチャの左上X座標
		float ty = 0.0f;// テクスチャの左上Y座標_

		SetSpriteColor(g_VertexBuffer,
			mx, my, mw, mh,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}

INTERACT_OB* GetIOB(void)
{
	return &g_IOBgame[0];
}

void InitIOB(void)
{
	g_Door.use = TRUE;
	g_Door.col = FALSE;
	g_Door.display = FALSE;
	g_Door.pos = XMFLOAT3(2000.0f, 2310.0f, 0.0f);
	g_Door.texNo = 15;
	g_Door.w = 270;
	g_Door.h = 290;

	g_Heroine.use = TRUE;
	g_Heroine.col = FALSE;
	g_Heroine.display = FALSE;
	g_Heroine.pos = XMFLOAT3(1750.0f, 680.0f, 0.0f);
	g_Heroine.texNo = 16;
	g_Heroine.w = 300;
	g_Heroine.h = 500;

	return;
}

void UpdateIOB(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		switch (GetMode())
		{
		case MODE_GAME:

			if (g_Door.use)
			{
				if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_Door.pos, g_Door.w - 10.0f, g_Door.h))
				{//プレイヤーがフィールドのinteractionOBJと当たったら
					g_Door.col = TRUE;

					if (!isGetKey())
					{
						player[i].pos = player[i].opos;
						SetMessage(NOKEY);
					}
					else
					{
						SetMessage(DOOR);
					}
				}
				else
				{
					g_Door.col = FALSE;
					SetMessage(-1);
				}
			}

			break;
		case MODE_BOSS:

			if (g_Heroine.use == TRUE)
			{
				if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_Heroine.pos, g_Heroine.w - 10.0f, g_Heroine.h))
				{//プレイヤーがフィールドのinteractionOBJと当たったら
					g_Heroine.col = TRUE;

					if (!isBossDead())
					{
						player[i].pos = player[i].opos;
						SetMessage(BOSSNODEAD);
					}
					else
					{
						SetMessage(HEROINE);
					}

				}
				else
				{
					g_Heroine.col = FALSE;
					SetMessage(-1);
				}
			}
			break;

		}

	}




}

INTERACT_OB* GetDoor(void)
{
	return &g_Door;
}

INTERACT_OB* GetHeroine(void)
{
	return &g_Heroine;
}






void DrawIOB(void)
{
	BG* bg = GetBG();

	switch (GetMode())
	{
	case MODE_GAME:
		if (g_Door.use == TRUE)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Door.texNo]);

			float mx = (float)g_Door.pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Door.pos.y - bg->pos.y;
			float mw = g_Door.w;
			float mh = g_Door.h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		break;
	case MODE_BOSS:

		if (g_Heroine.use == TRUE)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Heroine.texNo]);

			float mx = (float)g_Heroine.pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Heroine.pos.y - bg->pos.y;
			float mw = g_Heroine.w;
			float mh = g_Heroine.h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


		break;

	}

}


void InitKey(void)
{
	if (GetMode() == MODE_GAME)
	{
		g_Key.use = TRUE;
		g_Key.pos = XMFLOAT3(535.0f, 2650.0f, 0.0f);
		g_Key.texNo = 7;
		g_Key.w = 160;
		g_Key.h = 160;
	}
}
void UpdateKey(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player[i].use && g_Key.use)
		{

			if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_Key.pos, g_Key.w, g_Key.h))
			{
				PlaySound(SOUND_LABEL_SE_key);
				g_GetKey = TRUE;
				g_Key.use = FALSE;
			}
		}
	}
}
void DrawKey(void)
{
	BG* bg = GetBG();

	if (g_Key.use == TRUE)		// このプレイヤーが使われている？
	{									// Yes
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Key.texNo]);

		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Key.pos.x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Key.pos.y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Key.w;		// プレイヤーの表示幅
		float ph = g_Key.h;		// プレイヤーの表示高さ

		// アニメーション用
		//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		float tw = 1.0f;							// テクスチャの幅	8パターン
		float th = 1.0f;								// テクスチャの高さ
		float tx = 0.0f;		// テクスチャの左上X座標
		float ty = 0.0f;								// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}


void InitToge(void)
{
	for (int i = 0; i < TOGE_MAX; i++)
	{
		g_Toge[i].use = TRUE;			// true:使っている  false:未使用
		g_Toge[i].w = 768;			// 幅と高さ
		g_Toge[i].h = 100;			// 幅と高さ
		g_Toge[i].pos = XMFLOAT3(735.0f + i * g_Toge[i].w * 0.95, 945.0f, 0.0f);			// ポリゴンの座標

		g_Toge[i].texNo = 8;			// テクスチャ番号
	}
	g_Toge[4].pos = XMFLOAT3(2905.0f, 2085.0f, 0.0f);
}

void UpdateToge(void)
{

	PLAYER* player = GetPlayer();

	for (int i = 0; i < TOGE_MAX; i++)
	{
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (player[j].use && g_Toge[i].use)
			{
				if (CollisionBB(g_Toge[i].pos, g_Toge[i].w, g_Toge[i].h, player[j].pos, player[j].w, player[j].h))
				{
					player[j].hp = 0;
					player[j].life--;
					return;
				}
			}

		}
	}
	return;
}


void DrawToge(void)
{
	BG* bg = GetBG();
	//とげとげを描画する
	for (int i = 0; i < TOGE_MAX; i++)
	{
		if (g_Toge[i].use)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Toge[i].texNo]);

			float mx = (float)g_Toge[i].pos.x - bg->pos.x;//ウィンドウの座標
			float my = (float)g_Toge[i].pos.y - bg->pos.y;
			float mw = g_Toge[i].w;
			float mh = g_Toge[i].h;

			float tw = 1.0f;	// テクスチャの幅の比率
			float th = 1.0f;		// テクスチャの高さの比率
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;// テクスチャの左上Y座標_

			SetSpriteColor(g_VertexBuffer,
				mx, my, mw, mh,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}


	}
}






//鍵をゲットした？
BOOL isGetKey(void)
{
	return g_GetKey;
}

//プレイヤーはゲームの中にどの段階にいる
int GetBGMmode(void)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (player[i].use)
		{
			if (CollisionBB(player[i].pos, player[i].w, player[i].h, enemy[ENEMY_MAX - 1].pos, enemy[ENEMY_MAX - 1].w, enemy[ENEMY_MAX - 1].h))
			{
				return BGM_BOSS;
			}
			else
			{
				return BGM_DUNGEON;
			}
		}
	}
}

void InitDropItem(void)
{
	for (int i = 0; i < DROPITEM_MAX; i++)
	{
		g_Dropitem[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);			// ポリゴンの座標
		g_Dropitem[i].use = FALSE;			// true:使っている  false:未使用
		g_Dropitem[i].w = 60;
		g_Dropitem[i].h = 60;
		g_Dropitem[i].type = 0;
		g_Dropitem[i].texNo = 10;			// テクスチャ番号
		g_Dropitem[i].countAnim = 0;		// アニメーションカウント
		g_Dropitem[i].patternAnim = 0;	// アニメーションパターンナンバー
	}
}

void UpdateDropItem(void)
{
	for (int i = 0; i < DROPITEM_MAX; i++)
	{
		if (g_Dropitem[i].use == FALSE)continue;

		if (g_Dropitem[i].countAnim++ > DROPITEM_WAIT_MAX)
		{
			g_Dropitem[i].countAnim = 0;
			g_Dropitem[i].patternAnim = (g_Dropitem[i].patternAnim + 1) % DROPITEM_PATTERN_MAX;
		}

		PLAYER* player = GetPlayer();
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (player[j].use == FALSE)continue;

			if (CollisionBB(g_Dropitem[i].pos, g_Dropitem[i].w, g_Dropitem[i].h,
				player[j].pos, player[j].w, player[j].h))
			{
				g_Dropitem[i].use = FALSE;
				AddPlayerHP(j, 50.0f);

			}
		}
	}
}


void DrawDropItem(void)
{
	BG* bg = GetBG();
	//とげとげを描画する
	for (int i = 0; i < DROPITEM_MAX; i++)
	{
		if (g_Dropitem[i].use == FALSE)continue;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Dropitem[i].texNo]);

		float mx = (float)g_Dropitem[i].pos.x - bg->pos.x;//ウィンドウの座標
		float my = (float)g_Dropitem[i].pos.y - bg->pos.y;
		float mw = g_Dropitem[i].w;
		float mh = g_Dropitem[i].h;

		float tw = 1.0f / DROPITEM_PATTERN_DIVIDE_X;	// テクスチャの幅の比率
		float th = 1.0f / DROPITEM_PATTERN_DIVIDE_Y;		// テクスチャの高さの比率
		float tx = g_Dropitem[i].patternAnim % DROPITEM_PATTERN_DIVIDE_X * tw;	// テクスチャの左上X座標
		float ty = g_Dropitem[i].patternAnim / DROPITEM_PATTERN_DIVIDE_X * th;// テクスチャの左上Y座標_

		SetSprite(g_VertexBuffer,
			mx, my, mw, mh,
			tx, ty, tw, th);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}

void SetDropItem(XMFLOAT3 pos)
{
	for (int i = 0; i < DROPITEM_MAX; i++)
	{
		if (g_Dropitem[i].use == FALSE)
		{
			g_Dropitem[i].use = TRUE;
			g_Dropitem[i].pos = pos;
			return;
		}
	}
}