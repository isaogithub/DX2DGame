//=============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author : 
//
//=============================================================================
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "collision.h"
#include "bg.h"
#include "effect.h"
#include "sound.h"
#include "functionSet.h"
#include "field.h"
#include "hitscore.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(3)		// �e�N�X�`���̐�

#define TEXTURE_NUMA_WIDTH			(120)	// �L�����T�C�Y
#define TEXTURE_NUMA_HEIGHT			(82.5)	// 

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����


#define SKILL01_WAIT_MAX							(5)
#define SKILL01_PATTERN_DIVIDE_X					(5)
#define SKILL01_PATTERN_DIVIDE_Y					(3)
#define SKILL01_ANIM_PATTERN_NUM	(SKILL01_PATTERN_DIVIDE_X * SKILL01_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define BULLETHP_WAIT_MAX							(5)
#define BULLETHP_PATTERN_DIVIDE_X					(5)
#define BULLETHP_PATTERN_DIVIDE_Y					(2)
#define BULLETH_ANIM_PATTERN_NUM	(BULLETHP_PATTERN_DIVIDE_X * BULLETHP_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^


#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define NUMA_WAIT_MAX				(8)

#define NUMA_PATTERN_DIVIDE_X		(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define NUMA_PATTERN_DIVIDE_Y		(11)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define NUMA_PATTERN_NUM			(NUMA_PATTERN_DIVIDE_X*NUMA_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[BULLET_MODE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[BULLET_MODE_MAX] = {
	"data/EFFECT/bullet01.png",
	"data/EFFECT/enemy_skill01.png",
	"data/EFFECT/numa.png",
	"data/EFFECT/bullet_hp.png",
	"data/EFFECT/player_bullet2.png",
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static BULLET	g_Bullet[BULLET_MAX];	// �o���b�g�\����
static NUMA		g_Numa[NUMA_MAX];

