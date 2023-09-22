//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "hitscore.h"
#include "file.h"
#include "field.h"
#include "functionSet.h"
#include "sound.h"
#include "fade.h"
#include "result.h"
#include "effect.h"
#include "ui.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(128)	// キャラサイズ
#define TEXTURE_HEIGHT				(128)	// 

#define TEXTURE_COLLISION_WIDTH		(80)	//
#define TEXTURE_COLLISION_HEIGHT	(100)	//

#define TEXTURE_MAX					(4)		// テクスチャの数
#define EFFECT_MAX					(1)
//#define TEXTURE_PATTERN_DIVIDE_X	(3)		// アニメパターンのテクスチャ内分割数（X)
//#define TEXTURE_PATTERN_DIVIDE_Y	(4)		// アニメパターンのテクスチャ内分割数（Y)
//#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X * TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
//#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値


// ジャンプ処理
#define	PLAYER_JUMP_CNT_MAX			(30)		// 30フレームで着地する
#define	PLAYER_JUMP_Y_MAX			(15.0f)	// ジャンプの高さ
#define PLAYER_JUMP_IN_ONE_FRAME 　	(20.0f)
#define PLAYER_JUMP_X_MAX			(20.0f)
#define	PLAYER_JUMP_ACC				(0.5f)
#define PLAYER_JUMP_TIMES_MAX		(2)		//2回ジャンプできる

#define PLAYER_FLASH_TIMES_MAX		(0.2f)

//攻撃
#define PLAYER_ATTACK_FRAME			(8)
#define PLAYER_ATTACK_EFFECT_WIDTH	(120.0f)
#define PLAYER_ATTACK_EFFECT_HEIGHT	(120.0f)


#define PLAYER_SKILL_MAX	(4)

#define BULLET_COST_MP		(10)
#define BLACKHOLE_COST_MP	(45)
#define ULT_COST_MP			(PLAYER_MP_MAX)

#define HIT_FRAME_MAX	(60)
#define HIT_CLEAR_FRAME_MAX	(20)
#define DEAD_FRAME_MAX	(10)

#define SAVEPOINT_MAX	(2)

//#define GAUGE_WIDTH			(220)
//#define GAUGE_HEIGHT		(30)

#define BULLET_DELAY_CNT	(10)




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawPlayerOffset(int no);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView* g_State[PLAYER_STATE_MAX] = { NULL };	// テクスチャ情報
static ID3D11ShaderResourceView* g_Effect[EFFECT_MAX] = { NULL };	// テクスチャ情報

static char* g_TextureName[TEXTURE_MAX] = {
		"data/TEXTURE/bar_white.png",
		"data/TEXTURE/state.png",
		"data/TEXTURE/heroicon.png",
		"data/EFFECT/effect_dark.png",
};

static char* g_StateName[PLAYER_STATE_MAX] = {
	"data/CHARA/player_stand.png",		//STAND
	"data/CHARA/player_run.png",		//WALK
	"data/CHARA/player_guard.png",		//GUARD
	"data/CHARA/player_jump.png",		//JUMP
	"data/CHARA/player_fall.png",		//FALL
	"data/CHARA/player_attack.png",		//BLADE
	"data/CHARA/player_blackhole.png",	//BLACKHOLE
	"data/CHARA/player_bullet.png",		//BULLET
	"data/CHARA/player_ult.png"	,		//ULT
};

static char* g_EffectName[EFFECT_MAX] = {
	"data/TEXTURE/shadow000.jpg",		//影
};


static	int g_player_state_AnimeMax[PLAYER_STATE_MAX][4] = { //各テクスチャの分割数
//  Pattern_Max	divideY  divideX	animewait
		 4,		1,			4,			10,	    //STAND 
		 9,		1,			9,			5,		//WALK
		 1,		1,			1,			20,		//GUARD
		 1,		1,			1,			5,		//JUMP
		 1,		1,			1,			5,		//FALL  
		 5,		1,			5,			2,		//BLADE
		 5,		1,			5,			3,		//BLACKHOLE
		 5,		1,			5,			3,		//BULLET
		 4,		1,			4,			20,		//ULT
};

static	int g_effect_state_AnimeMax[EFFECT_MAX][4] = { //各テクスチャの分割数
//  Pattern_Max	divideY  divideX	animewait
		 1,		1,			1,			5,		//影
};

static XMFLOAT3 g_CheckPoint[CHECKPOINT_MAX] = {

	XMFLOAT3(3800.0f,1080.0f,0.0f),
	XMFLOAT3(300.0f,2160.0f,0.0f),
};

//static float  g_CheckPoint[CHECKPOINT_MAX] = {
//	1080.0f,2160.0f,
//};

static  BOOL	BGXScroll;
static  BOOL	BGYScroll;


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static PLAYER	g_Player[PLAYER_MAX];	// プレイヤー構造体
static PLAYERICON	g_PlayerIcon;
static int      g_jumpCnt;
static int		g_jumpinFrame;
static int		g_bulletTime;
static float	g_deadCnt;
static float	g_gravityCnt;
static DWORD    g_start_time;
BOOL			g_Changable;
BOOL			g_Ult;

static	int		g_BulletDelay;
static	int		g_SkillFrameCnt;
static	int		g_BlackholeFCnt;
static	int		g_GuardFrameCnt;
XMFLOAT3 g_clickpos;
XMFLOAT3 g_SavePoint[SAVEPOINT_MAX];

