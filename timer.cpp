//=============================================================================
//
// UI���� [ui.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "main.h"
#include "timer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define COMMAND_MAX 100

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


struct COMMAND_INT
{
	BOOL use;
	int param;
	void (*func)(int);	//void ???(int ???)�@�̌`�̊֐��̃A�h���X��ۑ�����
	float time;

};

struct COMMAND_VOID
{
	BOOL use;
	void (*func)(void);	//void ???(int ???)�@�̌`�̊֐��̃A�h���X��ۑ�����
	float time;

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************



COMMAND_VOID g_commandVoid[COMMAND_MAX];	//�R�}���h
COMMAND_INT g_commandInt[COMMAND_MAX];		//�R�}���h


//=============================================================================
// ����������
//=============================================================================


void InitCommand(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{	//CommandVoid�̏�����
		g_commandVoid[i].use = FALSE;
		g_commandVoid[i].func = NULL;
		g_commandVoid[i].time = 0;
	}

	for (int i = 0; i < COMMAND_MAX; i++)
	{	//CommandInt�̏�����
		g_commandInt[i].use = FALSE;
		g_commandInt[i].param = 0;
		g_commandInt[i].func = NULL;
		g_commandInt[i].time = 0;
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitCommand(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCommand(void)
{
	//if (IsHitStop())
	//{
	//	return;
	//}

	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandVoid[i].use == FALSE)
		{
			continue;
		}

		if (g_commandVoid[i].time >= 0)
		{
			g_commandVoid[i].time -= GetDeltatime();
		}

		if (g_commandVoid[i].time < 0)
		{
			g_commandVoid[i].use = FALSE;
			g_commandVoid[i].func();
		}
	}


	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].use == FALSE)
		{
			continue;
		}

		if (g_commandInt[i].time >= 0)
		{
			g_commandInt[i].time -= GetDeltatime();
		}

		if (g_commandInt[i].time < 0)
		{
			g_commandInt[i].use = FALSE;
			g_commandInt[i].func(g_commandInt[i].param);
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCommand(void)
{
#ifdef DEBUG
	//�f�o�b�O�\���Ɏg������
#endif // DEBUG

}



//�֐��̃A�h���X��n��
void CommandInt(void(&func)(int), int param, float time)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].use == FALSE)		// ���g�p��Ԃ̃^�C�}�[��������
		{
			g_commandInt[i].use = TRUE;
			g_commandInt[i].param = param;
			g_commandInt[i].func = &func;
			g_commandInt[i].time = time * 1000;//�~���b��

			return;							// �Z�b�g�����̂ŏI������
		}
	}
}

//�֐��̃A�h���X��n��
void CommandVoid(void(&func)(void), float time)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandVoid[i].use == FALSE)		// ���g�p��Ԃ̃^�C�}�[��������
		{
			g_commandVoid[i].use = TRUE;
			g_commandVoid[i].func = &func;
			g_commandVoid[i].time = time * 1000;//�~���b��

			return;							// �Z�b�g�����̂ŏI������
		}
	}
}

void ClearCommandVoid(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		g_commandVoid[i].use = FALSE;
	}
}

void ClearCommandInt(void)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		g_commandInt[i].use = FALSE;
	}
}

void ClearCommandIntCONDParam(int param)
{
	for (int i = 0; i < COMMAND_MAX; i++)
	{
		if (g_commandInt[i].param == param)
		{
			g_commandInt[i].use = FALSE;
		}
	}
}