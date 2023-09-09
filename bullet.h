//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_MAX		(100)		// バレットのMax数
#define NUMA_MAX		(20)		// バレットのMax数

#define BULLET_SPEED	(20.0f)		// バレットの移動スピード
#define	BULLET_DAMAGE				(50)
#define	BULLET_COSTMP				(35)


enum BULLET_DIR
{
	BULLET_DIR_RIGHT,
	BULLET_DIR_LEFT
};

enum BULLET_MODE
{
	BLACKHOLE,
	BULLET_MODE_ENEMY,
	NUMA1,
	BULLET_MODE_HP,
	BULLET_1,
	BULLET_MODE_MAX,
};

// バレット構造体
struct BULLET
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	XMFLOAT3			initMove;				// バレットの座標
	XMFLOAT3			rot;				// バレットの回転量
	XMFLOAT3			move;				// バレットの移動量
	int					dir;				//バレットの向き
	int					countAnim;			// アニメーションカウント
	int					patternAnim;		// アニメーションパターンナンバー
	int					texNo;				// 何番目のテクスチャーを使用するのか
	int					frameCnt;
	float				timer;
	int					damage;
	BOOL				mode;
	//敵の攻撃に使用する
	float				gravityCnt;


};
struct NUMA
{
	BOOL	use;
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	int					texNo;				// 何番目のテクスチャーを使用するのか
	float				timer;
	int					countAnim;			// アニメーションカウント
	int					patternAnim;		// アニメーションパターンナンバー

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
//void SetBullet(XMFLOAT3 pos);
void SetBullet(XMFLOAT3 pos, BOOL mode);
void SetBullet(XMFLOAT3 pos, BOOL mode, int dir, XMFLOAT3 initMove);

//弾の発射はプレイヤーかエネミー
void SetBulletMode(int num, BOOL mode);

//敵の攻撃目標を設定する
void SetTarget(XMFLOAT3 pos);


void UpdateNuma(void);
void DrawNuma(void);
void SetNuma(XMFLOAT3 pos);