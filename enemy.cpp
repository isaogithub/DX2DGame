//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "bg.h"
#include "field.h"
#include "hitscore.h"
#include "sound.h"
#include "bullet.h"
#include "effect.h"
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//テクスチャ
#define TEXTURE_WIDTH				(128)	// キャラサイズ
#define TEXTURE_HEIGHT				(128)	// 

#define TEXTURE_MAX					(7)		// テクスチャの数
#define TEXTURE_SLIME_MAX			(3)		//スライムのテクスチャ数
#define TEXTURE_MANDRAKE_MAX		(3)
#define TEXTURE_WYVERN_MAX			(5)
#define TEXTURE_ENEMY_MAX			(TEXTURE_SLIME_MAX + TEXTURE_MANDRAKE_MAX + TEXTURE_WYVERN_MAX)
//アニメーション
#define TEXTURE_PATTERN_DIVIDE_X	(2)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(7)		// アニメーションの切り替わるWait値

#define HIT_CLEAR_MAX				(20)

//エネミー移動デファイン
#define ENEMY_MOVE_MAX				(100.0f)
#define KNOCK_BACK_DIS				(50.0f)
#define STOP_TIME_FRAME_MAX			(120)
#define ENEMY_SOUND_DISMAX			(700)

//Slime
#define TEXTURE_SLIME_WIDTH		(80)	//
#define TEXTURE_SLIME_HEIGHT	(40)	//

#define TEXTURE_SLIME_WAIST_WIDTH (64)	//体の当たり判定
#define TEXTURE_SLIME_WAIST_HEIGHT (45)

#define TEXTURE_SLIME_FOOT_WIDTH (50)	//足元の当たり判定
#define TEXTURE_SLIME_FOOT_HEIGHT (20)

#define	SLIME_HPMAX					(100)

//Mandrake
#define TEXTURE_MANDRAKE_WIDTH		(60)	//
#define TEXTURE_MANDRAKE_HEIGHT		(100)	//

#define TEXTURE_MANDRAKE_FOOT_WIDTH (50)	//足元の当たり判定
#define TEXTURE_MANDRAKE_FOOT_HEIGHT (20)

#define	MANDRAKE_HPMAX					(100)

//Wyvern
#define TEXTURE_WYVERN_WIDTH		(360)	//
#define TEXTURE_WYVERN_HEIGHT		(340)	//
#define	WYVERN_HPMAX					(500)
#define WYVERN_MOVETBL_MAX			(3)
#define SKILLFIRE_DAMAGE			(20)
#define TEXTURE_SKILLFIRE_WIDTH		(768)
#define TEXTURE_SKILLFIRE_HEIGHT	(768)

#define TEXTURE_SKILLFIRE_DIVIDEX	(5)
#define TEXTURE_SKILLFIRE_DIVIDEY	(4)
#define TEXTURE_SKILLFIRE_PATTERN_MAX	(TEXTURE_SKILLFIRE_DIVIDEX * TEXTURE_SKILLFIRE_DIVIDEY)
#define SKILLFIRE_WAITMAX	(1)


#define FIREBALL_MAX								(50)//数
#define FIREBALL_RADIUS				(80)
#define FIREBALL_TIME_MAX			(500)
#define TEXTURE_FIREBALL_WIDTH		(86)
#define TEXTURE_FIREBALL_HEIGHT		(86)
#define TEXTURE_FIREBALL_WAIT_MAX							(5)
#define TEXTURE_FIREBALL_PATTERN_DIVIDE_X					(5)
#define TEXTURE_FIREBALL_PATTERN_DIVIDE_Y					(1)
#define TEXTURE_FIREBALL_ANIM_PATTERN_MAX					(TEXTURE_FIREBALL_PATTERN_DIVIDE_X *   TEXTURE_FIREBALL_PATTERN_DIVIDE_Y)	// アニメーションパターン数





//BOSS
#define TEXTURE_BOSS_WIDTH	(520)	//
#define TEXTURE_BOSS_HEIGHT	(520)	//

#define TEXTURE_BOSS_WAIST_WIDTH (400)	//体の当たり判定
#define TEXTURE_BOSS_WAIST_HEIGHT (260)

#define TEXTURE_BOSS_FOOT_WIDTH (500)	//足元の当たり判定
#define TEXTURE_BOSS_FOOT_HEIGHT (20)

#define	BOSS_HPMAX					(900)

#define ATTACK_TIME_MAX			(30)

#define SKILL1_BULLET_MAX		(10)
#define	BOSS_DEADTIME_MAX			(1.0f)
//SKILL2
#define SKILL2_MAX								(5)
#define SKILL2_WAIT_MAX							(5)
#define SKILL2_PATTERN_DIVIDE_X					(5)
#define SKILL2_PATTERN_DIVIDE_Y					(2)
#define SKILL2_ANIM_PATTERN_NUM					(SKILL2_PATTERN_DIVIDE_Y *  SKILL2_PATTERN_DIVIDE_X)	// アニメーションパターン数

//SKILL2POS
#define SKILL2POS_WAIT_MAX						(5)
#define SKILL2POS_PATTERN_DIVIDE_X				(2)
#define SKILL2POS_PATTERN_DIVIDE_Y				(1)
#define SKILL2POS_ANIM_PATTERN_NUM				(SKILL2POS_PATTERN_DIVIDE_X *  SKILL2POS_PATTERN_DIVIDE_Y)	// アニメーションパターン数

//SKILL3
#define SKILL3_MAX								(20)
#define SKILL3_WAIT_MAX							(5)
#define SKILL3_PATTERN_DIVIDE_X					(4)
#define SKILL3_PATTERN_DIVIDE_Y					(1)
#define SKILL3_ANIM_PATTERN_NUM					(SKILL3_PATTERN_DIVIDE_Y *  SKILL3_PATTERN_DIVIDE_X)

//SKILL4

#define SKILL4_MAX								(100)
#define SKILL4_BULLET_MAX						(6)
#define SKILL4_DISGAP							(140)
#define SKILL4_POS_MAX							(20)

#define SKILL4_WAIT_MAX							(5)
#define SKILL4_PATTERN_DIVIDE_X					(5)
#define SKILL4_PATTERN_DIVIDE_Y					(3)
#define SKILL4_ANIM_PATTERN_NUM					(SKILL4_PATTERN_DIVIDE_Y *  SKILL4_PATTERN_DIVIDE_X)

//？
//#define TEXTURE_COLLISION_WIDTH		(80)	//
//#define TEXTURE_COLLISION_HEIGHT	(100)	//
//
//#define TEXTURE_COLLISION_WAIST_WIDTH (64)	//体の当たり判定
//#define TEXTURE_COLLISION_WAIST_HEIGHT (45)
//
//#define TEXTURE_COLLISION_HEAD_WIDTH (40)	//頭の当たり判定
//#define TEXTURE_COLLISION_HEAD_HEIGHT (30)
//
//#define TEXTURE_COLLISION_FOOT_WIDTH (80)	//足元の当たり判定
//#define TEXTURE_COLLISION_FOOT_HEIGHT (10)
// 
//#define ENEMY1_MOVE_TIME			(200)
//#define ENEMY2_MOVE_TIME			(300)
//#define ENEMY3_MOVE_TIME			(400)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static ID3D11ShaderResourceView *g_Texture_Enemy[TEXTURE_ENEMY_MAX] = { NULL };	// スライムテクスチャ情報
static ID3D11ShaderResourceView* g_Texture_Boss[BOSS_STATE_MAX] = { NULL };	// スライムテクスチャ情報

static char *g_TextureName[TEXTURE_MAX] = {

	"data/TEXTURE/bar_white.png",
	"data/EFFECT/enemy_skill02.png",
	"data/EFFECT/skill02_pos.png",
	"data/CHARA/Mandrake-Walk.png",
	"data/EFFECT/enemy_skill04.png",
	"data/EFFECT/Wyvern_Fire.png",
	"data/EFFECT/effect_wyvern_attack.png",

};

//Slimeのテクスチャ設定
static char* g_TextureName_Enemy[TEXTURE_ENEMY_MAX] = {
	"data/CHARA/enemy_slime_stand.png",
	"data/CHARA/enemy_slime_walk.png",
	"data/CHARA/enemy_slime_attack.png",
	"data/CHARA/Mandrake-Idle.png",
	"data/CHARA/Mandrake-Walk.png",
	"data/CHARA/Mandrake-Hit.png",
	"data/CHARA/Wyvern-Idle.png",
	"data/CHARA/Wyvern-Walk.png",
	"data/CHARA/Wyvern-Attack.png",
	"data/CHARA/Wyvern-Skill.png",
	"data/CHARA/Wyvern-Hit.png",
};

//BOSSのテクスチャ設定
static char* g_TextureName_Boss[] = {

	"data/CHARA/Tree-Idle.png",
	"data/CHARA/Tree-Walk.png",
	"data/CHARA/Tree-Hit.png",
	"data/CHARA/Tree-Attack.png",
	"data/CHARA/Tree-Skill02.png",
	"data/CHARA/Tree-Skill03.png",
	"data/CHARA/Tree-Skill04.png",
	"data/CHARA/Tree-Walk.png",
	"data/CHARA/Tree-Down.png",
};

static	int g_Enemy_state_AnimeMax[TEXTURE_ENEMY_MAX][4] = { //各テクスチャの分割数
//  Pattern_Max	divideY  divideX	animewait
		//SLIME
		 1,		1,			1,			10,	    //STAND
		 4,		1,			4,			 5,		//WALK
		 7,		1,			7,			 4,		//ENEMY_STATE_ATTACK001
		//MANDRAKE
		 4,		1,			4,			 5,	    //STAND
		 4,		1,			4,			 5,		//WALK
		 4,		1,			4,			 5,		//HIT
		 //WYVERN
		 8,		1,			8,			 3,		//IDLE
		 8,		1,			8,			 2,		//WALK
		 8,		1,			8,			 5,		//ATTACK
		 8,		1,			8,			 10,	//SKILL
		 8,		1,			8,			 10,		//HIT
};

static	int g_boss_state_AnimeMax[BOSS_STATE_MAX][4] = { //各テクスチャの分割数
//  Pattern_Max	divideY  divideX	animewait
		 8,		1,			8,			 2,		//IDLE
		 8,		1,			8,			 5,		//WALK
		 8,		1,			8,			 10,	//HIT
		 8,		1,			8,			 5,		//SKILL001
		 8,		1,			8,			 5,		//SKILL002
		 8,		1,			8,			 8,		//SKILL003
		 8,		1,			8,			 8,		//SKILL004
		 8,		1,			8,			 1,		//SKILL005
		 2,		1,			2,			 10,	//DOWN
};


static BOOL		 g_Load = FALSE;			// 初期化を行ったかのフラグ
	
static ENEMY	g_Enemy[ENEMY_MAX];		// エネミー構造体
static WYVERN	g_Wyvern[WYVERN_MAX];
static BOSS		g_Boss[BOSS_MAX];

static SKILLFIRE	g_SkillFire;
static FIREBALL		g_Fireball[FIREBALL_MAX];


static SKILL	g_Skill002[SKILL2_MAX];
static SKILL3	g_Skill003[SKILL3_MAX];
static SKILL3	g_Skill004[SKILL4_MAX];

static ANI		g_Skill002Pos[SKILL2_MAX];
static int		g_EnemyCnt;
static float    g_S2Remaintimer;


int	g_clearCnt;
float g_knockmoveCnt;
float g_time;
int g_stoptime;
int g_EnemyMax;
int g_initMoveX;
int g_bulletdir;
int g_HpbulletNum;
float g_timer;

static int FireballOnetimeCnt;

float g_Bosswalktimer;
float g_skill01timer;
float g_skill02timer;

static int g_skill02Cnt;
static int g_skill02frameCnt;

static int g_skill03Cnt;
static int g_skill03delayCnt;

static int g_skill04Cnt;
static int g_skill04delayCnt;

static int g_Downtimer;
static int g_EPdelayCnt;
static BOOL g_EPdelayFlag;
static float g_Walktimer;
static float g_Idletimer;
static float g_movetimer;
static	float g_Deadtimer;
static int g_DeadCnt;
static BOOL	g_DeadBoss;

BOOL g_closeP;
BOOL g_changeable;
BOOL g_Redclear;

float g_walkTimer;

//チュートリアルのエネミーの位置
static XMFLOAT3 g_Tutorial_EPos[] = {
	XMFLOAT3(2000.0f,300.0f,0.0f),
	XMFLOAT3(2500.0f,300.0f,0.0f),
	XMFLOAT3(3300.0f,300.0f,0.0f),
};

static XMFLOAT3 g_Game_EPos[] = {
	XMFLOAT3(2000.0f,300.0f,0.0f),
	XMFLOAT3(3000.0f,300.0f,0.0f),
	XMFLOAT3(3500.0f,300.0f,0.0f),
};

static INTERPOLATION_DATA2 g_MoveTbl0[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(1600.0f,  650.0f, 0.0f),	60 },
	{ XMFLOAT3(1400.0f,  650.0f, 0.0f),	60 },
};

static INTERPOLATION_DATA2 g_MoveTbl1[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(1600.0f,  650.0f, 0.0f),	60 },
	{ XMFLOAT3(200.0f,  650.0f, 0.0f),	50 },
};

static INTERPOLATION_DATA2 g_WyvernMoveTbl[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(1000.0f,  2450.0f, 0.0f),	100 },
	{ XMFLOAT3(650.0f,  2600.0f, 0.0f),	100 },
	{ XMFLOAT3(1665.0f,  2800.0f, 0.0f),100 },
};


//static INTERPOLATION_DATA2 g_WyvernMoveTbl1[] = {
//	//座標									回転率							拡大率							時間
//	{ XMFLOAT3(900.0f,  2300.0f, 0.0f),	60 },
//	{ XMFLOAT3(650.0f,  2700.0f, 0.0f),	60 },
//};
//
//static INTERPOLATION_DATA2 g_WyvernMoveTbl2[] = {
//	//座標									回転率							拡大率							時間
//	{ XMFLOAT3(650.0f,  2700.0f, 0.0f),	60 },
//	{ XMFLOAT3(1000.0f,  2800.0f, 0.0f),60 },
//};
//
//static INTERPOLATION_DATA2 g_WyvernMoveTbl3[] = {
//	//座標									回転率							拡大率							時間
//	{ XMFLOAT3(1000.0f,  2800.0f, 0.0f),60 },
//	{ XMFLOAT3(900.0f,  2300.0f, 0.0f),	60 },
//};

static XMFLOAT3 g_Skill004_Pos[SKILL4_POS_MAX][SKILL4_BULLET_MAX] =
{
	
	//100	240	380	520	660	800
	//1
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	//2
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	//3

	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	//4
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),

	//5
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f)	,XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),

};

