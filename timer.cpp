//=============================================================================
//
// UI処理 [ui.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "main.h"
#include "timer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


#define COMMAND_MAX 100

//*****************************************************************************
// 構造体定義
//*****************************************************************************


struct COMMAND_INT
{
	BOOL use;
	int param;
	void (*func)(int);	//void ???(int ???)　の形の関数のアドレスを保存する
	float time;

};

struct COMMAND_VOID
{
	BOOL use;
	void (*func)(void);	//void ???(int ???)　の形の関数のアドレスを保存する
	float time;

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



COMMAND_VOID g_commandVoid[COMMAND_MAX];	//コマンド
COMMAND_INT g_commandInt[COMMAND_MAX];		//コマンド


//=============================================================================
// 初期化処理
//=============================================================================


void InitCommand(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{	//CommandVoidの初期化
		g_commandVoid[i].use = FALSE;
		g_commandVoid[i].func = NULL;
		g_commandVoid[i].time = 0;
	}

	for (int i = 0; i < COMMAND_MAX; i++)
	{	//CommandIntの初期化
		g_commandInt[i].use = FALSE;
		g_commandInt[i].param = 0;
		g_commandInt[i].func = NULL;
		g_commandInt[i].time = 0;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCommand(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCommand(void)
{
	//if (IsHitStop())
	//{
	//	return;
	//}

	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandVoid[i].use == FALSE)
		{
			continue;
		}

		if (g_commandVoid[i].time >= 0)
		{
			g_commandVoid[i].time -= GetDeltatime();
		}

		if (g_commandVoid[i].time < 0)
		{
			g_commandVoid[i].use = FALSE;
			g_commandVoid[i].func();
		}
	}


	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].use == FALSE)
		{
			continue;
		}

		if (g_commandInt[i].time >= 0)
		{
			g_commandInt[i].time -= GetDeltatime();
		}

		if (g_commandInt[i].time < 0)
		{
			g_commandInt[i].use = FALSE;
			g_commandInt[i].func(g_commandInt[i].param);
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCommand(void)
{
#ifdef DEBUG
	//デバッグ表示に使うかも
#endif // DEBUG

}



//関数のアドレスを渡す
void CommandInt(void(&func)(int), int param, float time)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].use == FALSE)		// 未使用状態のタイマーを見つける
		{
			g_commandInt[i].use = TRUE;
			g_commandInt[i].param = param;
			g_commandInt[i].func = &func;
			g_commandInt[i].time = time * 1000;//ミリ秒に

			return;							// セットしたので終了する
		}
	}
}

//関数のアドレスを渡す
void CommandVoid(void(&func)(void), float time)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandVoid[i].use == FALSE)		// 未使用状態のタイマーを見つける
		{
			g_commandVoid[i].use = TRUE;
			g_commandVoid[i].func = &func;
			g_commandVoid[i].time = time * 1000;//ミリ秒に

			return;							// セットしたので終了する
		}
	}
}

void ClearCommandVoid(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		g_commandVoid[i].use = FALSE;
	}
}

void ClearCommandInt(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		g_commandInt[i].use = FALSE;
	}
}

void ClearCommandIntCONDParam(int param)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].param == param)
		{
			g_commandInt[i].use = FALSE;
		}
	}
}