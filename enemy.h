//=============================================================================
//
// �G�l�~�[���� [enemy.h]
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
#define ENEMY_TUTORIAL_MAX				(3)
#define ENEMY_MAX						(15)		// �G�l�~�[��Max�l��

#define	ENEMY_SLIME_MAX					(3)
#define BOSS_MAX					(1)


enum ENEMY_TYPE
{
	ENEMY_TYPE_SLIME,
	ENEMY_TYPE_BOSS,
	ENEMY_TYPE_MAX,
};

enum ENEMY_SLIME_STATE
{
	SLIME_STATE_IDLE,
	SLIME_STATE_WALK,
	SLIME_STATE_ATTACK001,
	SLIME_STATE_ATTACK002,
	SLIME_STATE_HIT,
	SLIME_STATE_MAX,
};

enum ENEMY_BOSS_STATE
{
	BOSS_STATE_IDLE,
	BOSS_STATE_WALK,
	BOSS_STATE_HIT,
	BOSS_STATE_SKILL001,
	BOSS_STATE_SKILL002,
	BOSS_STATE_SKILL003,
	BOSS_STATE_SKILL004,
	BOSS_STATE_SKILL005,
	BOSS_STATE_DOWN,
	BOSS_STATE_MAX,
};

enum ENEMY_SKILL
{
	ENEMY_SKILL001,
	ENEMY_SKILL002,
	ENEMY_SKILL003,
	ENEMY_SKILL004,
	ENEMY_SKILLMAX,
};

enum DAMAGED
{
	DAMAGED_BLADE,
	DAMAGED_BULLET,
	DAMAGED_BULLET2,
	DAMAGED_ULT,
	DAMAGED_MAX,
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct ENEMY
{
	BOOL		use;			// true:�g���Ă���  false:���g�p
	BOOL		display;
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�

	int			type;			//�G�l�~�[�̃^�C�v
	int			state;
	int			ostate;
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	opos;			
	XMFLOAT3	nextpos;		//���ɍs���Ƃ���

	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	scl;			// �|���S���̊g��k��


	int			hp;				// �G�l�~�[��HP
	int			damagedType;	//�������_���[�W�͉��̋Z�H

	float	needmovedis;		//�G�l�~�[���ړ��K�v�ȋ���
	float	movedis;			//��~��Ԃ���ǂꂭ�炢�ړ�������
	float	knockmoveX;		//�m�b�N�A�E�g�̖��t���C���ړ�

	BOOL		stop;
	float		stopframe;
	
	//XMFLOAT3	knockmoveY;		//�m�b�N�A�E�g�̖��t���C���ړ�
	float		walktimer;

	int			hitCnt;			// �����蔻��J�E���^
	BOOL		hit;			// ���Ă�ꂽ
	BOOL		hitting;		// ���Ă�ꂽ�r��
	BOOL		clear;			// �����t���O

	BOOL		attack;			//�U���t���O

	BOOL		skill;
	BOOL		skillCnt;
	BOOL		skilldelaytime;

	BOOL		skill02;


	int			attacktime;		//�U�����ԃJ�E���^�[


	BOOL		onGround;		// �G�l�~�[��
	float		gravityCnt;

	int			str;			// �U����
	BOOL		dir;			//�G�l�~�[����
	XMFLOAT3	move;			// �ړ����x
	XMFLOAT3	amove;			// �v���C���[�����������x

};

struct BOSS
{
	BOOL		use;			// true:�g���Ă���  false:���g�p
	BOOL		display;
	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�

	int			type;			//�G�l�~�[�̃^�C�v
	int			state;
	int			ostate;
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	opos;

	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	scl;			// �|���S���̊g��k��

	int			hp;				// �G�l�~�[��HP
	int			damagedType;	//�������_���[�W�͉��̋Z�H

	int			hitCnt;			// �����蔻��J�E���^
	BOOL		hit;			// ���Ă�ꂽ
	BOOL		hitting;		// ���Ă�ꂽ�r��
	BOOL		clear;			// �����t���O

	BOOL		attack;			//�U���t���O

	BOOL		skill01;
	BOOL		skill01Cnt;
	BOOL		skill01delaytime;

	BOOL		skill02;


	int			attacktime;		//�U�����ԃJ�E���^�[


	BOOL		onGround;		// �G�l�~�[��
	float		gravityCnt;

	int			str;			// �U����
	BOOL		dir;			//�G�l�~�[����
	XMFLOAT3	move;			// �ړ����x
	XMFLOAT3	amove;			// �v���C���[�����������x

	float		time;			// ���`��ԗp
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);
void UpdateSlime(void);

void InitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

// �A�j���[�V����  
void AnimetionProcess(int num);
//�v���C���[���߂�
BOOL IsPlayerClose(int num);

//�ړ�����
void MoveProcess(int num);
//�v���C���[�ɍU�����ꂽ��̈ړ��p�^�[��
void MoveProcessP(int num);
void EnemyMoveSound(int num);

//�v���C���[�ւ̍U���p�^�[��
void AttackProcess(int num);
//�d�͏���
void GravityProcess(int num);
//�q�b�g�o�b�N����
void EHitbackProcess(int num);

//�q�b�g�o�b�N����
void BossHit(int num);
void BossHitback(int num);//�G�l�~�[��HP�\��
void DrawEnemyHP(int num);

int DisBulletDir(void);

void SetSkill000(int num);
void SetSkill005(int num);
void UpdateBMove(int num);
void MoveProcessBP(int num);

void SetEDamagedType(int num, int type);

void BGravityProcess(int num);
//�G�l�~�[�̃X�L���P
void InitSkill001(void);
void Skill001Timer(int num);

void SetSkill002Pos(void);

void SetSkill002(XMFLOAT3 pos);
void Skill002Timer(void);
void InitSkill002(void);
void UpdateSkill002(void);
void DrawSkill002(void);

void InitSkill2Pos(void);
void UpdateSkill2Pos(void);
void DrawSkill2Pos(void);

void SetSkill003(XMFLOAT3 pos);
void Skill003Timer(void);
void InitSkill003(void);
void UpdateSkill003(void);
void DrawSkill003(void);

void SetSkill004(XMFLOAT3 pos);
void Skill004Timer(void);
void InitSkill004(void);
void UpdateSkill004(void);
void DrawSkill004(void);

void Skill005(int num);



BOOL isBossDead(void);
BOSS* GetBoss(void);
void SetBDamagedType(int type);
void DrawBossHP(int num);
void Down(void);