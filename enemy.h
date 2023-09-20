//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_TUTORIAL_MAX				(3)
#define ENEMY_MAX						(15)		// エネミーのMax人数

#define	ENEMY_SLIME_MAX					(3)
#define BOSS_MAX					(1)


enum ENEMY_TYPE
{
	ENEMY_TYPE_SLIME,
	ENEMY_TYPE_BOSS,
	ENEMY_TYPE_MAX,
};

enum ENEMY_SLIME_STATE
{
	SLIME_STATE_IDLE,
	SLIME_STATE_WALK,
	SLIME_STATE_ATTACK001,
	SLIME_STATE_ATTACK002,
	SLIME_STATE_HIT,
	SLIME_STATE_MAX,
};

enum ENEMY_BOSS_STATE
{
	BOSS_STATE_IDLE,
	BOSS_STATE_WALK,
	BOSS_STATE_HIT,
	BOSS_STATE_SKILL001,
	BOSS_STATE_SKILL002,
	BOSS_STATE_SKILL003,
	BOSS_STATE_SKILL004,
	BOSS_STATE_SKILL005,
	BOSS_STATE_DOWN,
	BOSS_STATE_MAX,
};

enum ENEMY_SKILL
{
	ENEMY_SKILL001,
	ENEMY_SKILL002,
	ENEMY_SKILL003,
	ENEMY_SKILL004,
	ENEMY_SKILLMAX,
};

enum DAMAGED
{
	DAMAGED_BLADE,
	DAMAGED_BULLET,
	DAMAGED_BULLET2,
	DAMAGED_ULT,
	DAMAGED_MAX,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct ENEMY
{
	BOOL		use;			// true:使っている  false:未使用
	BOOL		display;
	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号

	int			type;			//エネミーのタイプ
	int			state;
	int			ostate;
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	opos;			
	XMFLOAT3	nextpos;		//次に行くところ

	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	scl;			// ポリゴンの拡大縮小


	int			hp;				// エネミーのHP
	int			damagedType;	//遭ったダメージは何の技？

	float	needmovedis;		//エネミーが移動必要な距離
	float	movedis;			//停止状態からどれくらい移動したか
	float	knockmoveX;		//ノックアウトの毎フレイム移動

	BOOL		stop;
	float		stopframe;
	
	//XMFLOAT3	knockmoveY;		//ノックアウトの毎フレイム移動
	float		walktimer;

	int			hitCnt;			// 当たり判定カウンタ
	BOOL		hit;			// 当てられた
	BOOL		hitting;		// 当てられた途中
	BOOL		clear;			// 透明フラグ

	BOOL		attack;			//攻撃フラグ

	BOOL		skill;
	BOOL		skillCnt;
	BOOL		skilldelaytime;

	BOOL		skill02;


	int			attacktime;		//攻撃時間カウンター


	BOOL		onGround;		// エネミーが
	float		gravityCnt;

	int			str;			// 攻撃力
	BOOL		dir;			//エネミー方向
	XMFLOAT3	move;			// 移動速度
	XMFLOAT3	amove;			// プレイヤーを見つけた速度

};

struct BOSS
{
	BOOL		use;			// true:使っている  false:未使用
	BOOL		display;
	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号

	int			type;			//エネミーのタイプ
	int			state;
	int			ostate;
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	opos;

	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	scl;			// ポリゴンの拡大縮小

	int			hp;				// エネミーのHP
	int			damagedType;	//遭ったダメージは何の技？

	int			hitCnt;			// 当たり判定カウンタ
	BOOL		hit;			// 当てられた
	BOOL		hitting;		// 当てられた途中
	BOOL		clear;			// 透明フラグ

	BOOL		attack;			//攻撃フラグ

	BOOL		skill01;
	BOOL		skill01Cnt;
	BOOL		skill01delaytime;

	BOOL		skill02;


	int			attacktime;		//攻撃時間カウンター


	BOOL		onGround;		// エネミーが
	float		gravityCnt;

	int			str;			// 攻撃力
	BOOL		dir;			//エネミー方向
	XMFLOAT3	move;			// 移動速度
	XMFLOAT3	amove;			// プレイヤーを見つけた速度

	float		time;			// 線形補間用
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);
void UpdateSlime(void);

void InitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

// アニメーション  
void AnimetionProcess(int num);
//プレイヤーが近い
BOOL IsPlayerClose(int num);

//移動処理
void MoveProcess(int num);
//プレイヤーに攻撃されたらの移動パターン
void MoveProcessP(int num);
void EnemyMoveSound(int num);

//プレイヤーへの攻撃パターン
void AttackProcess(int num);
//重力処理
void GravityProcess(int num);
//ヒットバック処理
void EHitbackProcess(int num);

//ヒットバック処理
void BossHit(int num);
void BossHitback(int num);//エネミーのHP表示
void DrawEnemyHP(int num);

int DisBulletDir(void);

void SetSkill000(int num);
void SetSkill005(int num);
void UpdateBMove(int num);
void MoveProcessBP(int num);

void SetEDamagedType(int num, int type);

void BGravityProcess(int num);
//エネミーのスキル１
void InitSkill001(void);
void Skill001Timer(int num);

void SetSkill002Pos(void);

void SetSkill002(XMFLOAT3 pos);
void Skill002Timer(void);
void InitSkill002(void);
void UpdateSkill002(void);
void DrawSkill002(void);

void InitSkill2Pos(void);
void UpdateSkill2Pos(void);
void DrawSkill2Pos(void);

void SetSkill003(XMFLOAT3 pos);
void Skill003Timer(void);
void InitSkill003(void);
void UpdateSkill003(void);
void DrawSkill003(void);

void SetSkill004(XMFLOAT3 pos);
void Skill004Timer(void);
void InitSkill004(void);
void UpdateSkill004(void);
void DrawSkill004(void);

void Skill005(int num);



BOOL isBossDead(void);
BOSS* GetBoss(void);
void SetBDamagedType(int type);
void DrawBossHP(int num);
void Down(void);