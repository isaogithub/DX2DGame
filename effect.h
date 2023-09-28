//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"


// マクロ定義
//#define EFFECT_TEXTURE			_T("data/TEXTURE/bomb.png")	// 画像

#define EFFECT_NUM_PARTS 30		//エフェクト内パーティクル数
#define EFFECT_NUM_EFFECTS 10	//エフェクト最大数
#define		EFFECT_MAX				(10)


enum EFFECTTYPE
{
	BOMB,
	HEAL1,
	JUMP,
	HIT,
	BLADE,
	GUARD,
	ULT1,
	ULT2,
	SKILL05,
	BOMB2,
	HIT2,
	HIT3,
	BOSS_DEFEND,
	BOSS_DEADEFFECT,
	MPUP,
	HIT_BULLET,
	BLACKHOLE_EFFECT,
	EFFECT_TYPE_MAX,
};

enum EFFECT_DIR
{
	EFFECT_LEFT,
	EFFECT_RIGHT,
};
//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// エフェクト構造体
{
	XMFLOAT3		pos;						// ポリゴンの移動量
	XMFLOAT3		move;						// 移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	int				liveTime;
	bool			isFinish;
}PARTICLE;

typedef struct	// エフェクト構造体
{
	int				use;
	int				isEnding;
	bool			isRemoveOnFinish;

	XMFLOAT3		pos;						// ポリゴンの移動量

	int				duration;					// 間隔
	int				elapsed;
	int				numFinish;

	int				effectCount;
	int				emitCounter;

	PARTICLE		pParticle[EFFECT_NUM_PARTS];

}EFFECT;

struct EFFECT2
{
	BOOL		use;
	XMFLOAT3	pos;
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
};	

struct EFFECT3
{
	BOOL		use;
	XMFLOAT3	pos;
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			type;
	int			effectCnt;
	int			dir;
	float		w;
	float		h;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(float x, float y, int duration);


void InitHealEffect(void);
void UpdateHealEffect(void);
void DrawHealEffect(void);
void SetHealEffect(XMFLOAT3 pos);


void InitEffect3(void);
void UpdateEffect3(void);
void DrawEffect3(void);

EFFECT3* GetEffect3(void);
void SetEffect3(XMFLOAT3 pos,int type);
void SetEffect3(XMFLOAT3 pos, int type, int dir);
void ResetEffect3(int type);

XMFLOAT3 GetEffect3Pos(int type);
int GetEffect3Dir(int type);
