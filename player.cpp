//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "hitscore.h"
#include "file.h"
#include "field.h"
#include "functionSet.h"
#include "sound.h"
#include "fade.h"
#include "result.h"
#include "effect.h"
#include "ui.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(128)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(128)	// 

#define TEXTURE_COLLISION_WIDTH		(80)	//
#define TEXTURE_COLLISION_HEIGHT	(100)	//

#define TEXTURE_MAX					(4)		// �e�N�X�`���̐�
#define EFFECT_MAX					(1)
//#define TEXTURE_PATTERN_DIVIDE_X	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
//#define TEXTURE_PATTERN_DIVIDE_Y	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
//#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X * TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
//#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l


// �W�����v����
#define	PLAYER_JUMP_CNT_MAX			(30)		// 30�t���[���Œ��n����
#define	PLAYER_JUMP_Y_MAX			(15.0f)	// �W�����v�̍���
#define PLAYER_JUMP_IN_ONE_FRAME �@	(20.0f)
#define PLAYER_JUMP_X_MAX			(20.0f)
#define	PLAYER_JUMP_ACC				(0.5f)
#define PLAYER_JUMP_TIMES_MAX		(2)		//2��W�����v�ł���

#define PLAYER_FLASH_TIMES_MAX		(0.2f)

//�U��
#define PLAYER_ATTACK_FRAME			(8)
#define PLAYER_ATTACK_EFFECT_WIDTH	(120.0f)
#define PLAYER_ATTACK_EFFECT_HEIGHT	(120.0f)


#define PLAYER_SKILL_MAX	(4)

#define BULLET_COST_MP		(10)
#define BLACKHOLE_COST_MP	(45)
#define ULT_COST_MP			(PLAYER_MP_MAX)

#define HIT_FRAME_MAX	(60)
#define HIT_CLEAR_FRAME_MAX	(20)
#define DEAD_FRAME_MAX	(10)

#define SAVEPOINT_MAX	(2)

//#define GAUGE_WIDTH			(220)
//#define GAUGE_HEIGHT		(30)

#define BULLET_DELAY_CNT	(10)




//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void DrawPlayerOffset(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11ShaderResourceView* g_State[PLAYER_STATE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11ShaderResourceView* g_Effect[EFFECT_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[TEXTURE_MAX] = {
		"data/TEXTURE/bar_white.png",
		"data/TEXTURE/state.png",
		"data/TEXTURE/heroicon.png",
		"data/EFFECT/effect_dark.png",
};

static char* g_StateName[PLAYER_STATE_MAX] = {
	"data/CHARA/player_stand.png",		//STAND
	"data/CHARA/player_run.png",		//WALK
	"data/CHARA/player_guard.png",		//GUARD
	"data/CHARA/player_jump.png",		//JUMP
	"data/CHARA/player_fall.png",		//FALL
	"data/CHARA/player_attack.png",		//BLADE
	"data/CHARA/player_blackhole.png",	//BLACKHOLE
	"data/CHARA/player_bullet.png",		//BULLET
	"data/CHARA/player_ult.png"	,		//ULT
};

static char* g_EffectName[EFFECT_MAX] = {
	"data/TEXTURE/shadow000.jpg",		//�e
};


static	int g_player_state_AnimeMax[PLAYER_STATE_MAX][4] = { //�e�e�N�X�`���̕�����
//  Pattern_Max	divideY  divideX	animewait
		 4,		1,			4,			10,	    //STAND 
		 9,		1,			9,			5,		//WALK
		 1,		1,			1,			20,		//GUARD
		 1,		1,			1,			5,		//JUMP
		 1,		1,			1,			5,		//FALL  
		 5,		1,			5,			2,		//BLADE
		 5,		1,			5,			3,		//BLACKHOLE
		 5,		1,			5,			3,		//BULLET
		 4,		1,			4,			20,		//ULT
};

static	int g_effect_state_AnimeMax[EFFECT_MAX][4] = { //�e�e�N�X�`���̕�����
//  Pattern_Max	divideY  divideX	animewait
		 1,		1,			1,			5,		//�e
};

static XMFLOAT3 g_CheckPoint[CHECKPOINT_MAX] = {

	XMFLOAT3(3800.0f,1080.0f,0.0f),
	XMFLOAT3(300.0f,2160.0f,0.0f),
};

//static float  g_CheckPoint[CHECKPOINT_MAX] = {
//	1080.0f,2160.0f,
//};

static  BOOL	BGXScroll;
static  BOOL	BGYScroll;


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static PLAYER	g_Player[PLAYER_MAX];	// �v���C���[�\����
static PLAYERICON	g_PlayerIcon;
static int      g_jumpCnt;
static int		g_jumpinFrame;
static int		g_bulletTime;
static float	g_deadCnt;
static float	g_gravityCnt;
static DWORD    g_start_time;
BOOL			g_Changable;
BOOL			g_Ult;

static	int		g_BulletDelay;
static	int		g_SkillFrameCnt;
static	int		g_BlackholeFCnt;
static	int		g_GuardFrameCnt;
XMFLOAT3 g_clickpos;
XMFLOAT3 g_SavePoint[SAVEPOINT_MAX];

