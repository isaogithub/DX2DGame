//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "bg.h"
#include "player.h"
#include "fade.h"
#include "collision.h"
#include "bg.h"
#include "field.h"
#include "hitscore.h"
#include "sound.h"
#include "bullet.h"
#include "effect.h"
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�e�N�X�`��
#define TEXTURE_WIDTH				(128)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(128)	// 

#define TEXTURE_MAX					(5)		// �e�N�X�`���̐�
#define TEXTURE_SLIME_MAX			(3)		//�X���C���̃e�N�X�`����

//�A�j���[�V����
#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(7)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define HIT_CLEAR_MAX				(20)

//�G�l�~�[�ړ��f�t�@�C��
#define ENEMY_MOVE_MAX				(100.0f)
#define KNOCK_BACK_DIS				(50.0f)
#define STOP_TIME_FRAME_MAX			(120)
#define ENEMY_SOUND_DISMAX			(700)

//Slime
#define TEXTURE_SLIME_WIDTH		(80)	//
#define TEXTURE_SLIME_HEIGHT	(40)	//

#define TEXTURE_SLIME_WAIST_WIDTH (64)	//�̂̓����蔻��
#define TEXTURE_SLIME_WAIST_HEIGHT (45)

#define TEXTURE_SLIME_FOOT_WIDTH (50)	//�����̓����蔻��
#define TEXTURE_SLIME_FOOT_HEIGHT (20)



#define	SLIME_HPMAX					(100)
//BOSS
#define TEXTURE_BOSS_WIDTH	(520)	//
#define TEXTURE_BOSS_HEIGHT	(520)	//

#define TEXTURE_BOSS_WAIST_WIDTH (400)	//�̂̓����蔻��
#define TEXTURE_BOSS_WAIST_HEIGHT (260)

#define TEXTURE_BOSS_FOOT_WIDTH (500)	//�����̓����蔻��
#define TEXTURE_BOSS_FOOT_HEIGHT (20)

#define	BOSS_HPMAX					(900)

#define ATTACK_TIME_MAX			(30)

#define SKILL1_BULLET_MAX		(10)
#define	BOSS_DEADTIME_MAX			(1.0f)
//SKILL2
#define SKILL2_MAX								(5)
#define SKILL2_WAIT_MAX							(5)
#define SKILL2_PATTERN_DIVIDE_X					(5)
#define SKILL2_PATTERN_DIVIDE_Y					(2)
#define SKILL2_ANIM_PATTERN_NUM					(SKILL2_PATTERN_DIVIDE_Y *  SKILL2_PATTERN_DIVIDE_X)	// �A�j���[�V�����p�^�[����

//SKILL2POS
#define SKILL2POS_WAIT_MAX						(5)
#define SKILL2POS_PATTERN_DIVIDE_X				(2)
#define SKILL2POS_PATTERN_DIVIDE_Y				(1)
#define SKILL2POS_ANIM_PATTERN_NUM				(SKILL2POS_PATTERN_DIVIDE_X *  SKILL2POS_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

//SKILL3
#define SKILL3_MAX								(20)
#define SKILL3_WAIT_MAX							(5)
#define SKILL3_PATTERN_DIVIDE_X					(4)
#define SKILL3_PATTERN_DIVIDE_Y					(1)
#define SKILL3_ANIM_PATTERN_NUM					(SKILL3_PATTERN_DIVIDE_Y *  SKILL3_PATTERN_DIVIDE_X)

//SKILL4

#define SKILL4_MAX								(100)
#define SKILL4_BULLET_MAX						(6)
#define SKILL4_DISGAP							(140)
#define SKILL4_POS_MAX							(20)

#define SKILL4_WAIT_MAX							(5)
#define SKILL4_PATTERN_DIVIDE_X					(5)
#define SKILL4_PATTERN_DIVIDE_Y					(3)
#define SKILL4_ANIM_PATTERN_NUM					(SKILL4_PATTERN_DIVIDE_Y *  SKILL4_PATTERN_DIVIDE_X)

//�H
//#define TEXTURE_COLLISION_WIDTH		(80)	//
//#define TEXTURE_COLLISION_HEIGHT	(100)	//
//
//#define TEXTURE_COLLISION_WAIST_WIDTH (64)	//�̂̓����蔻��
//#define TEXTURE_COLLISION_WAIST_HEIGHT (45)
//
//#define TEXTURE_COLLISION_HEAD_WIDTH (40)	//���̓����蔻��
//#define TEXTURE_COLLISION_HEAD_HEIGHT (30)
//
//#define TEXTURE_COLLISION_FOOT_WIDTH (80)	//�����̓����蔻��
//#define TEXTURE_COLLISION_FOOT_HEIGHT (10)
// 
//#define ENEMY1_MOVE_TIME			(200)
//#define ENEMY2_MOVE_TIME			(300)
//#define ENEMY3_MOVE_TIME			(400)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static ID3D11ShaderResourceView *g_Texture_Enemy[TEXTURE_SLIME_MAX] = { NULL };	// �X���C���e�N�X�`�����
static ID3D11ShaderResourceView* g_Texture_Boss[BOSS_STATE_MAX] = { NULL };	// �X���C���e�N�X�`�����

static char *g_TextureName[TEXTURE_MAX] = {

	"data/TEXTURE/bar_white.png",
	"data/EFFECT/enemy_skill02.png",
	"data/EFFECT/skill02_pos.png",
	"data/CHARA/Mandrake-Walk.png",
	"data/EFFECT/enemy_skill04.png",

};

//Slime�̃e�N�X�`���ݒ�
static char* g_TextureName_Enemy[] = {
	"data/CHARA/enemy_slime_stand.png",
	"data/CHARA/enemy_slime_walk.png",
	"data/CHARA/enemy_slime_attack.png"
};

//BOSS�̃e�N�X�`���ݒ�
static char* g_TextureName_Boss[] = {

	"data/CHARA/Tree-Idle.png",
	"data/CHARA/Tree-Walk.png",
	"data/CHARA/Tree-Hit.png",
	"data/CHARA/Tree-Attack.png",
	"data/CHARA/Tree-Skill02.png",
	"data/CHARA/Tree-Skill03.png",
	"data/CHARA/Tree-Skill04.png",
	"data/CHARA/Tree-Walk.png",
	"data/CHARA/Tree-Down.png",
};

static	int g_slime_state_AnimeMax[TEXTURE_SLIME_MAX][4] = { //�e�e�N�X�`���̕�����
//  Pattern_Max	divideY  divideX	animewait
		 1,		1,			1,			10,	    //STAND
		 4,		1,			4,			 5,		//WALK
		 7,		1,			7,			 4,		//ENEMY_STATE_ATTACK001
};

static	int g_boss_state_AnimeMax[BOSS_STATE_MAX][4] = { //�e�e�N�X�`���̕�����
//  Pattern_Max	divideY  divideX	animewait
		 8,		1,			8,			 2,		//IDLE
		 8,		1,			8,			 5,		//WALK
		 8,		1,			8,			 10,	//HIT
		 8,		1,			8,			 5,		//SKILL001
		 8,		1,			8,			 5,		//SKILL002
		 8,		1,			8,			 8,		//SKILL003
		 8,		1,			8,			 8,		//SKILL004
		 8,		1,			8,			 1,		//SKILL005
		 2,		1,			2,			 10,	//DOWN
};


static BOOL		 g_Load = FALSE;			// ���������s�������̃t���O
	
static ENEMY	g_Enemy[ENEMY_MAX];		// �G�l�~�[�\����
static BOSS		g_Boss[BOSS_MAX];

static SKILL	g_Skill002[SKILL2_MAX];
static SKILL3	g_Skill003[SKILL3_MAX];
static SKILL3	g_Skill004[SKILL4_MAX];

static ANI		g_Skill002Pos[SKILL2_MAX];
static int		g_EnemyCnt;
static float    g_S2Remaintimer;


int	g_clearCnt;
float g_knockmoveCnt;
float g_time;
int g_stoptime;
int g_EnemyMax;
int g_initMoveX;
int g_bulletdir;
int g_HpbulletNum;
float g_timer;

float g_Bosswalktimer;
float g_skill01timer;
float g_skill02timer;

static int g_skill02Cnt;
static int g_skill02frameCnt;

static int g_skill03Cnt;
static int g_skill03delayCnt;

static int g_skill04Cnt;
static int g_skill04delayCnt;

static int g_Downtimer;
static int g_EPdelayCnt;
static BOOL g_EPdelayFlag;
static float g_Walktimer;
static float g_Idletimer;
static float g_movetimer;
static	float g_Deadtimer;
static int g_DeadCnt;
static BOOL	g_DeadBoss;

BOOL g_closeP;
BOOL g_changeable;


//�`���[�g���A���̃G�l�~�[�̈ʒu
static XMFLOAT3 g_Tutorial_EPos[] = {
	XMFLOAT3(2000.0f,300.0f,0.0f),
	XMFLOAT3(2500.0f,300.0f,0.0f),
	XMFLOAT3(3300.0f,300.0f,0.0f),
};

static XMFLOAT3 g_Game_EPos[] = {
	XMFLOAT3(2000.0f,300.0f,0.0f),
	XMFLOAT3(3000.0f,300.0f,0.0f),
	XMFLOAT3(3500.0f,300.0f,0.0f),
};

static INTERPOLATION_DATA2 g_MoveTbl0[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(1600.0f,  650.0f, 0.0f),	60 },
	{ XMFLOAT3(1400.0f,  650.0f, 0.0f),	60 },
};

static INTERPOLATION_DATA2 g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(1600.0f,  650.0f, 0.0f),	60 },
	{ XMFLOAT3(200.0f,  650.0f, 0.0f),	50 },
};


static XMFLOAT3 g_Skill004_Pos[SKILL4_POS_MAX][SKILL4_BULLET_MAX] =
{
	
	//100	240	380	520	660	800
	//1
	// 
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	//2
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	//3

	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	//4
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),
	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,800.0f,0.0f),
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(0.0f,0.0f,0.0f),		XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(1700.0f,800.0f,0.0f),

	//5
	XMFLOAT3(1700.0f,100.0f,0.0f),	XMFLOAT3(1700.0f,240.0f,0.0f),	XMFLOAT3(1700.0f,380.0f,0.0f)	,XMFLOAT3(1700.0f,520.0f,0.0f),	XMFLOAT3(1700.0f,660.0f,0.0f),	XMFLOAT3(0.0f,0.0f,0.0f),

};