//static int		g_jump[PLAYER_JUMP_CNT_MAX] =
//{
//	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
//	//  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
//};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device* pDevice = GetDevice();

	BGXScroll = TRUE;
	BGYScroll = TRUE;

	g_bulletTime = 0;
	g_clickpos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//グローバル変数初期化
	g_jumpCnt = 0;
	g_jumpinFrame = 0;
	g_bulletTime;
	g_deadCnt = 1.0f;
	g_gravityCnt = 0;

	g_Changable = TRUE;
	g_BulletDelay = BULLET_DELAY_CNT;
	g_BlackholeFCnt = 0;
	g_SkillFrameCnt = 0;
	g_GuardFrameCnt = 0;
	g_Ult = FALSE;
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

	for (int i = 0; i < PLAYER_STATE_MAX; i++)
	{
		g_State[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_StateName[i],
			NULL,
			NULL,
			&g_State[i],
			NULL);
	}

	//テクスチャ生成
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_EffectName[i],
			NULL,
			NULL,
			&g_Effect[i],
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


	// プレイヤー構造体の初期化
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].use = TRUE;
		g_Player[i].pos = XMFLOAT3(100.0f, 0.0f, 0.0f);	// 中心点から表示
		g_Player[i].opos = g_Player[i].pos;
		g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].w = TEXTURE_WIDTH;
		g_Player[i].h = TEXTURE_HEIGHT;
		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;
		g_Player[i].texNo = 2;
		g_Player[i].time = 0;
		//基本ステータス

		g_Player[i].dir = CHAR_DIR_RIGHT;					// 向き（0:上 1:右 2:下 3:左）
		g_Player[i].state = PLAYER_STATE_STAND;				//プレイヤーの状態
		g_Player[i].prestate = g_Player[i].state;			//前の状態

		g_Player[i].life = 2;
		g_Player[i].hp = PLAYER_HP_MAX;
		g_Player[i].mp = PLAYER_MP_MAX;

		g_Player[i].str = 35.0f;
		g_Player[i].jumptimes = 2;
		g_Player[i].jumpplus = 4.0f;
		g_Player[i].move = XMFLOAT3(6.0f, 0.0f, 0.0f);		// 移動量
		g_Player[i].flashCnt = 0;		// 移動量
		g_Player[i].flashExeTimer = 0.0f;
		g_Player[i].flashExeCnt = 0;
		//ステートフラグ
		g_Player[i].moving = FALSE;							// 移動中フラグ
		g_Player[i].flash = FALSE;							// 移動中フラグ
		g_Player[i].onGround = FALSE;
		g_Player[i].attack = FALSE;
		g_Player[i].bullet = FALSE;
		g_Player[i].jump = FALSE;



		// ジャンプ相関の初期化
		g_Player[i].beforejumppos = g_Player[i].pos;
		g_Player[i].jumpCnt = 0;

		//Skill001
		g_Player[i].bullet = FALSE;
		//g_Player[i].bulletTime = 0;
		// 
		//ヒット
		g_Player[i].hit = FALSE;		//ヒットされている
		g_Player[i].hitting = FALSE;		//ヒットされている
		g_Player[i].hitdir = CHAR_DIR_LEFT;			//ヒットされているの方向
		g_Player[i].hitCnt = 0;
		g_Player[i].clearCnt = 0;
		g_Player[i].clear = FALSE;

		// 分身用
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}

	}
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		g_Player[0].pos = XMFLOAT3(100.0f, 500.0f, 0.0f);	// 中心点から表示
		break;
	case MODE_GAME:

		//g_Player[0].pos = XMFLOAT3(1000.0f, 2000.0f, 0.0f);	// 中心点から表示
		g_Player[0].pos = XMFLOAT3(100.0f, 900.0f, 0.0f);	// 中心点から表示
#ifndef _DEBUG
		//g_Player[0].pos = XMFLOAT3(500.0f, 1900.0f, 0.0f);	// 中心点から表示