//static int		g_jump[PLAYER_JUMP_CNT_MAX] =
//{
//	-15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5,-4,-3,-2,-1,
//	//  1,   2,   3,   4,   5,   6,  7,  8,  9, 10, 11,12,13,14,15
//};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	ID3D11Device* pDevice = GetDevice();

	BGXScroll = TRUE;
	BGYScroll = TRUE;

	g_bulletTime = 0;
	g_clickpos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//�O���[�o���ϐ�������
	g_jumpCnt = 0;
	g_jumpinFrame = 0;
	g_bulletTime;
	g_deadCnt = 1.0f;
	g_gravityCnt = 0;

	g_Changable = TRUE;
	g_BulletDelay = BULLET_DELAY_CNT;
	g_BlackholeFCnt = 0;
	g_SkillFrameCnt = 0;
	g_GuardFrameCnt = 0;
	g_Ult = FALSE;
	//�e�N�X�`������

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	for (int i = 0; i < PLAYER_STATE_MAX; i++)
	{
		g_State[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_StateName[i],
			NULL,
			NULL,
			&g_State[i],
			NULL);
	}

	//�e�N�X�`������
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_EffectName[i],
			NULL,
			NULL,
			&g_Effect[i],
			NULL);
	}

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �v���C���[�\���̂̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].use = TRUE;
		g_Player[i].pos = XMFLOAT3(100.0f, 0.0f, 0.0f);	// ���S�_����\��
		g_Player[i].opos = g_Player[i].pos;
		g_Player[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].w = TEXTURE_WIDTH;
		g_Player[i].h = TEXTURE_HEIGHT;
		g_Player[i].countAnim = 0;
		g_Player[i].patternAnim = 0;
		g_Player[i].texNo = 2;
		g_Player[i].time = 0;
		//��{�X�e�[�^�X

		g_Player[i].dir = CHAR_DIR_RIGHT;					// �����i0:�� 1:�E 2:�� 3:���j
		g_Player[i].state = PLAYER_STATE_STAND;				//�v���C���[�̏��
		g_Player[i].prestate = g_Player[i].state;			//�O�̏��

		g_Player[i].life = 2;
		g_Player[i].hp = PLAYER_HP_MAX;
		g_Player[i].mp = PLAYER_MP_MAX;

		g_Player[i].str = 35.0f;
		g_Player[i].jumptimes = 2;
		g_Player[i].jumpplus = 4.0f;
		g_Player[i].move = XMFLOAT3(6.0f, 0.0f, 0.0f);		// �ړ���
		g_Player[i].flashCnt = 0;		// �ړ���
		g_Player[i].flashExeTimer = 0.0f;
		g_Player[i].flashExeCnt = 0;
		//�X�e�[�g�t���O
		g_Player[i].moving = FALSE;							// �ړ����t���O
		g_Player[i].flash = FALSE;							// �ړ����t���O
		g_Player[i].onGround = FALSE;
		g_Player[i].attack = FALSE;
		g_Player[i].bullet = FALSE;
		g_Player[i].jump = FALSE;



		// �W�����v���ւ̏�����
		g_Player[i].beforejumppos = g_Player[i].pos;
		g_Player[i].jumpCnt = 0;

		//Skill001
		g_Player[i].bullet = FALSE;
		//g_Player[i].bulletTime = 0;
		// 
		//�q�b�g
		g_Player[i].hit = FALSE;		//�q�b�g����Ă���
		g_Player[i].hitting = FALSE;		//�q�b�g����Ă���
		g_Player[i].hitdir = CHAR_DIR_LEFT;			//�q�b�g����Ă���̕���
		g_Player[i].hitCnt = 0;
		g_Player[i].clearCnt = 0;
		g_Player[i].clear = FALSE;

		// ���g�p
		for (int j = 0; j < PLAYER_OFFSET_CNT; j++)
		{
			g_Player[i].offset[j] = g_Player[i].pos;
		}

	}
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		g_Player[0].pos = XMFLOAT3(100.0f, 500.0f, 0.0f);	// ���S�_����\��
		break;
	case MODE_GAME:

		//g_Player[0].pos = XMFLOAT3(1000.0f, 2000.0f, 0.0f);	// ���S�_����\��
		g_Player[0].pos = XMFLOAT3(100.0f, 900.0f, 0.0f);	// ���S�_����\��
#ifndef _DEBUG
		//g_Player[0].pos = XMFLOAT3(500.0f, 1900.0f, 0.0f);	// ���S�_����\��
