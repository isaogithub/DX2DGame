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

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define		MAP_MAX							(3)
#define		MAP_WIDTH						(SCREEN_WIDTH * 2)	//�}�b�v
#define		MAP_HEIGHT						(SCREEN_HEIGHT * MAP_MAX)

#define		GRAVITY			(0.02f)

enum TUTORIAL_FIELD_ITEM_TEXTURE
{
	TUTORIAL_MAPCHIP,
	TUTORIAL_ARROW,
	TUTORIAL_CAVE,
};

enum IOB_MESSAGE
{
	NOKEY,
	DOOR,
	BOSSNODEAD,
	HEROINE,
};
enum INGAMEBGM
{
	BGM_DUNGEON,
	BGM_BOSS,
	BGM_MAX,
};

enum BUS_TYPE
{
	LR_BUS,
	UD_BUS,
};
/*******************************************************************************
* �\���̒�`
*******************************************************************************/

struct ROPE
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	float		theta;			//���[�v�̉�]�p�x
	float		x;
	float		y;
	float		r;				//CollisionBC�p���a
};

struct ITEM
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	BOOL		clear;			// ����
	float		w, h;			// ���ƍ���
	int			type;
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	

	float		r;				//CollisionBC�p���a
	XMFLOAT3	move;
};

struct ITEM2
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
};

struct TRAPWALL
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�

	XMFLOAT3	move;

};


struct DROP_ITEM
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			type;
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	

};

struct TOGE
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
};


struct TRAP
{
	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	BOOL		trig;

	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
	float		gravityCnt;
};

struct FIELD
{

	XMFLOAT3	pos;			// �|���S���̍��W
	XMFLOAT3	scl;			// �|���S���̊g��k��
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�

};

struct PAUSE
{
	BOOL use;
	XMFLOAT3 pos;
};


struct INTERACT_OB
{
	BOOL		use;
	BOOL		col;
	BOOL		display;
	XMFLOAT3	pos;
	int			texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
	float		w, h;	// ���ƍ���
};

struct MESSAGE
{
	BOOL		use;
	BOOL		display;
	int			texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	float		w, h;	// ���ƍ���
};

struct MESSAGEPOS
{
	BOOL		use;
	BOOL		display;
	XMFLOAT3	pos;
	int			texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	float		w, h;	// ���ƍ���
};


struct HINT
{
	BOOL		use;
	int			texNo;					// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	float		w, h;	// ���ƍ���
	XMFLOAT3	pos;
};

struct SKILL
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
	XMFLOAT3	move;
	int			frameCnt;
	int			delayCnt;
	BOOL		startFlag;
};

struct SKILL3
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
	int			frameCnt;
	int			delayCnt;
	BOOL		clear;
	float		gravityCnt;

};

struct SKILL4
{
	XMFLOAT3	pos;			// �|���S���̍��W
	BOOL		use;			// true:�g���Ă���  false:���g�p
	float		w, h;			// ���ƍ���
	int			texNo;			// �e�N�X�`���ԍ�
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
	int			frameCnt;
	int			delayCnt;
	BOOL		clear;

};



struct ANI
{
	int			countAnim;		// �A�j���[�V�����J�E���g
	int			patternAnim;	// �A�j���[�V�����p�^�[���i���o�[	
};

struct ANIMATION
{
	int		divideX;
	int		divideY;
	int		animewait;
	float		w;
	float		h;
};
/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

void InitIOB(void);
void UninitIOB(void);
void UpdateIOB(void);
void DrawIOB(void);

INTERACT_OB* GetIOB(void);
INTERACT_OB* GetDoor(void);
INTERACT_OB* GetHeroine(void);


float GetScrollX(XMFLOAT3 pos);

float GetScrollY(XMFLOAT3 pos);

//�����̈ʒu�𒆐S�ɑS�}�b�v�̍��W���Q�Ƃ��A�A����ʂ̑傫���ɂ��X�̏����ʒu�����߂�
float GetStartCollusionX(XMFLOAT3 pos);

//�����̈ʒu�𒆐S�ɑS�}�b�v�̍��W���Q�Ƃ��A�A����ʂ̑傫���ɂ��Y�̏����ʒu�����߂�
float GetStartCollusionY(XMFLOAT3 pos);

float GetStartDownCollusionY(XMFLOAT3 pos);

float GetStartWaistCollusionY(XMFLOAT3 pos);

//�t�B�[���h�̃A�h���X��Ԃ�
int* GetFieldPtr(void);

//�w�肳�ꂽ�w�x���W�̃t�B�[���h�f�[�^��Ԃ�
int GetFieldData(int y, int x);

//pos�̍��W�̓}�b�v�`�b�v�̓����蔻��BLOCK�Ɠ����H
BOOL FieldCollision(XMFLOAT3 pos, float width, float height);
BOOL DownFieldCollision(XMFLOAT3 pos, float width, float height);
BOOL WaistFieldCollision(XMFLOAT3 pos, float width, float height);

BOOL SkillMouse(ITEM skill);

//�}�b�v���[�h
void LoadMap(void);

void Arcmove(void);
//�d�͈ʒu�v�Z
XMFLOAT3 GravityPosCal(XMFLOAT3 pos, float gravityCnt);


void InitTrap(void);
void UpdateTrap(void);
void DrawTrap(void);
void DrawFaketrap(void);

void InitBus(void);
void UpdateBus(void);
void DrawBus(void);

void InitRope(void);
void InitHint(void);
void UpdateHint(void);
void DrawHint(void);

void InitKey(void);
void UpdateKey(void);
void DrawKey(void);


void InitToge(void);
void UpdateToge(void);
void DrawToge(void);

void InitDropItem(void);

//�����Q�b�g�����H
BOOL isGetKey(void);

//�v���C���[�̓Q�[���̒��ɂǂ̒i�K�ɂ���
int GetBGMmode(void);


void InitDropItem(void);
void UpdateDropItem(void);
void DrawDropItem(void);
void SetDropItem(XMFLOAT3 pos);

void InitTrapWall(void);
void UpdateTrapWall(void);
void DrawTrapWall(void);