//
//
//static INTERPOLATION_DATA g_MoveTbl1[] = {
//	//���W									��]��							�g�嗦							����
//	{ XMFLOAT3(2000.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY2_MOVE_TIME },
//	{ XMFLOAT3(1200.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY2_MOVE_TIME },
//};
//
//
//static INTERPOLATION_DATA g_MoveTbl2[] = {
//	//���W									��]��							�g�嗦							����
//	{ XMFLOAT3(3000.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY3_MOVE_TIME },
//	{ XMFLOAT3(3400.0f,  500.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f) ,	XMFLOAT3(1.0f, 1.0f, 1.0f),			ENEMY3_MOVE_TIME },
//};
//
//
//
//
//static INTERPOLATION_DATA* g_MoveTblAdr[] =
//{
//	g_MoveTbl0,
//	g_MoveTbl1,
//	g_MoveTbl2,
//
//};
static INTERPOLATION_DATA2* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	ID3D11Device *pDevice = GetDevice();

	
	// �G�l�~�[�\���̂̏�����
	g_clearCnt = 0;
	g_EnemyCnt = 0;
	g_knockmoveCnt = 0;
	g_time = 0;
	g_stoptime = 0;
	g_Bosswalktimer = 0;
	g_skill01timer = 0.0f;

	g_skill02Cnt = 0;
	g_skill02timer = 0.0f;
	g_skill02frameCnt = 0;

	g_skill03Cnt = 0;
	g_skill03delayCnt = 0;

	g_S2Remaintimer = 0.0f;
	g_initMoveX = 5.0f;
	g_bulletdir = BULLET_DIR_LEFT;
	g_HpbulletNum = 0;
	g_closeP = FALSE;
	g_changeable = TRUE;
	g_EPdelayCnt = 0;
	g_Walktimer = 0.0f;
	g_Idletimer = 0.0f;
	g_Downtimer = 0.0f;
	g_movetimer = 0.0f;
	g_Deadtimer = 1.0f;
	g_DeadBoss = FALSE;
	g_DeadCnt = 0;
	//�e�N�X�`������
	switch (GetMode())
	{
	case MODE_TUTORIAL:

		g_EnemyMax = ENEMY_TUTORIAL_MAX;


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

		for (int i = 0; i < TEXTURE_SLIME_MAX; i++)
		{
			g_Texture_Enemy[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_TextureName_Enemy[i],
				NULL,
				NULL,
				&g_Texture_Enemy[i],
				NULL);
		}

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_Enemy[i].use = FALSE;
			g_Enemy[i].state = BOSS_STATE_IDLE;
			g_Enemy[i].type = ENEMY_TYPE_SLIME;
			//���W�����Ȃ�
			g_Enemy[i].opos = g_Enemy[i].pos;							//�O�̍��W��ۑ�����
			g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			
			//�e�N�X�`��
			g_Enemy[i].texNo = 0;
			g_Enemy[i].w = TEXTURE_SLIME_WIDTH;
			g_Enemy[i].h = TEXTURE_SLIME_HEIGHT;
			//�A�j���[�V����
			g_Enemy[i].countAnim = 0;
			g_Enemy[i].patternAnim = 0;

			//�X�e�[�^�X
			g_Enemy[i].hp = 100;
			g_Enemy[i].damagedType = -1;

			//�����������̃q�b�g
			g_Enemy[i].clear = FALSE;						//�������
			g_Enemy[i].hit = FALSE;							//�q�b�g����Ă���HNO
			g_Enemy[i].hitting = FALSE;						//�q�b�g����Ă���Œ�
			g_Enemy[i].hitCnt = 0;							
	
			//�ړ��p�����[�^
			g_Enemy[i].onGround = FALSE;					//���n���Ă���HNO
			g_Enemy[i].gravityCnt = 0.0f;					//�d��

			g_Enemy[i].dir = CHAR_DIR_RIGHT;				//�i�ޕ��������߂�
			g_Enemy[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);	//�ړ���
			g_Enemy[i].movedis = 0.0f;						//��~��Ԃ���ǂꂭ�炢�ړ������H
			g_Enemy[i].needmovedis = 0.0f;					//�G�l�~�[���ړ��K�v�ȑS����

			g_Enemy[i].knockmoveX = g_Enemy[i].move.x;		//�m�b�N�o�b�N���ꂽ��ړ�����

			g_Enemy[i].stop = TRUE;							//��~��ԂɂȂ��Ă�HYES
			g_Enemy[i].stopframe = 60.0f;					//��~��ԑ����Ă��鎞��

			g_EnemyCnt++;
		}

		for (int i = 0; i < ENEMY_TUTORIAL_MAX; i++)
		{	//�`���[�g���A���̓G�������߂�
			g_Enemy[i].use = TRUE;
			g_Enemy[i].pos = g_Tutorial_EPos[i];
			g_Enemy[i].str = 10;
		}

		g_EnemyCnt = ENEMY_TUTORIAL_MAX;
		break;

	case MODE_GAME:

		g_EnemyMax = ENEMY_MAX;

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

		for (int i = 0; i < TEXTURE_SLIME_MAX; i++)
		{
			g_Texture_Enemy[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_TextureName_Enemy[i],
				NULL,
				NULL,
				&g_Texture_Enemy[i],
				NULL);
		}

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			g_Enemy[i].use = TRUE;
			g_Enemy[i].type = ENEMY_TYPE_SLIME;
			g_Enemy[i].state = BOSS_STATE_IDLE;
			g_Enemy[i].pos = XMFLOAT3(200.0f + i * 400.0f, 500.0f, 0.0f);	// ���S�_����\��
			g_Enemy[i].opos = g_Enemy[i].pos;							//�O�̍��W��ۑ�����
			g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_Enemy[i].w = TEXTURE_SLIME_WIDTH;
			g_Enemy[i].h = TEXTURE_SLIME_HEIGHT;
			g_Enemy[i].texNo = 0;
			g_Enemy[i].countAnim = 0;
			g_Enemy[i].patternAnim = 0;

			//�����������̃q�b�g
			g_Enemy[i].clear = FALSE;
			g_Enemy[i].hit = FALSE;
			g_Enemy[i].hitting = FALSE;
			g_Enemy[i].hitCnt = 0;

			g_Enemy[i].attack = FALSE;			//�U���t���O
			g_Enemy[i].attacktime = 0;		//�U�����ԃJ�E���^�[

			g_Enemy[i].skill01 = FALSE;
			g_Enemy[i].skill01Cnt = 0;
			g_Enemy[i].skill01delaytime = 0;

			g_Enemy[i].skill02 = FALSE;

			g_Enemy[i].hp = SLIME_HPMAX;
			g_Enemy[i].damagedType = -1;


			g_Enemy[i].dir = CHAR_DIR_RIGHT;
			g_Enemy[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);		// �ړ���
			g_Enemy[i].onGround = FALSE;
			g_Enemy[i].gravityCnt = 0.0f;


			g_Enemy[i].movedis = 0.0f;
			g_Enemy[i].needmovedis = 0.0f;
			g_Enemy[i].knockmoveX = g_Enemy[i].move.x;

			g_Enemy[i].stop = TRUE;
			g_Enemy[i].stopframe = 60.0f;

			g_EnemyCnt++;
		}
		//���ʂ̃G�l�~�[�̏���������
		g_Enemy[0].pos = XMFLOAT3(2800.0f, 500.0f, 0.0f);
		g_Enemy[1].pos = XMFLOAT3(3100.0f, 500.0f, 0.0f);
		g_Enemy[2].pos = XMFLOAT3(3600.0f, 1200.0f, 0.0f);
		g_Enemy[3].pos = XMFLOAT3(2540.0f, 1600.0f, 0.0f);
		g_Enemy[4].pos = XMFLOAT3(2000.0f, 1300.0f, 0.0f);
		g_Enemy[5].pos = XMFLOAT3(1800.0f, 1450.0f, 0.0f);
		g_Enemy[6].pos = XMFLOAT3(2200.0f, 1920.0f, 0.0f);
		g_Enemy[7].pos = XMFLOAT3(2000.0f, 1920.0f, 0.0f);
		g_Enemy[8].pos = XMFLOAT3(660.0f, 1800.0f, 0.0f);
		g_Enemy[9].pos = XMFLOAT3(400.0f,1800.0f, 0.0f);
		g_Enemy[10].pos = XMFLOAT3(550.0f, 2600.0f, 0.0f);
		g_Enemy[11].pos = XMFLOAT3(1100.0f, 2600.0f, 0.0f);
		g_Enemy[12].pos = XMFLOAT3(1780.0f, 2800.0f, 0.0f);
		g_Enemy[13].pos = XMFLOAT3(550.0f, 2000.0f, 0.0f);
		g_Enemy[14].pos = XMFLOAT3(550.0f, 2000.0f, 0.0f);
		g_EnemyCnt = ENEMY_MAX;

		break;

	case MODE_BOSS:

		//�{�X�̏���������

		g_EnemyMax = BOSS_MAX;

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

		for (int i = 0; i < BOSS_STATE_MAX; i++)
		{
			g_Texture_Boss[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_TextureName_Boss[i],
				NULL,
				NULL,
				&g_Texture_Boss[i],
				NULL);
		}

		InitBoss();
		InitSkill002();
		InitSkill2Pos();
		InitSkill003();
		InitSkill004();
		g_EnemyCnt = BOSS_MAX;

		break;

	default:
		break;
	}
	
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);




	//// 0�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//// 1�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	//// 2�Ԃ������`��Ԃœ������Ă݂�
	//g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	//g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	//g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
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

	switch (GetMode())
	{
	case MODE_TUTORIAL:
		for (int i = 0; i < TEXTURE_SLIME_MAX; i++)
		{
			if (g_Texture_Enemy[i])
			{
				g_Texture_Enemy[i]->Release();
				g_Texture_Enemy[i] = NULL;
			}
		}
		break;
	case MODE_GAME:

		for (int i = 0; i < TEXTURE_SLIME_MAX; i++)
		{
			if (g_Texture_Enemy[i])
			{
				g_Texture_Enemy[i]->Release();
				g_Texture_Enemy[i] = NULL;
			}
		}
		break;

	case MODE_BOSS:

		for (int i = 0; i < BOSS_STATE_MAX; i++)
		{
			if (g_Texture_Boss[i])
			{
				g_Texture_Boss[i]->Release();
				g_Texture_Boss[i] = NULL;
			}
		}
		break;
	}
	

	
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	g_EnemyCnt = 0;	// �����Ă�G�l�~�[�̐�
	
	switch (GetMode())
	{
	case MODE_TUTORIAL:

		UpdateSlime();
		break;

	case MODE_GAME:

		UpdateSlime();
		break;
	case MODE_BOSS:

		UpdateBoss();
		break;
	}