#endif // !_DEBUG
		break;
	case MODE_BOSS:
		g_Player[0].pos = XMFLOAT3(100.0f, 500.0f, 0.0f);	// ���S�_����\��
		g_Player[0].w = 100.0f;
		g_Player[0].h = 100.0f;
		//g_Player[0].move =  XMFLOAT3(8.0f, 0.0f, 0.0f);		// �ړ���
		g_Player[0].jumpplus = 6.0f;
		//g_Player[0].pos = XMFLOAT3(1000.0f, 3000.0f, 0.0f);	// ���S�_����\��


		//g_Player[0].savePoint = XMFLOAT3(100.0f, 900.0f, 0.0f);

		break;
	}




	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	for (int i = 0; i < PLAYER_STATE_MAX; i++)
	{
		if (g_State[i])
		{
			g_State[i]->Release();
			g_State[i] = NULL;
		}
	}

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i])
		{
			g_Effect[i]->Release();
			g_Effect[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//==============================

void UpdatePlayer(void)
{

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].hp <= 0)
		{
			g_Player[i].use = FALSE;
			g_deadCnt += 0.1;
			if (g_deadCnt >= DEAD_FRAME_MAX)
			{
				g_deadCnt = 0.0f;
				//if (g_Player[i].life > 0) SetFade(FADE_OUT, MODE_GAME);
				if (GetMode() == MODE_GAME)
				{
					SetResult(GAMEOVER, RESULTTYPE_GAME);
					SetFade(FADE_OUT, MODE_RESULT);

				}
				else if (GetMode() == MODE_BOSS)
				{
					SetResult(GAMEOVER, RESULTTYPE_BOSS);
					SetFade(FADE_OUT, MODE_RESULT);
				}
			}
			continue;
		}

		if (g_Player[i].use == FALSE)continue;

		ENEMY* enemy = GetEnemy();
		BG* bg = GetBG();
		UI* ui = GetSkillUI();
		int mskill = -1;

		// �����Ă�v���C���[��������������

		if (g_Player[i].pos.x >= 1900.0f && g_Player[i].pos.y >= 2800.0f)SetFade(FADE_OUT, MODE_BOSS);
		// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���

		g_Player[i].opos = g_Player[i].pos;
		g_Player[i].prestate = g_Player[i].state;

		if (g_Player[i].mp <= PLAYER_MP_MAX)g_Player[i].mp += 0.1f;


		{// �A�j���[�V����  

			if (g_Player[i].countAnim++ > g_player_state_AnimeMax[g_Player[i].state][3])
			{
				g_Player[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_Player[i].patternAnim = (g_Player[i].patternAnim + 1) % g_player_state_AnimeMax[g_Player[i].state][0];
			}
		}


		{	// ���g�p
			for (int j = PLAYER_OFFSET_CNT - 1; j > 0; j--)
			{
				g_Player[i].offset[j] = g_Player[i].offset[j - 1];
			}
			g_Player[i].offset[0] = g_Player[i].opos;
		}

		{// �L�[���͂ňړ� 
			XMFLOAT3 nextpos = g_Player[i].pos;
			float speed = g_Player[i].move.x;


			if (g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT &&
				g_Player[i].state != PLAYER_STATE_GUARD
				)
			{
				//if (GetKeyboardTrigger(DIK_LSHIFT) || IsButtonPressed(0, BUTTON_B))
				//{
				//	g_Player[i].dash = TRUE;
				//	speed *= 4;
				//	//if (g_Player[i].dir == CHAR_DIR_RIGHT)nextpos.x += 20.0f;
				//	//else if (g_Player[i].dir == CHAR_DIR_LEFT)nextpos.x -= 20.0f;

				//}


				if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
				{

					g_Player[i].state = PLAYER_STATE_WALK;
					g_Player[i].dir = CHAR_DIR_RIGHT;
					nextpos.x += speed;
				}
				else if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
				{
					g_Player[i].state = PLAYER_STATE_WALK;
					g_Player[i].dir = CHAR_DIR_LEFT;

					nextpos.x -= speed;
				}


				{//�t���b�V��
					if (((GetKeyboardTrigger(DIK_D) || IsButtonTriggered(0, BUTTON_RIGHT)) ||
						(GetKeyboardTrigger(DIK_A) || IsButtonTriggered(0, BUTTON_LEFT))) &&
						g_Player[i].flash == FALSE)
					{
						g_Player[i].moving = TRUE;
						g_Player[i].flashExeTimer = 0.0f;
						g_Player[i].flashExeCnt++;
					}
					if (g_Player[i].moving == TRUE)
					{
						g_Player[i].flashExeTimer += GetDeltatime();

						if (g_Player[i].flashExeTimer > PLAYER_FLASH_TIMES_MAX * 1000.0f)
						{
							g_Player[i].moving = FALSE;
							g_Player[i].flashExeTimer = 0.0f;
							g_Player[i].flashExeCnt = 0;
						}
					}
					if (g_Player[i].flashExeCnt >= 2)
					{
						g_Player[i].flash = TRUE;
						PlayerFlash(&nextpos, i);
					}
					else g_Player[i].flash = FALSE;

				}

				//�ړ���t�B�[���h�Ƃ̓����蔻��

				if (!WaistFieldCollision(nextpos, TEXTURE_COLLISION_WIDTH, TEXTURE_COLLISION_HEIGHT))
				{
					g_Player[i].opos = g_Player[i].pos;
					g_Player[i].pos = nextpos;
				}


				if ((GetKeyboardTrigger(DIK_SPACE) || (IsButtonTriggered(0, BUTTON_X)) || (GetSkillUIClicked() == UI_JUMP)))
				{
					g_Player[i].jumptimes--;

					if ((g_Player[i].jumptimes > 0))
					{
						g_Player[i].state = PLAYER_STATE_JUMP;
						SetJump(i);
						PlaySound(SOUND_LABEL_SE_jump);
						SetEffect3(g_Player[i].pos, JUMP);
					}
				}

				if (g_Player[i].jump && g_Player[i].jumptimes > 0)
				{
					PlayerJump(i);
				}
				if (!g_Player[i].onGround && !g_Player[i].jump)
				{
					g_Player[i].state = PLAYER_STATE_FALL;
				}
				if (g_Player[i].onGround)
				{
					g_Player[i].jumptimes = PLAYER_JUMP_TIMES_MAX;
				}
			}
			////�d�͏���
			PlayerGravity(i);
			{
				{// MAP�O�`�F�b�N
					if (g_Player[i].pos.x < 0.0f)	g_Player[i].pos.x = 0.0f;
					if (g_Player[i].pos.x > bg->w)	g_Player[i].pos.x = bg->w;
					if (g_Player[i].pos.y < 0.0f)	g_Player[i].pos.y = 0.0f;
					if (g_Player[i].pos.y > bg->h)	g_Player[i].pos.y = bg->h;
				}

				if (GetMode() == MODE_TUTORIAL)
				{// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����
					bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
					if (bg->pos.x < 0) bg->pos.x = 0;
					if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;
				}
				else if (GetMode() == MODE_GAME)
				{// �v���C���[�̗����ʒu����MAP�̃X�N���[�����W���v�Z����

					bg->pos.x = g_Player[i].pos.x - PLAYER_DISP_X;
					if (bg->pos.x < 0) bg->pos.x = 0;
					if (bg->pos.x > bg->w - SCREEN_WIDTH) bg->pos.x = bg->w - SCREEN_WIDTH;

					for (int j = 0; j < CHECKPOINT_MAX; j++)
					{
						if (CollisionBB(g_Player[i].pos, g_Player[i].w, g_Player[i].h, g_CheckPoint[j], 1000.0f, 10.0f))
						{
							SetBGSFrame(0, SCREEN_HEIGHT, 60);
							
						}							
					}

				}
			}

			/////////////////////////////////////////////////////////////

			if (g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BLACKHOLE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT)
			{
				g_Changable = TRUE;
			}

			//�X�L������
			if (g_Changable)
			{
				//�K�[�h
				if (((GetKeyboardPress(DIK_H)) && (g_Player[i].state == PLAYER_STATE_STAND)) ||
					(IsButtonPressed(0, BUTTON_L) && (g_Player[i].state == PLAYER_STATE_STAND)) ||
					((GetSkillUIPressed() == UI_GUARD) && (g_Player[i].state == PLAYER_STATE_STAND)))
				{
					if (g_Player[i].onGround == TRUE)
					{
						{
							g_Player[i].state = PLAYER_STATE_GUARD;
							g_Changable = FALSE;

							g_SkillFrameCnt = 0;

						}
					}
				}

				if (((GetKeyboardRelease(DIK_H)) && (g_Player[i].state == PLAYER_STATE_GUARD)) ||
					((GetKeyboardRelease(DIK_H)) && !IsButtonPressed(0, BUTTON_L) && (g_Player[i].state == PLAYER_STATE_GUARD)) ||
					(((GetSkillUIPressed() == UI_GUARD) && (g_Player[i].state == PLAYER_STATE_GUARD))))

				{
					g_Player[i].state = PLAYER_STATE_STAND;
					g_Changable = TRUE;

					ResetEffect3(GUARD);
				}

				//�u���C�h
				if ((GetKeyboardTrigger(DIK_J)) || IsButtonTriggered(0, BUTTON_B) || GetSkillUIClicked() == UI_BLADE)
				{
					g_Player[i].state = PLAYER_STATE_BLADE;
					g_SkillFrameCnt = 0;
				}
				//�o���b�g
				if (GetKeyboardPress(DIK_K) || IsButtonPressed(0, BUTTON_Y) || GetSkillUIClicked() == UI_BULLET)
				{
					g_BulletDelay--;
					if (g_BulletDelay < 0)
					{
						g_BulletDelay = BULLET_DELAY_CNT;
						if (g_Player[i].mp >= BULLET_COST_MP)
						{
							g_Player[i].mp -= BULLET_COST_MP;

							g_Player[i].state = PLAYER_STATE_BULLET;
							g_SkillFrameCnt = 0;
						}
						else
						{
							PlaySound(SOUND_LABEL_SE_mp);
						}
					}

				}
				//�d�͒e
				if (GetKeyboardTrigger(DIK_L) || IsButtonTriggered(0, BUTTON_R) || GetSkillUIClicked() == UI_BLACKHOLE)
				{
					if (g_Player[i].mp >= BLACKHOLE_COST_MP)
					{
						g_Player[i].mp -= BLACKHOLE_COST_MP;

						g_Player[i].state = PLAYER_STATE_BLACKHOLE;

						g_SkillFrameCnt = 0;

					}
					else
					{
						PlaySound(SOUND_LABEL_SE_mp);
					}
				}
				//�E���g��
				if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_R2) || GetSkillUIClicked() == UI_ULT)
				{
					if (g_Player[i].mp >= PLAYER_MP_MAX)
					{
						g_Player[i].mp -= ULT_COST_MP;
						g_Player[i].state = PLAYER_STATE_ULT;
						g_SkillFrameCnt = 0;
					}
					else
					{
						PlaySound(SOUND_LABEL_SE_mp);
					}
				}


			}

			float w = 0;
			switch (g_Player[i].state)
			{
			case PLAYER_STATE_BLADE:

				PlayerAttackProcess(i);
				break;

			case PLAYER_STATE_GUARD:

				Guard(i);


				break;

			case PLAYER_STATE_BLACKHOLE:

				BlackholeTimer(i);

				break;
			case PLAYER_STATE_BULLET:
				BulletTimer(i);
				break;
			case PLAYER_STATE_ULT:
				UltTimer(i);
				break;

			}

			//�`���[�g���A���}�b�v�O�ɏo����
			if (g_Player[i].pos.y > SCREEN_HEIGHT - 100 && GetMode() == MODE_TUTORIAL)
			{
				g_Player[i].pos.y = 0.0f;
				//PlayerReturn(i);
				//g_Player[i].pos = g_Player[i].opos;
			}
		}

		if (g_Player[i].hit)
		{
			SetVolumeSound(SOUND_LABEL_SE_hitplayer, 0.2f);
			PlaySound(SOUND_LABEL_SE_hitplayer);
			g_Player[i].hitting = TRUE;
			g_Player[i].hit = FALSE;
		}
		if (g_Player[i].hitting)
		{
			PHitbackProcess(i);
		}


		if (GetMode() == MODE_BOSS)
		{// �ړ����I�������G�l�~�[�Ƃ̓����蔻��

			BOSS* boss = GetBoss();
			for (int j = 0; j < BOSS_MAX; j++)
			{
				if (boss[j].use == FALSE)continue;
				int ans = CollisionBB(g_Player[i].pos, g_Player[i].w / 2, g_Player[i].h,
					boss[j].pos, boss[j].w, boss[j].h * 0.8);

				if (ans == TRUE)
				{
					g_Player[i].hit = TRUE;
					AddPlayerHP(i, -15);
				}
			}
		}




		{//�ړ������H
			XMVECTOR temp = XMLoadFloat3(&g_Player[i].pos) - XMLoadFloat3(&g_Player[i].opos);
			temp = XMVector3LengthSq(temp);
			float dis;
			XMStoreFloat(&dis, temp);
			if (dis == 0 &&
				g_Player[i].state != PLAYER_STATE_BLADE &&
				g_Player[i].state != PLAYER_STATE_BLACKHOLE &&
				g_Player[i].state != PLAYER_STATE_BULLET &&
				g_Player[i].state != PLAYER_STATE_ULT &&
				g_Player[i].state != PLAYER_STATE_GUARD)
			{//�ړ����Ă��Ȃ��Ȃ�
				g_Player[i].state = PLAYER_STATE_STAND;
			}
		}
	}

	//// ������Z�[�u����
	//if (GetKeyboardTrigger(DIK_S))
	//{
	//	SaveData();
	//}


