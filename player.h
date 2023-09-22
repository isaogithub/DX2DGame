//=============================================================================
//
// プレイヤー処理 [player.h]
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
#define PLAYER_MAX			(1)		// プレイヤーのMax人数

#define	PLAYER_OFFSET_CNT	(4)	// 16分身


#define TEXTURE_COLLISION_WAIST_WIDTH (64)	//体の当たり判定
#define TEXTURE_COLLISION_WAIST_HEIGHT (45)

#define TEXTURE_COLLISION_HEAD_WIDTH (40)	//頭の当たり判定
#define TEXTURE_COLLISION_HEAD_HEIGHT (30)

#define TEXTURE_COLLISION_FOOT_WIDTH (63)	//足元の当たり判定
#define TEXTURE_COLLISION_FOOT_HEIGHT (10)

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

#define ULT_CHANT_FRAMEMAX		(100)
#define ULT_UNLEASH_FRAMEMAX	(180)

#define PLAYER_HP_MAX	(200)
#define PLAYER_MP_MAX	(100)
#define CHECKPOINT_MAX	(2)

enum CHAR_DIR
{
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_MAX
};

enum PLAYER_STATE
{
	PLAYER_STATE_STAND,
	PLAYER_STATE_WALK,
	PLAYER_STATE_GUARD,
	PLAYER_STATE_JUMP,
	PLAYER_STATE_FALL,
	PLAYER_STATE_BLADE,
	PLAYER_STATE_BLACKHOLE, 
	PLAYER_STATE_BULLET, 
	PLAYER_STATE_ULT,
	PLAYER_STATE_MAX,
};

enum SKILL_TYPE
{
	SKILL_ATTACK,
	SKILL_SKILL001,
	SKILL_SKILL002,
	SKILL_JUMP,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYERICON
{
	BOOL		use;			// true:使っている  false:未使用
	XMFLOAT3	pos;			// ポリゴンの座標
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	float		theta;

};
struct PLAYER
{
	BOOL		use;			// true:使っている  false:未使用
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	opos;			// プレイヤーが移動する前の座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	XMFLOAT3	savePoint;		// 復活する地点

	float		w, h;			// 幅と高さ
	float		countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号
	int		time;			//プレイヤーの時間計算
	//基本ステータス
	int			dir;			// 向き（0:上 1:右 2:下 3:左）
	int			state;			//プレイヤーの状態
	int			prestate;		//前の状態

	int			life;			//残機
	float		hp;				//プレイヤーHP
	float		mp;				//プレイヤーMP
	float		str;

	int			jumptimes;
	float		jumpplus;		// ジャンプ加速度


	int			flashExeCnt;
	int			flashCnt;
	float		flashExeTimer;
	float		flashTimer;

	XMFLOAT3	move;			// 移動速度

	//ステートフラグ
	BOOL		moving;			// 移動中フラグ
	BOOL		flash;			// ダッシュ中フラグ
	BOOL		onGround;		//プレイヤー今地面にいる
	BOOL		attack;
	BOOL		bullet;
	BOOL		jump;			// ジャンプフラグ
	//
	BOOL		hit;			//ヒットフラグ
	BOOL		hitting;		//ヒットされている
	BOOL		clear;			//ヒットの表現
	int			hitdir;			//ヒットされているの方向
	int			hitCnt;			//ヒットカウンター
	int			clearCnt;
	
	//移動パラメータ
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// 残像ポリゴンの座標
	XMFLOAT3	beforejumppos;	//ジャンプ前の座標を保存する
	int			jumpCnt;		// ジャンプ中のカウント


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);

//重力処理
void PlayerGravity(int num);

//ジャンプ処理
void SetJump(int num);
void PlayerJump(int num);
void PlayerFlash(XMFLOAT3* pos,int num);
void SetPlayerJump(void);

// 影表示
void DrawPlayerShadow(void);
//攻撃描画
void DrawAttackEffect(void);

//プレイヤー攻撃処理
void PlayerAttackProcess(int num);

// バレット処理
BOOL PlayerBulletProcess(int  num);
//下に落ちた処理
void PlayerReturn(int num);

////プレイヤーのHP表示
//void DrawPlayerHP(int num);
////プレイヤーのMP表示
//void DrawPlayerMP(int num);
//
////プレイヤー状態表示
//void DrawPlayerState(int num);

//足場判断処理
BOOL IsFootCol(int num);
//判断処理
BOOL IsWaistCol(int num);
//ヒットバック処理
void PHitbackProcess(int num);

//プレイヤーのHPを引く数の値を引く
void AddPlayerHP(int num,float hp);
void AddPlayerMP(int num,float mp);
//プレイヤーのジャンプ状態をリセット
void JumpReset(void);
void BlackholeTimer(int num);
void BulletTimer(int num);
void UltTimer(int num);
void Guard(int num);

//
//void InitPlayerIcon(void);
//void UpdatePlayerIcon(void);
//void  DrawPlayerIcon(void);
void DrawEyeDark(void);