#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
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

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		for (int i = 0; i < ENEMY_TUTORIAL_MAX; i++)
		{
			if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
			{									// Yes
				// �e�N�X�`���ݒ�
					
				float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX;
				float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY;
				float pw = TEXTURE_WIDTH;		// �G�l�~�[�̕\����
				float ph = TEXTURE_HEIGHT;		// �G�l�~�[�̕\������;

				int animeMaxX = g_slime_state_AnimeMax[g_Enemy[i].state][2];
				int animeMaxY = g_slime_state_AnimeMax[g_Enemy[i].state][1];

				// �A�j��
				float tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
				float th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
				float tx = (float)(g_Enemy[i].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
				float ty = (float)(g_Enemy[i].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

				float alpha = 1.0f;
				float R = 1.0f;
				float G = 1.0f;
				float B = 1.0f;

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Enemy[i].state]);

				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f

				// �A�j���[�V�����p
				if (g_Enemy[i].dir == CHAR_DIR_RIGHT && g_Enemy[i].state != BOSS_STATE_IDLE)
				{
					tw *= (-1);
				}

				if (g_Enemy[i].clear == TRUE)
				{
					alpha = 0.7f;
					R = 0.8f;
					G = 0.3f,
					B = 0.5f;
				}

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, alpha),
					g_Enemy[i].rot.z);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);


				DrawEnemyHP(i);

			}

			
		}

		break;
	case MODE_GAME:

		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
			{									// Yes
				// �e�N�X�`���ݒ�
				float px = g_Enemy[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX;
				float py = g_Enemy[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY;
				float pw;
				float ph;

				// �A�j��
				float tw;
				float th;
				float tx;
				float ty;

				int animeMaxX;
				int animeMaxY;

				float alpha = 1.0f;
				float R = 1.0f;
				float G = 1.0f;
				float B = 1.0f;

				switch (g_Enemy[i].type)
				{
				case ENEMY_TYPE_SLIME:

					pw = TEXTURE_WIDTH;		// �G�l�~�[�̕\����
					ph = TEXTURE_HEIGHT;		// �G�l�~�[�̕\������;
					animeMaxX = g_slime_state_AnimeMax[g_Enemy[i].state][2];
					animeMaxY = g_slime_state_AnimeMax[g_Enemy[i].state][1];
					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Enemy[g_Enemy[i].state]);
					//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f

					// �A�j���[�V�����p
					tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
					th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
					tx = (float)(g_Enemy[i].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
					ty = (float)(g_Enemy[i].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

					if (g_Enemy[i].dir == CHAR_DIR_RIGHT && g_Enemy[i].state != BOSS_STATE_IDLE) tw *= (-1);
					if (g_Enemy[i].clear == TRUE)
					{
						alpha = 0.8f;
						R = 0.5f;
						G = 0.0f,
						B = 0.0f;
					}

					SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
						XMFLOAT4(R, G, B, alpha),
						g_Enemy[i].rot.z);

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);

					break;
				}

				DrawEnemyHP(i);
			}
		}

		break;


	case MODE_BOSS:

		DrawBoss();
		
		DrawSkill003();
		DrawSkill004();
		break;




	default:
		break;
	}
}


//=============================================================================
// Enemy�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}

void UpdateSlime(void)
{
	for (int i = 0; i < g_EnemyMax; i++)
	{
		if (g_Enemy[i].type == ENEMY_TYPE_SLIME)
		{
			if (g_Enemy[i].hp <= 0 && g_Enemy[i].use == TRUE && GetMode()!= MODE_BOSS)
			{
				PlaySound(SOUND_LABEL_SE_slimedead);
				g_Enemy[i].use = FALSE;
				int temp = rand() % 5;
				if (temp == 0)
				{
					PlaySound(SOUND_LABEL_SE_itemdrop);
					SetDropItem(g_Enemy[i].pos);

				}

			}
			// �����Ă�G�l�~�[��������������
			if (g_Enemy[i].use == TRUE)
			{
				PLAYER* player = GetPlayer();
				// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
				g_Enemy[i].opos = g_Enemy[i].pos;
				XMFLOAT3 nextpos = g_Enemy[i].pos;

				//�A�j���[�V��������
				AnimetionProcess(i);

				switch (GetMode())
				{
				case MODE_TUTORIAL:

					MoveProcess(i);
					break;

				case MODE_GAME:

					BOOL ans = CollisionBB(player[0].pos, player[0].w, player[0].h, g_Enemy[i].pos, SCREEN_WIDTH, g_Enemy[i].h + 200.0f);
					if(ans)
					{
						//�ړ�����
						MoveProcessP(i);

						 ans = CollisionBB(player[0].pos, player[0].w, player[0].h, g_Enemy[i].pos, g_Enemy[i].w + 400.0f, g_Enemy[i].h);
						if (ans)
						{
							//�ړ�����

							if (CollisionBB(g_Enemy[i].pos, g_Enemy[i].w * 2, g_Enemy[i].h, player[0].pos, player[0].w, player[0].h))
							{
								int temp = rand() % 101;
								if (temp % 100 == 0)
								{
									g_Enemy[i].attack = TRUE;

								}
							}
							if (g_Enemy[i].attack == TRUE)
							{
								g_Enemy[i].state = SLIME_STATE_ATTACK001;
								AttackProcess(i);
							}
							else
							{
								g_Enemy[i].stop = TRUE;
							}
							//�v���C���[�̈ʒu�ɋ߂��Ƃ��U�����s��

						}
					}
				
				break;

				}


				//�d�͏���
				GravityProcess(i);

				if (g_Enemy[i].hit)
				{
					EHitbackProcess(i);

				}
				//�q�b�g�o�b�N����

				// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
				{


					// �G�l�~�[�̐��������蔻����s��
					for (int j = 0; j < PLAYER_MAX; j++)
					{
						// �����Ă�G�l�~�[�Ɠ����蔻�������
						if (player[j].use == TRUE)
						{
							BOOL ans = CollisionBB(g_Enemy[i].pos, g_Enemy[i].w, g_Enemy[i].h,
								player[j].pos, player[j].w, player[j].h);
							// �������Ă���H
							if (ans == TRUE)
							{
								// �����������̏���
								//player[j].hp -= 10;
							//	player[j].use = FALSE;	// �f�o�b�O�ňꎞ�I�ɖ��G�ɂ��Ă�����
							}
						}
					}
				}
				g_EnemyCnt++;		// �����Ă��G�̐�
			}
		}
	}
}

void InitBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		g_Boss[i].use = TRUE;
		g_Boss[i].display = TRUE;
		g_Boss[i].state = BOSS_STATE_IDLE;
		g_Boss[i].ostate = BOSS_STATE_IDLE;
		g_Boss[i].opos = g_Boss[i].pos;							//�O�̍��W��ۑ�����
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Boss[i].countAnim = 0;
		g_Boss[i].patternAnim = 0;

		g_Boss[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		g_Boss[i].type = ENEMY_TYPE_BOSS;
		g_Boss[i].pos = XMFLOAT3(1600.0f, 500.0f, 0.0f);	// ���S�_����\��
		g_Boss[i].w = TEXTURE_BOSS_WIDTH;
		g_Boss[i].h = TEXTURE_BOSS_HEIGHT;
		g_Boss[i].texNo = 0;
		g_Boss[i].move = XMFLOAT3(3.0f, 0.0f, 0.0f);
		g_Boss[i].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Boss[i].dir = CHAR_DIR_LEFT;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g

		g_Boss[i].damagedType = DAMAGED_BLADE;
		g_Boss[i].hp = 900;

		//�����������̃q�b�g
		g_Boss[i].clear = FALSE;
		g_Boss[i].hit = FALSE;
		g_Boss[i].hitting = FALSE;
		g_Boss[i].hitCnt = 0;

		g_Boss[i].attack = FALSE;			//�U���t���O
		g_Boss[i].attacktime = 0;		//�U�����ԃJ�E���^�[

		g_Boss[i].skill01 = FALSE;
		g_Boss[i].skill01Cnt = 0;
		g_Boss[i].skill01delaytime = 0;

		g_Boss[i].skill02 = FALSE;

		g_Boss[i].onGround = FALSE;
		g_Boss[i].gravityCnt = 0.0f;


		g_Boss[i].movedis = 0.0f;
		g_Boss[i].needmovedis = 0.0f;
		g_Boss[i].knockmoveX = g_Boss[i].move.x;

		g_Boss[i].stop = TRUE;
		g_Boss[i].stopframe = 60.0f;

		//g_Enemy[i].knockmoveY = g_Enemy[i].move;

		g_Boss[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA2);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
#ifdef _DEBUG
//g_Boss[i].hp = 1;
#endif // !_DEBUG
	}
}



void UpdateBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].hp <= 0)
		{
			g_Boss[i].use = FALSE;
			g_DeadBoss = TRUE;
			g_Deadtimer -= 0.025;
			
			if (g_Deadtimer <= 0.3f)
			{
				g_DeadCnt++;
				g_Boss[i].display = FALSE;
				if (g_DeadCnt == 1)
				{
					PlaySound(SOUND_LABEL_SE_bossbomb1);
					SetEffect3(g_Boss[i].pos, BOSS_DEADEFFECT);
				}

			}

		}
		if (g_Boss[i].use == FALSE)continue;
					
	// �����Ă�G�l�~�[��������������
	
		//�A�j���[�V��������
		if (g_Boss[i].countAnim++ > g_boss_state_AnimeMax[g_Boss[i].state][3])
		{
			g_Boss[i].countAnim = 0.0f;
			// �p�^�[���̐؂�ւ�
	
			g_Boss[i].patternAnim = (g_Boss[i].patternAnim + 1) % g_boss_state_AnimeMax[g_Boss[i].state][0];
		}

		PLAYER* player = GetPlayer();
		// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���

		g_Boss[i].opos = g_Boss[i].pos;
		XMFLOAT3 nextpos = g_Boss[i].pos;
		g_Boss[i].ostate = g_Boss[i].state;

		switch (GetMode())
		{
		case MODE_BOSS:

		{//�ړ��̃p�^�[�����f

			if (g_Boss[i].hit)
			{
				//�q�b�g�o�b�N����
				if (g_Boss[i].damagedType == DAMAGED_ULT)
				{
					g_changeable = FALSE;
					g_Boss[i].state = BOSS_STATE_HIT;
				}
				else 
					BossHit(i);
			}

			int temp = rand() % 100;

			if (g_changeable)
			{
				g_Idletimer += GetDeltatime();

				float idleMax = 2.0f;
				if (g_Idletimer <= idleMax * 1000.0f)
				{
					g_Boss[i].state = BOSS_STATE_IDLE;
				}
				else
				{
					g_Idletimer = 0.0f;
					if (temp >= 0 && temp < 10)
					{//�����ŃG�l�~�[���U�����邩�ǂ��������߂�@YES�U�������߂�
						SetSkill000(i);
						g_Boss[i].state = BOSS_STATE_WALK;
					}
					else if (temp >= 10 && temp < 25)
					{
						g_Boss[i].state = BOSS_STATE_SKILL001;
						g_HpbulletNum = rand() % SKILL1_BULLET_MAX;

					}
					else if (temp >= 25 && temp < 40)
					{//�X�L��1
						g_Boss[i].state = BOSS_STATE_SKILL002;
					}
					else if (temp >= 40 && temp < 55)
					{
						g_Boss[i].state = BOSS_STATE_SKILL003;
					}
					else if (temp >= 55 && temp < 70)
					{
						g_Boss[i].state = BOSS_STATE_SKILL004;
						//g_Boss[i].state = BOSS_STATE_WALK;
					}
					else if (temp >= 70 && temp < 85)
					{
						SetSkill005(i);
						g_Boss[i].state = BOSS_STATE_SKILL005;
						//g_Boss[i].state = BOSS_STATE_WALK;
					}
					else
					{
						g_Boss[i].state = BOSS_STATE_DOWN;
					}

				}
			}

			switch (g_Boss[i].state)
			{
			case BOSS_STATE_WALK:

				UpdateBMove(i);
				break;

			case BOSS_STATE_HIT:
				BossHitback(i);
				break;

			case BOSS_STATE_SKILL001:

				Eskill1(i);
				break;

			case BOSS_STATE_SKILL002:
				Skill002Timer();
				break;

			case BOSS_STATE_SKILL003:

				Skill003Timer();
				break;
			case BOSS_STATE_SKILL004:
				Skill004Timer();

				break;
			case BOSS_STATE_SKILL005:

				Skill005(i);
				break;
			case BOSS_STATE_DOWN:
				Down();
				break;

			}
				
				

		}
		break;

		}

		if (g_Boss[i].hit == TRUE)
		{
			InitSkill001();
			InitSkill002();
			InitSkill003();
			InitSkill004();
		}
		if (g_Boss[i].state != BOSS_STATE_WALK && g_Boss[i].state != BOSS_STATE_SKILL005)
		{
			g_Boss[i].dir = CHAR_DIR_LEFT;
		}
		if (g_Boss[i].state != g_Boss[i].ostate)
		{
			g_Boss[i].countAnim = 0.0f;
			g_Boss[i].patternAnim = 0;

		}

		//�d�͏���
		BGravityProcess(i);

		// �ړ����I�������G�l�~�[�Ƃ̓����蔻��
		{
			// �G�l�~�[�̐��������蔻����s��
			for (int j = 0; j < PLAYER_MAX; j++)
			{
				// �����Ă�G�l�~�[�Ɠ����蔻�������
				if (player[j].use == TRUE)
				{
					BOOL ans = CollisionBB(g_Boss[i].pos, g_Boss[i].w, g_Boss[i].h,
						player[j].pos, player[j].w, player[j].h);
					// �������Ă���H
					if (ans == TRUE)
					{
						// �����������̏���
						//player[j].hp -= 10;
					//	player[j].use = FALSE;	// �f�o�b�O�ňꎞ�I�ɖ��G�ɂ��Ă�����
					}
				}
			}
		}
	}

	UpdateSkill002();
	UpdateSkill2Pos();
	UpdateSkill003();
	UpdateSkill004();
}

void DrawBoss(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].display == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			float px = g_Boss[i].pos.x;	// �G�l�~�[�̕\���ʒuX;
			float py = g_Boss[i].pos.y;	// �G�l�~�[�̕\���ʒuY;
			float pw;
			float ph;

			// �A�j��
			float tw;
			float th;
			float tx;
			float ty;

			int animeMaxX;
			int animeMaxY;

			float alpha = 1.0f;
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			switch (g_Boss[i].type)
			{

			case ENEMY_TYPE_BOSS:

				pw = g_Boss[i].w * g_Deadtimer;		// �G�l�~�[�̕\����
				ph = g_Boss[i].h * g_Deadtimer;		// �G�l�~�[�̕\������;

				animeMaxX = g_boss_state_AnimeMax[g_Boss[i].state][2];
				animeMaxY = g_boss_state_AnimeMax[g_Boss[i].state][1];
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture_Boss[g_Boss[i].state]);

				if (g_Boss[i].state == BOSS_STATE_SKILL001 || g_Boss[i].state == BOSS_STATE_SKILL002
					|| g_Boss[i].state == BOSS_STATE_SKILL003 || g_Boss[i].state == BOSS_STATE_SKILL004)
				{
					pw = 720.0f;
				}

				tw = 1.0f / animeMaxX;	// �e�N�X�`���̕�
				th = 1.0f / animeMaxY;	// �e�N�X�`���̍���
				tx = (float)(g_Boss[i].patternAnim % animeMaxX) * tw;	// �e�N�X�`���̍���X���W
				ty = (float)(g_Boss[i].patternAnim / animeMaxX) * th;	// �e�N�X�`���̍���Y���W

				if (g_Boss[i].dir == CHAR_DIR_RIGHT && g_Boss[i].state != BOSS_STATE_IDLE)tw *= (-1);
				if (g_Boss[i].state != BOSS_STATE_HIT &&
					g_Boss[i].state != BOSS_STATE_IDLE&&
					g_Boss[i].state != BOSS_STATE_DOWN) {
					alpha = 0.5f;
	
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					XMFLOAT4(R, G, B, alpha));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
				break;
			}
			DrawBossHP(i);
		}
	}
}


// �A�j���[�V����  
void AnimetionProcess(int num)
{
	g_Enemy[num].countAnim += 1.0f;
	switch (g_Enemy[num].type)
	{
	case ENEMY_TYPE_SLIME:

		if (g_Enemy[num].countAnim > g_slime_state_AnimeMax[g_Enemy[num].state][3])
		{
			g_Enemy[num].countAnim = 0.0f;
			// �p�^�[���̐؂�ւ�
			g_Enemy[num].patternAnim = (g_Enemy[num].patternAnim + 1) % g_slime_state_AnimeMax[g_Enemy[num].state][0];
		}
		break;

	case ENEMY_TYPE_BOSS:

		if (g_Enemy[num].countAnim > g_boss_state_AnimeMax[g_Enemy[num].state][3])
		{
			g_Enemy[num].countAnim = 0.0f;
			// �p�^�[���̐؂�ւ�
			g_Enemy[num].patternAnim = (g_Enemy[num].patternAnim + 1) % g_boss_state_AnimeMax[g_Enemy[num].state][0];
		}

		break;

	}
}

