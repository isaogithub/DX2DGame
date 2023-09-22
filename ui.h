//=============================================================================
//
// �v���C���[���� [player.h]
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


// �v���C���[�̉�ʓ��z�u���W


enum UI_TYPE
{
	UI_BLADE,
	UI_BULLET,
	UI_BLACKHOLE,
	UI_GUARD,
	UI_ULT,
	UI_JUMP,
	UI_MAX,
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct UI
{
	BOOL		use;			// true:�g���Ă���  false:���g�p
	XMFLOAT3	pos;			// �|���S���̍��W

	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	float		time;			//�v���C���[�̎��Ԍv�Z
	float		r;
	//��{�X�e�[�^�X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUI(void);
void InitSkillUI(void);
void InitPlayerIconUI(void);

void UninitUI(void);

void UpdateUI(void);
void UpdateSkillUI(void);
void UpdatePlayerIconUI(void);

void DrawUI(void);
void DrawSkillUI(void);
void DrawPlayerIconUI(void);
//�v���C���[��HP�\��
void DrawPlayerHP(void);
//�v���C���[��MP�\��
void DrawPlayerMP(void);
//�v���C���[��ԕ\��
void DrawPlayerState(void);


UI* GetSkillUI(void);
void SetSkillUIUSE(int type, BOOL tf);
int	GetSkillUIClicked();
int GetSkillUIPressed();
BOOL isPressGuard(void);




////�v���C���[��HP�\��
//void DrawUIHP(int num);
////�v���C���[��MP�\��
//void DrawUIMP(int num);
//
////�v���C���[��ԕ\��
//void DrawUIState(int num);

