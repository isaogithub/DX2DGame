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
void UninitUI(void);
void UpdateUI(void);
void DrawUI(void);

UI* GetUI(void);
void SetUIUSE(int type, BOOL tf);
int	GetUIClicked();
int GetUIPressed();
BOOL isPressGuard(void);
////�v���C���[��HP�\��
//void DrawUIHP(int num);
////�v���C���[��MP�\��
//void DrawUIMP(int num);
//
////�v���C���[��ԕ\��
//void DrawUIState(int num);