//
//
//static INTERPOLATION_DATA g_MoveTbl1[] = {
//	//座標									回転率							拡大率							時間
//	{ XMFLOAT3(2000.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY2_MOVE_TIME },
//	{ XMFLOAT3(1200.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY2_MOVE_TIME },
//};
//
//
//static INTERPOLATION_DATA g_MoveTbl2[] = {
//	//座標									回転率							拡大率							時間
//	{ XMFLOAT3(3000.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY3_MOVE_TIME },
//	{ XMFLOAT3(3400.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY3_MOVE_TIME },
//};
//
//
//
//
//static INTERPOLATION_DATA* g_MoveTblAdr[] =
//{
//	g_MoveTbl0,
//	g_MoveTbl1,
//	g_MoveTbl2,
//
//};
static INTERPOLATION_DATA2* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
};

static INTERPOLATION_DATA2* g_MoveTblWAdr[] =
{
	 g_WyvernMoveTbl,
	/* g_WyvernMoveTbl2,
	 g_WyvernMoveTbl3,*/
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

	
	// エネミー構造体の初期化
	g_clearCnt = 0;
	g_EnemyCnt = 0;
	g_knockmoveCnt = 0;
	g_time = 0;
	g_stoptime = 0;
	g_Bosswalktimer = 0;
	g_skill01timer = 0.0f;

	g_skill02Cnt = 0;
	g_skill02timer = 0.0f;
	g_skill02frameCnt = 0;

	g_skill03Cnt = 0;
	g_skill03delayCnt = 0;

	g_S2Remaintimer = 0.0f;
	g_initMoveX = 5.0f;
	g_bulletdir = BULLET_DIR_LEFT;
	g_HpbulletNum = 0;
	g_closeP = FALSE;
	g_changeable = TRUE;
	g_EPdelayCnt = 0;
	g_Walktimer = 0.0f;
	g_Idletimer = 0.0f;
	g_Downtimer = 0.0f;
	g_movetimer = 0.0f;
	g_Deadtimer = 1.0f;
	g_DeadBoss = FALSE;
	g_DeadCnt = 0;
	FireballOnetimeCnt = 0;

	g_Redclear = FALSE;

	g_walkTimer = 0.0f;
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

	for (int i = 0; i < TEXTURE_ENEMY_MAX; i++)
	{
		g_Texture_Enemy[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName_Enemy[i],
			NULL,
			NULL,
			&g_Texture_Enemy[i],
			NULL);
	}

	for (int i = 0; i < BOSS_STATE_MAX; i++)
	{
		g_Texture_Boss[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName_Boss[i],
			NULL,
			NULL,
			&g_Texture_Boss[i],
			NULL);
	}



	switch (GetMode())
	{
	case MODE_TUTORIAL:

		g_EnemyMax = ENEMY_TUTORIAL_MAX;

		for (int i = 0; i < ENEMY_MAX; i++)
			g_Enemy[i].type = ENEMY_TYPE_SLIME;

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_Enemy[i].use = FALSE;
			g_Enemy[i].state = SLIME_STATE_IDLE;
			//座標方向など
			g_Enemy[i].opos = g_Enemy[i].pos;							//前の座標を保存する
			g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			
			//テクスチャ
			g_Enemy[i].texNo = 0;
			g_Enemy[i].w = TEXTURE_SLIME_WIDTH;
			g_Enemy[i].h = TEXTURE_SLIME_HEIGHT;
			//アニメーション
			g_Enemy[i].countAnim = 0;
			g_Enemy[i].patternAnim = 0;

			//ステータス
			g_Enemy[i].hp = 100;
			g_Enemy[i].damagedType = -1;

			//当たった時のヒット
			g_Enemy[i].clear = FALSE;						//透明状態
			g_Enemy[i].hit = FALSE;							//ヒットされている？NO
			g_Enemy[i].hitting = FALSE;						//ヒットされている最中
			g_Enemy[i].hitCnt = 0;							
	
			//移動パラメータ
			g_Enemy[i].onGround = FALSE;					//着地している？NO
			g_Enemy[i].gravityCnt = 0.0f;					//重力

			g_Enemy[i].dir = CHAR_DIR_RIGHT;				//進む方向を決める
			g_Enemy[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);	//移動量
			g_Enemy[i].movedis = 0.0f;						//停止状態からどれくらい移動した？
			g_Enemy[i].needmovedis = 0.0f;					//エネミーが移動必要な全距離

			g_Enemy[i].knockmoveX = g_Enemy[i].move.x;		//ノックバックされたら移動する

			g_Enemy[i].stop = TRUE;							//停止状態になってる？YES
			g_Enemy[i].stopframe = 60.0f;					//停止状態続いている時間

			g_EnemyCnt++;
		}

		for (int i = 0; i < ENEMY_TUTORIAL_MAX; i++)
		{	//チュートリアルの敵数を決める
			g_Enemy[i].use = TRUE;
			g_Enemy[i].pos = g_Tutorial_EPos[i];
			g_Enemy[i].str = 10;
		}

		g_EnemyCnt = ENEMY_TUTORIAL_MAX;
		break;

	case MODE_GAME:

		g_EnemyMax = ENEMY_MAX;

		//普通のエネミーの初期化処理


		for (int i = 0; i < ENEMY_MAX; i++)
			g_Enemy[i].state = ENEMY_TYPE_SLIME;

		g_Enemy[2].type = ENEMY_TYPE_MANDRAKE;
		g_Enemy[6].type = ENEMY_TYPE_MANDRAKE;
		g_Enemy[7].type = ENEMY_TYPE_MANDRAKE;
		g_Enemy[10].type = ENEMY_TYPE_MANDRAKE;
		g_Enemy[12].type = ENEMY_TYPE_MANDRAKE;
		g_Enemy[13].type = ENEMY_TYPE_MANDRAKE;

		InitSlime();
		InitMandrake();
		InitWyvern();
		InitFireSkill();
		InitFireball();


		//座標セット

		g_Enemy[0].pos = XMFLOAT3(2800.0f, 500.0f, 0.0f);
		g_Enemy[1].pos = XMFLOAT3(3100.0f, 500.0f, 0.0f);
		g_Enemy[2].pos = XMFLOAT3(3600.0f, 1200.0f, 0.0f);
		g_Enemy[3].pos = XMFLOAT3(2540.0f, 1600.0f, 0.0f);
		g_Enemy[4].pos = XMFLOAT3(2000.0f, 1300.0f, 0.0f);
		g_Enemy[5].pos = XMFLOAT3(1800.0f, 1450.0f, 0.0f);
		g_Enemy[6].pos = XMFLOAT3(2200.0f, 1920.0f, 0.0f);
		g_Enemy[7].pos = XMFLOAT3(2000.0f, 1920.0f, 0.0f);
		g_Enemy[8].pos = XMFLOAT3(660.0f, 1800.0f, 0.0f);
		g_Enemy[9].pos = XMFLOAT3(400.0f, 1800.0f, 0.0f);
		g_Enemy[10].pos = XMFLOAT3(550.0f, 2600.0f, 0.0f);
		g_Enemy[11].pos = XMFLOAT3(1100.0f, 2600.0f, 0.0f);
		g_Enemy[12].pos = XMFLOAT3(1780.0f, 2800.0f, 0.0f);
		g_Enemy[13].pos = XMFLOAT3(550.0f, 2000.0f, 0.0f);
		g_Enemy[14].pos = XMFLOAT3(550.0f, 2000.0f, 0.0f);

		g_EnemyCnt = ENEMY_MAX;

		break;

	case MODE_BOSS:

		//ボスの初期化処理

		g_EnemyMax = BOSS_MAX;

		InitBoss();
		InitSkill002();
		InitSkill2Pos();
		InitSkill003();
		InitSkill004();
		g_EnemyCnt = BOSS_MAX;

		break;
	}
	
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);




	//// 0番だけ線形補間で動かしてみる
	//g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//// 1番だけ線形補間で動かしてみる
	//g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	//// 2番だけ線形補間で動かしてみる
	//g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	//g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	g_Load = TRUE;
	return S_OK;
}


void InitSlime(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].type != ENEMY_TYPE_SLIME)continue;

		g_Enemy[i].use = TRUE;
		g_Enemy[i].state = SLIME_STATE_IDLE;
		g_Enemy[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 中心点から表示
		g_Enemy[i].opos = g_Enemy[i].pos;							//前の座標を保存する

		//テクスチャ
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_SLIME_WIDTH;
		g_Enemy[i].h = TEXTURE_SLIME_HEIGHT;
		g_Enemy[i].texNo = SLIME_STATE_IDLE;
		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		//当たった時のヒット
		g_Enemy[i].clear = FALSE;
		g_Enemy[i].hit = FALSE;
		g_Enemy[i].hitting = FALSE;
		g_Enemy[i].hitCnt = 0;

		//ステータス

		g_Enemy[i].hp = SLIME_HPMAX;
		g_Enemy[i].damagedType = -1;

		//攻撃スキル
		g_Enemy[i].attack = FALSE;			//攻撃フラグ
		g_Enemy[i].attacktime = 0;		//攻撃時間カウンター

		g_Enemy[i].skill = FALSE;
		g_Enemy[i].skillCnt = 0;
		g_Enemy[i].skilldelaytime = 0;



		//移動
		g_Enemy[i].dir = CHAR_DIR_RIGHT;
		g_Enemy[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);		// 移動量
		g_Enemy[i].movedis = 0.0f;
		g_Enemy[i].needmovedis = 0.0f;
		g_Enemy[i].knockmoveX = g_Enemy[i].move.x;

		g_Enemy[i].onGround = FALSE;
		g_Enemy[i].gravityCnt = 0.0f;


		g_Enemy[i].stop = TRUE;
		g_Enemy[i].stopframe = 60.0f;





	}
	return;

}

void InitMandrake(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].type != ENEMY_TYPE_MANDRAKE)continue;

		g_Enemy[i].use = TRUE;
		g_Enemy[i].state = MANDRAKE_STATE_IDLE;
		g_Enemy[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 中心点から表示
		g_Enemy[i].opos = g_Enemy[i].pos;							//前の座標を保存する

		//テクスチャ
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Enemy[i].w = TEXTURE_MANDRAKE_WIDTH;
		g_Enemy[i].h = TEXTURE_MANDRAKE_HEIGHT;
		g_Enemy[i].texNo = MANDRAKE_STATE_IDLE;
		g_Enemy[i].countAnim = 0;
		g_Enemy[i].patternAnim = 0;

		//当たった時のヒット
		g_Enemy[i].clear = FALSE;
		g_Enemy[i].hit = FALSE;
		g_Enemy[i].hitting = FALSE;
		g_Enemy[i].hitCnt = 0;

		//攻撃
		g_Enemy[i].attack = FALSE;			//攻撃フラグ
		g_Enemy[i].attacktime = 0;		//攻撃時間カウンター

		//スキル
		g_Enemy[i].skill = FALSE;
		g_Enemy[i].skillCnt = 0;
		g_Enemy[i].skilldelaytime = 0;

		//ステータス
		g_Enemy[i].hp = MANDRAKE_HPMAX;
		g_Enemy[i].damagedType = -1;


		//移動
		g_Enemy[i].dir = CHAR_DIR_RIGHT;
		g_Enemy[i].move = XMFLOAT3(5.0f, 0.0f, 0.0f);	//移動量
		g_Enemy[i].movedis = 0.0f;
		g_Enemy[i].needmovedis = 0.0f;
		g_Enemy[i].onGround = FALSE;
		g_Enemy[i].gravityCnt = 0.0f;
		g_Enemy[i].stop = TRUE;
		g_Enemy[i].stopframe = 60.0f;
		g_Enemy[i].knockmoveX = g_Enemy[i].move.x;

	}
	return;
}

void InitWyvern(void)
{
	for (int i = 0; i < WYVERN_MAX; i++)
	{
		g_Wyvern[i].use = TRUE;
		g_Wyvern[i].state = WYVERN_STATE_IDLE;
		g_Wyvern[i].ostate = WYVERN_STATE_IDLE;

		g_Wyvern[i].pos = XMFLOAT3(1000.0f, 2300.0f, 0.0f);	// 中心点から表示
		g_Wyvern[i].opos = g_Wyvern[i].pos;							//前の座標を保存する

		g_Wyvern[i].countAnim = 0;
		g_Wyvern[i].patternAnim = 0;

		g_Wyvern[i].type = ENEMY_TYPE_WYVERN;
		//テクスチャ設定
		g_Wyvern[i].w = TEXTURE_WYVERN_WIDTH;
		g_Wyvern[i].h = TEXTURE_WYVERN_HEIGHT;
		g_Wyvern[i].texNo = WYVERN_STATE_IDLE;
		g_Wyvern[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);
		g_Wyvern[i].dir = CHAR_DIR_LEFT;		// 再生するアニメデータの先頭アドレスをセット

		//ステータス設定
		g_Wyvern[i].damagedType = -1;
		g_Wyvern[i].hp = WYVERN_HPMAX;
		g_Wyvern[i].flyframeCnt = 0;
		g_Wyvern[i].deadframeCnt = 0;
		g_Wyvern[i].isdead = FALSE;
		//スキル関係
		g_Wyvern[i].changable = TRUE;
		//当たった時のヒット
		g_Wyvern[i].clear = FALSE;
		g_Wyvern[i].hit = FALSE;
		g_Wyvern[i].hitting = FALSE;
		g_Wyvern[i].hitCnt = 0;

		g_Wyvern[i].attack = FALSE;			//攻撃フラグ
		g_Wyvern[i].attacktime = 0;		//攻撃時間カウンター
		g_Wyvern[i].idletime = 0.0f;

		g_Wyvern[i].skill01 = FALSE;
		g_Wyvern[i].skill01Cnt = 0;
		g_Wyvern[i].skill01delaytime = 0;
		//線形補間
		g_Wyvern[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		g_Wyvern[i].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		g_Wyvern[i].tblMax = sizeof(g_WyvernMoveTbl) / sizeof(INTERPOLATION_DATA2);	// 再生するアニメデータのレコード数をセット
	}
}



void SetSkillfire(int num)
{
	InitFireSkill();

	g_SkillFire.use = TRUE;
	g_SkillFire.pos = g_Wyvern[num].pos;
	g_SkillFire.dir = g_Wyvern[num].dir;

}

void InitFireSkill(void)
{
	g_SkillFire.use = FALSE;
	g_SkillFire.pos = XMFLOAT3(0.0f,0.0f,0.0f);
	g_SkillFire.countAnim = 0;
	g_SkillFire.patternAnim = 0;
	g_SkillFire.effectCnt = 0;
	g_SkillFire.dir = EFFECT_LEFT;
	g_SkillFire.w = TEXTURE_SKILLFIRE_WIDTH;
	g_SkillFire.h = TEXTURE_SKILLFIRE_HEIGHT;

	return;
}

void UpdateFireSkill(void)
{
	if (g_SkillFire.use == FALSE)return;

	PLAYER* player = GetPlayer();

	for (int i = 0; i < WYVERN_MAX; i++)
	{
		if (g_Wyvern[i].use == FALSE)continue;

		if (g_SkillFire.effectCnt++ > TEXTURE_SKILLFIRE_PATTERN_MAX * SKILLFIRE_WAITMAX)
		{
			InitFireSkill();
			g_Wyvern[i].changable = TRUE;
			g_Wyvern[i].state = g_Wyvern[i].state = WYVERN_STATE_IDLE;
		}
		//アニメーション
		if (g_SkillFire.countAnim++ > SKILLFIRE_WAITMAX)
		{
			g_SkillFire.countAnim = 0;
			g_SkillFire.patternAnim = (g_SkillFire.patternAnim + 1) % TEXTURE_SKILLFIRE_PATTERN_MAX;
		}

		//向き更新処理
		if (g_Wyvern[i].dir == CHAR_DIR_RIGHT)
		{
			g_SkillFire.dir = EFFECT_RIGHT;
		}
		else
		{
			g_SkillFire.dir = EFFECT_LEFT;
		}

		float posx = g_Wyvern[i].w;
		float posy = g_Wyvern[i].h + 120.0f;
		if (g_SkillFire.dir == EFFECT_LEFT)
		{
			posx *= (-1);
		}
		XMFLOAT3 pos = XMFLOAT3(g_Wyvern[i].pos.x + posx, g_Wyvern[i].pos.y + posy, 0.0f);
		g_SkillFire.pos = pos;

		//プレイヤーとの当たり判定
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (CollisionBB(g_SkillFire.pos, g_SkillFire.w, g_SkillFire.h, player[j].pos, player[j].w, player[j].h))
			{
				player[j].hit = TRUE;
				AddPlayerHP(j, -(SKILLFIRE_DAMAGE), NOTDEFENDABLE);
			}
		}

	}

	
}

