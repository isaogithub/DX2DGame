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


// プレイヤーの画面内配置座標


enum UI_TYPE
{
	UI_BLADE,
	UI_BULLET,
	UI_BLACKHOLE,
	UI_GUARD,
	UI_ULT,
	UI_JUMP,
	UI_MAX,
};
//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct UI
{
	BOOL		use;			// true:使っている  false:未使用
	XMFLOAT3	pos;			// ポリゴンの座標

	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	float		time;			//プレイヤーの時間計算
	float		r;
	//基本ステータス
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUI(void);
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

UI* GetUI(void);
void SetUIUSE(int type, BOOL tf);
int	GetUIClicked();
int GetUIPressed();
BOOL isPressGuard(void);
////プレイヤーのHP表示
//void DrawUIHP(int num);
////プレイヤーのMP表示
//void DrawUIMP(int num);
//
////プレイヤー状態表示
//void DrawUIState(int num);

