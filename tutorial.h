/*******************************************************************************
* タイトル:		
* プログラム名:	????.h
* 作成者:		GP11A132 99 外岡高明
* 作成日:		2021/04/26
*******************************************************************************/

#pragma once	// インクルードガード（多重インクルード防止）

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "player.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
enum TUTORIAL
{
	TUTORIAL_MOVE,
	TUTORIAL_JUMP,
	TUTORIAL_ATTACK_BULLET,
	TUTORIAL_ATTACK_BLADE,
	TUTORIAL_ATTACK_GUARD,
	TUTORIAL_ATTACK_BLACKHOLE,
	TUTORIAL_ATTACK_ULT,
	TUTORIAL_LAST,
	TUTORIAL_MAX,
};


enum MESSAGETYPE
{
	MESSAGE_TYPE_BOSS,
	MESSAGE_TYPE_BOSS1,
	MESSAGE_TYPE_MAX,
};

enum 
{
	MESSAGE_DOOR01,
	MESSAGE_DOOR02,
	MESSAGE_DOOR03,

	MESSAGE_HIROINE0,
	MESSAGE_HIROINE1,
	MESSAGE_HIROINE2,
	MESSAGE_HIROINE3,
	MESSAGE_MAX,
};
/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/



HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

void DrawDark(void);
//チュートリアル表示
void DrawMessage(void);

void SetMessage(int type);