void DrawFireSkill(void)
{
	if (g_SkillFire.use == FALSE)return;

	BG* bg = GetBG();
	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);

	//プレイヤーの位置やテクスチャー座標を反映
	float px = g_SkillFire.pos.x - bg->pos.x;	// プレイヤーの表示位置X
	float py = g_SkillFire.pos.y - bg->pos.y;	// プレイヤーの表示位置Y
	float pw = g_SkillFire.w;		// プレイヤーの表示幅
	float ph = g_SkillFire.h;		// プレイヤーの表示高さ

	float tw = 1.0f / TEXTURE_SKILLFIRE_DIVIDEX;							// テクスチャの幅	8パターン
	float th = 1.0f / TEXTURE_SKILLFIRE_DIVIDEY;								// テクスチャの高さ
	float tx = tw * (g_SkillFire.patternAnim % TEXTURE_SKILLFIRE_DIVIDEX);		// テクスチャの左上X座標
	float ty = th * (g_SkillFire.patternAnim / TEXTURE_SKILLFIRE_DIVIDEX);								// テクスチャの左上Y座標

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	if (g_Wyvern[0].dir == CHAR_DIR_RIGHT)tw *= (-1);
	SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

}







//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
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
	for (int i = 0; i < TEXTURE_ENEMY_MAX; i++)
	{
		if (g_Texture_Enemy[i])
		{
			g_Texture_Enemy[i]->Release();
			g_Texture_Enemy[i] = NULL;
		}
	}

	for (int i = 0; i < BOSS_STATE_MAX; i++)
	{
		if (g_Texture_Boss[i])
		{
			g_Texture_Boss[i]->Release();
			g_Texture_Boss[i] = NULL;
		}
	}
	
	

	
	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	g_EnemyCnt = 0;	// 生きてるエネミーの数
	
	switch (GetMode())
	{
	case MODE_TUTORIAL:

		UpdateSlime();
		break;

	case MODE_GAME:

		UpdateSlime();
		UpdateMandrake();
		UpdateWyvern();
		break;
	case MODE_BOSS:

		UpdateBoss();
		break;
	}


#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}


void UpdateMandrake(void)
{
	for (int i = 0; i < g_EnemyMax; i++)
	{
		if (g_Enemy[i].type != ENEMY_TYPE_MANDRAKE) continue;
		if (g_Enemy[i].use == FALSE)continue;
		if (g_Enemy[i].hp <= 0 && g_Enemy[i].use == TRUE && GetMode() != MODE_BOSS)
		{
			PlaySound(SOUND_LABEL_SE_slimedead);
			g_Enemy[i].use = FALSE;
		}
		// 生きてるエネミーだけ処理をする

		PLAYER* player = GetPlayer();
		// 地形との当たり判定用に座標のバックアップを取っておく
		g_Enemy[i].opos = g_Enemy[i].pos;
		XMFLOAT3 nextpos = g_Enemy[i].pos;

		//アニメーション処理
		AnimetionProcess(i);

		switch (GetMode())
		{
		case MODE_GAME:

			BOOL ans = CollisionBB(player[0].pos, player[0].w, player[0].h, g_Enemy[i].pos, SCREEN_WIDTH, g_Enemy[i].h + 200.0f);
			if (ans && !g_Enemy[i].hit)
			{
				//移動処理
				float waitMax = 0.5f;
				g_walkTimer += GetDeltatime();
				if (g_walkTimer >= waitMax * 1000.0f)
				{
					g_walkTimer = 0.0f;
					PlaySound(SOUND_LABEL_SE_enemyskill02);
				}

				g_Enemy[i].state = MANDRAKE_STATE_WALK;
				MoveToPlayer(i);
			}
			else
				g_Enemy[i].state = MANDRAKE_STATE_IDLE;
			//重力処理
			GravityProcess(i);

			//ヒットバック処理
			if (g_Enemy[i].hit)
			{
				g_Enemy[i].state = MANDRAKE_STATE_HIT;
				EHitbackProcess(i,ENEMY_TYPE_MANDRAKE);
			}

			// 移動が終わったらエネミーとの当たり判定
			{
				// エネミーの数分当たり判定を行う
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					// 生きてるエネミーと当たり判定をする
					if (player[j].use == TRUE)
					{
						BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
							player[j].pos, player[j].w, player[j].h);
						// 当たっている？
						if (ans == TRUE)
						{
							// 当たった時の処理
							SetBomb(g_Enemy[i].pos);
							AddPlayerHP(j, -10, DEFENDABLE);
							g_Enemy[i].use = FALSE;
						}
					}
				}
			}

			break;
		}
	}
}

void SetBomb(XMFLOAT3 pos)
{
	//for (int i = 0; i < BOMB_MAX; i++)
	//{
	//	if (g_bomb[i].use == FALSE)
	//	{
	//		g_bomb
	//	}
	//}
	SetEffect3(pos, BOMB2);
	PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
	return;
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
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

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		for (int i = 0; i < ENEMY_TUTORIAL_MAX; i++)
		{
			if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
			{									// Yes
				// テクスチャ設定
					
				float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X;
				float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y;
				float pw = TEXTURE_WIDTH;		// エネミーの表示幅
				float ph = TEXTURE_HEIGHT;		// エネミーの表示高さ;

				int animeMaxX = g_Enemy_state_AnimeMax[g_Enemy[i].state][2];
				int animeMaxY = g_Enemy_state_AnimeMax[g_Enemy[i].state][1];

				// アニメ
				float tw = 1.0f / animeMaxX;	// テクスチャの幅
				float th = 1.0f / animeMaxY;	// テクスチャの高さ
				float tx = (float)(g_Enemy[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
				float ty = (float)(g_Enemy[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

				float alpha = 1.0f;
				float R = 1.0f;
				float G = 1.0f;
				float B = 1.0f;
				
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Enemy[i].state]);

				//エネミーの位置やテクスチャー座標を反映

				// アニメーション用
				if (g_Enemy[i].dir == CHAR_DIR_RIGHT && g_Enemy[i].state != SLIME_STATE_IDLE)
				{
					tw *= (-1);
				}

				if (g_Enemy[i].clear == TRUE)
				{
					alpha = 0.7f;
					R = 0.8f;
					G = 0.3f,
					B = 0.5f;
				}

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, alpha),
					g_Enemy[i].rot.z);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);


				DrawEnemyHP(i);

			}

			
		}

		break;
	case MODE_GAME:

		DrawSlime();
		DrawMandrake();
		DrawWyvern();

		break;


	case MODE_BOSS:

		DrawBoss();
		
		DrawSkill003();
		DrawSkill004();
		break;

	}
}


void DrawSlime(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == FALSE)continue;			// このエネミーが使われている？
		if (g_Enemy[i].type != ENEMY_TYPE_SLIME)continue;

		{									// Yes
			// テクスチャ設定
			float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X;
			float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y;
			float pw = TEXTURE_WIDTH;;
			float ph = TEXTURE_HEIGHT;

			// アニメ
			int animeMaxX = g_Enemy_state_AnimeMax[g_Enemy[i].state][2];
			int animeMaxY = g_Enemy_state_AnimeMax[g_Enemy[i].state][1];
			// アニメーション用
			float tw = 1.0f / animeMaxX;	// テクスチャの幅
			float th = 1.0f / animeMaxY;	// テクスチャの高さ
			float tx = (float)(g_Enemy[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Enemy[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

			float alpha = 1.0f;
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Enemy[i].state]);
			//エネミーの位置やテクスチャー座標を反映

			if (g_Enemy[i].dir == CHAR_DIR_RIGHT) tw *= (-1);
			if (g_Enemy[i].clear == TRUE)
			{
				alpha = 0.8f;
				R = 0.5f;
				G = 0.0f,
				B = 0.0f;
			}

			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, alpha),
				g_Enemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		DrawEnemyHP(i);

	}
}

void DrawMandrake(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == FALSE)continue;			// このエネミーが使われている？
		if (g_Enemy[i].type != ENEMY_TYPE_MANDRAKE)continue;

		{									// Yes
			// テクスチャ設定
			float px = g_Enemy[i].pos.x - bg->pos.x;	// エネミーの表示位置X;
			float py = g_Enemy[i].pos.y - bg->pos.y;	// エネミーの表示位置Y;
			float pw = g_Enemy[i].w * 2;
			float ph = g_Enemy[i].h * 2;		// エネミーの表示高さ;


			// アニメ
			int animeMaxX = g_Enemy_state_AnimeMax[g_Enemy[i].state][2];
			int animeMaxY = g_Enemy_state_AnimeMax[g_Enemy[i].state][1];
			// アニメーション用
			float tw = 1.0f / animeMaxX;	// テクスチャの幅
			float th = 1.0f / animeMaxY;	// テクスチャの高さ
			float tx = (float)(g_Enemy[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Enemy[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

			float alpha = 1.0f;
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Enemy[i].state]);
			//エネミーの位置やテクスチャー座標を反映

			if (g_Enemy[i].dir == CHAR_DIR_RIGHT) tw *= (-1);
			if (g_Enemy[i].clear == TRUE)
			{
				alpha = 0.8f;
				R = 0.5f;
				G = 0.0f,
					B = 0.0f;
			}

			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, alpha),
				g_Enemy[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
		DrawEnemyHP(i);

	}
}