#endif // !_DEBUG
		break;
	case MODE_BOSS:
		g_Player[0].pos = XMFLOAT3(100.0f, 500.0f, 0.0f);	// 中心点から表示
		g_Player[0].w = 100.0f;
		g_Player[0].h = 100.0f;
		//g_Player[0].move =  XMFLOAT3(8.0f, 0.0f, 0.0f);		// 移動量
		g_Player[0].jumpplus = 6.0f;
		//g_Player[0].pos = XMFLOAT3(1000.0f, 3000.0f, 0.0f);	// 中心点から表示


		//g_Player[0].savePoint = XMFLOAT3(100.0f, 900.0f, 0.0f);

		break;
	}




	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
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

	for (int i = 0; i < PLAYER_STATE_MAX; i++)
	{
		if (g_State[i])
		{
			g_State[i]->Release();
			g_State[i] = NULL;
		}
	}

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i])
		{
			g_Effect[i]->Release();
			g_Effect[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//==============================

void UpdatePlayer(void)
{

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].hp <= 0)
		{
			g_Player[i].use = FALSE;
			g_deadCnt += 0.1;
			if (g_deadCnt >= DEAD_FRAME_MAX)
			{
				g_deadCnt = 0.0f;
				//if (g_Player[i].life > 0) SetFade(FADE_OUT, MODE_GAME);
				if (GetMode() == MODE_GAME)
				{
					SetResult(GAMEOVER, RESULTTYPE_GAME);
					SetFade(FADE_OUT, MODE_RESULT);

				}
				else if (GetMode() == MODE_BOSS)
				{
					SetResult(GAMEOVER, RESULTTYPE_BOSS);
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
			continue;
		}

		if (g_Player[i].use == FALSE)continue;

		ENEMY* enemy = GetEnemy();
		BG* bg = GetBG();
		UI* ui = GetSkillUI();
		int mskill = -1;

		// 生きてるプレイヤーだけ処理をする

		if (g_Player[i].pos.x >= 1900.0f && g_Player[i].pos.y >= 2800.0f)SetFade(FADE_OUT, MODE_BOSS);
		// 地形との当たり判定用に座標のバックアップを取っておく

		g_Player[i].opos = g_Player[i].pos;
		g_Player[i].prestate = g_Player[i].state;

		if (g_Player[i].mp <= PLAYER_MP_MAX)g_Player[i].mp += 0.1f;


		{// アニメーション  

			if (g_Player[i].countAnim++ > g_player_state_AnimeMax[g_Player[i].state][3])
			{
				g_Player[i].countAnim = 0.0f;
				// パターンの切り替え
				g_Player[i].patternAnim = (g_Player[i].patternAnim + 1) % g_player_state_AnimeMax[g_Player[i].state][0];
			}
		}


		{	// 分身用
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = g_Player[i].opos;
		}

		{// キー入力で移動 
			XMFLOAT3 nextpos = g_Player[i].pos;
			float speed = g_Player[i].move.x;


			if (g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT &&
				g_Player[i].state != PLAYER_STATE_GUARD
				)
			{
				//if (GetKeyboardTrigger(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_B))
				//{
				//	g_Player[i].dash = TRUE;
				//	speed *= 4;
				//	//if (g_Player[i].dir == CHAR_DIR_RIGHT)nextpos.x += 20.0f;
				//	//else if (g_Player[i].dir == CHAR_DIR_LEFT)nextpos.x -= 20.0f;

				//}


				if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
				{

					g_Player[i].state = PLAYER_STATE_WALK;
					g_Player[i].dir = CHAR_DIR_RIGHT;
					nextpos.x += speed;
				}
				else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
				{
					g_Player[i].state = PLAYER_STATE_WALK;
					g_Player[i].dir = CHAR_DIR_LEFT;

					nextpos.x -= speed;
				}


				{//フラッシュ
					if (((GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT)) ||
						(GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT))) &&
						g_Player[i].flash == FALSE)
					{
						g_Player[i].moving = TRUE;
						g_Player[i].flashExeTimer = 0.0f;
						g_Player[i].flashExeCnt++;
					}
					if (g_Player[i].moving == TRUE)
					{
						g_Player[i].flashExeTimer += GetDeltatime();

						if (g_Player[i].flashExeTimer > PLAYER_FLASH_TIMES_MAX * 1000.0f)
						{
							g_Player[i].moving = FALSE;
							g_Player[i].flashExeTimer = 0.0f;
							g_Player[i].flashExeCnt = 0;
						}
					}
					if (g_Player[i].flashExeCnt >= 2)
					{
						g_Player[i].flash = TRUE;
						PlayerFlash(&nextpos, i);
					}
					else g_Player[i].flash = FALSE;

				}

				//移動後フィールドとの当たり判定

				if (!WaistFieldCollision(nextpos, TEXTURE_COLLISION_WIDTH, TEXTURE_COLLISION_HEIGHT))
				{
					g_Player[i].opos = g_Player[i].pos;
					g_Player[i].pos = nextpos;
				}


				if ((GetKeyboardTrigger(DIK_SPACE) || (IsButtonTriggered(0, BUTTON_X)) || (GetSkillUIClicked() == UI_JUMP)))
				{
					g_Player[i].jumptimes--;

					if ((g_Player[i].jumptimes > 0))
					{
						g_Player[i].state = PLAYER_STATE_JUMP;
						SetJump(i);
						PlaySound(SOUND_LABEL_SE_jump);
						SetEffect3(g_Player[i].pos, JUMP);
					}
				}

				if (g_Player[i].jump && g_Player[i].jumptimes > 0)
				{
					PlayerJump(i);
				}
				if (!g_Player[i].onGround && !g_Player[i].jump)
				{
					g_Player[i].state = PLAYER_STATE_FALL;
				}
				if (g_Player[i].onGround)
				{
					g_Player[i].jumptimes = PLAYER_JUMP_TIMES_MAX;
				}
			}
			////重力処理
			PlayerGravity(i);
			{
				{// MAP外チェック
					if (g_Player[i].pos.x < 0.0f)	g_Player[i].pos.x = 0.0f;
					if (g_Player[i].pos.x > bg->w)	g_Player[i].pos.x = bg->w;
					if (g_Player[i].pos.y < 0.0f)	g_Player[i].pos.y = 0.0f;
					if (g_Player[i].pos.y > bg->h)	g_Player[i].pos.y = bg->h;
				}

				if (GetMode() == MODE_TUTORIAL)
				{// プレイヤーの立ち位置からMAPのスクロール座標を計算する
					bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
					if (bg->pos.x < 0) bg->pos.x = 0;
					if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;
				}
				else if (GetMode() == MODE_GAME)
				{// プレイヤーの立ち位置からMAPのスクロール座標を計算する

					bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
					if (bg->pos.x < 0) bg->pos.x = 0;
					if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

					for (int j = 0; j < CHECKPOINT_MAX; j++)
					{
						if (CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h, g_CheckPoint[j], 1000.0f, 10.0f))
						{
							SetBGSFrame(0, SCREEN_HEIGHT, 60);
							
						}							
					}

				}
			}

			/////////////////////////////////////////////////////////////

			if (g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BLACKHOLE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT)
			{
				g_Changable = TRUE;
			}

			//スキル処理
			if (g_Changable)
			{
				//ガード
				if (((GetKeyboardPress(DIK_H)) && (g_Player[i].state == PLAYER_STATE_STAND)) ||
					(IsButtonPressed(0, BUTTON_L) && (g_Player[i].state == PLAYER_STATE_STAND)) ||
					((GetSkillUIPressed() == UI_GUARD) && (g_Player[i].state == PLAYER_STATE_STAND)))
				{
					if (g_Player[i].onGround == TRUE)
					{
						{
							g_Player[i].state = PLAYER_STATE_GUARD;
							g_Changable = FALSE;

							g_SkillFrameCnt = 0;

						}
					}
				}

				if (((GetKeyboardRelease(DIK_H)) && (g_Player[i].state == PLAYER_STATE_GUARD)) ||
					((GetKeyboardRelease(DIK_H)) && !IsButtonPressed(0, BUTTON_L) && (g_Player[i].state == PLAYER_STATE_GUARD)) ||
					(((GetSkillUIPressed() == UI_GUARD) && (g_Player[i].state == PLAYER_STATE_GUARD))))

				{
					g_Player[i].state = PLAYER_STATE_STAND;
					g_Changable = TRUE;

					ResetEffect3(GUARD);
				}

				//ブレイド
				if ((GetKeyboardTrigger(DIK_J)) || IsButtonTriggered(0, BUTTON_B) || GetSkillUIClicked() == UI_BLADE)
				{
					g_Player[i].state = PLAYER_STATE_BLADE;
					g_SkillFrameCnt = 0;
				}
				//バレット
				if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_Y) || GetSkillUIClicked() == UI_BULLET)
				{
					g_BulletDelay--;
					if (g_BulletDelay < 0)
					{
						g_BulletDelay = BULLET_DELAY_CNT;
						if (g_Player[i].mp >= BULLET_COST_MP)
						{
							g_Player[i].mp -= BULLET_COST_MP;

							g_Player[i].state = PLAYER_STATE_BULLET;
							g_SkillFrameCnt = 0;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_mp);
						}
					}

				}
				//重力弾
				if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(0, BUTTON_R) || GetSkillUIClicked() == UI_BLACKHOLE)
				{
					if (g_Player[i].mp >= BLACKHOLE_COST_MP)
					{
						g_Player[i].mp -= BLACKHOLE_COST_MP;

						g_Player[i].state = PLAYER_STATE_BLACKHOLE;

						g_SkillFrameCnt = 0;

					}
					else
					{
						PlaySound(SOUND_LABEL_SE_mp);
					}
				}
				//ウルトラ
				if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_R2) || GetSkillUIClicked() == UI_ULT)
				{
					if (g_Player[i].mp >= PLAYER_MP_MAX)
					{
						g_Player[i].mp -= ULT_COST_MP;
						g_Player[i].state = PLAYER_STATE_ULT;
						g_SkillFrameCnt = 0;
					}
					else
					{
						PlaySound(SOUND_LABEL_SE_mp);
					}
				}


			}

			float w = 0;
			switch (g_Player[i].state)
			{
			case PLAYER_STATE_BLADE:

				PlayerAttackProcess(i);
				break;

			case PLAYER_STATE_GUARD:

				Guard(i);


				break;

			case PLAYER_STATE_BLACKHOLE:

				BlackholeTimer(i);

				break;
			case PLAYER_STATE_BULLET:
				BulletTimer(i);
				break;
			case PLAYER_STATE_ULT:
				UltTimer(i);
				break;

			}

			//チュートリアルマップ外に出たら
			if (g_Player[i].pos.y > SCREEN_HEIGHT - 100 && GetMode() == MODE_TUTORIAL)
			{
				g_Player[i].pos.y = 0.0f;
				//PlayerReturn(i);
				//g_Player[i].pos = g_Player[i].opos;
			}
		}

		if (g_Player[i].hit)
		{
			SetVolumeSound(SOUND_LABEL_SE_hitplayer, 0.2f);
			PlaySound(SOUND_LABEL_SE_hitplayer);
			g_Player[i].hitting = TRUE;
			g_Player[i].hit = FALSE;
		}
		if (g_Player[i].hitting)
		{
			PHitbackProcess(i);
		}


		if (GetMode() == MODE_BOSS)
		{// 移動が終わったらエネミーとの当たり判定

			BOSS* boss = GetBoss();
			for (int j = 0; j < BOSS_MAX; j++)
			{
				if (boss[j].use == FALSE)continue;
				int ans = CollisionBB(g_Player[i].pos, g_Player[i].w / 2, g_Player[i].h,
					boss[j].pos, boss[j].w, boss[j].h * 0.8);

				if (ans == TRUE)
				{
					g_Player[i].hit = TRUE;
					AddPlayerHP(i, -15);
				}
			}
		}




		{//移動した？
			XMVECTOR temp = XMLoadFloat3(&g_Player[i].pos) - XMLoadFloat3(&g_Player[i].opos);
			temp = XMVector3LengthSq(temp);
			float dis;
			XMStoreFloat(&dis, temp);
			if (dis == 0 &&
				g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BLACKHOLE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT &&
				g_Player[i].state != PLAYER_STATE_GUARD)
			{//移動していないなら
				g_Player[i].state = PLAYER_STATE_STAND;
			}
		}
	}

	//// 現状をセーブする
	//if (GetKeyboardTrigger(DIK_S))
	//{
	//	SaveData();
	//}


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			//影表示
			DrawPlayerShadow();

			// プレイヤーの分身を描画
			if (g_Player[i].flash)
			{	// ダッシュ中だけ分身処理
				DrawPlayerOffset(i);
			}

			{//プレイヤー表示
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[i].state]);	//0 : STAND, 1:walk 2

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				if (g_Player[i].state == PLAYER_STATE_BLADE)
				{
					pw = 160.0f;		// プレイヤーの表示幅
					ph = 160.0f;
					py -= 30.0f;
				}


				//py += g_Player[i].jumpY;		// ジャンプ中の高さを足す
				// アニメーション用

				int animeMaxX = g_player_state_AnimeMax[g_Player[i].state][2];
				int animeMaxY = g_player_state_AnimeMax[g_Player[i].state][1];

				float tw = 1.0f / animeMaxX;	// テクスチャの幅
				float th = 1.0f / animeMaxY;	// テクスチャの高さ
				float tx = (float)(g_Player[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
				float ty = (float)(g_Player[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				if (g_Player[i].dir == CHAR_DIR_LEFT)
				{
					tw *= (-1);
				}
				float R = 1.0f;
				float G = 1.0f;
				float B = 1.0f;
				float Alpha = 1.0f;

				if (g_Player[i].clear)
				{
					G = 0.0f;
					B = 0.0f;
					Alpha = 0.7f;
				}
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, Alpha),
					g_Player[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

			//攻撃表示
			//DrawAttackEffect();


		}
		else if (!g_Player[i].use && g_deadCnt <= DEAD_FRAME_MAX)
		{
			{//プレイヤー表示
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[i].state]);	//0 : STAND, 1:walk 2

				//プレイヤーの位置やテクスチャー座標を反映
				float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
				float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
				float pw = g_Player[i].w;		// プレイヤーの表示幅
				float ph = g_Player[i].h;		// プレイヤーの表示高さ

				if (g_Player[i].state == PLAYER_STATE_BLADE)
				{
					pw = 160.0f;		// プレイヤーの表示幅
					ph = 160.0f;
					py -= 30.0f;
				}


				//py += g_Player[i].jumpY;		// ジャンプ中の高さを足す
				// アニメーション用

				int animeMaxX = g_player_state_AnimeMax[g_Player[i].state][2];
				int animeMaxY = g_player_state_AnimeMax[g_Player[i].state][1];

				float tw = 1.0f / animeMaxX;	// テクスチャの幅
				float th = 1.0f / animeMaxY;	// テクスチャの高さ
				float tx = (float)(g_Player[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
				float ty = (float)(g_Player[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				if (g_Player[i].dir == CHAR_DIR_LEFT)
				{
					tw *= (-1);
				}

				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f / g_deadCnt),
					g_Player[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}
		}
		//プレイヤーのHP表示
		//DrawPlayerState(i);
		//プレイヤーSikll表示
		//DrawPlayerSkill(i);
	}

}







//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}

void SetJump(int num)
{
	if (g_Player[num].jumptimes > 0)
	{
		{//ジャンプの初期化処理
			g_Player[num].state = PLAYER_STATE_JUMP;
			g_gravityCnt = 0.0f;
			if (GetMode() == MODE_BOSS)	g_Player[num].jumpplus = (-1) * (PLAYER_JUMP_Y_MAX + 5.0f);
			else						g_Player[num].jumpplus = (-1) * (PLAYER_JUMP_Y_MAX);

		}
		g_Player[num].jump = TRUE;
	}
}

//ジャンプ処理
void PlayerJump(int num)
{

	if (g_Player[num].jump)
	{//ジャンプ処理
		XMFLOAT3 nextpos = g_Player[num].pos;
		{//ジャンプ中

			g_Player[num].jumpplus += PLAYER_JUMP_ACC;

			XMFLOAT3   jumpplus = XMFLOAT3(0.0f, g_Player[num].jumpplus, 0.0f);
			XMVECTOR  vjumppluse = XMLoadFloat3(&jumpplus);
			XMVECTOR       vPpos = XMLoadFloat3(&nextpos);

			XMStoreFloat3(&nextpos, vPpos + vjumppluse);

			if (g_Player[num].jumpplus >= 0)
			{
				g_Player[num].jumpplus = 0;
				g_Player[num].jump = FALSE;
				g_Player[num].state = PLAYER_STATE_FALL;
			}
			else g_Player[num].state = PLAYER_STATE_JUMP;
			//if(!g_Player[num].jump)
			//else PLAYER_STATE_FALL;
		}
		//頭が壁にぶつかったらジャンプ状態をキャンセル
		XMFLOAT3 phead = XMFLOAT3(nextpos.x, nextpos.y - g_Player[num].h / 2, 0.0f);
		if (!FieldCollision(phead, TEXTURE_COLLISION_WIDTH, TEXTURE_COLLISION_HEAD_HEIGHT))
		{
			g_Player[num].pos = nextpos;
		}
		else
		{
			g_Player[num].jump = FALSE;
			g_Player[num].jumpplus = 0;
		}

		//足が壁にぶつかったらジャンプ状態をキャンセル
		if (IsFootCol(num))
		{
			g_Player[num].jump = FALSE;
			g_Player[num].jumpplus = 0;
		}
	}

}

void PlayerFlash(XMFLOAT3* nextpos, int num)
{
	if (g_Player[num].use == FALSE)return;
	if (g_Player[num].flash)
	{
		float speed = g_Player[num].move.x * 2;

		if (g_Player[num].dir == CHAR_DIR_LEFT)
			nextpos->x -= speed;
		else
			nextpos->x += speed;
	}
	return;
}
//足場判断処理
BOOL IsFootCol(int num)
{

	XMFLOAT3 pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//足元の座標
	//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
	BOOL ans = DownFieldCollision(pfootpos, TEXTURE_COLLISION_FOOT_WIDTH, TEXTURE_COLLISION_FOOT_HEIGHT);

	if (ans)return TRUE;//足場がある

	return FALSE;
}
//判断処理
BOOL IsWaistCol(int num)
{

	XMFLOAT3 pwaistpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y, 0.0f);//足元の座標
	//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
	BOOL ans = WaistFieldCollision(pwaistpos, TEXTURE_COLLISION_WAIST_WIDTH / 2, TEXTURE_COLLISION_WAIST_HEIGHT / 2);

	if (ans)return TRUE;//足場がある

	return FALSE;
}

//重力処理
void PlayerGravity(int num)
{
	{//重力処理
		if (g_Player[num].onGround)
		{
			g_gravityCnt = 0;

		}
		XMFLOAT3	nextpos = g_Player[num].pos;
		XMFLOAT3	pfootpos;
		//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
		BOOL		ans;

		if (GetMode() == MODE_BOSS)
		{
			pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//足元の座標]
			ans = FieldCollision(pfootpos, g_Player[num].w / 4, g_Player[num].h / 4);
			if (ans)//足場がある
			{
				g_Player[num].onGround = TRUE;
				g_gravityCnt = 0;
			}
			else
			{
				g_Player[num].onGround = FALSE;
				g_gravityCnt += GRAVITY;
				float acc = g_gravityCnt * 5;
				float totalmove = g_gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 10)
				{
					totalmove = TEXTURE_HEIGHT / 10;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Player[num].w / 2, g_Player[num].h / 16);
			if (!ans)
			{
				g_Player[num].pos = nextpos;
			}

		}
		else
		{
			pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//足元の座標
			ans = FieldCollision(pfootpos, TEXTURE_COLLISION_FOOT_WIDTH, TEXTURE_COLLISION_FOOT_HEIGHT);
			if (ans)//足場がある
			{
				g_Player[num].onGround = TRUE;
				g_gravityCnt = 0;
			}
			else
			{
				g_Player[num].onGround = FALSE;
				g_gravityCnt += GRAVITY;
				float acc = g_gravityCnt * 5;
				float totalmove = g_gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Player[num].w / 2, g_Player[num].h / 16);
			if (!ans)
			{
				g_Player[num].pos = nextpos;
			}


		}



	}

}
//=============================================================================
//攻撃表示
//=============================================================================
void DrawAttackEffect(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].state == PLAYER_STATE_BLADE)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect[1]);

			float temp1 = 70.0f;
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				temp1 *= (-1);
			}
			float px = g_Player[i].pos.x - temp1 - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = TEXTURE_WIDTH * 2;		// プレイヤーの表示幅
			float ph = TEXTURE_HEIGHT * 2;		// プレイヤーの表示高さ

			float temp = g_Player[i].w;
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				temp *= (-1);
			}
			px += temp;

			int animeMaxX = g_effect_state_AnimeMax[1][2];
			int animeMaxY = g_effect_state_AnimeMax[1][1];

			float tw = 1.0f / animeMaxX;	// テクスチャの幅
			float th = 1.0f / animeMaxY;	// テクスチャの高さ
			float tx = (float)((int)g_Player[i].time % animeMaxX) * tw;	// テクスチャの左上X座標
			float ty = (float)((int)g_Player[i].time / animeMaxX) * th;	// テクスチャの左上Y座標
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				tw *= (-1);
			}

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

}