static ANIMATION g_bulletAni[] =
{//divideX divideY animewait  w		 h
	{5,		2,		10,		192,	192,},
	{5,		3,		3,		80,		80,},
	{1,		11,		5,		100,	75,},
	{5,		2,		5,		 60,	60,},
	{5,		5,		3,		 192,	192,},
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < BULLET_MODE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
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


	// �o���b�g�\���̂̏�����
	for (int i = 0; i < BULLET_MODE_MAX; i++)
	{
		g_Bullet[i].use   = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Bullet[i].w     = 0;
		g_Bullet[i].h     = 0;
		g_Bullet[i].pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;

		g_Bullet[i].dir = BULLET_DIR_RIGHT;
		g_Bullet[i].countAnim = 0;
		g_Bullet[i].patternAnim = 0;

		//g_Bullet[i].move = XMFLOAT3(0.0f, -BULLET_SPEED, 0.0f);	// �ړ��ʂ�������
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
		g_Bullet[i].damage = 0;
		g_Bullet[i].timer = 0.0f;	
		g_Bullet[i].frameCnt = 0;	
		g_Bullet[i].mode = BULLET_1;
		g_Bullet[i].gravityCnt = 0.0f;

	}

	for (int i = 0; i < NUMA_MAX; i++)
	{
		g_Numa[i].use = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Numa[i].w = TEXTURE_NUMA_WIDTH;
		g_Numa[i].h = TEXTURE_NUMA_HEIGHT;
		g_Numa[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Numa[i].texNo = 2;
		g_Numa[i].timer = 0.0f;	// �ړ��ʂ�������
		g_Numa[i].countAnim = 0;	// �ړ��ʂ�������
		g_Numa[i].patternAnim = 0;	// �ړ��ʂ�������

	}
	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
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

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	int bulletCount = 0;				// ���������o���b�g�̐�
	PLAYER* player = GetPlayer();
	ENEMY * enemy  =  GetEnemy();
	BG* bg = GetBG();
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)continue;	// ���̃o���b�g���g���Ă���H

		// Yes
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float waitMax = 0.5f;//second


		// �A�j���[�V����  
		if ((g_Bullet[i].countAnim++ > g_bulletAni[g_Bullet[i].mode].animewait))
		{
			g_Bullet[i].countAnim = 0;
			// �p�^�[���̐؂�ւ�
			int divideX = g_bulletAni[g_Bullet[i].mode].divideX;
			int divideY = g_bulletAni[g_Bullet[i].mode].divideY;
			g_Bullet[i].patternAnim = (g_Bullet[i].patternAnim + 1) % (divideX * divideY);
		}

		//���ԃJ�E���^�[
		switch (g_Bullet[i].mode)
		{
		case	BLACKHOLE:
			if (g_Bullet[i].frameCnt++ >= 280)
			{
				g_Bullet[i].frameCnt = 0;
				g_Bullet[i].use = FALSE;
				continue;
			}
			break;
		}

		// �o���b�g�̈ړ�����
		switch (g_Bullet[i].mode)
		{
		case BLACKHOLE://�d�͒e
			
			{//�ړ�
				XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);
				XMVECTOR vopos = XMLoadFloat3(&g_Bullet[i].pos);//�O�̍��W���L������
				XMVECTOR vpos = XMLoadFloat3(&g_Bullet[i].pos);

				if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
				{
					move = XMFLOAT3(0.1, 0.0f, 0.0f);
				}
				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					move = XMFLOAT3(-0.1, 0.0f, 0.0f);

				}
				g_Bullet[i].move = move;

				XMVECTOR vmove = XMLoadFloat3(&g_Bullet[i].move);
				vpos += vmove;
				XMStoreFloat3(&g_Bullet[i].pos, vpos);
			}
			
			// �����蔻�菈��
			if (GetMode() == MODE_BOSS)
			{//�{�X�ɓ��������������

				ENEMY* boss = GetBoss();
				for (int j = 0; j < BOSS_MAX; j++)
				{
					if (boss[j].use == FALSE)continue;
					int ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h,
						boss[j].pos, boss[j].w, boss[j].h);

					if (ans == TRUE)
					{
						g_Bullet[i].use = FALSE;
						boss[j].hit = TRUE;

						SetEffect3(g_Bullet[i].pos, HIT3);
						SetBDamagedType(DAMAGED_BULLET);
					}
				}
			}
			else
			{//�{�X�ȊO�͋z�����܂��
				ENEMY* enemy = GetEnemy();
				for (int j = 0; j < ENEMY_MAX; j++)
				{// �G�l�~�[�̐��������蔻����s��
					if (enemy[j].use == TRUE)
					{// �����Ă�G�l�~�[�Ɠ����蔻�������

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, enemy[j].pos, enemy[j].w, enemy[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							enemy[j].hit = TRUE;

							SetEDamagedType(j, DAMAGED_BULLET);
							XMFLOAT3 epos = XMFLOAT3((g_Bullet[i].pos.x + enemy[j].pos.x) * 0.5,
								(g_Bullet[i].pos.y + enemy[j].pos.y) * 0.5, 0.0f);
							SetEffect3(g_Bullet[i].pos, HIT2);

						}
					}
				}
			}
			
		break;
		case BULLET_1:

		{//�ړ�
			XMFLOAT3 move = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMVECTOR vopos = XMLoadFloat3(&g_Bullet[i].pos);//�O�̍��W���L������
			XMVECTOR vpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				move = XMFLOAT3(15.0f, 0.0f, 0.0f);
			}
			if (g_Bullet[i].dir == BULLET_DIR_LEFT)
			{
				move = XMFLOAT3(-15.0f, 0.0f, 0.0f);

			}
			g_Bullet[i].move = move;

			XMVECTOR vmove = XMLoadFloat3(&g_Bullet[i].move);
			vpos += vmove;
			XMStoreFloat3(&g_Bullet[i].pos, vpos);
		}


		{// �����蔻�菈��
			if (GetMode() == MODE_BOSS)
			{

				ENEMY* boss = GetBoss();
				for (int j = 0; j < BOSS_MAX; j++)
				{
					if (boss[j].use == FALSE)continue;

					int ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h,
						boss[j].pos, boss[j].w, boss[j].h);

					if (ans == TRUE)
					{
						g_Bullet[i].use = FALSE;
						boss[j].hit = TRUE;
						SetEffect3(g_Bullet[i].pos, HIT2);
						SetBDamagedType(DAMAGED_BULLET2);
					}
				}
			}
			else
			{
				ENEMY* enemy = GetEnemy();
				for (int j = 0; j < ENEMY_MAX; j++)
				{// �G�l�~�[�̐��������蔻����s��
					if (enemy[j].use == TRUE)
					{// �����Ă�G�l�~�[�Ɠ����蔻�������

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, enemy[j].pos, enemy[j].w, enemy[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							enemy[j].hit = TRUE;
							SetEffect3(g_Bullet[i].pos, HIT3);
							SetEDamagedType(j, DAMAGED_BULLET2);
							g_Bullet[i].use = FALSE;
						}
					}
				}
			}
			//
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w/2, g_Bullet[i].h / 2))
			{
				g_Bullet[i].use = FALSE;
			}

			//�}�b�v�O�ɏo����
			if (GetMode() == MODE_BOSS)
			{
				if (g_Bullet[i].pos.x >= SCREEN_WIDTH || g_Bullet[i].pos.x <= 0.0f)g_Bullet[i].use = 0;
				else if (g_Bullet[i].pos.y >= SCREEN_HEIGHT || g_Bullet[i].pos.y <= 0.0f)g_Bullet[i].use = 0;
			}
			else
			{
				if (g_Bullet[i].pos.x >= SCREEN_WIDTH * 2 || g_Bullet[i].pos.x <= 0.0f)g_Bullet[i].use = 0;
				else if (g_Bullet[i].pos.y >= SCREEN_HEIGHT * 3 || g_Bullet[i].pos.y <= 0.0f)g_Bullet[i].use = 0;

			}
		}


		break;
		case BULLET_MODE_ENEMY:

		{

			XMVECTOR vM = XMLoadFloat3(&g_Bullet[i].initMove);
			XMVECTOR vBpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				g_Bullet[i].initMove.x -= 0.2f;
				g_Bullet[i].initMove.y += 0.2f;
				if (g_Bullet[i].initMove.x <= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			else
			{
				g_Bullet[i].initMove.x += 0.2f;
				g_Bullet[i].initMove.y += 0.2f;

				if (g_Bullet[i].initMove.x >= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			XMStoreFloat3(&g_Bullet[i].pos, vBpos + vM);

			{
				g_Bullet[i].gravityCnt += GRAVITY;
				XMFLOAT3 nextpos = GravityPosCal(g_Bullet[i].pos, g_Bullet[i].gravityCnt);

				g_Bullet[i].pos = nextpos;
			}
			if (g_Bullet[i].pos.y >= MAP_HEIGHT)
			{
				g_Bullet[i].use = FALSE;
			}

			// �����蔻�菈��
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{// �G�l�~�[�̐��������蔻����s��
					if (player[j].use == TRUE)
					{// �����Ă�G�l�~�[�Ɠ����蔻�������

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							player[j].hit = TRUE;
							int str = 10 + rand() % 10;
#ifndef _DEBUG	// �����[�X�Ԃ���HP������
							AddPlayerHP(j,-str);
#endif
							
							g_Bullet[i].use = FALSE;
						}
					}
				}
			}
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h))
			{
				SetNuma(g_Bullet[i].pos);
				g_Bullet[i].use = FALSE;
			}
			break;

		}
				
		break;
		case BULLET_MODE_HP:

			XMVECTOR vM = XMLoadFloat3(&g_Bullet[i].initMove);
			XMVECTOR vBpos = XMLoadFloat3(&g_Bullet[i].pos);

			if (g_Bullet[i].dir == BULLET_DIR_RIGHT)
			{
				g_Bullet[i].initMove.x -= 0.2f;
				g_Bullet[i].initMove.y += 0.2f;
				if (g_Bullet[i].initMove.x <= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			else
			{
				g_Bullet[i].initMove.x += 0.2f;
				g_Bullet[i].initMove.y += 0.2f;

				if (g_Bullet[i].initMove.x >= 0.0f)g_Bullet[i].initMove.x = 0;
				if (g_Bullet[i].initMove.y >= 0.0f)g_Bullet[i].initMove.y = 0;
			}

			XMStoreFloat3(&g_Bullet[i].pos, vBpos + vM);

			{
				g_Bullet[i].gravityCnt += GRAVITY;
				XMFLOAT3 nextpos = GravityPosCal(g_Bullet[i].pos, g_Bullet[i].gravityCnt);

				g_Bullet[i].pos = nextpos;
			}
			if (g_Bullet[i].pos.y >= MAP_HEIGHT)
			{
				g_Bullet[i].use = FALSE;
			}

			// �����蔻�菈��
			{
				for (int j = 0; j < PLAYER_MAX; j++)
				{// �G�l�~�[�̐��������蔻����s��
					if (player[j].use == TRUE)
					{// �����Ă�G�l�~�[�Ɠ����蔻�������

						BOOL ans = CollisionBB(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h, player[j].pos, player[j].w, player[j].h);
						// �������Ă���H
						if (ans == TRUE)
						{
							// �����������̏���
							AddPlayerHP(j,20);
							SetHealEffect(player[j].pos);
							SetHitScore(20, player[j].pos, HEAL);

							g_Bullet[i].use = FALSE;

							PlaySound(SOUND_LABEL_SE_heal001);
						}
					}
				}
			}
			if (FieldCollision(g_Bullet[i].pos, g_Bullet[i].w, g_Bullet[i].h))
			{
				g_Bullet[i].use = FALSE;
			}


		break;

		
		}

		bulletCount++;
		
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
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
	PLAYER* player = GetPlayer();
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)continue;	
		// ���̃o���b�g���g���Ă���H
		
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bullet[i].mode]);
			
		//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Bullet[i].pos.x - bg->pos.x;	// �o���b�g�̕\���ʒuX
		float py = g_Bullet[i].pos.y - bg->pos.y;	// �o���b�g�̕\���ʒuY
		float pw = g_bulletAni[g_Bullet[i].mode].w;		// �o���b�g�̕\����
		float ph = g_bulletAni[g_Bullet[i].mode].h;		// �o���b�g�̕\������

		int divideX = g_bulletAni[g_Bullet[i].mode].divideX;
		int divideY = g_bulletAni[g_Bullet[i].mode].divideY;

		float tw = 1.0f / (float)divideX;	// �e�N�X�`���̕�
		float th = 1.0f / (float)divideY;	// �e�N�X�`���̍���
		float tx = (float)(g_Bullet[i].patternAnim % divideX) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(g_Bullet[i].patternAnim / divideX) * th;	// �e�N�X�`���̍���Y���W

		if (g_Bullet[i].dir == BULLET_DIR_LEFT)
		{
			tw *= (-1);
		}
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColorRotation(g_VertexBuffer, 
			px, py, pw, ph, 
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			g_Bullet[i].rot.z);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
		
	}

}