void DrawWyvern(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < WYVERN_MAX; i++)
	{
		if (g_Wyvern[i].use == FALSE)continue;
										
		// テクスチャ設定
		float px = g_Wyvern[i].pos.x - bg->pos.x;	// エネミーの表示位置X;
		float py = g_Wyvern[i].pos.y - bg->pos.y;	// エネミーの表示位置Y;
		float pw = g_Wyvern[i].w;
		float ph = g_Wyvern[i].h;
		// アニメ

		int animeMaxX = g_Enemy_state_AnimeMax[g_Wyvern[i].state][2];
		int animeMaxY = g_Enemy_state_AnimeMax[g_Wyvern[i].state][1];

		float tw = 1.0f / animeMaxX;	// テクスチャの幅
		float th = 1.0f / animeMaxY;	// テクスチャの高さ
		float tx = (float)(g_Wyvern[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
		float ty = (float)(g_Wyvern[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

		float Alpha = 1.0f;
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;

			
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Wyvern[i].state]);

		if (g_Wyvern[i].state == WYVERN_STATE_HIT)
		{
			pw = 300.0f;
		}
		if (g_Wyvern[i].clear == TRUE)
		{
			G = 0.0f;
			B = 0.0f;
			Alpha = 0.8f;
		}

		if (g_Wyvern[i].dir == CHAR_DIR_RIGHT)tw *= (-1);
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(R, G, B, Alpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		
		DrawWyvernHP(i);

	}
	DrawFireSkill();
	DrawFireball();
}



//=============================================================================
// Enemy構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}

void UpdateSlime(void)
{
	for (int i = 0; i < g_EnemyMax; i++)
	{
		if (g_Enemy[i].type == ENEMY_TYPE_SLIME)
		{
			if (g_Enemy[i].hp <= 0 && g_Enemy[i].use == TRUE && GetMode()!= MODE_BOSS)
			{
				PlaySound(SOUND_LABEL_SE_slimedead);
				g_Enemy[i].use = FALSE;
				int temp = rand() % 5;
				if (temp == 0)
				{
					PlaySound(SOUND_LABEL_SE_itemdrop);
					SetDropItem(g_Enemy[i].pos);

				}

			}
			// 生きてるエネミーだけ処理をする
			if (g_Enemy[i].use == TRUE)
			{
				PLAYER* player = GetPlayer();
				// 地形との当たり判定用に座標のバックアップを取っておく
				g_Enemy[i].opos = g_Enemy[i].pos;
				XMFLOAT3 nextpos = g_Enemy[i].pos;

				//アニメーション処理
				AnimetionProcess(i);

				switch (GetMode())
				{
				case MODE_TUTORIAL:

					Move(i);
					break;

				case MODE_GAME:

					BOOL ans = CollisionBB(player[0].pos, player[0].w, player[0].h, g_Enemy[i].pos, SCREEN_WIDTH, g_Enemy[i].h + 200.0f);
					if(ans)
					{
						//移動処理
						MoveToPlayer(i);

						 ans = CollisionBB(player[0].pos, player[0].w, player[0].h, g_Enemy[i].pos, g_Enemy[i].w + 400.0f, g_Enemy[i].h);
						if (ans)
						{
							//移動処理

							if (CollisionBB(g_Enemy[i].pos, g_Enemy[i].w * 2, g_Enemy[i].h, player[0].pos, player[0].w, player[0].h))
							{
								int temp = rand() % 101;
								if (temp % 100 == 0)
								{
									g_Enemy[i].attack = TRUE;

								}
							}
							if (g_Enemy[i].attack == TRUE)
							{
								g_Enemy[i].state = SLIME_STATE_ATTACK;
								Attack(i);
							}
							else
							{
								g_Enemy[i].stop = TRUE;
							}
							//プレイヤーの位置に近いとき攻撃を行う

						}
					}
				
				break;

				}


				//重力処理
				GravityProcess(i);

				if (g_Enemy[i].hit)
				{
					EHitbackProcess(i,ENEMY_TYPE_SLIME);

				}
				//ヒットバック処理

				// 移動が終わったらエネミーとの当たり判定
				{


					// エネミーの数分当たり判定を行う
					for (int j = 0; j < PLAYER_MAX; j++)
					{
						// 生きてるエネミーと当たり判定をする
						if (player[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
								player[j].pos, player[j].w, player[j].h);
							// 当たっている？
							if (ans == TRUE)
							{
								// 当たった時の処理
								//player[j].hp -= 10;
							//	player[j].use = FALSE;	// デバッグで一時的に無敵にしておくか
							}
						}
					}
				}
				g_EnemyCnt++;		// 生きてた敵の数
			}
		}
	}
}




void InitBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		g_Boss[i].use = TRUE;
		g_Boss[i].display = TRUE;
		g_Boss[i].state = BOSS_STATE_IDLE;
		g_Boss[i].ostate = BOSS_STATE_IDLE;
		g_Boss[i].opos = g_Boss[i].pos;							//前の座標を保存する
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Boss[i].countAnim = 0;
		g_Boss[i].patternAnim = 0;

		g_Boss[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		g_Boss[i].type = ENEMY_TYPE_BOSS;
		g_Boss[i].pos = XMFLOAT3(1600.0f, 500.0f, 0.0f);	// 中心点から表示
		g_Boss[i].w = TEXTURE_BOSS_WIDTH;
		g_Boss[i].h = TEXTURE_BOSS_HEIGHT;
		g_Boss[i].texNo = 0;
		g_Boss[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);
		g_Boss[i].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		g_Boss[i].dir = CHAR_DIR_LEFT;		// 再生するアニメデータの先頭アドレスをセット

		g_Boss[i].damagedType = DAMAGED_BLADE;
		g_Boss[i].hp = 900;

		//当たった時のヒット
		g_Boss[i].clear = FALSE;
		g_Boss[i].hit = FALSE;
		g_Boss[i].hitting = FALSE;
		g_Boss[i].hitCnt = 0;

		g_Boss[i].attack = FALSE;			//攻撃フラグ
		g_Boss[i].attacktime = 0;		//攻撃時間カウンター

		g_Boss[i].skill01 = FALSE;
		g_Boss[i].skill01Cnt = 0;
		g_Boss[i].skill01delaytime = 0;

		g_Boss[i].skill02 = FALSE;

		g_Boss[i].onGround = FALSE;
		g_Boss[i].gravityCnt = 0.0f;


		//g_Enemy[i].knockmoveY = g_Enemy[i].move;

		g_Boss[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA2);	// 再生するアニメデータのレコード数をセット
#ifdef _DEBUG
//g_Boss[i].hp = 1;
#endif // !_DEBUG
	}
}

void UpdateWyvern(void)
{

	for (int i = 0; i < WYVERN_MAX; i++)
	{

		if (g_Wyvern[i].use == FALSE)continue;

		// 生きてるエネミーだけ処理をする

		//
		if (g_Wyvern[i].hp <= 0 && g_Wyvern[i].isdead == FALSE)
		{
			g_Wyvern[i].state = WYVERN_STATE_HIT;
			g_Wyvern[i].countAnim = 0;
			g_Wyvern[i].patternAnim = 0;
			g_Wyvern[i].isdead = TRUE;
		}

		//アニメーション処理

		if (!g_Wyvern[i].isdead)
		{
			if (g_Wyvern[i].countAnim++ > g_Enemy_state_AnimeMax[g_Wyvern[i].state][3])
			{
				g_Wyvern[i].countAnim = 0;
				// パターンの切り替え
				g_Wyvern[i].patternAnim = (g_Wyvern[i].patternAnim + 1) % g_Enemy_state_AnimeMax[g_Wyvern[i].state][0];
			}

		}

		if (g_Wyvern[i].isdead)
		{
			WyvernDeadTimer(i);
		}
		else
		{

			PLAYER* player = GetPlayer();
			// 地形との当たり判定用に座標のバックアップを取っておく

			g_Wyvern[i].opos = g_Wyvern[i].pos;
			XMFLOAT3 nextpos = g_Wyvern[i].pos;
			g_Wyvern[i].ostate = g_Wyvern[i].state;

			for (int j = 0; i < PLAYER_MAX; i++)
			{
				if (player[j].use == FALSE)continue;

				switch (GetMode())
				{
				case MODE_GAME:

					if (!CollisionBB(player[j].pos, player[j].w, player[j].h, g_Wyvern[i].pos, g_Wyvern[i].w * 5, g_Wyvern[i].h * 5))
						break;
					else
					{
						if (player[j].pos.x < g_Wyvern[i].pos.x)g_Wyvern[i].dir = CHAR_DIR_LEFT;
						else g_Wyvern[i].dir = CHAR_DIR_RIGHT;

						//WyvernMove(i);
						if (g_Wyvern[i].hit)
						{
							EHitbackProcess(i, ENEMY_TYPE_WYVERN);

						}

						int temp = rand() % 100;

						if (g_Wyvern[i].changable)
						{
							g_Wyvern[i].idletime += GetDeltatime();

							float idleMax = 2.0f;
							if (g_Wyvern[i].idletime <= idleMax * 1000.0f)
							{
								g_Wyvern[i].state = WYVERN_STATE_IDLE;

								int waitframeMax = 40;
								if (g_Wyvern[i].flyframeCnt++ % waitframeMax == 0)
								{
									PlaySound(SOUND_LABEL_SE_Wwing);
								}
								int waitframeMax1 = 200;
								if (g_Wyvern[i].flyframeCnt % waitframeMax1 == 0)
								{
									PlaySound(SOUND_LABEL_SE_Wroar2);
								}

							}
							else
							{
								g_Wyvern[i].idletime = 0.0f;

								if ((temp >= 0 && temp < 30) || CollisionBB(g_Wyvern[i].pos, g_Wyvern[i].w, g_Wyvern[i].h, player[j].pos, player[j].w, player[j].h))
								{//乱数でエネミーを攻撃するかどうかを決める　YES攻撃を決めた
									g_Wyvern[i].state = WYVERN_STATE_SKILL;
									SetWyvernSkill(i);
								}
								else if (temp >= 30 && temp < 60)
								{
									g_Wyvern[i].state = WYVERN_STATE_ATTACK;
									PlaySound(SOUND_LABEL_SE_Wroar1);
								}
								else
								{
									g_Wyvern[i].state = WYVERN_STATE_WALK;
									SetWyvernMoveTLB(i);

								}

							}
						}

						switch (g_Wyvern[i].state)
						{
						case WYVERN_STATE_WALK:

							WyvernMoveTLB(i);
							break;

						case WYVERN_STATE_ATTACK:

							FireballTimer(i);
							break;

						case WYVERN_STATE_SKILL:

							WyvernSkillTimer(i);
							break;
						}


					}

					break;

				}
			}
		}
		if (g_Wyvern[i].state != g_Wyvern[i].ostate)
		{
			g_Wyvern[i].countAnim = 0.0f;
			g_Wyvern[i].patternAnim = 0;
		}

	}

	UpdateFireSkill();
	UpdateFireball();
}

void WyvernMove(int num)
{
	if (g_Wyvern[num].use == FALSE)return;
	
	PLAYER* player = GetPlayer();
	float Pdis = fabsf(g_Wyvern[num].pos.x - player[0].pos.x);

	if (Pdis > 400)
	{
		if (g_Wyvern[num].dir == CHAR_DIR_RIGHT)g_Wyvern[num].pos.x += 1.0f;
		else
		{
			g_Wyvern[num].pos.x -= 1.0f;
		}
	}
}

void WyvernDeadTimer(int num)
{
	if (g_Wyvern[num].hp > 0)return;

	int deadMax = 300;
	if (g_Wyvern[num].deadframeCnt++ > deadMax)
	{
		g_Wyvern[num].use = FALSE;
		SetKey(g_Wyvern[num].pos);
	}

	if (g_Wyvern[num].countAnim++ > g_Enemy_state_AnimeMax[WYVERN_STATE_HIT][3])
		g_Wyvern[num].countAnim = 0.0f;
	// パターンの切り替え
	int MaxPattern = g_Enemy_state_AnimeMax[WYVERN_STATE_HIT][0] - 1;
	if (g_Wyvern[num].patternAnim >= MaxPattern)g_Wyvern[num].patternAnim = MaxPattern;
	else g_Wyvern[num].patternAnim++;
		

}
void SetWyvernSkill(int num)
{
	if (g_Wyvern[num].use == FALSE)return;

	g_Wyvern[num].changable = FALSE;
	g_Wyvern[num].skill01 = TRUE;
	g_Wyvern[num].state = WYVERN_STATE_SKILL;
	PlaySound(SOUND_LABEL_SE_Wfire);
}

void WyvernSkillTimer(int num)
{

	if (g_Wyvern[num].use == FALSE)return;
	if (g_Wyvern[num].skill01 == FALSE)return;
	if (g_Wyvern[num].state != WYVERN_STATE_SKILL)return;
				 
	if (g_Wyvern[num].skill01Cnt++ > 80)
	{			 
		g_Wyvern[num].skill01Cnt = 0;
		g_Wyvern[num].changable = TRUE;
		g_Wyvern[num].skill01 = FALSE;
		g_Wyvern[num].state = WYVERN_STATE_IDLE;
	}	
	else if (g_Wyvern[num].skill01Cnt == 60)
	{
		SetSkillfire(num);
	}
	else 
	{			
		g_Wyvern[num].changable = FALSE;
		g_Wyvern[num].skill01 = TRUE;
		g_Wyvern[num].state = WYVERN_STATE_SKILL;
	}
	
	return;
}



void UpdateBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].hp <= 0)
		{
			g_Boss[i].use = FALSE;
			g_DeadBoss = TRUE;
			g_Deadtimer -= 0.025;
			
			if (g_Deadtimer <= 0.3f)
			{
				g_DeadCnt++;
				g_Boss[i].display = FALSE;
				if (g_DeadCnt == 1)
				{
					PlaySound(SOUND_LABEL_SE_bossbomb1);
					SetEffect3(g_Boss[i].pos, BOSS_DEADEFFECT);
				}

			}

		}
		if (g_Boss[i].use == FALSE)continue;
					
	// 生きてるエネミーだけ処理をする
	
		//アニメーション処理
		if (g_Boss[i].countAnim++ > g_boss_state_AnimeMax[g_Boss[i].state][3])
		{
			g_Boss[i].countAnim = 0.0f;
			// パターンの切り替え
	
			g_Boss[i].patternAnim = (g_Boss[i].patternAnim + 1) % g_boss_state_AnimeMax[g_Boss[i].state][0];
		}

		PLAYER* player = GetPlayer();
		// 地形との当たり判定用に座標のバックアップを取っておく

		g_Boss[i].opos = g_Boss[i].pos;
		XMFLOAT3 nextpos = g_Boss[i].pos;
		g_Boss[i].ostate = g_Boss[i].state;

		switch (GetMode())
		{
		case MODE_BOSS:

		{//移動のパターン判断

			if (g_Boss[i].hit)
			{
				//ヒットバック処理
				if (g_Boss[i].damagedType == DAMAGED_ULT)
				{
					g_changeable = FALSE;
					g_Boss[i].state = BOSS_STATE_HIT;
				}
				else 
					BossHit(i);
			}

			int temp = rand() % 100;

			if (g_changeable)
			{
				g_Idletimer += GetDeltatime();

				float idleMax = 2.0f;
				if (g_Idletimer <= idleMax * 1000.0f)
				{
					g_Boss[i].state = BOSS_STATE_IDLE;
				}
				else
				{
					g_Idletimer = 0.0f;
					if (temp >= 0 && temp < 5)
					{//乱数でエネミーを攻撃するかどうかを決める　YES攻撃を決めた
						SetSkill000(i);
						g_Boss[i].state = BOSS_STATE_WALK;
					}
					else if (temp >= 5 && temp < 25)
					{//回復あるやつ
						g_Boss[i].state = BOSS_STATE_SKILL001;
						g_HpbulletNum = rand() % SKILL1_BULLET_MAX;

					}
					else if (temp >= 25 && temp < 50)
					{//根っ子
						g_Boss[i].state = BOSS_STATE_SKILL002;
					}
					else if (temp >= 50 && temp < 65)
					{//爆弾
						g_Boss[i].state = BOSS_STATE_SKILL003;
					}
					else if (temp >= 65 && temp < 75)
					{//紅蓮地獄
						g_Boss[i].state = BOSS_STATE_SKILL004;
						//g_Boss[i].state = BOSS_STATE_WALK;
					}
					else if (temp >= 75 && temp < 85)
					{//ダッシュ
						SetSkill005(i);
						g_Boss[i].state = BOSS_STATE_SKILL005;
						//g_Boss[i].state = BOSS_STATE_WALK;
					}
					else
					{
						g_Boss[i].state = BOSS_STATE_DOWN;
	/*					g_Boss[i].state = BOSS_STATE_SKILL001;
						g_Boss[i].state = BOSS_STATE_SKILL002;
						g_Boss[i].state = BOSS_STATE_SKILL003;
						g_Boss[i].state = BOSS_STATE_SKILL004;
						g_Boss[i].state = BOSS_STATE_SKILL005;*/

					}

				}
			}

			switch (g_Boss[i].state)
			{
			case BOSS_STATE_WALK:

				UpdateBMove(i);
				break;

			case BOSS_STATE_HIT:
				BossHitback(i);
				break;

			case BOSS_STATE_SKILL001:

				Skill001Timer(i);
				break;

			case BOSS_STATE_SKILL002:
				Skill002Timer();
				break;

			case BOSS_STATE_SKILL003:

				Skill003Timer();
				break;
			case BOSS_STATE_SKILL004:
				Skill004Timer();

				break;
			case BOSS_STATE_SKILL005:

				Skill005(i);
				break;
			case BOSS_STATE_DOWN:
				Down();
				break;

			}
				
				

		}
		break;

		}

		if (g_Boss[i].hit == TRUE)
		{
			InitSkill001();
			InitSkill002();
			InitSkill003();
			InitSkill004();
		}
		if (g_Boss[i].state != BOSS_STATE_WALK && g_Boss[i].state != BOSS_STATE_SKILL005)
		{
			g_Boss[i].dir = CHAR_DIR_LEFT;
		}
		if (g_Boss[i].state != g_Boss[i].ostate)
		{
			g_Boss[i].countAnim = 0.0f;
			g_Boss[i].patternAnim = 0;

		}

		//重力処理
		BGravityProcess(i);

		// 移動が終わったらエネミーとの当たり判定
		{
			// エネミーの数分当たり判定を行う
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// 生きてるエネミーと当たり判定をする
				if (player[j].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Boss[i].pos, g_Boss[i].w, g_Boss[i].h,
						player[j].pos, player[j].w, player[j].h);
					// 当たっている？
					if (ans == TRUE)
					{
						// 当たった時の処理
						//player[j].hp -= 10;
					//	player[j].use = FALSE;	// デバッグで一時的に無敵にしておくか
					}
				}
			}
		}
	}

	UpdateSkill002();
	UpdateSkill2Pos();
	UpdateSkill003();
	UpdateSkill004();
}

void DrawBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].display == TRUE)			// このエネミーが使われている？
		{									// Yes
			// テクスチャ設定
			float px = g_Boss[i].pos.x;	// エネミーの表示位置X;
			float py = g_Boss[i].pos.y;	// エネミーの表示位置Y;
			float pw;
			float ph;

			// アニメ
			float tw;
			float th;
			float tx;
			float ty;

			int animeMaxX;
			int animeMaxY;

			float alpha = 1.0f;
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			switch (g_Boss[i].type)
			{

			case ENEMY_TYPE_BOSS:

				pw = g_Boss[i].w * g_Deadtimer;		// エネミーの表示幅
				ph = g_Boss[i].h * g_Deadtimer;		// エネミーの表示高さ;

				animeMaxX = g_boss_state_AnimeMax[g_Boss[i].state][2];
				animeMaxY = g_boss_state_AnimeMax[g_Boss[i].state][1];
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Boss[g_Boss[i].state]);

				if (g_Boss[i].state == BOSS_STATE_SKILL001 || g_Boss[i].state == BOSS_STATE_SKILL002
					|| g_Boss[i].state == BOSS_STATE_SKILL003 || g_Boss[i].state == BOSS_STATE_SKILL004)
				{
					pw = 720.0f;
				}

				tw = 1.0f / animeMaxX;	// テクスチャの幅
				th = 1.0f / animeMaxY;	// テクスチャの高さ
				tx = (float)(g_Boss[i].patternAnim % animeMaxX) * tw;	// テクスチャの左上X座標
				ty = (float)(g_Boss[i].patternAnim / animeMaxX) * th;	// テクスチャの左上Y座標

				if (g_Boss[i].dir == CHAR_DIR_RIGHT && g_Boss[i].state != BOSS_STATE_IDLE)tw *= (-1);
				if (g_Boss[i].state != BOSS_STATE_HIT &&
					g_Boss[i].state != BOSS_STATE_IDLE&&
					g_Boss[i].state != BOSS_STATE_DOWN) {
					alpha = 0.5f;
	
				}
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, alpha));

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
				break;
			}
			DrawBossHP(i);
		}
	}
}


// アニメーション  
void AnimetionProcess(int num)
{
	g_Enemy[num].countAnim += 1.0f;
	switch (g_Enemy[num].type)
	{
	case ENEMY_TYPE_SLIME:
	case ENEMY_TYPE_MANDRAKE:

		if (g_Enemy[num].countAnim > g_Enemy_state_AnimeMax[g_Enemy[num].state][3])
		{
			g_Enemy[num].countAnim = 0.0f;
			// パターンの切り替え
			g_Enemy[num].patternAnim = (g_Enemy[num].patternAnim + 1) % g_Enemy_state_AnimeMax[g_Enemy[num].state][0];
		}
		break;


	case ENEMY_TYPE_BOSS:

		if (g_Enemy[num].countAnim > g_boss_state_AnimeMax[g_Enemy[num].state][3])
		{
			g_Enemy[num].countAnim = 0.0f;
			// パターンの切り替え
			g_Enemy[num].patternAnim = (g_Enemy[num].patternAnim + 1) % g_boss_state_AnimeMax[g_Enemy[num].state][0];
		}

		break;

	}
}

//移動処理
void Move(int num)
{

	if (g_Enemy[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Enemy[num].pos;

		if ((g_Enemy[num].stop))//移動していないなら
		{

			float movedis = (float)(rand() % 150);		//エネミーが移動すべき距離
			float dir = (float)(rand() % 2);		//エネミーの向き

			g_Enemy[num].state = BOSS_STATE_IDLE;		//エネミーたちっぱ
			g_Enemy[num].needmovedis = movedis;		//移動必要な距離を初期化
			g_Enemy[num].movedis = 0.0f;     //移動計算
			g_Enemy[num].walktimer = 0.0f;     //移動計算

			if (dir)						//移動の方向を決める
			{
				g_Enemy[num].dir = CHAR_DIR_LEFT;
			}
			else g_Enemy[num].dir = CHAR_DIR_RIGHT;

			
			g_timer += GetDeltatime();
			float waitMax = 0.1f;//second
			if (g_timer >= waitMax * 1000.0f)//to mili
			{
				
				g_Enemy[num].stop = FALSE;
				
				g_timer = 0.0f;
			}

		}

		else
		{//移動している

			if (g_Enemy[num].movedis >= g_Enemy[num].needmovedis)
			{
				g_Enemy[num].stop = TRUE;
				g_Enemy[num].state = BOSS_STATE_IDLE;
			}
			else
			{
				g_Enemy[num].state = BOSS_STATE_WALK;
				if (g_Enemy[num].dir == CHAR_DIR_LEFT)
				{
					nextpos.x -= g_Enemy[num].move.x;
				}
				else if (g_Enemy[num].dir == CHAR_DIR_RIGHT)
				{
					nextpos.x += g_Enemy[num].move.x;
				}


				EnemyMoveSound(num);
			



				switch (g_Enemy[num].type)
				{
				case ENEMY_TYPE_SLIME:

					if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
					{
						g_Enemy[num].pos = nextpos;
						g_Enemy[num].movedis += g_Enemy[num].move.x;
					}
					else
					{

						g_Enemy[num].dir = !(g_Enemy[num].dir);
					}
					break;

				case ENEMY_TYPE_BOSS:

					if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
					{
						g_Enemy[num].pos = nextpos;
						g_Enemy[num].movedis += g_Enemy[num].move.x;
					}
					else
					{
						g_Enemy[num].dir = !(g_Enemy[num].dir);
					}
					break;

				default:
					break;
				}




				//MAPの右端まで来た？
				if (g_Enemy[num].pos.x > MAP_W)
				{
					g_Enemy[num].pos.x = MAP_W;
					g_Enemy[num].dir = CHAR_DIR_LEFT;
				}


				//MAPの左端まで来た？
				if (g_Enemy[num].pos.x < 0.0f)
				{
					g_Enemy[num].pos.x = 0.0f;
					g_Enemy[num].dir = CHAR_DIR_RIGHT;

				}
			}

			

		}
		
	}

	return;
}


void EnemyMoveSound(int num)

{
	g_Enemy[num].walktimer += GetDeltatime();

	float waitMax = 0.6f;
	if (g_Enemy[num].walktimer >= waitMax * 1000.0f)
	{
		g_Enemy[num].walktimer = 0;
		
		PlaySound(SOUND_LABEL_SE_slimemove);

		PLAYER* player = GetPlayer();
		float disx = (float)fabs(player[0].pos.x - g_Enemy[num].pos.x);
		float disy = (float)fabs(player[0].pos.y - g_Enemy[num].pos.y);
		float soundDis = disx + disy;
		if (soundDis == 0)soundDis = 1;
		float soundMax = ENEMY_SOUND_DISMAX / soundDis;
		(soundMax > 1.0) ? soundMax = 1.0f : soundMax;

		if (soundDis <= ENEMY_SOUND_DISMAX)
		{
			SetVolumeSound(SOUND_LABEL_SE_slimemove, soundMax);
		}
		else
		{
			SetVolumeSound(SOUND_LABEL_SE_slimemove, 0.0f);
		}


	}
}

//プレイヤーに攻撃されたらの移動パターン
void MoveToPlayer(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Enemy[num].use == TRUE)
	{

		{
			//状態初期化
			if(g_Enemy[num].type == ENEMY_TYPE_SLIME)
				g_Enemy[num].state = SLIME_STATE_WALK;

			else if(g_Enemy[num].type == ENEMY_TYPE_MANDRAKE)
				g_Enemy[num].state = MANDRAKE_STATE_WALK;

			//エネミーが移動する先はプレイヤーのどころ
			g_Enemy[num].nextpos = player[0].pos;

			XMFLOAT3 nextpos = g_Enemy[num].pos;
			{//移動している

				{//プレイヤーの位置はエネミーの右か左かを判断する
					if (g_Enemy[num].pos.x - player[0].pos.x >= 0)g_Enemy[num].dir = CHAR_DIR_LEFT;
					else if (g_Enemy[num].pos.x - player[0].pos.x < 0)g_Enemy[num].dir = CHAR_DIR_RIGHT;
				}


				if (g_Enemy[num].dir == CHAR_DIR_LEFT)
				{
					if (!CollisionBB(g_Enemy[num].pos, g_Enemy[num].w, g_Enemy[num].h, player[0].pos, player[0].w, player[0].h))
					{
						nextpos.x -= g_Enemy[num].move.x;

					}
				}
				else if (g_Enemy[num].dir == CHAR_DIR_RIGHT)
				{
					if (!CollisionBB(g_Enemy[num].pos, g_Enemy[num].w, g_Enemy[num].h, player[0].pos, player[0].w, player[0].h))
					{
						nextpos.x += g_Enemy[num].move.x;

					}
				}

				EnemyMoveSound(num);

				if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
				{
					g_Enemy[num].pos = nextpos;
					g_Enemy[num].movedis += g_Enemy[num].move.x;
				}
				else
				{

					g_Enemy[num].dir = !(g_Enemy[num].dir);
				}




				//MAPの右端まで来た？
				if (g_Enemy[num].pos.x > MAP_W)
				{
					g_Enemy[num].pos.x = MAP_W;
					g_Enemy[num].dir = CHAR_DIR_LEFT;
				}


				//MAPの左端まで来た？
				if (g_Enemy[num].pos.x < 0.0f)
				{
					g_Enemy[num].pos.x = 0.0f;
					g_Enemy[num].dir = CHAR_DIR_RIGHT;

				}
			}
		}
		
	}

	return;
}