//=============================================================================
// プレイヤー攻撃処理
//=============================================================================

void PlayerAttackProcess(int num)
{

	{//攻撃処理

		if (g_Player[num].state != PLAYER_STATE_BLADE)return;
		//カウンター
		g_Changable = FALSE;

		int waitMax = g_player_state_AnimeMax[g_Player[num].state][3] * g_player_state_AnimeMax[g_Player[num].state][0];
		if (g_Player[num].time++ > waitMax)
		{
			g_Player[num].time = 0;
			g_Player[num].state = PLAYER_STATE_STAND;
			g_Changable = TRUE;
		}

		float attackx = 0.0f;
		if (g_Player[num].dir == CHAR_DIR_RIGHT)		attackx = (g_Player[num].w / 2);
		else if (g_Player[num].dir == CHAR_DIR_LEFT)	attackx = (-1) * (g_Player[num].w / 2);


		XMFLOAT3 blade_pos = XMFLOAT3(g_Player[num].pos.x + attackx, g_Player[num].pos.y, 0.0f);

		if (g_Player[num].time == 1)
		{
			int temp = rand() % 2;
			if (temp % 2 == 0)	PlaySound(SOUND_LABEL_SE_ken001);
			else				PlaySound(SOUND_LABEL_SE_ken002);

			SetEffect3(blade_pos, BLADE, g_Player[num].dir);

		}


		switch (GetMode())
		{
		case MODE_TUTORIAL:
		case MODE_GAME:

			//攻撃処理時の当たり判定
		{
			ENEMY* enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				if (enemy[j].use == FALSE)continue;
				int ans = CollisionBB(blade_pos, PLAYER_ATTACK_EFFECT_WIDTH + 30.0f, PLAYER_ATTACK_EFFECT_HEIGHT + 30.0f, enemy[j].pos, enemy[j].w, enemy[j].h);

				if (ans == TRUE)
				{



					if (g_Player[num].time == waitMax)
					{
						SetEffect3(g_Player[num].pos, HIT);
						AddPlayerMP(num, 4.0f);
						//SetEffect3(g_Player[num].pos, MPUP);
					}

					enemy[j].hit = TRUE;
					SetEDamagedType(j, DAMAGED_BLADE);
				}
			}
		}


		break;
		case MODE_BOSS:

			if (g_Player[num].time == 1)
			{
				//攻撃処理時の当たり判定
				{

					BOSS* boss = GetBoss();
					for (int j = 0; j < BOSS_MAX; j++)
					{
						if (boss[j].use == FALSE)continue;
						int ans = CollisionBB(blade_pos, PLAYER_ATTACK_EFFECT_WIDTH + 30.0f, PLAYER_ATTACK_EFFECT_HEIGHT + 30.0f,
							boss[j].pos, boss[j].w, boss[j].h);

						if (ans == TRUE)
						{
							int temp = rand() % 5;
							if (temp == 0)
							{
								AddPlayerMP(num, 10.0f);
							}

							boss[j].hit = TRUE;
							SetBDamagedType(DAMAGED_BLADE);
						}
					}
				}
			}
			break;

		}

	}
}