#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			//�e�\��
			DrawPlayerShadow();

			// �v���C���[�̕��g��`��
			if (g_Player[i].flash)
			{	// �_�b�V�����������g����
				DrawPlayerOffset(i);
			}

			{//�v���C���[�\��
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[i].state]);	//0 : STAND, 1:walk 2

				//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
				float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
				float pw = g_Player[i].w;		// �v���C���[�̕\����
				float ph = g_Player[i].h;		// �v���C���[�̕\������

				if (g_Player[i].state == PLAYER_STATE_BLADE)
				{
					pw = 160.0f;		// �v���C���[�̕\����
					ph = 160.0f;
					py -= 30.0f;
				}


				//py += g_Player[i].jumpY;		// �W�����v���̍����𑫂�
				// �A�j���[�V�����p

				int animeMaxX = g_player_state_AnimeMax[g_Player[i].state][2];
				int animeMaxY = g_player_state_AnimeMax[g_Player[i].state][1];

				float tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
				float th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
				float tx = (float)(g_Player[i].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
				float ty = (float)(g_Player[i].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				if (g_Player[i].dir == CHAR_DIR_LEFT)
				{
					tw *= (-1);
				}
				float R = 1.0f;
				float G = 1.0f;
				float B = 1.0f;
				float Alpha = 1.0f;

				if (g_Player[i].clear)
				{
					G = 0.0f;
					B = 0.0f;
					Alpha = 0.7f;
				}
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, Alpha),
					g_Player[i].rot.z);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}

			//�U���\��
			//DrawAttackEffect();


		}
		else if (!g_Player[i].use && g_deadCnt <= DEAD_FRAME_MAX)
		{
			{//�v���C���[�\��
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[i].state]);	//0 : STAND, 1:walk 2

				//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
				float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
				float pw = g_Player[i].w;		// �v���C���[�̕\����
				float ph = g_Player[i].h;		// �v���C���[�̕\������

				if (g_Player[i].state == PLAYER_STATE_BLADE)
				{
					pw = 160.0f;		// �v���C���[�̕\����
					ph = 160.0f;
					py -= 30.0f;
				}


				//py += g_Player[i].jumpY;		// �W�����v���̍����𑫂�
				// �A�j���[�V�����p

				int animeMaxX = g_player_state_AnimeMax[g_Player[i].state][2];
				int animeMaxY = g_player_state_AnimeMax[g_Player[i].state][1];

				float tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
				float th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
				float tx = (float)(g_Player[i].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
				float ty = (float)(g_Player[i].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				if (g_Player[i].dir == CHAR_DIR_LEFT)
				{
					tw *= (-1);
				}

				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f / g_deadCnt),
					g_Player[i].rot.z);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
		//�v���C���[��HP�\��
		//DrawPlayerState(i);
		//�v���C���[Sikll�\��
		//DrawPlayerSkill(i);
	}

}