//プレイヤーへの攻撃パターン
void Attack(int num)
{
	PLAYER* player = GetPlayer();
	BOOL ans;
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (g_Enemy[num].use == TRUE && g_Enemy[num].attack == TRUE)
		{
			//状態初期化
			g_Enemy[num].state = SLIME_STATE_ATTACK;

			if (g_Enemy[num].attacktime++ >= ATTACK_TIME_MAX)
			{//攻撃終了確認
				g_Enemy[num].attacktime = 0;
				g_Enemy[num].attack = FALSE;
				g_Enemy[num].state = SLIME_STATE_WALK;
			}
			if (g_Enemy[num].attacktime == 20)
			{
				PlaySound(SOUND_LABEL_SE_slimeattack);
				switch (g_Enemy[num].type)
				{
				case ENEMY_TYPE_SLIME:

					if (player[j].state == PLAYER_STATE_GUARD && (player[j].dir != g_Enemy[num].dir))
					{
						EFFECT3* effect = GetEffect3();
						for (int i = 0; i < EFFECT_MAX; i++)
						{
							if (effect[i].type == GUARD && g_Skill003[i].use == TRUE && 
								(CollisionBB(effect[i].pos, effect[i].w / 2, effect[i].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
							{
								PlaySound(SOUND_LABEL_SE_guard);
							}
						}

					}
					else if (CollisionBB(g_Enemy[num].pos, TEXTURE_SLIME_WAIST_WIDTH * 2, TEXTURE_SLIME_WAIST_HEIGHT, player[0].pos, player[0].w, player[0].h))
					{
						int str = rand() % 10;
						//SetPlayerHP(20);

						player[j].hit = TRUE;
						player[j].hp -= str;
						SetEffect3(player[j].pos,HIT2);
						SetHitScore(-str,player[j].pos, DAMAGE_PLAYER);

#ifndef _DEBUG	// リリース番だけHPを減る
						
#endif
					}
					break;

				}
			}
		}

	}
	
	return;
}

//重力処理
void GravityProcess(int num)
{

	if (g_Enemy[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Enemy[num].pos;
		{//重力処理
							//足元の判定
			nextpos = g_Enemy[num].pos;
			XMFLOAT3 pfootpos;
			BOOL ans = FALSE;
			switch (g_Enemy[num].type)
			{
			case ENEMY_TYPE_SLIME:

				pfootpos = XMFLOAT3(g_Enemy[num].pos.x, g_Enemy[num].pos.y + g_Enemy[num].h / 2 + 8, 0.0f);//足元の座標
				ans = FieldCollision(pfootpos, TEXTURE_SLIME_FOOT_WIDTH, TEXTURE_SLIME_FOOT_HEIGHT);

				break;

			case ENEMY_TYPE_MANDRAKE:

				pfootpos = XMFLOAT3(g_Enemy[num].pos.x, g_Enemy[num].pos.y + g_Enemy[num].h, 0.0f);//足元の座標
				ans = FieldCollision(pfootpos, TEXTURE_MANDRAKE_FOOT_WIDTH, TEXTURE_MANDRAKE_FOOT_HEIGHT);

				break;

			case ENEMY_TYPE_BOSS:

				pfootpos = XMFLOAT3(g_Enemy[num].pos.x, g_Enemy[num].pos.y + g_Enemy[num].h / 2 - 10, 0.0f);//足元の座標
				ans = FieldCollision(pfootpos, TEXTURE_BOSS_FOOT_WIDTH, TEXTURE_BOSS_FOOT_HEIGHT);

				break;
			}

			if (ans)//足場がある
			{
				g_Enemy[num].onGround = TRUE;
				g_Enemy[num].gravityCnt = 0;
			}
			else
			{
				g_Enemy[num].onGround = FALSE;
				g_Enemy[num].gravityCnt += GRAVITY;
				float acc = g_Enemy[num].gravityCnt * 5;
				float totalmove = g_Enemy[num].gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Enemy[num].w / 2, g_Enemy[num].h / 16);

			if (!ans)
			{

				g_Enemy[num].pos = nextpos;
			}
		}
	}
}

void EHitbackProcess(int num,int etype)
{
	PLAYER* player = GetPlayer();

	int damage = 0;

	switch (etype)
	{
	case ENEMY_TYPE_SLIME:
	case ENEMY_TYPE_MANDRAKE:
		if (g_Enemy[num].use == FALSE)return;
		if (g_Enemy[num].hit == FALSE)return;


		switch (g_Enemy[num].damagedType) {//○○に当てられた処理
		case DAMAGED_BLADE:

			if (g_Enemy[num].hitCnt > 60)
			{	//時間オーバー
				g_Enemy[num].hit = FALSE;
				g_Enemy[num].hitting = FALSE;
				g_Enemy[num].clear = FALSE;
				g_Enemy[num].hitCnt = 0;
			}

			if (g_Enemy[num].hit == TRUE && g_Enemy[num].hitting == FALSE)
			{//あてられた
				//g_Enemy[num].hp -= player[0].str;
				g_Enemy[num].hitting = TRUE;
				g_Enemy[num].hitCnt = 0;
				g_time = 0;
				g_knockmoveCnt = 0;
				g_Enemy[num].knockmoveX = 20.0f;

				int str = player[0].str + rand() % (int)(player[0].str / 2);

				g_Enemy[num].hp -= player[0].str;
				SetHitScore(-str, g_Enemy[num].pos, DAMAGE_ENEMY);

				PlaySound(SOUND_LABEL_SE_slimehit);

			}

			if (g_Enemy[num].hitting == TRUE && g_Enemy[num].hit == TRUE)
			{//当てられた途中？
				{	//まだ時間内

					if (g_Enemy[num].hitCnt % HIT_CLEAR_MAX == 0)
					{
						g_Enemy[num].clear = !(g_Enemy[num].clear);
					}


					XMFLOAT3 tempmovex;	//エネミーが毎フレイム移動する距離
					//if (player[i].dir == CHAR_DIR_RIGHT)
					//{
					//	tempmovex = XMFLOAT3(g_Enemy[num].knockmoveX, 0.0f, 0.0f);
					//}
					//else
					//{
					//	tempmovex = XMFLOAT3((-1) * g_Enemy[num].knockmoveX, 0.0f, 0.0f);
					//}
					if (g_Enemy[num].pos.x - player[0].pos.x >= 0)
					{
						tempmovex = XMFLOAT3(g_Enemy[num].knockmoveX, 0.0f, 0.0f);
					}
					else
					{
						tempmovex = XMFLOAT3((-1) * g_Enemy[num].knockmoveX, 0.0f, 0.0f);
					}

					XMVECTOR vtempmove = XMLoadFloat3(&tempmovex);
					XMVECTOR vepos = XMLoadFloat3(&g_Enemy[num].pos);
					XMFLOAT3 epos = g_Enemy[num].pos;
					XMStoreFloat3(&epos, vepos + vtempmove);//移動距離を加える
					if (!FieldCollision(epos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
					{
						g_Enemy[num].pos = epos;

					}


					g_Enemy[num].knockmoveX = g_Enemy[num].knockmoveX - (g_knockmoveCnt);
					g_knockmoveCnt += 1.5f;
					if (g_Enemy[num].knockmoveX <= 0.0f) g_Enemy[num].knockmoveX = 0.0f;
					g_Enemy[num].hitCnt++;
				}

			}
			break;

		case DAMAGED_BULLET:

		{//あてられた
			//g_Enemy[num].hp -= player[0].str;

			g_Enemy[num].hp = 0;

			g_Enemy[num].hit = FALSE;
			g_Enemy[num].hitting = FALSE;
			g_Enemy[num].hitCnt = 0;
			g_time = 0;
			g_knockmoveCnt = 0;
			g_Enemy[num].knockmoveX = 0;

			PlaySound(SOUND_LABEL_SE_warp);

		}
		break;
		case DAMAGED_BLACKHOLE:

			damage = rand() % 10 + (int)player[0].str * 0.3;

			g_Enemy[num].hp -= damage;
			g_Enemy[num].hit = FALSE;
			g_Enemy[num].hitting = FALSE;
			g_Enemy[num].hitCnt = 0;
			g_time = 0;
			g_knockmoveCnt = 0;
			g_Enemy[num].knockmoveX = 0;

			SetHitScore(-damage, g_Enemy[num].pos, DAMAGE_ENEMY);

			PlaySound(SOUND_LABEL_SE_eShock);

		case DAMAGED_ULT:


			PLAYER* player = GetPlayer();

			if (g_Enemy[num].use == FALSE)return;
			if (g_Enemy[num].hit == FALSE)return;

			int hitMax = 40;

			if (g_Enemy[num].hitCnt++ > (ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX))
			{	//時間オーバー

				g_Enemy[num].hit = FALSE;
				g_Enemy[num].clear = FALSE;
				g_Enemy[num].hitCnt = 0;
				g_Enemy[num].state = SLIME_STATE_IDLE;
			}
			else
			{
				if (g_Enemy[num].hitCnt % 50 == 0)
				{
					int critical = rand() % 5;
					int str = rand() % 10 + player[0].str;
					if (critical == 0)str *= 2;

					g_Enemy[num].hp -= str;
					SetHitScore(-str, g_Enemy[num].pos, DAMAGE_ENEMY);
					SetEffect3(g_Enemy[num].pos, HIT);
				}

			}

			break;
		}

		break;
	case ENEMY_TYPE_WYVERN:

		if (g_Wyvern[num].use == FALSE)return;
		if (g_Wyvern[num].hit == FALSE)return;


		switch (g_Wyvern[num].damagedType) {//○○に当てられた処理
		case DAMAGED_BLADE:

			if (g_Wyvern[num].hitCnt++ > 60)
			{	//時間オーバー
				g_Wyvern[num].hitting = FALSE;
				g_Wyvern[num].clear = FALSE;
				g_Wyvern[num].hitCnt = 0;
			}
			if (g_Wyvern[num].hitCnt % 30 == 0)
			{
				g_Wyvern[num].clear = !(g_Wyvern[num].clear);
			}

			if (g_Wyvern[num].hit == TRUE && g_Wyvern[num].hitting == FALSE)
			{//あてられた
				g_Wyvern[num].hitting = TRUE;
				g_Wyvern[num].hitCnt = 0;
			
				int str = player[0].str + rand() % (int)(player[0].str / 2);
				g_Wyvern[num].hp -= str;
				SetHitScore(-str, g_Wyvern[num].pos, DAMAGE_ENEMY);
				g_Wyvern[num].hit = FALSE;

				PlaySound(SOUND_LABEL_SE_slimehit);

			}

		
			break;

		case DAMAGED_BULLET:

		{//あてられた

			g_Wyvern[num].hp -= player[0].str;
			g_Wyvern[num].hit = FALSE;
			g_Wyvern[num].hitting = FALSE;
			g_Wyvern[num].hitCnt = 0;
		
			PlaySound(SOUND_LABEL_SE_warp);
		}
		break;
		case DAMAGED_BLACKHOLE:

			damage = rand() % 10 + (int)player[0].str * 0.3;

			g_Wyvern[num].hp -= damage;
			g_Wyvern[num].hit = FALSE;
			g_Wyvern[num].hitting = FALSE;
			g_Wyvern[num].hitCnt = 0;

			SetHitScore(-damage, g_Wyvern[num].pos, DAMAGE_ENEMY);

			PlaySound(SOUND_LABEL_SE_eShock);

		case DAMAGED_ULT:


			PLAYER* player = GetPlayer();

			if (g_Wyvern[num].use == FALSE)return;
			if (g_Wyvern[num].hit == FALSE)return;

			int hitMax = 40;

			if (g_Wyvern[num].hitCnt++ > (ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX))
			{	//時間オーバー

				g_Wyvern[num].hit = FALSE;
				g_Wyvern[num].clear = FALSE;
				g_Wyvern[num].hitCnt = 0;
				g_Wyvern[num].state = SLIME_STATE_IDLE;
			}
			else
			{
				if (g_Wyvern[num].hitCnt % 50 == 0)
				{
					int critical = rand() % 5;
					int str = rand() % 10 + player[0].str;
					if (critical == 0)str *= 2;

					g_Wyvern[num].hp -= str;
					SetHitScore(-str, g_Wyvern[num].pos, DAMAGE_ENEMY);
					SetEffect3(g_Wyvern[num].pos, HIT);
				}

			}

			break;
		}


		break;

	}
	


}


void SetEDamagedType(int num,int eType,int type)
{
	if (eType == ENEMY_TYPE_WYVERN)
	{
		g_Wyvern[num].damagedType = type;
	}
	else
		g_Enemy[num].damagedType = type;

	return;
}
//エネミーのHP表示
void DrawEnemyHP(int num)
{
	// HPの表示
	BG* bg = GetBG();
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
		//ゲージの位置やテクスチャー座標を反映
		float px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;		// ゲージの表示位置X
		float py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h;		// ゲージの表示位置Y
		float pw = 100;	// ゲージの表示幅
		float ph = 5.0f;		// ゲージの表示高さ

		switch (g_Enemy[num].type)
		{
		case ENEMY_TYPE_SLIME:
	
			px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;
			py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h;
			pw = 100;	// ゲージの表示幅
			ph = 5.0f;		// ゲージの表示高さ
			break;

		case ENEMY_TYPE_MANDRAKE:

			px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;
			py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h;
			pw = 150;	// ゲージの表示幅
			ph = 5.0f;		// ゲージの表示高さ
			break;

		}

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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//ゲージの位置やテクスチャー座標を反映
		switch (g_Enemy[num].type)
		{
		case ENEMY_TYPE_SLIME:
			pw = pw * ((float)g_Enemy[num].hp / SLIME_HPMAX);
			break;

		case ENEMY_TYPE_MANDRAKE:
			pw = pw * ((float)g_Enemy[num].hp / MANDRAKE_HPMAX);
			break;

		case ENEMY_TYPE_BOSS:
			pw = pw * ((float)g_Enemy[num].hp / BOSS_HPMAX);
			break;
		default:
			break;
		}


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);


	}

}



void SetSkill000(int num)
{
	g_Boss[num].time = 0.0f;
	g_Boss[num].tblNo = 0;
	g_Boss[num].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA2);

}
void SetSkill005(int num)
{
	g_Boss[num].time = 0.0f;
	g_Boss[num].tblNo = 1;
	g_Boss[num].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA2);
}

void Skill005(int num)
{
	if (g_Boss[num].use == TRUE)
	{
		float waitMax = 0.2f;
		g_Walktimer += GetDeltatime();
		if (g_Walktimer >= waitMax * 1000.0f)
		{
			g_Walktimer = 0.0f;
			PlaySound(SOUND_LABEL_SE_bosswalk);
		}
		//else
		{
			g_changeable = FALSE;

			XMFLOAT3 pos_old = g_Boss[num].pos;
			// 移動処理
			if (g_Boss[num].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理

				int nowNo = (int)g_Boss[num].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Boss[num].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA2* tbl = g_MoveTblAdr[g_Boss[num].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している

				float nowTime = g_Boss[num].time - nowNo;	// 時間部分である少数を取り出している

				{//向き設定
					XMFLOAT3 temp;
					XMStoreFloat3(&temp, Pos);
					if (temp.x <= 0)g_Boss[num].dir = CHAR_DIR_LEFT;
					else g_Boss[num].dir = CHAR_DIR_RIGHT;
				}

				Pos *= nowTime;								// 現在の移動量を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている

				XMStoreFloat3(&g_Boss[num].pos, nowPos + Pos);

				// frameを使て時間経過処理をする
				g_Boss[num].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

				if ((int)g_Boss[num].time >= maxNo || g_Boss[num].hit)			// 登録テーブル最後まで移動したか？
				{
					g_Boss[num].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
					g_changeable = TRUE;

				}
	
			}
		}
		float w = g_Boss[num].w / 4;
		if (g_Boss[num].dir == CHAR_DIR_LEFT)w *= (-1);
		XMFLOAT3 epos = XMFLOAT3(g_Boss[num].pos.x + w, g_Boss[num].pos.y, 0.0f);
		SetEffect3(epos,SKILL05,g_Boss[num].dir);

	}
}





void UpdateBMove(int num)
{
	if (g_Boss[num].use == TRUE)
	{
		float waitMax = 0.5f;
		g_Walktimer += GetDeltatime();
		if (g_Walktimer >= waitMax * 1000.0f)
		{
			g_Walktimer = 0.0f;
			PlaySound(SOUND_LABEL_SE_bosswalk);
		}

		//else
		{
			g_changeable = FALSE;

			XMFLOAT3 pos_old = g_Boss[num].pos;
			

			// 移動処理
			if (g_Boss[num].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Boss[num].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Boss[num].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA2* tbl = g_MoveTblAdr[g_Boss[num].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している

				float nowTime = g_Boss[num].time - nowNo;	// 時間部分である少数を取り出している

				XMFLOAT3 temp;
				XMStoreFloat3(&temp,Pos);
				if (temp.x <= 0)g_Boss[num].dir = CHAR_DIR_LEFT;
				else g_Boss[num].dir = CHAR_DIR_RIGHT;
				Pos *= nowTime;								// 現在の移動量を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている

				XMStoreFloat3(&g_Boss[num].pos, nowPos + Pos);

				// frameを使て時間経過処理をする
				g_Boss[num].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

				if ((int)g_Boss[num].time >= maxNo || g_Boss[num].hit)			// 登録テーブル最後まで移動したか？
				{
					g_Boss[num].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
					g_changeable = TRUE;

				}
				if (g_Boss[num].time <= 0)
				{
				}
			}
		}
	}	
}



//ボスの重力処理
void BGravityProcess(int num)
{

	if (g_Boss[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Boss[num].pos;
		{//重力処理
							//足元の判定
			nextpos = g_Boss[num].pos;
			XMFLOAT3 pfootpos;
			BOOL ans;
			switch (g_Boss[num].type)
			{
			case ENEMY_TYPE_BOSS:

				pfootpos = XMFLOAT3(g_Boss[num].pos.x, g_Boss[num].pos.y + g_Boss[num].h / 2 - 10, 0.0f);//足元の座標
				ans = FieldCollision(pfootpos, TEXTURE_BOSS_FOOT_WIDTH, TEXTURE_BOSS_FOOT_HEIGHT);

				break;

			default:
				break;
			}

			if (ans)//足場がある
			{
				g_Boss[num].onGround = TRUE;
				g_Boss[num].gravityCnt = 0;
			}
			else
			{
				g_Boss[num].onGround = FALSE;
				g_Boss[num].gravityCnt += GRAVITY;
				float acc = g_Boss[num].gravityCnt * 5;
				float totalmove = g_Boss[num].gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Boss[num].w / 2, g_Boss[num].h / 16);

			if (!ans)
			{

				g_Boss[num].pos = nextpos;
			}
		}
	}
}

void BossHit(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Boss[num].use == FALSE)return;
	if (g_Boss[num].hit == FALSE)return;

	if (g_Boss[num].state != BOSS_STATE_IDLE && g_Boss[num].state != BOSS_STATE_DOWN)
	{
		g_Boss[num].hit = FALSE;
		XMFLOAT3 ePos = XMFLOAT3(g_Boss[num].pos.x - g_Boss[num].w / 2 , g_Boss[num].pos.y, 0.0f);
		SetEffect3(ePos, BOSS_DEFEND);
		PlaySound(SOUND_LABEL_SE_hitshield);

		return;
	}
	switch (g_Boss[num].damagedType)
	{
	case DAMAGED_BLADE:

	{//ブレイドに当てられた処理
		{//あてられた
			int str = rand() % 10 + player[0].str / 2;

			int critical = rand() % 5;

			if (critical == 0)
			{
				str =(float)str * 1.5;
				SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
			}
			else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

			g_Boss[num].hp -= str;
			SetEffect3(g_Boss[num].pos, HIT);
			g_Boss[num].hit = FALSE;
			//g_Boss[num].state = g_Boss[num].ostate;
			//g_Boss[num].damagedType = DAMAGED_MAX;
			PlaySound(SOUND_LABEL_SE_enemyhit);
		}
	}
	break;
	case DAMAGED_BULLET:

	{//たまに当てられた処理

		{//あてられた
			int str = rand() % 10 + player[0].str / 2;

			int critical = rand() % 5;

			if (critical == 0)
			{
				str = (float)str * 1.4;
				SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
			}
			else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

			g_Boss[num].hp -= str;
			SetEffect3(g_Boss[num].pos, HIT);
			g_Boss[num].hit = FALSE;
			PlaySound(SOUND_LABEL_SE_enemyhit);

		}
	}
	break;
	case DAMAGED_BLACKHOLE:

	{//あてられた
		int str = rand() % 10 + player[0].str / 3;

		int critical = rand() % 5;

		if (critical == 0)
		{
			str = (float)str * 1.7;
			SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
		}
		else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

		g_Boss[num].hp -= str;
		SetEffect3(g_Boss[num].pos, HIT);
		g_Boss[num].hit = FALSE;
		PlaySound(SOUND_LABEL_SE_enemyhit);
	}
	break;

	}
}

//ヒットバック処理
void BossHitback(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Boss[num].use == FALSE)return;

	int hitMax;
	switch (g_Boss[num].damagedType)
	{

	case DAMAGED_ULT:

		hitMax = g_boss_state_AnimeMax[BOSS_STATE_HIT][0] * g_boss_state_AnimeMax[BOSS_STATE_HIT][3];
		if (g_Boss[num].hitCnt++ > (ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX))
		{	//時間オーバー

			g_Boss[num].hit = FALSE;
			g_Boss[num].clear = FALSE;
			g_Boss[num].hitCnt = 0;
			g_changeable = TRUE;
			g_Boss[num].state = BOSS_STATE_IDLE;
			return;
		}
		else
		{
			if (g_Boss[num].hitCnt % 50 == 0)
			{
				int critical = rand() % 5;
				int str = rand() % 10 + player[0].str;
				if (critical == 0)
				{
					str *= 2;
					SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
				}
				else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

				g_Boss[num].hp -= str;
				g_Boss[num].state = BOSS_STATE_HIT;
				SetEffect3(g_Boss[num].pos, HIT);
				PlaySound(SOUND_LABEL_SE_ulthit);
			}

			g_changeable = FALSE;
		}
		break;

	}
	return;
}



//エネミーのHP表示
void DrawBossHP(int num)
{
	// HPの表示
	BG* bg = GetBG();
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		int h = g_Boss[num].hp / 300;

		for (int i = 0; i < 3; i++)
		{//ゲージの位置やテクスチャー座標を反映
			
			float px = px = g_Boss[num].pos.x - bg->pos.x;
			float py = g_Boss[num].pos.y - bg->pos.y - g_Boss[num].h / 2 + 20.0f * i;
			float pw = 300;	// ゲージの表示幅
			float ph = 5.0f;		// ゲージの表示高さ

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
			if (i > h)continue;
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//ゲージの位置やテクスチャー座標を反映
			if (i < h)pw = 300;
			else
			{
				pw = h > 0 ? (g_Boss[num].hp % (h * 300)) : g_Boss[num].hp;
			}
				

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}



		

	}
}




int DisBulletDir(void)
{
	return g_bulletdir;
}

//エネミーのスキル１

void InitSkill001(void)
{
	g_Boss[0].skill01delaytime = 0;
	g_Boss[0].skill01Cnt = 0;
	g_Boss[0].skill01 = FALSE;

	g_initMoveX = 5.0f;
	

	return;


}
void Skill001Timer(int num)
{
	PLAYER* player = GetPlayer();
	{

		g_bulletdir = BULLET_DIR_LEFT;
		//プレイヤーはエネミーの右側
		if (g_Boss[num].skill01delaytime++ >= 25)
		{
			g_Boss[num].skill01delaytime = 0;
			g_initMoveX += 2.3f;
			//float x = (float)rand() / ((float)RAND_MAX / 20);
			//float y = (float)rand() / ((float)RAND_MAX / 10);
			float y = -15.0f;
			XMFLOAT3 initMove = XMFLOAT3(g_initMoveX, y, 0.0f);

			if (g_Boss[num].skill01Cnt == g_HpbulletNum)
			{
				SetBullet(g_Boss[num].pos, BULLET_MODE_HP, g_Boss[num].dir, initMove);
			}
			else
			{
				SetBullet(g_Boss[num].pos, BULLET_MODE_ENEMY, g_Boss[num].dir, initMove);

			}
			g_Boss[num].skill01Cnt++;
		}

		if (g_Boss[num].skill01Cnt > SKILL1_BULLET_MAX || g_Boss[num].hit)
		{
			g_Boss[num].skill01 = FALSE;
			g_Boss[num].skill01delaytime = 0;
			g_Boss[num].skill01Cnt = 0;
			//g_Boss[num].state = ENEMY_STATE_STAND;
			g_initMoveX = 5.0f;

			g_Boss[num].state = BOSS_STATE_IDLE;
			g_Boss[num].skill01Cnt = 0;
			g_changeable = TRUE;
		}

		else g_changeable = FALSE;
	}
}


void SetSkill002(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].use == FALSE)
		{
			//g_Skill002[i].use = TRUE;
			g_Skill002[i].startFlag = TRUE;
			g_Skill002[i].pos = XMFLOAT3(pos.x, 680.0f, 0.0f);
			g_Skill002[i].countAnim = 0;
			g_Skill002[i].patternAnim = 0;
			return;
		}
	}
}
void InitSkill002(void)
{
	//SKILL002

	g_skill02Cnt = 0;
	g_changeable = TRUE;
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		g_Skill002[i].use = FALSE;
		g_Skill002[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Skill002[i].w = 660.0f;
		g_Skill002[i].h = 480.0f;
		g_Skill002[i].texNo = 1;
		g_Skill002[i].countAnim = 0;
		g_Skill002[i].patternAnim = 0;
		g_Skill002[i].move = XMFLOAT3(0.0f,0.0f,0.0f);
		g_Skill002[i].frameCnt = 0;
		g_Skill002[i].delayCnt = 0;
		g_Skill002[i].startFlag = FALSE;
	}	
}