//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
//void SetBullet(XMFLOAT3 pos)
//{
//	PLAYER* player = GetPlayer();
//	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
//	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
//	PlaySound(SOUND_LABEL_SE_skill001);
//	for (int i = 0; i < BULLET_MAX; i++)
//	{
//		if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
//		{
//			g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
//			g_Bullet[i].pos = pos;			// ���W���Z�b�g
//			g_Bullet[i].mode = BULLET_1;			// ���W���Z�b�g
//			g_Bullet[i].texNo = 0;
//			g_Bullet[i].w = 150;
//			g_Bullet[i].h = 150;
//			g_Bullet[i].countAnim = 0;
//			g_Bullet[i].patternAnim = 0;
//
//			if (player[0].dir == CHAR_DIR_LEFT)
//			{
//				g_Bullet[i].dir = BULLET_DIR_LEFT;
//			}
//			if (player[0].dir == CHAR_DIR_RIGHT)
//			{
//				g_Bullet[i].dir = BULLET_DIR_RIGHT;
//			}
//			player[0].bullet = FALSE;
//
//			return;							// 1���Z�b�g�����̂ŏI������
//		}
//	}
//}

void SetBullet(XMFLOAT3 pos, BOOL mode)
{

	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	PLAYER* player = GetPlayer();

	switch (mode)
	{
	case BLACKHOLE:
		PlaySound(SOUND_LABEL_SE_gravityBall);

		// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
			{
				g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
				g_Bullet[i].pos = pos;			// ���W���Z�b�g
				g_Bullet[i].mode = mode;			// ���W���Z�b�g
				g_Bullet[i].texNo = mode;
				g_Bullet[i].w = g_bulletAni[mode].w;
				g_Bullet[i].h = g_bulletAni[mode].h;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				if (player[0].dir == CHAR_DIR_LEFT)
				{
					g_Bullet[i].dir = BULLET_DIR_LEFT;
				}
				if (player[0].dir == CHAR_DIR_RIGHT)
				{
					g_Bullet[i].dir = BULLET_DIR_RIGHT;
				}

				return;							// 1���Z�b�g�����̂ŏI������
			}
		}

		break;
	case BULLET_1:

		PlaySound(SOUND_LABEL_SE_bullethp);

		// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
			{
				g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
				g_Bullet[i].pos = pos;			// ���W���Z�b�g
				g_Bullet[i].mode = mode;			// ���W���Z�b�g
				g_Bullet[i].texNo = mode;
				g_Bullet[i].w = g_bulletAni[mode].w;
				g_Bullet[i].h = g_bulletAni[mode].h;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;

				if (player[0].dir == CHAR_DIR_LEFT)
				{
					g_Bullet[i].dir = BULLET_DIR_LEFT;
				}
				if (player[0].dir == CHAR_DIR_RIGHT)
				{
					g_Bullet[i].dir = BULLET_DIR_RIGHT;
				}

				return;							// 1���Z�b�g�����̂ŏI������
			}
		}
		break;
	}

	




}

