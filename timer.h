//=============================================================================
//
// UI���� [ui.h]
// Author : 
//	���b��ɂǂ̊֐������s���邩�Ȃǂ��Ǘ�����@�R�}���h�p�^�[�����R���[�`����g�ݍ��킹���Ǘ��N���X
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

enum
{	//�R�}���h�̘g�̂ǂ����g�p�������� �X���b�h�̂悤�Ȃ���
	COMMAND_GENERAL,
	COMMAND_PLAYER1,
	COMMAND_PLAYER2
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCommand(void);
void UninitCommand(void);
void UpdateCommand(void);
void DrawCommand(void);

void CommandInt(void(&func)(int), int param, float time);
void CommandVoid(void(&func)(void), float time);


void ClearCommandVoid(void);
void ClearCommandInt(void);
void ClearCommandIntCONDParam(int param);