void Skill002Timer(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		//if (g_Skill002[i].startFlag)
		{
			
			int waitMax = g_boss_state_AnimeMax[BOSS_STATE_SKILL002][3] * g_boss_state_AnimeMax[BOSS_STATE_SKILL002][0];

			g_changeable = FALSE;

			if (g_Skill002[i].delayCnt == 0)
			{
				SetSkill002(player[0].pos);
			}

			if (g_Skill002[i].delayCnt++ >= waitMax)
			{
				g_Skill002[i].delayCnt = 0;
				g_Skill002[i].startFlag = FALSE;
				g_Skill002[i].use = TRUE;
				
				g_skill02Cnt++;
				PlaySound(SOUND_LABEL_SE_enemyskill02);
			}

			if (g_skill02Cnt >= SKILL2_MAX || g_Boss[0].hit)
			{
				g_skill02Cnt = 0;
				g_changeable = TRUE;

			}
		}
	}
}

//=============================================================================
// スキル2の更新処理
//=============================================================================
void UpdateSkill002(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		
		int waitMax = SKILL2_ANIM_PATTERN_NUM * SKILL2_WAIT_MAX;
		if (g_Skill002[i].use)
		{

			if (g_Skill002[i].frameCnt++ > SKILL2_ANIM_PATTERN_NUM * SKILL2_WAIT_MAX)
			{
				g_Skill002[i].frameCnt = 0;
				g_Skill002[i].use = FALSE;
			}


			if (g_Skill002[i].countAnim++ > SKILL2_WAIT_MAX)
			{
				g_Skill002[i].countAnim = 0;
				// パターンの切り替え
				g_Skill002[i].patternAnim = (g_Skill002[i].patternAnim + 1) % SKILL2_ANIM_PATTERN_NUM;
			}

			if (g_Skill002[i].frameCnt == waitMax / 2 && CollisionBB(g_Skill002[i].pos, g_Skill002[i].w / 8, g_Skill002[i].h, player[0].pos, player[0].w, player[0].h))
			{
				//SetPlayerHP(30.0f);
				player[0].hit = TRUE;
				int str = 30 + rand() % 10;
				player[0].hp -= str;
				SetHitScore(-str, player[0].pos, DAMAGE_PLAYER);
			}
		}

	}
}

//=============================================================================
// スキル2の描画処理
//=============================================================================
void DrawSkill002(void)
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

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].use)		// このバレットが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill002[i].texNo]);

			//バレットの位置やテクスチャー座標を反映

			float px = g_Skill002[i].pos.x;	// バレットの表示位置X
			float py = g_Skill002[i].pos.y;	// バレットの表示位置Y
			float pw = g_Skill002[i].w;		// バレットの表示幅
			float ph = g_Skill002[i].h;		// バレットの表示高さ

			float tw = 1.0f / SKILL2_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
			float th = 1.0f / SKILL2_PATTERN_DIVIDE_Y;								// テクスチャの高さ
			float tx = tw * (g_Skill002[i].patternAnim % SKILL2_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
			float ty = th * (g_Skill002[i].patternAnim / SKILL2_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標



			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
	
	DrawSkill2Pos();

}

void InitSkill2Pos(void)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		g_Skill002Pos[i].countAnim = 0;
		g_Skill002Pos[i].patternAnim = 0;
	}
}
void UpdateSkill2Pos(void)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		//if (g_Skill002[i].startFlag)
		{
			if (g_Skill002Pos[i].countAnim++ > SKILL2POS_WAIT_MAX)
			{
				g_Skill002Pos[i].countAnim = 0;
				// パターンの切り替え
				g_Skill002Pos[i].patternAnim = (g_Skill002Pos[i].patternAnim + 1) % SKILL2POS_ANIM_PATTERN_NUM;
			}
		}
	}

}