//=============================================================================
//下に落ちた処理
//=============================================================================
void PlayerReturn(int num)
{
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (g_Player[i].pos.y >= SCREEN_HEIGHT)
			{
				g_Player[i].pos.y = 0.0f;
				//g_Player[i].pos = g_Player[i].opos;
			}
		}
		break;
	case MODE_GAME:
		break;

	default:
		break;
	}


}


//=============================================================================
// プレイヤーの分身を描画
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[no].state]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Player[no].offset[j].x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Player[no].offset[j].y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Player[no].w;		// プレイヤーの表示幅
		float ph = g_Player[no].h;		// プレイヤーの表示高さ

		// アニメーション用
		int animeMaxX = g_player_state_AnimeMax[g_Player[no].state][2];
		int animeMaxY = g_player_state_AnimeMax[g_Player[no].state][1];

		float tw = 1.0f / animeMaxX;	// テクスチャの幅
		float th = 1.0f / animeMaxY;	// テクスチャの高さ
		float tx = (float)(g_Player[no].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Player[no].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

		if (g_Player[no].dir == CHAR_DIR_LEFT)
		{
			tw *= (-1);
		}
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


}

// 影表示
void DrawPlayerShadow(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use && g_Player[i].onGround)
		{
			SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成

		// テクスチャ設定


			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect[0]);

			float px = g_Player[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Player[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Player[i].w;		// プレイヤーの表示幅
			float ph = g_Player[i].h / 4;		// プレイヤーの表示高さ
			py += 60.0f;		// 足元に表示

			float tw = 1.0f;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = 0.0f;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
		}


	}
}




