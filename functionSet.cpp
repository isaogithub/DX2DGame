//=============================================================================
//
// いろんな関数 [functionSet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "functionSet.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COMMAND_MAX		(1)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************



//線形補間
XMVECTOR Linearinterpolation(XMVECTOR P0, XMVECTOR P1, float s)
{
	XMVECTOR A0;
	A0 = P0 * (1 - s) + P1 * s;
	return A0;
}

XMVECTOR QuadraticBezierCurve(XMVECTOR P0, XMVECTOR P1, XMVECTOR P2, float s)
{
	XMVECTOR A0;
	A0 = Linearinterpolation(P0, P1, s);
	XMVECTOR A1;
	A1 = Linearinterpolation(P1, P2, s);
	XMVECTOR B0;
	B0 = Linearinterpolation(A0, A1, s);
	return B0;
}

XMVECTOR CubicBezierCurve(XMVECTOR P0, XMVECTOR  P1, XMVECTOR P2, XMVECTOR P3, float s)
{
	XMVECTOR A0;
	A0 = QuadraticBezierCurve(P0, P1, P2, s);
	XMVECTOR A1;
	A1 = QuadraticBezierCurve(P1, P2, P3, s);
	XMVECTOR A2;
	A2 = Linearinterpolation(P1, P2, s);
	XMVECTOR B0;
	B0 = QuadraticBezierCurve(A0, A2, A1, s);

	return B0;
}

//関数のアドレスを渡す
//void CommandInt(void(&func)(int), int param, float time)
//{
//	for (int i = 0; i < COMMAND_MAX; i++)
//	{
//		if()
//	}
//}