void SetBullet(XMFLOAT3 pos,BOOL mode,int dir, XMFLOAT3 initMove)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();

	//player[0].state = PLAYER_STATE_ATTACK_BULLET;
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	switch (mode)
	{
	case BULLET_MODE_ENEMY:
		PlaySound(SOUND_LABEL_SE_enemyskill01);
		break;
	case BULLET_MODE_HP:
		PlaySound(SOUND_LABEL_SE_bullethp);
		break;
	}

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Bullet[i].mode = mode;

			switch (mode)
			{
			case BULLET_MODE_ENEMY:

				g_Bullet[i].texNo = 1;
				g_Bullet[i].mode = BULLET_MODE_ENEMY;
				g_Bullet[i].gravityCnt = 0;
				g_Bullet[i].w = 80;
				g_Bullet[i].h = 80;
				g_Bullet[i].pos = pos;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				//�v���C���[�̓G�l�~�[�̍���
				g_Bullet[i].dir = DisBulletDir();

				//if (player[0].pos.x <= enemy[ENEMY_MAX - 1].pos.x) g_Bullet[i].dir = BULLET_DIR_LEFT;
				////�v���C���[�̓G�l�~�[�̉E��
				//else  if (player[0].pos.x > enemy[ENEMY_MAX - 1].pos.x)g_Bullet[i].dir = BULLET_DIR_RIGHT;

				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					XMFLOAT3 tempW = XMFLOAT3(-g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);

					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// ���W���Z�b�g
					g_Bullet[i].initMove = XMFLOAT3(-initMove.x, initMove.y, initMove.z);
				}
				else
				{
					XMFLOAT3 tempW = XMFLOAT3(g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);
					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// ���W���Z�b�g

					g_Bullet[i].initMove = XMFLOAT3(initMove.x, initMove.y, initMove.z);
				}
				
				break;
			case BULLET_MODE_HP:

				g_Bullet[i].texNo = 3;
				g_Bullet[i].mode = BULLET_MODE_HP;
				g_Bullet[i].gravityCnt = 0;
				g_Bullet[i].w = 60;
				g_Bullet[i].h = 60;
				g_Bullet[i].pos = pos;
				g_Bullet[i].countAnim = 0;
				g_Bullet[i].patternAnim = 0;
				//�v���C���[�̓G�l�~�[�̍���
				g_Bullet[i].dir = DisBulletDir();

				//if (player[0].pos.x <= enemy[ENEMY_MAX - 1].pos.x) g_Bullet[i].dir = BULLET_DIR_LEFT;
				////�v���C���[�̓G�l�~�[�̉E��
				//else  if (player[0].pos.x > enemy[ENEMY_MAX - 1].pos.x)g_Bullet[i].dir = BULLET_DIR_RIGHT;

				if (g_Bullet[i].dir == BULLET_DIR_LEFT)
				{
					XMFLOAT3 tempW = XMFLOAT3(-g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);

					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// ���W���Z�b�g
					g_Bullet[i].initMove = XMFLOAT3(-initMove.x, initMove.y, initMove.z);
				}
				else
				{
					XMFLOAT3 tempW = XMFLOAT3(g_Bullet[ENEMY_MAX - 1].w, 0.0f, 0.0f);
					XMVECTOR vtempW = XMLoadFloat3(&tempW);
					XMVECTOR vtempBp = XMLoadFloat3(&g_Bullet[i].pos);

					XMStoreFloat3(&g_Bullet[i].pos, vtempBp + vtempW);
					//g_Bullet[i].pos = enemy[ENEMY_MAX - 1].pos;			// ���W���Z�b�g

					g_Bullet[i].initMove = XMFLOAT3(initMove.x, initMove.y, initMove.z);
				}

				break;
			}
			return;							// 1���Z�b�g�����̂ŏI������

		}
	}
}

