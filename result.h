//=============================================================================
//
// リザルト画面処理 [result.h]
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

enum RESULT
{
	GAMEOVER,
	GAMECLEAR,
};

enum RESULTTYPE
{
	RESULTTYPE_GAME,
	RESULTTYPE_BOSS,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

//結果がゲームクリアかゲームオーバーか