//�ړ�����
void MoveProcess(int num)
{

	if (g_Enemy[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Enemy[num].pos;

		if ((g_Enemy[num].stop))//�ړ����Ă��Ȃ��Ȃ�
		{

			float movedis = (float)(rand() % 150);		//�G�l�~�[���ړ����ׂ�����
			float dir = (float)(rand() % 2);		//�G�l�~�[�̌���

			g_Enemy[num].state = BOSS_STATE_IDLE;		//�G�l�~�[��������
			g_Enemy[num].needmovedis = movedis;		//�ړ��K�v�ȋ�����������
			g_Enemy[num].movedis = 0.0f;     //�ړ��v�Z
			g_Enemy[num].walktimer = 0.0f;     //�ړ��v�Z

			if (dir)						//�ړ��̕��������߂�
			{
				g_Enemy[num].dir = CHAR_DIR_LEFT;
			}
			else g_Enemy[num].dir = CHAR_DIR_RIGHT;

			
			g_timer += GetDeltatime();
			float waitMax = 0.1f;//second
			if (g_timer >= waitMax * 1000.0f)//to mili
			{
				
				g_Enemy[num].stop = FALSE;
				
				g_timer = 0.0f;
			}

		}

		else
		{//�ړ����Ă���

			if (g_Enemy[num].movedis >= g_Enemy[num].needmovedis)
			{
				g_Enemy[num].stop = TRUE;
				g_Enemy[num].state = BOSS_STATE_IDLE;
			}
			else
			{
				g_Enemy[num].state = BOSS_STATE_WALK;
				if (g_Enemy[num].dir == CHAR_DIR_LEFT)
				{
					nextpos.x -= g_Enemy[num].move.x;
				}
				else if (g_Enemy[num].dir == CHAR_DIR_RIGHT)
				{
					nextpos.x += g_Enemy[num].move.x;
				}


				EnemyMoveSound(num);
			



				switch (g_Enemy[num].type)
				{
				case ENEMY_TYPE_SLIME:

					if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
					{
						g_Enemy[num].pos = nextpos;
						g_Enemy[num].movedis += g_Enemy[num].move.x;
					}
					else
					{

						g_Enemy[num].dir = !(g_Enemy[num].dir);
					}
					break;

				case ENEMY_TYPE_BOSS:

					if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
					{
						g_Enemy[num].pos = nextpos;
						g_Enemy[num].movedis += g_Enemy[num].move.x;
					}
					else
					{
						g_Enemy[num].dir = !(g_Enemy[num].dir);
					}
					break;

				default:
					break;
				}




				//MAP�̉E�[�܂ŗ����H
				if (g_Enemy[num].pos.x > MAP_W)
				{
					g_Enemy[num].pos.x = MAP_W;
					g_Enemy[num].dir = CHAR_DIR_LEFT;
				}


				//MAP�̍��[�܂ŗ����H
				if (g_Enemy[num].pos.x < 0.0f)
				{
					g_Enemy[num].pos.x = 0.0f;
					g_Enemy[num].dir = CHAR_DIR_RIGHT;

				}
			}

			

		}
		
	}

	return;
}


void EnemyMoveSound(int num)

{
	g_Enemy[num].walktimer += GetDeltatime();

	float waitMax = 0.6f;
	if (g_Enemy[num].walktimer >= waitMax * 1000.0f)
	{
		g_Enemy[num].walktimer = 0;
		PlaySound(SOUND_LABEL_SE_slimemove);

		PLAYER* player = GetPlayer();
		float disx = (float)fabs(player[0].pos.x - g_Enemy[num].pos.x);
		float disy = (float)fabs(player[0].pos.y - g_Enemy[num].pos.y);
		float soundDis = disx + disy;
		if (soundDis == 0)soundDis = 1;
		float soundMax = ENEMY_SOUND_DISMAX / soundDis;
		(soundMax > 1.0) ? soundMax = 1.0f : soundMax;

		if (soundDis <= ENEMY_SOUND_DISMAX)
		{
			SetVolumeSound(SOUND_LABEL_SE_slimemove, soundMax);
		}
		else
		{
			SetVolumeSound(SOUND_LABEL_SE_slimemove, 0.0f);
		}


	}
}

//�v���C���[�ɍU�����ꂽ��̈ړ��p�^�[��
void MoveProcessP(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Enemy[num].use == TRUE)
	{

		{
			//��ԏ�����
			g_Enemy[num].state = BOSS_STATE_WALK;
			//�G�l�~�[���ړ������̓v���C���[�̂ǂ���
			g_Enemy[num].nextpos = player[0].pos;

			XMFLOAT3 nextpos = g_Enemy[num].pos;
			{//�ړ����Ă���

				{//�v���C���[�̈ʒu�̓G�l�~�[�̉E�������𔻒f����
					if (g_Enemy[num].pos.x - player[0].pos.x >= 0)g_Enemy[num].dir = CHAR_DIR_LEFT;
					else if (g_Enemy[num].pos.x - player[0].pos.x < 0)g_Enemy[num].dir = CHAR_DIR_RIGHT;
				}


				if (g_Enemy[num].dir == CHAR_DIR_LEFT)
				{
					if (!CollisionBB(g_Enemy[num].pos, g_Enemy[num].w, g_Enemy[num].h, player[0].pos, player[0].w, player[0].h))
					{
						nextpos.x -= g_Enemy[num].move.x;

					}
				}
				else if (g_Enemy[num].dir == CHAR_DIR_RIGHT)
				{
					if (!CollisionBB(g_Enemy[num].pos, g_Enemy[num].w, g_Enemy[num].h, player[0].pos, player[0].w, player[0].h))
					{
						nextpos.x += g_Enemy[num].move.x;

					}
				}

				EnemyMoveSound(num);

				if (!FieldCollision(nextpos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
				{
					g_Enemy[num].pos = nextpos;
					g_Enemy[num].movedis += g_Enemy[num].move.x;
				}
				else
				{

					g_Enemy[num].dir = !(g_Enemy[num].dir);
				}




				//MAP�̉E�[�܂ŗ����H
				if (g_Enemy[num].pos.x > MAP_W)
				{
					g_Enemy[num].pos.x = MAP_W;
					g_Enemy[num].dir = CHAR_DIR_LEFT;
				}


				//MAP�̍��[�܂ŗ����H
				if (g_Enemy[num].pos.x < 0.0f)
				{
					g_Enemy[num].pos.x = 0.0f;
					g_Enemy[num].dir = CHAR_DIR_RIGHT;

				}
			}
		}
		
	}

	return;
}


//�v���C���[�ւ̍U���p�^�[��
void AttackProcess(int num)
{
	PLAYER* player = GetPlayer();
	BOOL ans;
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (g_Enemy[num].use == TRUE && g_Enemy[num].attack == TRUE)
		{
			//��ԏ�����
			g_Enemy[num].state = SLIME_STATE_ATTACK001;

			if (g_Enemy[num].attacktime++ >= ATTACK_TIME_MAX)
			{//�U���I���m�F
				g_Enemy[num].attacktime = 0;
				g_Enemy[num].attack = FALSE;
				g_Enemy[num].state = BOSS_STATE_WALK;
			}
			if (g_Enemy[num].attacktime == 20)
			{
				PlaySound(SOUND_LABEL_SE_slimeattack);
				switch (g_Enemy[num].type)
				{
				case ENEMY_TYPE_SLIME:

					if (player[j].state == PLAYER_STATE_GUARD && (player[j].dir != g_Enemy[num].dir))
					{
						EFFECT3* effect = GetEffect3();
						for (int i = 0; i < EFFECT_MAX; i++)
						{
							if (effect[i].type == GUARD && g_Skill003[i].use == TRUE && 
								(CollisionBB(effect[i].pos, effect[i].w / 2, effect[i].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
							{
								PlaySound(SOUND_LABEL_SE_guard);
							}
						}

					}
					else if (CollisionBB(g_Enemy[num].pos, TEXTURE_SLIME_WAIST_WIDTH * 2, TEXTURE_SLIME_WAIST_HEIGHT, player[0].pos, player[0].w, player[0].h))
					{
						int str = rand() % 10;
						//SetPlayerHP(20);

						player[j].hit = TRUE;
						player[j].hp -= str;
						SetEffect3(player[j].pos,HIT2);
						SetHitScore(-str,player[j].pos, DAMAGE_PLAYER);

#ifndef _DEBUG	// �����[�X�Ԃ���HP������
						
#endif
					}
					break;

				case ENEMY_TYPE_BOSS:


					break;
		}


	}
}

	}
	
	return;
}

//�d�͏���
void GravityProcess(int num)
{

	if (g_Enemy[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Enemy[num].pos;
		{//�d�͏���
							//�����̔���
			nextpos = g_Enemy[num].pos;
			XMFLOAT3 pfootpos;
			BOOL ans;
			switch (g_Enemy[num].type)
			{
			case ENEMY_TYPE_SLIME:

				pfootpos = XMFLOAT3(g_Enemy[num].pos.x, g_Enemy[num].pos.y + g_Enemy[num].h / 2 + 8, 0.0f);//�����̍��W
				ans = FieldCollision(pfootpos, TEXTURE_SLIME_FOOT_WIDTH, TEXTURE_SLIME_FOOT_HEIGHT);

				break;

			case ENEMY_TYPE_BOSS:

				pfootpos = XMFLOAT3(g_Enemy[num].pos.x, g_Enemy[num].pos.y + g_Enemy[num].h / 2 - 10, 0.0f);//�����̍��W
				ans = FieldCollision(pfootpos, TEXTURE_BOSS_FOOT_WIDTH, TEXTURE_BOSS_FOOT_HEIGHT);

				break;

			default:
				break;
			}

			if (ans)//���ꂪ����
			{
				g_Enemy[num].onGround = TRUE;
				g_Enemy[num].gravityCnt = 0;
			}
			else
			{
				g_Enemy[num].onGround = FALSE;
				g_Enemy[num].gravityCnt += GRAVITY;
				float acc = g_Enemy[num].gravityCnt * 5;
				float totalmove = g_Enemy[num].gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Enemy[num].w / 2, g_Enemy[num].h / 16);

			if (!ans)
			{

				g_Enemy[num].pos = nextpos;
			}
		}
	}
}

void EHitbackProcess(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Enemy[num].use == FALSE)return;
	if (g_Enemy[num].hit == FALSE)return;
	
	int damage = 0;

	switch(g_Enemy[num].damagedType){//�����ɓ��Ă�ꂽ����
	case DAMAGED_BLADE:

		if (g_Enemy[num].hitCnt > 60)
		{	//���ԃI�[�o�[
			g_Enemy[num].hit = FALSE;
			g_Enemy[num].hitting = FALSE;
			g_Enemy[num].clear = FALSE;
			g_Enemy[num].hitCnt = 0;
		}

		if (g_Enemy[num].hit == TRUE && g_Enemy[num].hitting == FALSE)
		{//���Ă�ꂽ
			//g_Enemy[num].hp -= player[0].str;
			g_Enemy[num].hitting = TRUE;
			g_Enemy[num].hitCnt = 0;
			g_time = 0;
			g_knockmoveCnt = 0;
			g_Enemy[num].knockmoveX = 20.0f;

			int str = player[0].str + rand() % (int)(player[0].str / 2);

			g_Enemy[num].hp -= player[0].str;
			SetHitScore(-str, g_Enemy[num].pos, DAMAGE_ENEMY);

			PlaySound(SOUND_LABEL_SE_slimehit);

		}

		if (g_Enemy[num].hitting == TRUE && g_Enemy[num].hit == TRUE)
		{//���Ă�ꂽ�r���H
			{	//�܂����ԓ�

				if (g_Enemy[num].hitCnt % HIT_CLEAR_MAX == 0)
				{
					g_Enemy[num].clear = !(g_Enemy[num].clear);
				}


				XMFLOAT3 tempmovex;	//�G�l�~�[�����t���C���ړ����鋗��
				//if (player[i].dir == CHAR_DIR_RIGHT)
				//{
				//	tempmovex = XMFLOAT3(g_Enemy[num].knockmoveX, 0.0f, 0.0f);
				//}
				//else
				//{
				//	tempmovex = XMFLOAT3((-1) * g_Enemy[num].knockmoveX, 0.0f, 0.0f);
				//}
				if (g_Enemy[num].pos.x - player[0].pos.x >= 0)
				{
					tempmovex = XMFLOAT3(g_Enemy[num].knockmoveX, 0.0f, 0.0f);
				}
				else
				{
					tempmovex = XMFLOAT3((-1) * g_Enemy[num].knockmoveX, 0.0f, 0.0f);
				}

				XMVECTOR vtempmove = XMLoadFloat3(&tempmovex);
				XMVECTOR vepos = XMLoadFloat3(&g_Enemy[num].pos);
				XMFLOAT3 epos = g_Enemy[num].pos;
				XMStoreFloat3(&epos, vepos + vtempmove);//�ړ�������������
				if (!FieldCollision(epos, TEXTURE_SLIME_WAIST_WIDTH, TEXTURE_SLIME_WAIST_HEIGHT))
				{
					g_Enemy[num].pos = epos;

				}


				g_Enemy[num].knockmoveX = g_Enemy[num].knockmoveX - (g_knockmoveCnt);
				g_knockmoveCnt += 1.5f;
				if (g_Enemy[num].knockmoveX <= 0.0f) g_Enemy[num].knockmoveX = 0.0f;
				g_Enemy[num].hitCnt++;
			}

		}
		break;

	case DAMAGED_BULLET:

		{//���Ă�ꂽ
			//g_Enemy[num].hp -= player[0].str;

			g_Enemy[num].hp = 0;
			
			g_Enemy[num].hit = FALSE;
			g_Enemy[num].hitting = FALSE;
			g_Enemy[num].hitCnt = 0;
			g_time = 0;
			g_knockmoveCnt = 0;
			g_Enemy[num].knockmoveX = 0;

			PlaySound(SOUND_LABEL_SE_warp);
		
		}
		break;
	case DAMAGED_BULLET2:

		damage = rand() % 10 + (int)player[0].str * 0.3;

		g_Enemy[num].hp -= damage;
		g_Enemy[num].hit = FALSE;
		g_Enemy[num].hitting = FALSE;
		g_Enemy[num].hitCnt = 0;
		g_time = 0;
		g_knockmoveCnt = 0;
		g_Enemy[num].knockmoveX = 0;

		SetHitScore(-damage, g_Enemy[num].pos, DAMAGE_ENEMY);

		PlaySound(SOUND_LABEL_SE_eShock);

	case DAMAGED_ULT:


		PLAYER* player = GetPlayer();

		if (g_Enemy[num].use == FALSE)return;
		if (g_Enemy[num].hit == FALSE)return;

		int hitMax = 40;

		if (g_Enemy[num].hitCnt++ > (ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX))
		{	//���ԃI�[�o�[

			g_Enemy[num].hit = FALSE;
			g_Enemy[num].clear = FALSE;
			g_Enemy[num].hitCnt = 0;
			g_Enemy[num].state = SLIME_STATE_IDLE;
		}
		else
		{
			if (g_Enemy[num].hitCnt % 50 == 0)
			{
				int critical = rand() % 5;
				int str = rand() % 10 + player[0].str;
				if (critical == 0)str *= 2;

				g_Enemy[num].hp -= str;
				SetHitScore(-str, g_Enemy[num].pos, DAMAGE_ENEMY);
				SetEffect3(g_Enemy[num].pos, HIT);
			}

		}

		break;
		}
	


}


void SetEDamagedType(int num,int type)
{
	g_Enemy[num].damagedType = type;
	return;
}
//�G�l�~�[��HP�\��
void DrawEnemyHP(int num)
{
	// HP�̕\��
	BG* bg = GetBG();
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;		// �Q�[�W�̕\���ʒuX
		float py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h;		// �Q�[�W�̕\���ʒuY
		float pw = 100;	// �Q�[�W�̕\����
		float ph = 5.0f;		// �Q�[�W�̕\������

		switch (g_Enemy[num].type)
		{
		case ENEMY_TYPE_SLIME:
			px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;
			py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h;
			pw = 100;	// �Q�[�W�̕\����
			ph = 5.0f;		// �Q�[�W�̕\������
			break;
		case ENEMY_TYPE_BOSS:
			px = g_Enemy[num].pos.x - bg->pos.x - g_Enemy[num].w / 2;
			py = g_Enemy[num].pos.y - bg->pos.y - g_Enemy[num].h / 2;
			pw = 300;	// �Q�[�W�̕\����
			ph = 5.0f;		// �Q�[�W�̕\������
			break;
		default:
			break;
		}

		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;	// �e�N�X�`���̍���
		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


		// �G�l�~�[�̐��ɏ]���ăQ�[�W�̒�����\�����Ă݂�
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		switch (g_Enemy[num].type)
		{
		case ENEMY_TYPE_SLIME:
			pw = pw * ((float)g_Enemy[num].hp / SLIME_HPMAX);
			break;
		case ENEMY_TYPE_BOSS:
			pw = pw * ((float)g_Enemy[num].hp / BOSS_HPMAX);
			break;
		default:
			break;
		}


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


	}

}



void SetSkill000(int num)
{
	g_Boss[num].time = 0.0f;
	g_Boss[num].tblNo = 0;
	g_Boss[num].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA2);

}
void SetSkill005(int num)
{
	g_Boss[num].time = 0.0f;
	g_Boss[num].tblNo = 1;
	g_Boss[num].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA2);
}

void Skill005(int num)
{
	if (g_Boss[num].use == TRUE)
	{
		float waitMax = 0.2f;
		g_Walktimer += GetDeltatime();
		if (g_Walktimer >= waitMax * 1000.0f)
		{
			g_Walktimer = 0.0f;
			PlaySound(SOUND_LABEL_SE_bosswalk);
		}
		//else
		{
			g_changeable = FALSE;

			XMFLOAT3 pos_old = g_Boss[num].pos;
			// �ړ�����
			if (g_Boss[num].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���

				int nowNo = (int)g_Boss[num].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Boss[num].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA2* tbl = g_MoveTblAdr[g_Boss[num].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

				float nowTime = g_Boss[num].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				{//�����ݒ�
					XMFLOAT3 temp;
					XMStoreFloat3(&temp, Pos);
					if (temp.x <= 0)g_Boss[num].dir = CHAR_DIR_LEFT;
					else g_Boss[num].dir = CHAR_DIR_RIGHT;
				}

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���

				XMStoreFloat3(&g_Boss[num].pos, nowPos + Pos);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Boss[num].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

				if ((int)g_Boss[num].time >= maxNo || g_Boss[num].hit)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Boss[num].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
					g_changeable = TRUE;

				}
	
			}
		}
		float w = g_Boss[num].w / 4;
		if (g_Boss[num].dir == CHAR_DIR_LEFT)w *= (-1);
		XMFLOAT3 epos = XMFLOAT3(g_Boss[num].pos.x + w, g_Boss[num].pos.y, 0.0f);
		SetEffect3(epos,SKILL05,g_Boss[num].dir);

	}
}





void UpdateBMove(int num)
{
	if (g_Boss[num].use == TRUE)
	{
		float waitMax = 0.5f;
		g_Walktimer += GetDeltatime();
		if (g_Walktimer >= waitMax * 1000.0f)
		{
			g_Walktimer = 0.0f;
			PlaySound(SOUND_LABEL_SE_bosswalk);
		}

		//else
		{
			g_changeable = FALSE;

			XMFLOAT3 pos_old = g_Boss[num].pos;
			

			// �ړ�����
			if (g_Boss[num].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Boss[num].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Boss[num].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA2* tbl = g_MoveTblAdr[g_Boss[num].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

				float nowTime = g_Boss[num].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				XMFLOAT3 temp;
				XMStoreFloat3(&temp,Pos);
				if (temp.x <= 0)g_Boss[num].dir = CHAR_DIR_LEFT;
				else g_Boss[num].dir = CHAR_DIR_RIGHT;
				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���

				XMStoreFloat3(&g_Boss[num].pos, nowPos + Pos);

				// frame���g�Ď��Ԍo�ߏ���������
				g_Boss[num].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

				if ((int)g_Boss[num].time >= maxNo || g_Boss[num].hit)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Boss[num].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
					g_changeable = TRUE;

				}
				if (g_Boss[num].time <= 0)
				{
				}
			}
		}
	}	
}



//�{�X�̏d�͏���
void BGravityProcess(int num)
{

	if (g_Boss[num].use == TRUE)
	{
		XMFLOAT3 nextpos = g_Boss[num].pos;
		{//�d�͏���
							//�����̔���
			nextpos = g_Boss[num].pos;
			XMFLOAT3 pfootpos;
			BOOL ans;
			switch (g_Boss[num].type)
			{
			case ENEMY_TYPE_BOSS:

				pfootpos = XMFLOAT3(g_Boss[num].pos.x, g_Boss[num].pos.y + g_Boss[num].h / 2 - 10, 0.0f);//�����̍��W
				ans = FieldCollision(pfootpos, TEXTURE_BOSS_FOOT_WIDTH, TEXTURE_BOSS_FOOT_HEIGHT);

				break;

			default:
				break;
			}

			if (ans)//���ꂪ����
			{
				g_Boss[num].onGround = TRUE;
				g_Boss[num].gravityCnt = 0;
			}
			else
			{
				g_Boss[num].onGround = FALSE;
				g_Boss[num].gravityCnt += GRAVITY;
				float acc = g_Boss[num].gravityCnt * 5;
				float totalmove = g_Boss[num].gravityCnt * (acc * acc);
				if (totalmove >= TEXTURE_HEIGHT / 8)
				{
					totalmove = TEXTURE_HEIGHT / 8;
				}
				nextpos.y += totalmove;
			}

			ans = FieldCollision(nextpos, g_Boss[num].w / 2, g_Boss[num].h / 16);

			if (!ans)
			{

				g_Boss[num].pos = nextpos;
			}
		}
	}
}

void BossHit(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Boss[num].use == FALSE)return;
	if (g_Boss[num].hit == FALSE)return;

	if (g_Boss[num].state != BOSS_STATE_IDLE && g_Boss[num].state != BOSS_STATE_DOWN)
	{
		g_Boss[num].hit = FALSE;
		XMFLOAT3 ePos = XMFLOAT3(g_Boss[num].pos.x - g_Boss[num].w / 2 , g_Boss[num].pos.y, 0.0f);
		SetEffect3(ePos, BOSS_DEFEND);
		PlaySound(SOUND_LABEL_SE_hitshield);

		return;
	}
	switch (g_Boss[num].damagedType)
	{
	case DAMAGED_BLADE:

	{//�u���C�h�ɓ��Ă�ꂽ����
		{//���Ă�ꂽ
			int str = rand() % 10 + player[0].str / 2;

			int critical = rand() % 5;

			if (critical == 0)
			{
				str =(float)str * 1.5;
				SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
			}
			else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

			g_Boss[num].hp -= str;
			SetEffect3(g_Boss[num].pos, HIT);
			g_Boss[num].hit = FALSE;
			//g_Boss[num].state = g_Boss[num].ostate;
			//g_Boss[num].damagedType = DAMAGED_MAX;
			PlaySound(SOUND_LABEL_SE_enemyhit);
		}
	}
	break;
	case DAMAGED_BULLET:

	{//���܂ɓ��Ă�ꂽ����

		{//���Ă�ꂽ
			int str = rand() % 10 + player[0].str / 2;

			int critical = rand() % 5;

			if (critical == 0)
			{
				str = (float)str * 1.4;
				SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
			}
			else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

			g_Boss[num].hp -= str;
			SetEffect3(g_Boss[num].pos, HIT);
			g_Boss[num].hit = FALSE;
			PlaySound(SOUND_LABEL_SE_enemyhit);

		}
	}
	break;
	case DAMAGED_BULLET2:

	{//���Ă�ꂽ
		int str = rand() % 10 + player[0].str / 3;

		int critical = rand() % 5;

		if (critical == 0)
		{
			str = (float)str * 1.7;
			SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
		}
		else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

		g_Boss[num].hp -= str;
		SetEffect3(g_Boss[num].pos, HIT);
		g_Boss[num].hit = FALSE;
		PlaySound(SOUND_LABEL_SE_enemyhit);
	}
	break;

	}
}

