//=============================================================================
//
// スコア処理 [score.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HITSCORE_MAX			(999)		// スコアの最大値
#define HITSCORE_DIGIT			(3)			// 桁数

enum SCORE_TYPE
{
	DAMAGE_PLAYER,
	HEAL,
	DAMAGE_ENEMY,
	DAMAGE_CRITICAL,
	MP1,

};
//*****************************************************************************
// 構造体宣言
//*****************************************************************************
struct SCORE
{
	BOOL		use;
	XMFLOAT3	pos;
	XMFLOAT3	scl;
	float		w, h;
	int			texNo;				// 何番目のテクスチャーを使用するのか
	int			score;
	int			g_ScoreframeCnt;
	int			type;
	XMFLOAT4	color;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitHitScore(void);
void UninitHitScore(void);
void UpdateHitScore(void);
void DrawHitScore(void);

void SetHitScore(int score,XMFLOAT3 pos,int type);