////プレイヤーのHP表示
//void DrawPlayerHP(int num)
//{
//	// HPの表示
//	BG* bg = GetBG();
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 263.0f;		// ゲージの表示位置X
//		float py = 135.0f;		// ゲージの表示位置Y
//		float pw = 220;	// ゲージの表示幅
//		float ph = 30.0f;		// ゲージの表示高さ
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
//		pw = pw * ((float)g_Player[num].hp / PLAYER_HP_MAX);
//
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}
//
////プレイヤーのMP表示
//void DrawPlayerMP(int num)
//{
//	// HPの表示
//	BG* bg = GetBG();
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 263.0f;		// ゲージの表示位置X
//		float py = 200.0f;		// ゲージの表示位置Y
//		float pw = GAUGE_WIDTH;	// ゲージの表示幅
//		float ph = GAUGE_HEIGHT;		// ゲージの表示高さ
//
//		float tw = 1.0f;	// テクスチャの幅
//		float th = 1.0f;	// テクスチャの高さ
//		float tx = 0.0f;	// テクスチャの左上X座標
//		float ty = 0.0f;	// テクスチャの左上Y座標
//
//		float R = 0.0f;
//		float G = 0.3f;
//		float B = 0.8f;
//		float Alpha = 1.0f;
//
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float temp = ((float)g_Player[num].mp / PLAYER_MP_MAX);
//		pw = pw * temp;
//		if (pw >= GAUGE_WIDTH)
//		{
//			R = 0.11765f;
//			G = 0.941f;
//			B = 1.0f;
//		}
//		// １枚のポリゴンの頂点とテクスチャ座標を設定
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(R, G, B, Alpha));
//
//		// ポリゴン描画
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}

