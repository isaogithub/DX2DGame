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
#define PLAYER_MAX			(1)		// �v���C���[��Max�l��

#define	PLAYER_OFFSET_CNT	(4)	// 16���g


#define TEXTURE_COLLISION_WAIST_WIDTH (64)	//�̂̓����蔻��
#define TEXTURE_COLLISION_WAIST_HEIGHT (45)

#define TEXTURE_COLLISION_HEAD_WIDTH (40)	//���̓����蔻��
#define TEXTURE_COLLISION_HEAD_HEIGHT (30)

#define TEXTURE_COLLISION_FOOT_WIDTH (63)	//�����̓����蔻��
#define TEXTURE_COLLISION_FOOT_HEIGHT (10)

// �v���C���[�̉�ʓ��z�u���W
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

#define ULT_CHANT_FRAMEMAX		(100)
#define ULT_UNLEASH_FRAMEMAX	(180)

#define PLAYER_HP_MAX	(200)
#define PLAYER_MP_MAX	(100)
#define CHECKPOINT_MAX	(2)

enum CHAR_DIR
{
	CHAR_DIR_LEFT,
	CHAR_DIR_RIGHT,
	CHAR_DIR_MAX
};

enum PLAYER_STATE
{
	PLAYER_STATE_STAND,
	PLAYER_STATE_WALK,
	PLAYER_STATE_GUARD,
	PLAYER_STATE_JUMP,
	PLAYER_STATE_FALL,
	PLAYER_STATE_BLADE,
	PLAYER_STATE_BLACKHOLE, 
	PLAYER_STATE_BULLET, 
	PLAYER_STATE_ULT,
	PLAYER_STATE_MAX,
};

enum SKILL_TYPE
{
	SKILL_ATTACK,
	SKILL_SKILL001,
	SKILL_SKILL002,
	SKILL_JUMP,
};
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYERICON
{
	BOOL		use;			// true:�g���Ă���  false:���g�p
	XMFLOAT3	pos;			// �|���S���̍��W
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	float		theta;

};
struct PLAYER
{
	BOOL		use;			// true:�g���Ă���  false:���g�p
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	opos;			// �v���C���[���ړ�����O�̍��W
	XMFLOAT3	rot;			// �|���S���̉�]��
	XMFLOAT3	savePoint;		// ��������n�_

	float		w, h;			// ���ƍ���
	float		countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int			texNo;			// �e�N�X�`���ԍ�
	int		time;			//�v���C���[�̎��Ԍv�Z
	//��{�X�e�[�^�X
	int			dir;			// �����i0:�� 1:�E 2:�� 3:���j
	int			state;			//�v���C���[�̏��
	int			prestate;		//�O�̏��

	int			life;			//�c�@
	float		hp;				//�v���C���[HP
	float		mp;				//�v���C���[MP
	float		str;

	int			jumptimes;
	float		jumpplus;		// �W�����v�����x


	int			flashExeCnt;
	int			flashCnt;
	float		flashExeTimer;
	float		flashTimer;

	XMFLOAT3	move;			// �ړ����x

	//�X�e�[�g�t���O
	BOOL		moving;			// �ړ����t���O
	BOOL		flash;			// �_�b�V�����t���O
	BOOL		onGround;		//�v���C���[���n�ʂɂ���
	BOOL		attack;
	BOOL		bullet;
	BOOL		jump;			// �W�����v�t���O
	//
	BOOL		hit;			//�q�b�g�t���O
	BOOL		hitting;		//�q�b�g����Ă���
	BOOL		clear;			//�q�b�g�̕\��
	int			hitdir;			//�q�b�g����Ă���̕���
	int			hitCnt;			//�q�b�g�J�E���^�[
	int			clearCnt;
	
	//�ړ��p�����[�^
	XMFLOAT3	offset[PLAYER_OFFSET_CNT];		// �c���|���S���̍��W
	XMFLOAT3	beforejumppos;	//�W�����v�O�̍��W��ۑ�����
	int			jumpCnt;		// �W�����v���̃J�E���g


};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);

//�d�͏���
void PlayerGravity(int num);

//�W�����v����
void SetJump(int num);
void PlayerJump(int num);
void PlayerFlash(XMFLOAT3* pos,int num);
void SetPlayerJump(void);

// �e�\��
void DrawPlayerShadow(void);
//�U���`��
void DrawAttackEffect(void);

//�v���C���[�U������
void PlayerAttackProcess(int num);

// �o���b�g����
BOOL PlayerBulletProcess(int  num);
//���ɗ���������
void PlayerReturn(int num);

////�v���C���[��HP�\��
//void DrawPlayerHP(int num);
////�v���C���[��MP�\��
//void DrawPlayerMP(int num);
//
////�v���C���[��ԕ\��
//void DrawPlayerState(int num);

//���ꔻ�f����
BOOL IsFootCol(int num);
//���f����
BOOL IsWaistCol(int num);
//�q�b�g�o�b�N����
void PHitbackProcess(int num);

//�v���C���[��HP���������̒l������
void AddPlayerHP(int num,float hp);
void AddPlayerMP(int num,float mp);
//�v���C���[�̃W�����v��Ԃ����Z�b�g
void JumpReset(void);
void BlackholeTimer(int num);
void BulletTimer(int num);
void UltTimer(int num);
void Guard(int num);

//
//void InitPlayerIcon(void);
//void UpdatePlayerIcon(void);
//void  DrawPlayerIcon(void);
void DrawEyeDark(void);