//=============================================================================
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}

void SetJump(int num)
{
	if (g_Player[num].jumptimes > 0)
	{
		{//�W�����v�̏���������
			g_Player[num].state = PLAYER_STATE_JUMP;
			g_gravityCnt = 0.0f;
			if (GetMode() == MODE_BOSS)	g_Player[num].jumpplus = (-1) * (PLAYER_JUMP_Y_MAX + 5.0f);
			else						g_Player[num].jumpplus = (-1) * (PLAYER_JUMP_Y_MAX);

		}
		g_Player[num].jump = TRUE;
	}
}

//�W�����v����
void PlayerJump(int num)
{

	if (g_Player[num].jump)
	{//�W�����v����
		XMFLOAT3 nextpos = g_Player[num].pos;
		{//�W�����v��

			g_Player[num].jumpplus += PLAYER_JUMP_ACC;

			XMFLOAT3   jumpplus = XMFLOAT3(0.0f, g_Player[num].jumpplus, 0.0f);
			XMVECTOR  vjumppluse = XMLoadFloat3(&jumpplus);
			XMVECTOR       vPpos = XMLoadFloat3(&nextpos);

			XMStoreFloat3(&nextpos, vPpos + vjumppluse);

			if (g_Player[num].jumpplus >= 0)
			{
				g_Player[num].jumpplus = 0;
				g_Player[num].jump = FALSE;
				g_Player[num].state = PLAYER_STATE_FALL;
			}
			else g_Player[num].state = PLAYER_STATE_JUMP;
			//if(!g_Player[num].jump)
			//else PLAYER_STATE_FALL;
		}
		//�����ǂɂԂ�������W�����v��Ԃ��L�����Z��
		XMFLOAT3 phead = XMFLOAT3(nextpos.x, nextpos.y - g_Player[num].h / 2, 0.0f);
		if (!FieldCollision(phead, TEXTURE_COLLISION_WIDTH, TEXTURE_COLLISION_HEAD_HEIGHT))
		{
			g_Player[num].pos = nextpos;
		}
		else
		{
			g_Player[num].jump = FALSE;
			g_Player[num].jumpplus = 0;
		}

		//�����ǂɂԂ�������W�����v��Ԃ��L�����Z��
		if (IsFootCol(num))
		{
			g_Player[num].jump = FALSE;
			g_Player[num].jumpplus = 0;
		}
	}

}

void PlayerFlash(XMFLOAT3* nextpos, int num)
{
	if (g_Player[num].use == FALSE)return;
	if (g_Player[num].flash)
	{
		float speed = g_Player[num].move.x * 2;

		if (g_Player[num].dir == CHAR_DIR_LEFT)
			nextpos->x -= speed;
		else
			nextpos->x += speed;
	}
	return;
}
//���ꔻ�f����
BOOL IsFootCol(int num)
{

	XMFLOAT3 pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//�����̍��W
	//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
	BOOL ans = DownFieldCollision(pfootpos, TEXTURE_COLLISION_FOOT_WIDTH, TEXTURE_COLLISION_FOOT_HEIGHT);

	if (ans)return TRUE;//���ꂪ����

	return FALSE;
}
//���f����
BOOL IsWaistCol(int num)
{

	XMFLOAT3 pwaistpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y, 0.0f);//�����̍��W
	//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
	BOOL ans = WaistFieldCollision(pwaistpos, TEXTURE_COLLISION_WAIST_WIDTH / 2, TEXTURE_COLLISION_WAIST_HEIGHT / 2);

	if (ans)return TRUE;//���ꂪ����

	return FALSE;
}

//�d�͏���
void PlayerGravity(int num)
{
	{//�d�͏���
		if (g_Player[num].onGround)
		{
			g_gravityCnt = 0;

		}
		XMFLOAT3	nextpos = g_Player[num].pos;
		XMFLOAT3	pfootpos;
		//BOOL ans = FieldCollision(pfootpos, g_Player[num].w / 2, g_Player[num].h -  128.0f);
		BOOL		ans;

		if (GetMode() == MODE_BOSS)
		{
			pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//�����̍��W]
			ans = FieldCollision(pfootpos, g_Player[num].w / 4, g_Player[num].h / 4);
			if (ans)//���ꂪ����
			{
				g_Player[num].onGround = TRUE;
				g_gravityCnt = 0;
			}
			else
			{
				g_Player[num].onGround = FALSE;
				g_gravityCnt += GRAVITY;
				float acc = g_gravityCnt * 5;
				float totalmove = g_gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 10)
				{
					totalmove = TEXTURE_HEIGHT / 10;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Player[num].w / 2, g_Player[num].h / 16);
			if (!ans)
			{
				g_Player[num].pos = nextpos;
			}

		}
		else
		{
			pfootpos = XMFLOAT3(g_Player[num].pos.x, g_Player[num].pos.y + g_Player[num].h / 2, 0.0f);//�����̍��W
			ans = FieldCollision(pfootpos, TEXTURE_COLLISION_FOOT_WIDTH, TEXTURE_COLLISION_FOOT_HEIGHT);
			if (ans)//���ꂪ����
			{
				g_Player[num].onGround = TRUE;
				g_gravityCnt = 0;
			}
			else
			{
				g_Player[num].onGround = FALSE;
				g_gravityCnt += GRAVITY;
				float acc = g_gravityCnt * 5;
				float totalmove = g_gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Player[num].w / 2, g_Player[num].h / 16);
			if (!ans)
			{
				g_Player[num].pos = nextpos;
			}


		}



	}

}
//=============================================================================
//�U���\��
//=============================================================================
void DrawAttackEffect(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].state == PLAYER_STATE_BLADE)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect[1]);

			float temp1 = 70.0f;
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				temp1 *= (-1);
			}
			float px = g_Player[i].pos.x - temp1 - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = TEXTURE_WIDTH * 2;		// �v���C���[�̕\����
			float ph = TEXTURE_HEIGHT * 2;		// �v���C���[�̕\������

			float temp = g_Player[i].w;
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				temp *= (-1);
			}
			px += temp;

			int animeMaxX = g_effect_state_AnimeMax[1][2];
			int animeMaxY = g_effect_state_AnimeMax[1][1];

			float tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
			float th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
			float tx = (float)((int)g_Player[i].time % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)((int)g_Player[i].time / animeMaxX) * th;	// �e�N�X�`���̍���Y���W
			if (g_Player[i].dir == CHAR_DIR_LEFT)
			{
				tw *= (-1);
			}

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

}



