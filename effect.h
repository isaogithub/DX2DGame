//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"


// �}�N����`
//#define EFFECT_TEXTURE			_T("data/TEXTURE/bomb.png")	// �摜

#define EFFECT_NUM_PARTS 30		//�G�t�F�N�g���p�[�e�B�N����
#define EFFECT_NUM_EFFECTS 10	//�G�t�F�N�g�ő吔
#define		EFFECT_MAX				(10)


enum EFFECTTYPE
{
	BOMB,
	HEAL1,
	JUMP,
	HIT,
	BLADE,
	GUARD,
	ULT1,
	ULT2,
	SKILL05,
	BOMB2,
	HIT2,
	HIT3,
	BOSS_DEFEND,
	BOSS_DEADEFFECT,
	MPUP,
	HIT_BULLET,
	BLACKHOLE_EFFECT,
	EFFECT_TYPE_MAX,
};

enum EFFECT_DIR
{
	EFFECT_LEFT,
	EFFECT_RIGHT,
};
//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct	// �G�t�F�N�g�\����
{
	XMFLOAT3		pos;						// �|���S���̈ړ���
	XMFLOAT3		move;						// �ړ���
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	int				liveTime;
	bool			isFinish;
}PARTICLE;

typedef struct	// �G�t�F�N�g�\����
{
	int				use;
	int				isEnding;
	bool			isRemoveOnFinish;

	XMFLOAT3		pos;						// �|���S���̈ړ���

	int				duration;					// �Ԋu
	int				elapsed;
	int				numFinish;

	int				effectCount;
	int				emitCounter;

	PARTICLE		pParticle[EFFECT_NUM_PARTS];

}EFFECT;

struct EFFECT2
{
	BOOL		use;
	XMFLOAT3	pos;
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
};	

struct EFFECT3
{
	BOOL		use;
	XMFLOAT3	pos;
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			type;
	int			effectCnt;
	int			dir;
	float		w;
	float		h;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(float x, float y, int duration);


void InitHealEffect(void);
void UpdateHealEffect(void);
void DrawHealEffect(void);
void SetHealEffect(XMFLOAT3 pos);


void InitEffect3(void);
void UpdateEffect3(void);
void DrawEffect3(void);

EFFECT3* GetEffect3(void);
void SetEffect3(XMFLOAT3 pos,int type);
void SetEffect3(XMFLOAT3 pos, int type, int dir);
void ResetEffect3(int type);

XMFLOAT3 GetEffect3Pos(int type);
int GetEffect3Dir(int type);
