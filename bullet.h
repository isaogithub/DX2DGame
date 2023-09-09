//=============================================================================
//
// �o���b�g���� [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_MAX		(100)		// �o���b�g��Max��
#define NUMA_MAX		(20)		// �o���b�g��Max��

#define BULLET_SPEED	(20.0f)		// �o���b�g�̈ړ��X�s�[�h
#define	BULLET_DAMAGE				(50)
#define	BULLET_COSTMP				(35)


enum BULLET_DIR
{
	BULLET_DIR_RIGHT,
	BULLET_DIR_LEFT
};

enum BULLET_MODE
{
	BLACKHOLE,
	BULLET_MODE_ENEMY,
	NUMA1,
	BULLET_MODE_HP,
	BULLET_1,
	BULLET_MODE_MAX,
};

// �o���b�g�\����
struct BULLET
{
	BOOL				use;				// true:�g���Ă���  false:���g�p
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	XMFLOAT3			initMove;				// �o���b�g�̍��W
	XMFLOAT3			rot;				// �o���b�g�̉�]��
	XMFLOAT3			move;				// �o���b�g�̈ړ���
	int					dir;				//�o���b�g�̌���
	int					countAnim;			// �A�j���[�V�����J�E���g
	int					patternAnim;		// �A�j���[�V�����p�^�[���i���o�[
	int					texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	int					frameCnt;
	float				timer;
	int					damage;
	BOOL				mode;
	//�G�̍U���Ɏg�p����
	float				gravityCnt;


};
struct NUMA
{
	BOOL	use;
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	int					texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	float				timer;
	int					countAnim;			// �A�j���[�V�����J�E���g
	int					patternAnim;		// �A�j���[�V�����p�^�[���i���o�[

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
//void SetBullet(XMFLOAT3 pos);
void SetBullet(XMFLOAT3 pos, BOOL mode);
void SetBullet(XMFLOAT3 pos, BOOL mode, int dir, XMFLOAT3 initMove);

//�e�̔��˂̓v���C���[���G�l�~�[
void SetBulletMode(int num, BOOL mode);

//�G�̍U���ڕW��ݒ肷��
void SetTarget(XMFLOAT3 pos);


void UpdateNuma(void);
void DrawNuma(void);
void SetNuma(XMFLOAT3 pos);