//=============================================================================
// �v���C���[�U������
//=============================================================================

void PlayerAttackProcess(int num)
{

	{//�U������

		if (g_Player[num].state != PLAYER_STATE_BLADE)return;
		//�J�E���^�[
		g_Changable = FALSE;

		int waitMax = g_player_state_AnimeMax[g_Player[num].state][3] * g_player_state_AnimeMax[g_Player[num].state][0];
		if (g_Player[num].time++ > waitMax)
		{
			g_Player[num].time = 0;
			g_Player[num].state = PLAYER_STATE_STAND;
			g_Changable = TRUE;
		}

		float attackx = 0.0f;
		if (g_Player[num].dir == CHAR_DIR_RIGHT)		attackx = (g_Player[num].w / 2);
		else if (g_Player[num].dir == CHAR_DIR_LEFT)	attackx = (-1) * (g_Player[num].w / 2);


		XMFLOAT3 blade_pos = XMFLOAT3(g_Player[num].pos.x + attackx, g_Player[num].pos.y, 0.0f);

		if (g_Player[num].time == 1)
		{
			int temp = rand() % 2;
			if (temp % 2 == 0)	PlaySound(SOUND_LABEL_SE_ken001);
			else				PlaySound(SOUND_LABEL_SE_ken002);

			SetEffect3(blade_pos, BLADE, g_Player[num].dir);

		}


		switch (GetMode())
		{
		case MODE_TUTORIAL:
		case MODE_GAME:

			//�U���������̓����蔻��
		{
			ENEMY* enemy = GetEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				if (enemy[j].use == FALSE)continue;
				int ans = CollisionBB(blade_pos, PLAYER_ATTACK_EFFECT_WIDTH + 30.0f, PLAYER_ATTACK_EFFECT_HEIGHT + 30.0f, enemy[j].pos, enemy[j].w, enemy[j].h);

				if (ans == TRUE)
				{



					if (g_Player[num].time == waitMax)
					{
						SetEffect3(g_Player[num].pos, HIT);
						AddPlayerMP(num, 4.0f);
						//SetEffect3(g_Player[num].pos, MPUP);
					}

					enemy[j].hit = TRUE;
					SetEDamagedType(j, DAMAGED_BLADE);
				}
			}
		}


		break;
		case MODE_BOSS:

			if (g_Player[num].time == 1)
			{
				//�U���������̓����蔻��
				{

					BOSS* boss = GetBoss();
					for (int j = 0; j < BOSS_MAX; j++)
					{
						if (boss[j].use == FALSE)continue;
						int ans = CollisionBB(blade_pos, PLAYER_ATTACK_EFFECT_WIDTH + 30.0f, PLAYER_ATTACK_EFFECT_HEIGHT + 30.0f,
							boss[j].pos, boss[j].w, boss[j].h);

						if (ans == TRUE)
						{
							int temp = rand() % 5;
							if (temp == 0)
							{
								AddPlayerMP(num, 10.0f);
							}

							boss[j].hit = TRUE;
							SetBDamagedType(DAMAGED_BLADE);
						}
					}
				}
			}
			break;

		}

	}
}

//=============================================================================
//���ɗ���������
//=============================================================================
void PlayerReturn(int num)
{
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (g_Player[i].pos.y >= SCREEN_HEIGHT)
			{
				g_Player[i].pos.y = 0.0f;
				//g_Player[i].pos = g_Player[i].opos;
			}
		}
		break;
	case MODE_GAME:
		break;

	default:
		break;
	}


}


//=============================================================================
// �v���C���[�̕��g��`��
//=============================================================================
void DrawPlayerOffset(int no)
{
	BG* bg = GetBG();
	float alpha = 0.0f;

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_State[g_Player[no].state]);

	for (int j = PLAYER_OFFSET_CNT - 1; j >= 0; j--)
	{
		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Player[no].offset[j].x - bg->pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_Player[no].offset[j].y - bg->pos.y;	// �v���C���[�̕\���ʒuY
		float pw = g_Player[no].w;		// �v���C���[�̕\����
		float ph = g_Player[no].h;		// �v���C���[�̕\������

		// �A�j���[�V�����p
		int animeMaxX = g_player_state_AnimeMax[g_Player[no].state][2];
		int animeMaxY = g_player_state_AnimeMax[g_Player[no].state][1];

		float tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
		float th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
		float tx = (float)(g_Player[no].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Player[no].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

		if (g_Player[no].dir == CHAR_DIR_LEFT)
		{
			tw *= (-1);
		}
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha),
			g_Player[no].rot.z);

		alpha += (1.0f / PLAYER_OFFSET_CNT);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}

// �e�\��
void DrawPlayerShadow(void)
{
	BG* bg = GetBG();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use && g_Player[i].onGround)
		{
			SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

		// �e�N�X�`���ݒ�


			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Effect[0]);

			float px = g_Player[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Player[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Player[i].w;		// �v���C���[�̕\����
			float ph = g_Player[i].h / 4;		// �v���C���[�̕\������
			py += 60.0f;		// �����ɕ\��

			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�
		}


	}
}