//�q�b�g�o�b�N����
void BossHitback(int num)
{
	PLAYER* player = GetPlayer();

	if (g_Boss[num].use == FALSE)return;

	int hitMax;
	switch (g_Boss[num].damagedType)
	{

	case DAMAGED_ULT:

		hitMax = g_boss_state_AnimeMax[BOSS_STATE_HIT][0] * g_boss_state_AnimeMax[BOSS_STATE_HIT][3];
		if (g_Boss[num].hitCnt++ > (ULT_CHANT_FRAMEMAX + ULT_UNLEASH_FRAMEMAX))
		{	//���ԃI�[�o�[

			g_Boss[num].hit = FALSE;
			g_Boss[num].clear = FALSE;
			g_Boss[num].hitCnt = 0;
			g_changeable = TRUE;
			g_Boss[num].state = BOSS_STATE_IDLE;
			return;
		}
		else
		{
			if (g_Boss[num].hitCnt % 50 == 0)
			{
				int critical = rand() % 5;
				int str = rand() % 10 + player[0].str;
				if (critical == 0)
				{
					str *= 2;
					SetHitScore(-str, g_Boss[num].pos, DAMAGE_CRITICAL);
				}
				else SetHitScore(-str, g_Boss[num].pos, DAMAGE_ENEMY);

				g_Boss[num].hp -= str;
				g_Boss[num].state = BOSS_STATE_HIT;
				SetEffect3(g_Boss[num].pos, HIT);
				PlaySound(SOUND_LABEL_SE_ulthit);
			}

			g_changeable = FALSE;
		}
		break;

	}
	return;
}



