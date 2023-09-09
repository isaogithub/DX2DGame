//=============================================================================
//
// �X�R�A���� [score.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HITSCORE_MAX			(999)		// �X�R�A�̍ő�l
#define HITSCORE_DIGIT			(3)			// ����

enum SCORE_TYPE
{
	DAMAGE_PLAYER,
	HEAL,
	DAMAGE_ENEMY,
	DAMAGE_CRITICAL,
	MP1,

};
//*****************************************************************************
// �\���̐錾
//*****************************************************************************
struct SCORE
{
	BOOL		use;
	XMFLOAT3	pos;
	XMFLOAT3	scl;
	float		w, h;
	int			texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	int			score;
	int			g_ScoreframeCnt;
	int			type;
	XMFLOAT4	color;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitHitScore(void);
void UninitHitScore(void);
void UpdateHitScore(void);
void DrawHitScore(void);

void SetHitScore(int score,XMFLOAT3 pos,int type);