////�v���C���[��HP�\��
//void DrawPlayerHP(int num)
//{
//	// HP�̕\��
//	BG* bg = GetBG();
//	{
//		// ���~���̃Q�[�W�i�g�I�ȕ��j
//		// �e�N�X�`���ݒ�
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
//		float px = 263.0f;		// �Q�[�W�̕\���ʒuX
//		float py = 135.0f;		// �Q�[�W�̕\���ʒuY
//		float pw = 220;	// �Q�[�W�̕\����
//		float ph = 30.0f;		// �Q�[�W�̕\������
//
//		float tw = 1.0f;	// �e�N�X�`���̕�
//		float th = 1.0f;	// �e�N�X�`���̍���
//		float tx = 0.0f;	// �e�N�X�`���̍���X���W
//		float ty = 0.0f;	// �e�N�X�`���̍���Y���W
//
//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));
//
//		// �|���S���`��
//		GetDeviceContext()->Draw(4, 0);
//
//
//		// �G�l�~�[�̐��ɏ]���ăQ�[�W�̒�����\�����Ă݂�
//		// �e�N�X�`���ݒ�
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
//		pw = pw * ((float)g_Player[num].hp / PLAYER_HP_MAX);
//
//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f));
//
//		// �|���S���`��
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}
//
////�v���C���[��MP�\��
//void DrawPlayerMP(int num)
//{
//	// HP�̕\��
//	BG* bg = GetBG();
//	{
//		// ���~���̃Q�[�W�i�g�I�ȕ��j
//		// �e�N�X�`���ݒ�
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
//		float px = 263.0f;		// �Q�[�W�̕\���ʒuX
//		float py = 200.0f;		// �Q�[�W�̕\���ʒuY
//		float pw = GAUGE_WIDTH;	// �Q�[�W�̕\����
//		float ph = GAUGE_HEIGHT;		// �Q�[�W�̕\������
//
//		float tw = 1.0f;	// �e�N�X�`���̕�
//		float th = 1.0f;	// �e�N�X�`���̍���
//		float tx = 0.0f;	// �e�N�X�`���̍���X���W
//		float ty = 0.0f;	// �e�N�X�`���̍���Y���W
//
//		float R = 0.0f;
//		float G = 0.3f;
//		float B = 0.8f;
//		float Alpha = 1.0f;
//
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
//
//		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
//		float temp = ((float)g_Player[num].mp / PLAYER_MP_MAX);
//		pw = pw * temp;
//		if (pw >= GAUGE_WIDTH)
//		{
//			R = 0.11765f;
//			G = 0.941f;
//			B = 1.0f;
//		}
//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(R, G, B, Alpha));
//
//		// �|���S���`��
//		GetDeviceContext()->Draw(4, 0);
//
//
//	}
//
//}

////�v���C���[��ԕ\��
//void DrawPlayerState(int num)
//{
//	DrawPlayerHP(num);
//	DrawPlayerMP(num);
//	{
//		// ���~���̃Q�[�W�i�g�I�ȕ��j
//		// �e�N�X�`���ݒ�
//		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
//
//		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
//		float px = 60.0f;		// �Q�[�W�̕\���ʒuX
//		float py = -10.0f;		// �Q�[�W�̕\���ʒuY
//		float pw = 480.0f;	// �Q�[�W�̕\����
//		float ph = 405.0f;		// �Q�[�W�̕\������
//
//		float tw = 1.0f;	// �e�N�X�`���̕�
//		float th = 1.0f;	// �e�N�X�`���̍���
//		float tx = 0.0f;	// �e�N�X�`���̍���X���W
//		float ty = 0.0f;	// �e�N�X�`���̍���Y���W
//
//		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
//		SetSpriteLTColor(g_VertexBuffer,
//			px, py, pw, ph,
//			tx, ty, tw, th,
//			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//
//		// �|���S���`��
//		GetDeviceContext()->Draw(4, 0);
//
//	}
//}


//�q�b�g�o�b�N����
void PHitbackProcess(int num)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[num].use == TRUE)
		{
			if (g_Player[num].hitting)
			{//�����ɓ��Ă�ꂽ����
				if (g_Player[num].hitCnt > HIT_FRAME_MAX)
				{	//���ԃI�[�o�[
					g_Player[num].hit = FALSE;
					g_Player[num].hitting = FALSE;
					g_Player[num].hitCnt = 0;

					g_Player[num].clear = FALSE;
					g_Player[num].clearCnt = 0;
				}

				{//���Ă�ꂽ�r���H

					g_Player[num].hitCnt++;
					g_Player[num].clearCnt++;
					if (g_Player[num].clearCnt >= HIT_CLEAR_FRAME_MAX)
					{
						g_Player[num].clearCnt = 0;
						g_Player[num].clear = !(g_Player[num].clear);
					}
				}
			}
		}
	}

}

//�v���C���[��HP���������̒l������
void AddPlayerHP(int num, float hp)
{

	if (g_Player[num].use == FALSE)return;
	if (g_Player[num].hitting == TRUE)return;

	if (g_Player[num].state != PLAYER_STATE_GUARD)
	{
		g_Player[num].hp += hp;
		if (hp < 0)
		{
			SetHitScore(hp, g_Player[num].pos, DAMAGE_PLAYER);
			SetEffect3(g_Player[num].pos, HIT2);
			if (g_Player[num].hp <= 0.0f)g_Player[num].hp = 0.0f;

		}
		else
		{//��
			SetHitScore(hp, g_Player[num].pos, HEAL);
			SetHealEffect(g_Player[num].pos);
			PlaySound(SOUND_LABEL_SE_heal001);
			if (g_Player[num].hp >= PLAYER_HP_MAX)g_Player[num].hp = PLAYER_HP_MAX;

		}
	}

}

void AddPlayerMP(int num, float mp)
{
	if (g_Player[num].use == FALSE)return;

	g_Player[num].mp += mp;
	SetHitScore(mp, g_Player[num].pos, MP1);
	SetEffect3(g_Player[num].pos, MPUP);
	PlaySound(SOUND_LABEL_SE_mpUP);
	{//��
		if (g_Player[num].mp >= PLAYER_MP_MAX)g_Player[num].mp = PLAYER_MP_MAX;
	}

}
//�v���C���[�̃W�����v��Ԃ����Z�b�g
void JumpReset(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_Player[i].use == TRUE)
		{
			g_Player[i].jump = FALSE;
			g_Player[i].jumpCnt = 0;
			g_Player[i].jumpplus = 0.0f;

		}
	}
	return;
}

void GetSavepoint(void)
{

}

void BlackholeTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[PLAYER_STATE_BLACKHOLE][0];
	int aniwaitMax = g_player_state_AnimeMax[PLAYER_STATE_BLACKHOLE][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_BlackholeFCnt++ > waitMax)
	{
		float x = g_Player[num].pos.x;
		if (g_Player[num].dir == CHAR_DIR_LEFT)
		{
			x -= g_Player[num].w;
		}
		else x += g_Player[num].w;

		SetBullet(XMFLOAT3(x, g_Player[num].pos.y, 0.0f), BLACKHOLE);
		g_BlackholeFCnt = 0;
		g_Changable = TRUE;
		g_Player[num].state = PLAYER_STATE_STAND;

	}
	else
	{
		g_Changable = FALSE;
	}
	return;

}
void BulletTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[PLAYER_STATE_BULLET][0];
	int aniwaitMax = g_player_state_AnimeMax[PLAYER_STATE_BULLET][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_SkillFrameCnt++ > waitMax)
	{

		SetBullet(g_Player[num].pos, BULLET_1);
		g_SkillFrameCnt = 0;
		g_Changable = TRUE;
		g_Player[num].state = PLAYER_STATE_STAND;
	}
	else
	{
		g_Changable = FALSE;
	}
	return;
}