void DrawSkill2Pos(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].startFlag)		// このバレットが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			//バレットの位置やテクスチャー座標を反映

			float px = g_Skill002[i].pos.x - bg->pos.x;	// バレットの表示位置X
			float py = g_Skill002[i].pos.y - bg->pos.y + g_Skill002[i].h / 2;	// バレットの表示位置Y
			float pw = 160;		// バレットの表示幅
			float ph = 30;		// バレットの表示高さ

			float tw = 1.0f / SKILL2POS_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
			float th = 1.0f / SKILL2POS_PATTERN_DIVIDE_Y;								// テクスチャの高さ
			float tx = tw * (g_Skill002Pos[i].patternAnim % SKILL2POS_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
			float ty = th * (g_Skill002Pos[i].patternAnim / SKILL2POS_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標



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


void SetSkill003(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (g_Skill003[i].use == FALSE)
		{
			g_Skill003[i].use = TRUE;
			g_Skill003[i].pos = pos;
			g_Skill003[i].countAnim = 0;
			g_Skill003[i].patternAnim = 0;
			g_Skill003[i].frameCnt = 0;
			return;
		}
	}
}

void Skill003Timer(void)
{
	
	if (g_skill03delayCnt++ >= 40)
	{
		g_skill03delayCnt = 0;
		

		XMFLOAT3 skill003Pos = XMFLOAT3(g_Boss[0].pos.x - g_Boss[0].w / 2,
			850,
			0.0f);
		SetSkill003(skill003Pos);
		PlaySound(SOUND_LABEL_SE_enemyskill02);
		g_skill03Cnt++;
	}

	if (g_skill03Cnt > 5 || g_Boss[0].hit)
	{
		g_skill03Cnt = 0;
		g_changeable = TRUE;
	}
	else
	{

		g_changeable = FALSE;

	}
	
}


void InitSkill003(void)
{
	g_skill03delayCnt = 0;
	g_skill03Cnt = 0;
	g_changeable = TRUE;

	for (int i = 0; i < SKILL3_MAX; i++)
	{
		
		g_Skill003[i].use = FALSE;
		g_Skill003[i].countAnim = 0;
		g_Skill003[i].patternAnim = 0;
		g_Skill003[i].frameCnt = 0;
		g_Skill003[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);
		g_Skill003[i].w = 120.0f;
		g_Skill003[i].h = 200.0f;
		g_Skill003[i].texNo = 3;
		g_Skill003[i].delayCnt = 0;
	}
}
void UpdateSkill003(void)
{
	PLAYER* player = GetPlayer();
	BULLET* bullet = GetBullet();
	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (!g_Skill003[i].use)continue;
		
		g_Skill003[i].pos.x -= 5.0f;
		if (g_Skill003[i].frameCnt++ >= 20)
		{
			g_Skill003[i].frameCnt = 0;
			g_Skill003[i].clear = !(g_Skill003[i].clear);
		}
		if (g_Skill003[i].countAnim++ > SKILL3_WAIT_MAX)
		{
			g_Skill003[i].countAnim = 0;
			// パターンの切り替え
			g_Skill003[i].patternAnim = (g_Skill003[i].patternAnim + 1) % SKILL3_ANIM_PATTERN_NUM;
		}

		//フィールドとの当たり判定
		if ((FieldCollision(g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h / 8)))
		{
			g_Skill003[i].use = FALSE;
		}

		//プレイヤーとの当たり判定

		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (player[j].use == FALSE)continue;
			if (player[j].state == PLAYER_STATE_GUARD)
			{
				EFFECT3* effect = GetEffect3();
				for (int i = 0; i < EFFECT_MAX; i++)
				{
					if (effect[i].type == GUARD && g_Skill003[i].use == TRUE &&
						(CollisionBB(effect[i].pos, effect[i].w / 2, effect[i].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
					{
						player[j].hit = TRUE;
						g_Skill003[i].use = FALSE;
						player[j].hp -= 3;
						SetHitScore(-3, g_Skill003[i].pos, DAMAGE_PLAYER);
						SetEffect3(g_Skill003[i].pos, BOMB2);

						int temp = rand()% 10;
						XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f,0.0f);
						if (temp == 0)SetDropItem(footPos);

						PlaySound(SOUND_LABEL_SE_hitshield);
						PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
					}
				}
				
			}
			if((CollisionBB(player[j].pos, player[j].w, player[j].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
			{
				player[j].hit = TRUE;
				player[j].hp -= 10;
				SetHitScore(-10, g_Skill003[i].pos, DAMAGE_PLAYER);
				g_Skill003[i].use = FALSE;
				SetEffect3(g_Skill003[i].pos, BOMB2);

				int temp = rand() % 20;
				XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f, 0.0f);
				if (temp == 0)SetDropItem(footPos);

				PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
			}
		}
		//弾との当たり判定
		for (int j = 0; j < BULLET_MAX; j++)
		{
			if (bullet[j].use == FALSE)continue;
			if (bullet[j].mode != BLACKHOLE)continue;

			if ((CollisionBB(bullet[j].pos, bullet[j].w, bullet[j].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
			{
				g_Skill003[i].use = FALSE;

				int temp = rand() % 10;
				XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f, 0.0f);
				if (temp == 0)SetDropItem(footPos);
				SetEffect3(g_Skill003[i].pos, BLACKHOLE_EFFECT);
				PlaySound(SOUND_LABEL_SE_warp);
			}
		}
		
	}
}

void DrawSkill003(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (g_Skill003[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill003[i].texNo]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Skill003[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Skill003[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Skill003[i].w;		// プレイヤーの表示幅
			float ph = g_Skill003[i].h;		// プレイヤーの表示高さ

			// アニメーション用
			//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			float tw = 1.0f / SKILL3_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
			float th = 1.0f / SKILL3_PATTERN_DIVIDE_Y;								// テクスチャの高さ
			float tx = tw * (g_Skill003[i].patternAnim % SKILL3_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
			float ty = th * (g_Skill003[i].patternAnim / SKILL3_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;
			
			if (g_Skill003[i].clear)
			{
				G = 0.1f;
				B = 0.1f;
			}
			
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void SetSkill004(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == FALSE)
		{
			g_Skill004[i].use = TRUE;
			g_Skill004[i].pos = pos;
			g_Skill004[i].countAnim = 0;
			g_Skill004[i].patternAnim = 0;
			g_Skill004[i].frameCnt = 0;
			return;
		}
	}
}

void Skill004Timer(void)
{
	if (g_skill04delayCnt++ >= 100)
	{
		g_skill04delayCnt = 0;

		//いくつの弾を生成する？

		int aNum = (rand() % SKILL4_POS_MAX);

		for (int i = 0; i < SKILL4_BULLET_MAX; i++)
		{
			//どの位置で初期化処理
			XMFLOAT3 bulletpos = g_Skill004_Pos[aNum][i];
			if (g_Skill004_Pos[aNum][i].x == 0.0f && g_Skill004_Pos[aNum][i].y == 0.0f)continue;

			SetSkill004(bulletpos);
		}


		//何回打つのか
		g_skill04Cnt++;

	}
	
	//　5回目終わったら終わり
	if (g_skill04Cnt > 5 )
	{
		g_skill04Cnt = 0;
		g_changeable = TRUE;
	}
	else
	{
		g_changeable = FALSE;

	}
}


void InitSkill004(void)
{
	g_skill04delayCnt = 0;
	g_skill04Cnt = 0;
	g_changeable = TRUE;
	for (int i = 0; i < SKILL4_MAX; i++)
	{

		g_Skill004[i].use = FALSE;
		g_Skill004[i].countAnim = 0;
		g_Skill004[i].patternAnim = 0;
		g_Skill004[i].frameCnt = 0;
		g_Skill004[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Skill004[i].w = 192.0f;
		g_Skill004[i].h = 192.0f;
		g_Skill004[i].texNo = 4;
		g_Skill004[i].delayCnt = 0;
	}
}

void UpdateSkill004(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == FALSE)continue;

		g_Skill004[i].pos.x -= 5.0f;


		if (g_Skill004[i].countAnim++ > SKILL4_WAIT_MAX)
		{
			g_Skill004[i].countAnim = 0;
			// パターンの切り替え
			g_Skill004[i].patternAnim = (g_Skill004[i].patternAnim + 1) % SKILL4_ANIM_PATTERN_NUM;
		}
		
		////フィールドとの当たり判定
		if ((FieldCollision(g_Skill004[i].pos, g_Skill004[i].w, g_Skill004[i].h / 8)))
		{
			g_Skill004[i].use = FALSE;
		}

		//プレイヤーとの当たり判定
		for (int j = 0; j < PLAYER_MAX; j++)
		{

			if (player[j].use && (CollisionBB(player[j].pos, player[j].w / 2, player[j].h - 40, g_Skill004[i].pos, g_Skill004[i].w - 40.0f, g_Skill004[i].h - 40.0f)))
			{
				player[j].hit = TRUE;
				//AddPlayerHP(-10);
				player[j].hp -= 10;
				SetHitScore(-10, g_Skill004[i].pos, DAMAGE_PLAYER);
				SetEffect3(player[j].pos, HIT2);
				g_Skill004[i].use = FALSE;
				PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
			}
		}
	}
}
void DrawSkill004(void)
{

	BG* bg = GetBG();

	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == TRUE)		// このプレイヤーが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill004[i].texNo]);

			//プレイヤーの位置やテクスチャー座標を反映
			float px = g_Skill004[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
			float py = g_Skill004[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
			float pw = g_Skill004[i].w;		// プレイヤーの表示幅
			float ph = g_Skill004[i].h;		// プレイヤーの表示高さ

			// アニメーション用
			//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			float tw = 1.0f / SKILL4_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
			float th = 1.0f / SKILL4_PATTERN_DIVIDE_Y;								// テクスチャの高さ
			float tx = tw * (g_Skill004[i].patternAnim % SKILL4_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
			float ty = th * (g_Skill004[i].patternAnim / SKILL4_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}
}



BOOL isBossDead(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if(g_Boss[i].use == FALSE)		return TRUE;

	}
	return FALSE;
}

BOSS *GetBoss(void)
{
	return &g_Boss[0];
}
void SetBDamagedType(int type)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].hit)g_Boss[i].damagedType = type;
	}
	return;
}

void Down(void)
{
	g_Downtimer += GetDeltatime();
	float waitMax = 2.0f;
	if (g_Downtimer > waitMax * 1000)
	{
		g_Downtimer = 0.0f;
		g_changeable = TRUE;
	}
	else
	{
		g_changeable = FALSE;
	}
}

void SetFireball(XMFLOAT3 center)
{
	int FireballOnetimeAmount = 10;
	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (g_Fireball[i].use == FALSE)
		{
			g_Fireball[i].use = TRUE;
			g_Fireball[i].center = center;
			g_Fireball[i].theta = (PI * 2) * FireballOnetimeCnt / FireballOnetimeAmount;

			if (FireballOnetimeCnt++ > FireballOnetimeAmount)
			{
				FireballOnetimeCnt = 0;
				return;
			}
		}
	}

	return;
}

void FireballTimer(int num)
{
	if (g_Wyvern[num].use == FALSE)return;

	int waitMax = 200;

	if (g_Wyvern[num].attacktime++ > waitMax)
	{
		g_Wyvern[num].attacktime = 0;
		g_Wyvern[num].state = WYVERN_STATE_IDLE;
		g_Wyvern[num].changable = TRUE;
	}
	else
	{
		g_Wyvern[num].changable = FALSE;
		g_Wyvern[num].state = WYVERN_STATE_ATTACK;

		if (g_Wyvern[num].attacktime % 50 == 0)
		{
			SetFireball(g_Wyvern[num].pos);
			PlaySound(SOUND_LABEL_SE_Wfireball);
		}
	}
}
void InitFireball(void)
{
	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		g_Fireball[i].use = FALSE;			// true:使っている  false:未使用
		g_Fireball[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);			// ポリゴンの座標
		g_Fireball[i].w = TEXTURE_FIREBALL_WIDTH;			// 幅と高さ
		g_Fireball[i].h = TEXTURE_FIREBALL_HEIGHT;			// 幅と高さ
		g_Fireball[i].texNo = 6;			// テクスチャ番号
		g_Fireball[i].countAnim = 0;		// アニメーションカウント
		g_Fireball[i].patternAnim = 0;	// アニメーションパターンナンバー

		g_Fireball[i].radius = FIREBALL_RADIUS;
		g_Fireball[i].theta	 = 0.0f;
		g_Fireball[i].livetimes = 3.0f;

	}
}

void UpdateFireball(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (g_Fireball[i].use == FALSE)continue;

		//アニメーション
		if (g_Fireball[i].countAnim++ > TEXTURE_FIREBALL_WAIT_MAX)
		{
			g_Fireball[i].countAnim = 0;
			g_Fireball[i].patternAnim = (g_Fireball[i].patternAnim + 1) % TEXTURE_FIREBALL_ANIM_PATTERN_MAX;
		}

		//半径の更新
		g_Fireball[i].radius += 3.0f;
		g_Fireball[i].theta += 0.01f;

		XMFLOAT3 center = g_Fireball[i].center;
		g_Fireball[i].pos.x = center.x + g_Fireball[i].radius * cos(g_Fireball[i].theta);
		g_Fireball[i].pos.y = center.y + g_Fireball[i].radius * sin(g_Fireball[i].theta);

		g_Fireball[i].livetimes -= GetDeltatime() * 0.001;
		if (g_Fireball[i].livetimes < 0.0f)
		{
			g_Fireball[i].livetimes = 3.0f;
			g_Fireball[i].radius = FIREBALL_RADIUS;
			//g_Fireball[i].theta = 0.0f;
			g_Fireball[i].use = FALSE;
		}

		//プレイヤーとの当たり判定
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (CollisionBB(g_Fireball[i].pos, g_Fireball[i].w, g_Fireball[i].h, player[j].pos, player[j].w, player[j].h))
			{
				player[j].hit = TRUE;
				AddPlayerHP(j, -(SKILLFIRE_DAMAGE), NOTDEFENDABLE);

				g_Fireball[i].use = FALSE;

			}
		}
		//フィールドとの当たり判定
		if (FieldCollision(g_Fireball[i].pos, g_Fireball[i].w, g_Fireball[i].h))
		{
			g_Fireball[i].use = FALSE;
		}

	}
	return;
}


void DrawFireball(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (g_Fireball[i].use == FALSE)	continue;
							
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Fireball[i].texNo]);

		//プレイヤーの位置やテクスチャー座標を反映
		float px = g_Fireball[i].pos.x - bg->pos.x;	// プレイヤーの表示位置X
		float py = g_Fireball[i].pos.y - bg->pos.y;	// プレイヤーの表示位置Y
		float pw = g_Fireball[i].w;		// プレイヤーの表示幅
		float ph = g_Fireball[i].h;		// プレイヤーの表示高さ

		float tw = 1.0f / TEXTURE_FIREBALL_PATTERN_DIVIDE_X;							// テクスチャの幅	8パターン
		float th = 1.0f / TEXTURE_FIREBALL_PATTERN_DIVIDE_Y;								// テクスチャの高さ
		float tx = tw * (g_Fireball[i].patternAnim % TEXTURE_FIREBALL_PATTERN_DIVIDE_X);		// テクスチャの左上X座標
		float ty = th * (g_Fireball[i].patternAnim / TEXTURE_FIREBALL_PATTERN_DIVIDE_X);								// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;

		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(R, G, B, 1.0f));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}
}


void SetWyvernMoveTLB(int num)
{
	g_Wyvern[num].time = 0.0f;
	g_Wyvern[num].tblMax = sizeof(g_WyvernMoveTbl) / sizeof(INTERPOLATION_DATA2);
}

void WyvernMoveTLB(int num)
{
	if (g_Wyvern[num].use == TRUE)
	{
		float waitMax = 0.5f;
		g_Walktimer += GetDeltatime();
		if (g_Walktimer >= waitMax * 1000.0f)
		{
			g_Walktimer = 0.0f;
			PlaySound(SOUND_LABEL_SE_Wwing);
		}
		
		g_Wyvern[num].changable = FALSE;

		XMFLOAT3 pos_old = g_Wyvern[num].pos;
		// 移動処理
		if (g_Wyvern[num].tblMax > 0)	// 線形補間を実行する？
		{	// 線形補間の処理

			int nowNo = g_Wyvern[num].tblNo;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Wyvern[num].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
			INTERPOLATION_DATA2* tbl = g_MoveTblWAdr[0];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している

			float nowTime = g_Wyvern[num].time;	// 時間部分である少数を取り出している

			{//向き設定
				XMFLOAT3 temp;
				XMStoreFloat3(&temp, Pos);
				if (temp.x <= 0)g_Wyvern[num].dir = CHAR_DIR_LEFT;
				else g_Wyvern[num].dir = CHAR_DIR_RIGHT;
			}

			Pos *= nowTime;								// 現在の移動量を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている

			XMStoreFloat3(&g_Wyvern[num].pos, nowPos + Pos);

			// frameを使て時間経過処理をする
			g_Wyvern[num].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

			if ((int)g_Wyvern[num].time >= 1.0f)			// 登録テーブル最後まで移動したか？
			{
				//g_Wyvern[num].time -= 1.0f;				// ０番目にリセットしつつも小数部分を引き継いでいる
				g_Wyvern[num].changable = TRUE;
				g_Wyvern[num].pos = tbl[nextNo].pos;
				g_Wyvern[num].tblNo = (g_Wyvern[num].tblNo + 1) % maxNo;

			}

		}
	}
}

WYVERN* GetWyvern(void)
{
	return &g_Wyvern[0];
}

//エネミーのHP表示
void DrawWyvernHP(int num)
{
	if (g_Wyvern[num].use == FALSE)return;
	// HPの表示
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
	{
		// 下敷きのゲージ（枠的な物）
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
		//ゲージの位置やテクスチャー座標を反映
		float px = g_Wyvern[num].pos.x - bg->pos.x - g_Wyvern[num].w / 3;		// ゲージの表示位置X
		float py = g_Wyvern[num].pos.y - bg->pos.y - 50.0f;		// ゲージの表示位置Y
		float pw = 250;	// ゲージの表示幅
		float ph = 10.0f;		// ゲージの表示高さ


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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		pw = pw * ((float)g_Wyvern[num].hp / WYVERN_HPMAX);
		//ゲージの位置やテクスチャー座標を反映

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}

}