////プレイヤー状態表示
//void DrawPlayerState(int num)
//{
//	DrawPlayerHP(num);
//	DrawPlayerMP(num);
//	{
//		// 下敷きのゲージ（枠的な物）
//		// テクスチャ設定
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
//
//		//ゲージの位置やテクスチャー座標を反映
//		float px = 60.0f;		// ゲージの表示位置X
//		float py = -10.0f;		// ゲージの表示位置Y
//		float pw = 480.0f;	// ゲージの表示幅
//		float ph = 405.0f;		// ゲージの表示高さ
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
//}


//ヒットバック処理
void PHitbackProcess(int num)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[num].use == TRUE)
		{
			if (g_Player[num].hitting)
			{//○○に当てられた処理
				if (g_Player[num].hitCnt > HIT_FRAME_MAX)
				{	//時間オーバー
					g_Player[num].hit = FALSE;
					g_Player[num].hitting = FALSE;
					g_Player[num].hitCnt = 0;

					g_Player[num].clear = FALSE;
					g_Player[num].clearCnt = 0;
				}

				{//当てられた途中？

					g_Player[num].hitCnt++;
					g_Player[num].clearCnt++;
					if (g_Player[num].clearCnt >= HIT_CLEAR_FRAME_MAX)
					{
						g_Player[num].clearCnt = 0;
						g_Player[num].clear = !(g_Player[num].clear);
					}
				}
			}
		}
	}

}

//プレイヤーのHPを引く数の値を引く
void AddPlayerHP(int num, float hp)
{

	if (g_Player[num].use == FALSE)return;
	if (g_Player[num].hitting == TRUE)return;

	if (g_Player[num].state != PLAYER_STATE_GUARD)
	{
		g_Player[num].hp += hp;
		if (hp < 0)
		{
			SetHitScore(hp, g_Player[num].pos, DAMAGE_PLAYER);
			SetEffect3(g_Player[num].pos, HIT2);
			if (g_Player[num].hp <= 0.0f)g_Player[num].hp = 0.0f;

		}
		else
		{//回復
			SetHitScore(hp, g_Player[num].pos, HEAL);
			SetHealEffect(g_Player[num].pos);
			PlaySound(SOUND_LABEL_SE_heal001);
			if (g_Player[num].hp >= PLAYER_HP_MAX)g_Player[num].hp = PLAYER_HP_MAX;

		}
	}

}

void AddPlayerMP(int num, float mp)
{
	if (g_Player[num].use == FALSE)return;

	g_Player[num].mp += mp;
	SetHitScore(mp, g_Player[num].pos, MP1);
	SetEffect3(g_Player[num].pos, MPUP);
	PlaySound(SOUND_LABEL_SE_mpUP);
	{//回復
		if (g_Player[num].mp >= PLAYER_MP_MAX)g_Player[num].mp = PLAYER_MP_MAX;
	}

}
//プレイヤーのジャンプ状態をリセット
void JumpReset(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)
		{
			g_Player[i].jump = FALSE;
			g_Player[i].jumpCnt = 0;
			g_Player[i].jumpplus = 0.0f;

		}
	}
	return;
}

void GetSavepoint(void)
{

}

void BlackholeTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[PLAYER_STATE_BLACKHOLE][0];
	int aniwaitMax = g_player_state_AnimeMax[PLAYER_STATE_BLACKHOLE][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_BlackholeFCnt++ > waitMax)
	{
		float x = g_Player[num].pos.x;
		if (g_Player[num].dir == CHAR_DIR_LEFT)
		{
			x -= g_Player[num].w;
		}
		else x += g_Player[num].w;

		SetBullet(XMFLOAT3(x, g_Player[num].pos.y, 0.0f), BLACKHOLE);
		g_BlackholeFCnt = 0;
		g_Changable = TRUE;
		g_Player[num].state = PLAYER_STATE_STAND;

	}
	else
	{
		g_Changable = FALSE;
	}
	return;

}
void BulletTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[PLAYER_STATE_BULLET][0];
	int aniwaitMax = g_player_state_AnimeMax[PLAYER_STATE_BULLET][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_SkillFrameCnt++ > waitMax)
	{

		SetBullet(g_Player[num].pos, BULLET_1);
		g_SkillFrameCnt = 0;
		g_Changable = TRUE;
		g_Player[num].state = PLAYER_STATE_STAND;
	}
	else
	{
		g_Changable = FALSE;
	}
	return;
}