void SetUlt(void)
{

}

void UltTimer(int num)
{
	if (g_Player[num].use == FALSE)return;

	int patternMax = g_player_state_AnimeMax[g_Player[num].state][0];
	int aniwaitMax = g_player_state_AnimeMax[g_Player[num].state][3];
	int waitMax = patternMax * aniwaitMax;

	if (g_SkillFrameCnt++ > ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX + 30)
	{
		//�����蔻��
		g_SkillFrameCnt = 0;
		g_Changable = TRUE;
		g_Ult = FALSE;
		g_Player[num].state = PLAYER_STATE_STAND;
	}
	else
	{
		g_Changable = FALSE;
		BG* bg = GetBG();

		XMFLOAT3 u1pos = XMFLOAT3(g_Player[0].pos.x - bg->pos.x, g_Player[0].pos.y - bg->pos.y, 0.0f);
		if (g_SkillFrameCnt == 1)
		{
			PlaySound(SOUND_LABEL_SE_ultstart);
			SetEffect3(u1pos, ULT1);
		}
		else if (g_SkillFrameCnt == ULT_CHANT_FRAMEMAX + 30)
		{
			if (GetMode() == MODE_BOSS)
			{
				BOSS* boss = GetBoss();

				for (int i = 0; i < BOSS_MAX; i++)
				{
					boss[i].hit = TRUE;

					SetBDamagedType(DAMAGED_ULT);
				}
			}
			else
			{
				ENEMY* enemy = GetEnemy();
				PLAYER* player = GetPlayer();
				for (int i = 0; i < ENEMY_MAX; i++)
				{
					if (enemy[i].use == FALSE)continue;

					if ((enemy[i].pos.x >= (player[0].pos.x - SCREEN_CENTER_X)) &&
						(enemy[i].pos.x <= (player[0].pos.x + SCREEN_CENTER_X)) &&
						(enemy[i].pos.y >= (player[0].pos.y - SCREEN_CENTER_Y)) &&
						(enemy[i].pos.y <= (player[0].pos.y + 100))
						)
					{
						enemy[i].hit = TRUE;
						SetEDamagedType(i, DAMAGED_ULT);
					}
				}
			}


			SetEffect3(u1pos, ULT2);

		}

		if (g_SkillFrameCnt % 40 == 0 && g_SkillFrameCnt > ULT_CHANT_FRAMEMAX + 30)
		{
			PlaySound(SOUND_LABEL_SE_ulthit);
		}
		//if (g_SkillFrameCnt == 90)
		//{
		//	SetEffect3();
		//}
	}
	return;


}

void Guard(int num)
{
	if (g_Player[num].state == PLAYER_STATE_GUARD)
	{
		if (g_GuardFrameCnt++ >= 20)
		{
			g_GuardFrameCnt = 0;
			PlaySound(SOUND_LABEL_SE_shield);
		}

		float w = g_Player[num].w / 2;
		if (g_Player[num].dir == CHAR_DIR_LEFT)w *= (-1);

		XMFLOAT3 epos = XMFLOAT3(g_Player[num].pos.x + w,
			g_Player[num].pos.y, 0.0f);
		SetEffect3(epos, GUARD, g_Player[num].dir);
	}

	else
	{
		g_GuardFrameCnt = 0;
	}

}


//
//void InitPlayerIcon(void)
//{
//	g_PlayerIcon.use = FALSE;
//	g_PlayerIcon.pos = XMFLOAT3(175.0f, 180.0f, 0.0f);
//	g_PlayerIcon.w = 180.0f;
//	g_PlayerIcon.h = 180.0f;
//	g_PlayerIcon.texNo = 2;
//	g_PlayerIcon.theta = 0.0f;
//
//	return;
//}
//void UpdatePlayerIcon(void)
//{
//	if (g_Player[0].hitting == TRUE)g_PlayerIcon.use = TRUE;
//	else g_PlayerIcon.use = FALSE;
//
//	if (g_PlayerIcon.use == FALSE)
//	{
//		g_PlayerIcon.theta = 0.0f;
//		return;
//	}
//
//	g_PlayerIcon.theta += 1.5f;
//
//
//}
//
//void  DrawPlayerIcon(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerIcon.texNo]);
//
//	float tx = g_PlayerIcon.pos.x;
//	float ty = g_PlayerIcon.pos.y;
//	float tw = g_PlayerIcon.w;
//	float th = g_PlayerIcon.h;
//
//	float u = 0.0f;
//	float v = 0.0f;
//	float uw = 1.0f;
//	float vh = 1.0f;
//
//	float R = 1.0f;
//	float G = 1.0f;
//	float B = 1.0f;
//	float Alpha = 1.0f;
//
//	if (g_PlayerIcon.use == TRUE)
//	{
//		G = 0.0f, B = 0.0f, Alpha = 0.7f;
//
//		tx += 10 * sin(g_PlayerIcon.theta);
//		/*	ty += 5 * cos(g_PlayerIcon.theta);*/
//		tw = g_PlayerIcon.w * (1 + 0.025 * sin(g_PlayerIcon.theta));
//		th = g_PlayerIcon.h * (1 + 0.025 * sin(
//			g_PlayerIcon.theta));
//	}
//	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);
//
//	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
//	//// �|���S���`��
//	GetDeviceContext()->Draw(4, 0);
//
//
//}

void DrawEyeDark(void)
{

	
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	BG* bg = GetBG();
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

	float tx = g_Player[0].pos.x - bg->pos.x;
	float ty = g_Player[0].pos.y - bg->pos.y;
	float tw = 7680.0f;
	float th = 4320.0f;

	float u = 0.0f;
	float v = 0.0f;
	float uw = 1.0f;
	float vh = 1.0f;

	float R = 1.0f;
	float G = 1.0f;
	float B = 1.0f;
	float Alpha = 1.0f;

	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);

	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

}