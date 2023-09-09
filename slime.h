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
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SLIME_MAX		(5)		// エネミーのMax人数


//*****************************************************************************
// 構造体定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSlime(void);
void UninitSlime(void);
void UpdateSlime(void);
void DrawSlime(void);

ENEMY* GetSlime(void);



