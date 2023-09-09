//=============================================================================
//
// BG���� [bg.h]
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
enum OPENING_NUM
{
	OPENING_NUM001,
	OPENING_NUM002,
	OPENING_NUM003,
	OPENING_NUM004,
	OPENING_MAX,
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitOpening(void);
void UninitOpening(void);
void UpdateOpening(void);
void DrawOpening(void);


//=============================================================================
// �I�[�v���j���O�ԍ��ݒ�
//=============================================================================
void SetOpening(int num);

void SetOpFade(int fade);