//�G�l�~�[��HP�\��
void DrawBossHP(int num)
{
	// HP�̕\��
	BG* bg = GetBG();
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		int h = g_Boss[num].hp / 300;

		for (int i = 0; i < 3; i++)
		{//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			
			float px = px = g_Boss[num].pos.x - bg->pos.x;
			float py = g_Boss[num].pos.y - bg->pos.y - g_Boss[num].h / 2 + 20.0f * i;
			float pw = 300;	// �Q�[�W�̕\����
			float ph = 5.0f;		// �Q�[�W�̕\������

			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(0.0f, 0.0f, 0.0f, 0.9f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);


			// �G�l�~�[�̐��ɏ]���ăQ�[�W�̒�����\�����Ă݂�
		// �e�N�X�`���ݒ�
			if (i > h)continue;
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
			if (i < h)pw = 300;
			else
			{
				pw = h > 0 ? (g_Boss[num].hp % (h * 300)) : g_Boss[num].hp;
			}
				

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}



		

	}
}




int DisBulletDir(void)
{
	return g_bulletdir;
}

//�G�l�~�[�̃X�L���P

void InitSkill001(void)
{
	g_Boss[0].skill01delaytime = 0;
	g_Boss[0].skill01Cnt = 0;
	g_Boss[0].skill01 = FALSE;

	g_initMoveX = 5.0f;
	

	return;


}
void Eskill1(int num)
{
	PLAYER* player = GetPlayer();
	{

		g_bulletdir = BULLET_DIR_LEFT;
		//�v���C���[�̓G�l�~�[�̉E��
		if (g_Boss[num].skill01delaytime++ >= 25)
		{
			g_Boss[num].skill01delaytime = 0;
			g_initMoveX += 2.3f;
			//float x = (float)rand() / ((float)RAND_MAX / 20);
			//float y = (float)rand() / ((float)RAND_MAX / 10);
			float y = -15.0f;
			XMFLOAT3 initMove = XMFLOAT3(g_initMoveX, y, 0.0f);

			if (g_Boss[num].skill01Cnt == g_HpbulletNum)
			{
				SetBullet(g_Boss[num].pos, BULLET_MODE_HP, g_Boss[num].dir, initMove);
			}
			else
			{
				SetBullet(g_Boss[num].pos, BULLET_MODE_ENEMY, g_Boss[num].dir, initMove);

			}
			g_Boss[num].skill01Cnt++;
		}

		if (g_Boss[num].skill01Cnt > SKILL1_BULLET_MAX || g_Boss[num].hit)
		{
			g_Boss[num].skill01 = FALSE;
			g_Boss[num].skill01delaytime = 0;
			g_Boss[num].skill01Cnt = 0;
			//g_Boss[num].state = ENEMY_STATE_STAND;
			g_initMoveX = 5.0f;

			g_Boss[num].stop = TRUE;
			g_Boss[num].state = BOSS_STATE_IDLE;
			g_Boss[num].skill01Cnt = 0;
			g_changeable = TRUE;
		}

		else g_changeable = FALSE;
	}
}


void SetSkill002(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].use == FALSE)
		{
			//g_Skill002[i].use = TRUE;
			g_Skill002[i].startFlag = TRUE;
			g_Skill002[i].pos = XMFLOAT3(pos.x, 680.0f, 0.0f);
			g_Skill002[i].countAnim = 0;
			g_Skill002[i].patternAnim = 0;
			return;
		}
	}
}
void InitSkill002(void)
{
	//SKILL002

	g_skill02Cnt = 0;
	g_changeable = TRUE;
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		g_Skill002[i].use = FALSE;
		g_Skill002[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	/*	g_Skill002[i].w = 440.0f;
		g_Skill002[i].h = 360.0f;	*/	
		g_Skill002[i].w = 660.0f;
		g_Skill002[i].h = 480.0f;
		g_Skill002[i].texNo = 1;
		g_Skill002[i].countAnim = 0;
		g_Skill002[i].patternAnim = 0;
		g_Skill002[i].move = XMFLOAT3(0.0f,0.0f,0.0f);
		g_Skill002[i].frameCnt = 0;
		g_Skill002[i].delayCnt = 0;
		g_Skill002[i].startFlag = FALSE;
	}	
}

void Skill002Timer(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		//if (g_Skill002[i].startFlag)
		{
			
			int waitMax = g_boss_state_AnimeMax[BOSS_STATE_SKILL002][3] * g_boss_state_AnimeMax[BOSS_STATE_SKILL002][0];

			g_changeable = FALSE;

			if (g_Skill002[i].delayCnt == 0)
			{
				SetSkill002(player[0].pos);
			}

			if (g_Skill002[i].delayCnt++ >= waitMax)
			{
				g_Skill002[i].delayCnt = 0;
				g_Skill002[i].startFlag = FALSE;
				g_Skill002[i].use = TRUE;
				
				g_skill02Cnt++;
				PlaySound(SOUND_LABEL_SE_enemyskill02);
			}

			if (g_skill02Cnt >= SKILL2_MAX || g_Boss[0].hit)
			{
				g_skill02Cnt = 0;
				g_changeable = TRUE;

			}
		}
	}
}

//=============================================================================
// �X�L��2�̍X�V����
//=============================================================================
void UpdateSkill002(void)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		
		int waitMax = SKILL2_ANIM_PATTERN_NUM * SKILL2_WAIT_MAX;
		if (g_Skill002[i].use)
		{

			if (g_Skill002[i].frameCnt++ > SKILL2_ANIM_PATTERN_NUM * SKILL2_WAIT_MAX)
			{
				g_Skill002[i].frameCnt = 0;
				g_Skill002[i].use = FALSE;
			}


			if (g_Skill002[i].countAnim++ > SKILL2_WAIT_MAX)
			{
				g_Skill002[i].countAnim = 0;
				// �p�^�[���̐؂�ւ�
				g_Skill002[i].patternAnim = (g_Skill002[i].patternAnim + 1) % SKILL2_ANIM_PATTERN_NUM;
			}

			if (g_Skill002[i].frameCnt == waitMax / 2 && CollisionBB(g_Skill002[i].pos, g_Skill002[i].w / 8, g_Skill002[i].h, player[0].pos, player[0].w, player[0].h))
			{
				//SetPlayerHP(30.0f);
				player[0].hit = TRUE;
				int str = 30 + rand() % 10;
				player[0].hp -= str;
				SetHitScore(-str, player[0].pos, DAMAGE_PLAYER);
			}
		}

	}
}

//=============================================================================
// �X�L��2�̕`�揈��
//=============================================================================
void DrawSkill002(void)
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

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].use)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill002[i].texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f

			float px = g_Skill002[i].pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Skill002[i].pos.y;	// �o���b�g�̕\���ʒuY
			float pw = g_Skill002[i].w;		// �o���b�g�̕\����
			float ph = g_Skill002[i].h;		// �o���b�g�̕\������

			float tw = 1.0f / SKILL2_PATTERN_DIVIDE_X;							// �e�N�X�`���̕�	8�p�^�[��
			float th = 1.0f / SKILL2_PATTERN_DIVIDE_Y;								// �e�N�X�`���̍���
			float tx = tw * (g_Skill002[i].patternAnim % SKILL2_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Skill002[i].patternAnim / SKILL2_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W



			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
	
	DrawSkill2Pos();

}

void InitSkill2Pos(void)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		g_Skill002Pos[i].countAnim = 0;
		g_Skill002Pos[i].patternAnim = 0;
	}
}
void UpdateSkill2Pos(void)
{
	for (int i = 0; i < SKILL2_MAX; i++)
	{
		//if (g_Skill002[i].startFlag)
		{
			if (g_Skill002Pos[i].countAnim++ > SKILL2POS_WAIT_MAX)
			{
				g_Skill002Pos[i].countAnim = 0;
				// �p�^�[���̐؂�ւ�
				g_Skill002Pos[i].patternAnim = (g_Skill002Pos[i].patternAnim + 1) % SKILL2POS_ANIM_PATTERN_NUM;
			}
		}
	}

}

void DrawSkill2Pos(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < SKILL2_MAX; i++)
	{
		if (g_Skill002[i].startFlag)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f

			float px = g_Skill002[i].pos.x - bg->pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Skill002[i].pos.y - bg->pos.y + g_Skill002[i].h / 2;	// �o���b�g�̕\���ʒuY
			float pw = 160;		// �o���b�g�̕\����
			float ph = 30;		// �o���b�g�̕\������

			float tw = 1.0f / SKILL2POS_PATTERN_DIVIDE_X;							// �e�N�X�`���̕�	8�p�^�[��
			float th = 1.0f / SKILL2POS_PATTERN_DIVIDE_Y;								// �e�N�X�`���̍���
			float tx = tw * (g_Skill002Pos[i].patternAnim % SKILL2POS_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Skill002Pos[i].patternAnim / SKILL2POS_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W



			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

}


void SetSkill003(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (g_Skill003[i].use == FALSE)
		{
			g_Skill003[i].use = TRUE;
			g_Skill003[i].pos = pos;
			g_Skill003[i].countAnim = 0;
			g_Skill003[i].patternAnim = 0;
			g_Skill003[i].frameCnt = 0;
			return;
		}
	}
}