//�e�̔��˂̓v���C���[���G�l�~�[
void SetBulletMode(int num,BOOL	mode)
{
	g_Bullet[num].mode = mode;
}

//�G�̍U���ڕW��ݒ肷��
void SetTarget(XMFLOAT3 pos)
{
	PLAYER* player = GetPlayer();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Bullet[i].initMove = pos;
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

void SetNuma(XMFLOAT3 pos)
{
	for (int i = 0; i < NUMA_MAX; i++)
	{
		if (g_Numa[i].use == FALSE)
		{
			g_Numa[i].use = TRUE;
			g_Numa[i].pos = pos;
			return;
		}
	}
}
void UpdateNuma(void)
{
	PLAYER* player = GetPlayer();
	for (int i = 0; i < NUMA_MAX; i++)
	{
		for (int j = 0; j < PLAYER_MAX; j++)
		{
			if (g_Numa[i].use == TRUE)
			{

				if (g_Numa[i].countAnim++ > NUMA_WAIT_MAX)
				{
					g_Numa[i].countAnim = 0;
					// �p�^�[���̐؂�ւ�
					g_Numa[i].patternAnim = (g_Numa[i].patternAnim + 1) % NUMA_PATTERN_NUM;
				}

				g_Numa[i].timer += GetDeltatime();
				float sec = 5.0;
				if (g_Numa[i].timer >= sec * 1000)
				{

					g_Numa[i].timer = 0;
					g_Numa[i].use = FALSE;

				}
				if (CollisionBB(player[j].pos, player[j].w, player[j].h, g_Numa[i].pos, g_Numa[i].w / 2 - 10.0f, g_Numa[i].h))
				{
					if (player[j].hitting == TRUE)
					{
						break;
					}
					else
					{
						player[j].hit = TRUE;


						#ifndef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
						AddPlayerHP(j,-5);
						#endif
						
					}
				}
			}
		}
	
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawNuma(void)
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
	PLAYER* player = GetPlayer();
	for (int i = 0; i < NUMA_MAX; i++)
	{
		if (g_Numa[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Numa[i].texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Numa[i].pos.x - bg->pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Numa[i].pos.y - bg->pos.y;	// �o���b�g�̕\���ʒuY
			float pw = g_Numa[i].w;		// �o���b�g�̕\����
			float ph = g_Numa[i].h;		// �o���b�g�̕\������

			

			float tw = 1.0f / NUMA_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / NUMA_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = tw * (g_Numa[i].patternAnim % NUMA_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Numa[i].patternAnim / NUMA_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W

			
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