void SetUlt(void)
{

}

void UltTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[g_Player[num].state][0];
	int aniwaitMax = g_player_state_AnimeMax[g_Player[num].state][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_SkillFrameCnt++ > ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX + 30)
	{
		//当たり判定
		g_SkillFrameCnt = 0;
		g_Changable = TRUE;
		g_Ult = FALSE;
		g_Player[num].state = PLAYER_STATE_STAND;
	}
	else
	{
		g_Changable = FALSE;
		BG* bg = GetBG();

		XMFLOAT3 u1pos = XMFLOAT3(g_Player[0].pos.x - bg->pos.x, g_Player[0].pos.y - bg->pos.y, 0.0f);
		if (g_SkillFrameCnt == 1)
		{
			PlaySound(SOUND_LABEL_SE_ultstart);
			SetEffect3(u1pos, ULT1);
		}
		else if (g_SkillFrameCnt == ULT_CHANT_FRAMEMAX + 30)
		{
			if (GetMode() == MODE_BOSS)
			{
				BOSS* boss = GetBoss();

				for (int i = 0; i < BOSS_MAX; i++)
				{
					boss[i].hit = TRUE;

					SetBDamagedType(DAMAGED_ULT);
				}
			}
			else
			{
				ENEMY* enemy = GetEnemy();
				PLAYER* player = GetPlayer();
				for (int i = 0; i < ENEMY_MAX; i++)
				{
					if (enemy[i].use == FALSE)continue;

					if ((enemy[i].pos.x >= (player[0].pos.x - SCREEN_CENTER_X)) &&
						(enemy[i].pos.x <= (player[0].pos.x + SCREEN_CENTER_X)) &&
						(enemy[i].pos.y >= (player[0].pos.y - SCREEN_CENTER_Y)) &&
						(enemy[i].pos.y <= (player[0].pos.y + 100))
						)
					{
						enemy[i].hit = TRUE;
						SetEDamagedType(i, DAMAGED_ULT);
					}
				}
			}


			SetEffect3(u1pos, ULT2);

		}

		if (g_SkillFrameCnt % 40 == 0 && g_SkillFrameCnt > ULT_CHANT_FRAMEMAX + 30)
		{
			PlaySound(SOUND_LABEL_SE_ulthit);
		}
		//if (g_SkillFrameCnt == 90)
		//{
		//	SetEffect3();
		//}
	}
	return;


}

void Guard(int num)
{
	if (g_Player[num].state == PLAYER_STATE_GUARD)
	{
		if (g_GuardFrameCnt++ >= 20)
		{
			g_GuardFrameCnt = 0;
			PlaySound(SOUND_LABEL_SE_shield);
		}

		float w = g_Player[num].w / 2;
		if (g_Player[num].dir == CHAR_DIR_LEFT)w *= (-1);

		XMFLOAT3 epos = XMFLOAT3(g_Player[num].pos.x + w,
			g_Player[num].pos.y, 0.0f);
		SetEffect3(epos, GUARD, g_Player[num].dir);
	}

	else
	{
		g_GuardFrameCnt = 0;
	}

}


//
//void InitPlayerIcon(void)
//{
//	g_PlayerIcon.use = FALSE;
//	g_PlayerIcon.pos = XMFLOAT3(175.0f, 180.0f, 0.0f);
//	g_PlayerIcon.w = 180.0f;
//	g_PlayerIcon.h = 180.0f;
//	g_PlayerIcon.texNo = 2;
//	g_PlayerIcon.theta = 0.0f;
//
//	return;
//}
//void UpdatePlayerIcon(void)
//{
//	if (g_Player[0].hitting == TRUE)g_PlayerIcon.use = TRUE;
//	else g_PlayerIcon.use = FALSE;
//
//	if (g_PlayerIcon.use == FALSE)
//	{
//		g_PlayerIcon.theta = 0.0f;
//		return;
//	}
//
//	g_PlayerIcon.theta += 1.5f;
//
//
//}
//
//void  DrawPlayerIcon(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerIcon.texNo]);
//
//	float tx = g_PlayerIcon.pos.x;
//	float ty = g_PlayerIcon.pos.y;
//	float tw = g_PlayerIcon.w;
//	float th = g_PlayerIcon.h;
//
//	float u = 0.0f;
//	float v = 0.0f;
//	float uw = 1.0f;
//	float vh = 1.0f;
//
//	float R = 1.0f;
//	float G = 1.0f;
//	float B = 1.0f;
//	float Alpha = 1.0f;
//
//	if (g_PlayerIcon.use == TRUE)
//	{
//		G = 0.0f, B = 0.0f, Alpha = 0.7f;
//
//		tx += 10 * sin(g_PlayerIcon.theta);
//		/*	ty += 5 * cos(g_PlayerIcon.theta);*/
//		tw = g_PlayerIcon.w * (1 + 0.025 * sin(g_PlayerIcon.theta));
//		th = g_PlayerIcon.h * (1 + 0.025 * sin(
//			g_PlayerIcon.theta));
//	}
//	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);
//
//	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
//	//// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//
//}

void DrawEyeDark(void)
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
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

	float tx = g_Player[0].pos.x - bg->pos.x;
	float ty = g_Player[0].pos.y - bg->pos.y;
	float tw = 7680.0f;
	float th = 4320.0f;

	float u = 0.0f;
	float v = 0.0f;
	float uw = 1.0f;
	float vh = 1.0f;

	float R = 1.0f;
	float G = 1.0f;
	float B = 1.0f;
	float Alpha = 1.0f;

	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);

	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
	//// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}