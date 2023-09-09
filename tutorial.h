/*******************************************************************************
* �^�C�g��:		
* �v���O������:	????.h
* �쐬��:		GP11A132 99 �O������
* �쐬��:		2021/04/26
*******************************************************************************/

#pragma once	// �C���N���[�h�K�[�h�i���d�C���N���[�h�h�~�j

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"
#include "player.h"

/*******************************************************************************
* �}�N����`
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
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/



HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

void DrawDark(void);
//�`���[�g���A���\��
void DrawMessage(void);

void SetMessage(int type);

