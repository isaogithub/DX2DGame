//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
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
// �\���̒�`
//*****************************************************************************

struct TITLE_PART
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	float		r;
	float		r2;
	float		theta;
	BOOL		flipx;
	BOOL		flipy;

};

struct TITLE_PART2
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�

};



struct MENU_BUTTON
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	XMFLOAT4	color;			// �{�^���̐F
};


//*****************************************************************************
// �v���g�^�C�v�錾
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