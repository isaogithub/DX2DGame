//=============================================================================
//
// タイトル画面処理 [title.h]
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

#define MENU_BUTTON_MAX		(2)

enum TITLE
{
	TITLE,
	SELECT,
	TITLE_MAX,

};

enum TITLE_PARTS
{
	TITLEBG,
	TITLETREE1,
	TITLEHBODY1,
	TITLEHBODY2,
	TITLELEFTOPPAI,
	TITLERIGHTOPPAI,
	TITLETREE2,
	TITLEHHAIR,
	TITLETREE3,
	TITLEBODY,
	TITLEHAIR,
	TITLEEFFECT1,
	TITLEEFFECT2,
	TITLE_LOGOEFFECT,
	TITLELOGO,
	TITLE_PARTS_MAX,
};

enum TITLE_MOVEOBJ
{
	BODY,
	LEFTOPPAI,
	RIGHTOPPAI,
	EFFECT2,
	MOVEOBJ_MAX,
};

enum BUTTON
{
	BUTTON_START_GAME,
	BUTTON_SETTING_GAME,
	BUTTON_QUIT_GAME,
	BUTTON_MAX,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************

struct TITLE_PART
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	float		r;
	float		r2;
	float		theta;
	BOOL		flipx;
	BOOL		flipy;

};

struct TITLE_PART2
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号

};



struct MENU_BUTTON
{
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	XMFLOAT4	color;			// ボタンの色
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void DrawMenuButton(void);
void DrawMenuMoji(void);
void OnClickButton(int button);

void DrawRobot(void);


int GetButtonSelected(void);

void UpdatePress(void);
void DrawPress(void);

void DrawHeroineBody(void);
void DrawMoveOBJ(void);
void DrawTreeM(void);
void DrawHeroineHair(void);

void DrawHeroBody(void);
void DrawHeroHair(void);

void DrawTitleEffect2(void);
void DrawLogoEffect(void);