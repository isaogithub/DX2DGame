//=============================================================================
//
// UI処理 [ui.h]
// Author : 
//	何秒後にどの関数を実行するかなどを管理する　コマンドパターン＆コルーチンを組み合わせた管理クラス
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

enum
{	//コマンドの枠のどこを使用したいか スレッドのようなもの
	COMMAND_GENERAL,
	COMMAND_PLAYER1,
	COMMAND_PLAYER2
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCommand(void);
void UninitCommand(void);
void UpdateCommand(void);
void DrawCommand(void);

void CommandInt(void(&func)(int), int param, float time);
void CommandVoid(void(&func)(void), float time);


void ClearCommandVoid(void);
void ClearCommandInt(void);
void ClearCommandIntCONDParam(int param);