void Skill003Timer(void)
{
	
	if (g_skill03delayCnt++ >= 40)
	{
		g_skill03delayCnt = 0;
		

		XMFLOAT3 skill003Pos = XMFLOAT3(g_Boss[0].pos.x - g_Boss[0].w / 2,
			850,
			0.0f);
		SetSkill003(skill003Pos);
		PlaySound(SOUND_LABEL_SE_enemyskill02);
		g_skill03Cnt++;
	}

	if (g_skill03Cnt > 5 || g_Boss[0].hit)
	{
		g_skill03Cnt = 0;
		g_changeable = TRUE;
	}
	else
	{

		g_changeable = FALSE;

	}
	
}


void InitSkill003(void)
{
	g_skill03delayCnt = 0;
	g_skill03Cnt = 0;
	g_changeable = TRUE;

	for (int i = 0; i < SKILL3_MAX; i++)
	{
		
		g_Skill003[i].use = FALSE;
		g_Skill003[i].countAnim = 0;
		g_Skill003[i].patternAnim = 0;
		g_Skill003[i].frameCnt = 0;
		g_Skill003[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);
		g_Skill003[i].w = 120.0f;
		g_Skill003[i].h = 200.0f;
		g_Skill003[i].texNo = 3;
		g_Skill003[i].delayCnt = 0;
	}
}
void UpdateSkill003(void)
{
	PLAYER* player = GetPlayer();
	BULLET* bullet = GetBullet();
	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (!g_Skill003[i].use)continue;
		
		g_Skill003[i].pos.x -= 5.0f;
		if (g_Skill003[i].frameCnt++ >= 20)
		{
			g_Skill003[i].frameCnt = 0;
			g_Skill003[i].clear = !(g_Skill003[i].clear);
		}
		if (g_Skill003[i].countAnim++ > SKILL3_WAIT_MAX)
		{
			g_Skill003[i].countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_Skill003[i].patternAnim = (g_Skill003[i].patternAnim + 1) % SKILL3_ANIM_PATTERN_NUM;
		}

		//�t�B�[���h�Ƃ̓����蔻��
		if ((FieldCollision(g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h / 8)))
		{
			g_Skill003[i].use = FALSE;
		}

		//�v���C���[�Ƃ̓����蔻��

		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (player[j].use == FALSE)continue;
			if (player[j].state == PLAYER_STATE_GUARD)
			{
				EFFECT3* effect = GetEffect3();
				for (int i = 0; i < EFFECT_MAX; i++)
				{
					if (effect[i].type == GUARD && g_Skill003[i].use == TRUE &&
						(CollisionBB(effect[i].pos, effect[i].w / 2, effect[i].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
					{
						player[j].hit = TRUE;
						g_Skill003[i].use = FALSE;
						player[j].hp -= 3;
						SetHitScore(-3, g_Skill003[i].pos, DAMAGE_PLAYER);
						SetEffect3(g_Skill003[i].pos, BOMB2);

						int temp = rand()% 10;
						XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f,0.0f);
						if (temp == 0)SetDropItem(footPos);

						PlaySound(SOUND_LABEL_SE_hitshield);
						PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
					}
				}
				
			}
			if((CollisionBB(player[j].pos, player[j].w, player[j].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
			{
				player[j].hit = TRUE;
				player[j].hp -= 10;
				SetHitScore(-10, g_Skill003[i].pos, DAMAGE_PLAYER);
				g_Skill003[i].use = FALSE;
				SetEffect3(g_Skill003[i].pos, BOMB2);

				int temp = rand() % 20;
				XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f, 0.0f);
				if (temp == 0)SetDropItem(footPos);

				PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
			}
		}
		//�e�Ƃ̓����蔻��
		for (int j = 0; j < BULLET_MAX; j++)
		{
			if (bullet[j].use == FALSE)continue;
			if (bullet[j].mode != BLACKHOLE)continue;

			if ((CollisionBB(bullet[j].pos, bullet[j].w, bullet[j].h, g_Skill003[i].pos, g_Skill003[i].w, g_Skill003[i].h)))
			{
				g_Skill003[i].use = FALSE;

				int temp = rand() % 10;
				XMFLOAT3 footPos = XMFLOAT3(g_Skill003[i].pos.x, g_Skill003[i].pos.y + 60.0f, 0.0f);
				if (temp == 0)SetDropItem(footPos);

				PlaySound(SOUND_LABEL_SE_warp);
			}
		}
		
	}
}

void DrawSkill003(void)
{
	BG* bg = GetBG();

	for (int i = 0; i < SKILL3_MAX; i++)
	{
		if (g_Skill003[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill003[i].texNo]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Skill003[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Skill003[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Skill003[i].w;		// �v���C���[�̕\����
			float ph = g_Skill003[i].h;		// �v���C���[�̕\������

			// �A�j���[�V�����p
			//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			float tw = 1.0f / SKILL3_PATTERN_DIVIDE_X;							// �e�N�X�`���̕�	8�p�^�[��
			float th = 1.0f / SKILL3_PATTERN_DIVIDE_Y;								// �e�N�X�`���̍���
			float tx = tw * (g_Skill003[i].patternAnim % SKILL3_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Skill003[i].patternAnim / SKILL3_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;
			
			if (g_Skill003[i].clear)
			{
				G = 0.1f;
				B = 0.1f;
			}
			
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, 1.0f));
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void SetSkill004(XMFLOAT3 pos)
{
	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == FALSE)
		{
			g_Skill004[i].use = TRUE;
			g_Skill004[i].pos = pos;
			g_Skill004[i].countAnim = 0;
			g_Skill004[i].patternAnim = 0;
			g_Skill004[i].frameCnt = 0;
			return;
		}
	}
}

void Skill004Timer(void)
{
	if (g_skill04delayCnt++ >= 100)
	{
		g_skill04delayCnt = 0;

		//�����̒e�𐶐�����H

		int aNum = (rand() % SKILL4_POS_MAX);

		for (int i = 0; i < SKILL4_BULLET_MAX; i++)
		{
			//�ǂ̈ʒu�ŏ���������
			XMFLOAT3 bulletpos = g_Skill004_Pos[aNum][i];
			if (g_Skill004_Pos[aNum][i].x == 0.0f && g_Skill004_Pos[aNum][i].y == 0.0f)continue;

			SetSkill004(bulletpos);
		}


		//����ł̂�
		g_skill04Cnt++;

	}
	
	//�@5��ڏI�������I���
	if (g_skill04Cnt > 5 )
	{
		g_skill04Cnt = 0;
		g_changeable = TRUE;
	}
	else
	{
		g_changeable = FALSE;

	}
}


void InitSkill004(void)
{
	g_skill04delayCnt = 0;
	g_skill04Cnt = 0;
	g_changeable = TRUE;
	for (int i = 0; i < SKILL4_MAX; i++)
	{

		g_Skill004[i].use = FALSE;
		g_Skill004[i].countAnim = 0;
		g_Skill004[i].patternAnim = 0;
		g_Skill004[i].frameCnt = 0;
		g_Skill004[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Skill004[i].w = 192.0f;
		g_Skill004[i].h = 192.0f;
		g_Skill004[i].texNo = 4;
		g_Skill004[i].delayCnt = 0;
	}
}

void UpdateSkill004(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == FALSE)continue;

		g_Skill004[i].pos.x -= 5.0f;


		if (g_Skill004[i].countAnim++ > SKILL4_WAIT_MAX)
		{
			g_Skill004[i].countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_Skill004[i].patternAnim = (g_Skill004[i].patternAnim + 1) % SKILL4_ANIM_PATTERN_NUM;
		}
		
		////�t�B�[���h�Ƃ̓����蔻��
		if ((FieldCollision(g_Skill004[i].pos, g_Skill004[i].w, g_Skill004[i].h / 8)))
		{
			g_Skill004[i].use = FALSE;
		}

		//�v���C���[�Ƃ̓����蔻��
		for (int j = 0; j < PLAYER_MAX; j++)
		{

			if (player[j].use && (CollisionBB(player[j].pos, player[j].w / 2, player[j].h - 40, g_Skill004[i].pos, g_Skill004[i].w - 40.0f, g_Skill004[i].h - 40.0f)))
			{
				player[j].hit = TRUE;
				//AddPlayerHP(-10);
				player[j].hp -= 10;
				SetHitScore(-10, g_Skill004[i].pos, DAMAGE_PLAYER);
				SetEffect3(player[j].pos, HIT2);
				g_Skill004[i].use = FALSE;
				PlaySound(SOUND_LABEL_SE_enemyskill03_explosion);
			}
		}
	}
}
void DrawSkill004(void)
{

	BG* bg = GetBG();

	for (int i = 0; i < SKILL4_MAX; i++)
	{
		if (g_Skill004[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Skill004[i].texNo]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Skill004[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Skill004[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_Skill004[i].w;		// �v���C���[�̕\����
			float ph = g_Skill004[i].h;		// �v���C���[�̕\������

			// �A�j���[�V�����p
			//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			float tw = 1.0f / SKILL4_PATTERN_DIVIDE_X;							// �e�N�X�`���̕�	8�p�^�[��
			float th = 1.0f / SKILL4_PATTERN_DIVIDE_Y;								// �e�N�X�`���̍���
			float tx = tw * (g_Skill004[i].patternAnim % SKILL4_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Skill004[i].patternAnim / SKILL4_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			float R = 1.0f;
			float G = 1.0f;
			float B = 1.0f;

			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(R, G, B, 1.0f));
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}



BOOL isBossDead(void)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if(g_Boss[i].use == FALSE)		return TRUE;

	}
	return FALSE;
}

BOSS *GetBoss(void)
{
	return &g_Boss[0];
}
void SetBDamagedType(int type)
{
	for (int i = 0; i < BOSS_MAX; i++)
	{
		if (g_Boss[i].hit)g_Boss[i].damagedType = type;
	}
	return;
}

void Down(void)
{
	g_Downtimer += GetDeltatime();
	float waitMax = 2.0f;
	if (g_Downtimer > waitMax * 1000)
	{
		g_Downtimer = 0.0f;
		g_changeable = TRUE;
	}
	else
	{
		g_changeable = FALSE;
	}
}