//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"
#include "bg.h"
#include "bg.h"
#include "player.h"
#include "field.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define EFFECT_TEXTURE_SIZE_X	(100)		// �e�N�X�`���T�C�Y
#define EFFECT_TEXTURE_SIZE_Y	(100)		// ����

#define EFFECT_TEXTURE_PATTERN_DIVIDE_X	(4)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECT_TEXTURE_PATTERN_DIVIDE_Y	(4)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define EFFECT_ANIM_PATTERN_NUM			(EFFECT_TEXTURE_PATTERN_DIVIDE_X*EFFECT_TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define EFFECT_TIME_ANIMATION			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define EMISSION_FULL 0		//�p�[�e�B�N���S�����t���O
#define EMISSION_RATE 5		//�p�[�e�B�N���̐����Ԋu(duration/EMISSION_RATE�̐����G�t�F�N�g���o��)

#define EMISSION_WIDTH  50	//�p�[�e�B�N�������͈́i�����j
#define EMISSION_HEIGHT 50	//�p�[�e�B�N�������͈́i�����j


#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

//

#define HEAL_WAIT_MAX							(5)
#define HEAL_PATTERN_DIVIDE_X					(10)
#define HEAL_PATTERN_DIVIDE_Y					(1)
#define HEAL_ANIM_PATTERN_NUM	( HEAL_PATTERN_DIVIDE_X *  HEAL_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ResetParticle(int i, int n);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[EFFECT_TYPE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[] = {
	"data/TEXTURE/bomb.png",
	"data/EFFECT/effect_heal.png",
	"data/EFFECT/effect_jump.png",
	"data/EFFECT/effect_hit.png",
	"data/EFFECT/effect_attack002.png",
	"data/EFFECT/effect_guard.png",
	"data/EFFECT/ult01.png",
	"data/EFFECT/effect_ult2.png",
	"data/EFFECT/skill05_dash.png",
	"data/EFFECT/effect_bomb.png",
	"data/EFFECT/effect_hit96.png",
	"data/EFFECT/effect_hit192.png",
	"data/EFFECT/defend.png",
	"data/EFFECT/deadeffect.png",
	"data/EFFECT/effect_mpUp.png",
};


static BOOL		g_Load = FALSE;					// ���������s�������̃t���O
static EFFECT	effectWk[EFFECT_NUM_EFFECTS];	// �G�t�F�N�g�\����
static EFFECT2	g_healeffect;
static float	g_healtimeCnt;


static EFFECT3	g_Effect[EFFECT_MAX];
static ANIMATION g_effectAni[EFFECT_TYPE_MAX] =
{//divideX divideY animewait  w		 h
	{7,		1,		5,		120,	120,},//BOMB
	{7,		1,		5,		120,	120,},//HEAL1
	{7,		1,		5,		120,	120,},//JUMP
	{1,		1,		20,		 96,	96,},//HIT
	{5,		2,		1,		 384,	384,},//BLADE
	{5,		4,		1,		 192,	192,},//GUARD
	{1,		5,		20,		 2560,	1920,},//ULT1
	{1,		18,		10,		 640,	480,},//ULT2
	{1,		1,		1,		 500,	500,},//SKILL05
	{3,		3,		3,		 342,	342,},//BOMB2
	{5,		1,		2,		 96,	96,},//HIT2
	{5,		1,		2,		 192,	192,},//HIT3
	{1,		1,		20,		 500,	500,},//BOSS_DEFEND
	{15,	1,		3,		800,	800,},//BOSS_DeadEffect
	{5,		4,		1,		192,	192,},//BOSS_DeadEffect

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
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


	// ����������
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		effectWk[i].use = false;
		effectWk[i].elapsed = 0;	//�o�ߎ���

		for (int n = 0; n < EFFECT_NUM_PARTS; n++)
		{
			ResetParticle(i, n);
		}
	}
	InitHealEffect();
	InitEffect3();

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �p�[�e�B�N���̃��Z�b�g
//=============================================================================
void ResetParticle(int i, int n) 
{
	effectWk[i].pParticle[n].pos = XMFLOAT3(effectWk[i].pos.x + rand() % EMISSION_WIDTH - (EMISSION_WIDTH/2), effectWk[i].pos.y + rand() % EMISSION_HEIGHT - (EMISSION_HEIGHT/2), 0.0f);	// ���W�f�[�^��������
	effectWk[i].pParticle[n].move = XMFLOAT3(0.0f, 0.0f, 0.0f);					// �ړ���

	effectWk[i].pParticle[n].PatternAnim = 0;									// �A�j���p�^�[���ԍ��������_���ŏ�����
	effectWk[i].pParticle[n].CountAnim = 0;										// �A�j���J�E���g��������
	effectWk[i].pParticle[n].liveTime = 2000;
	effectWk[i].pParticle[n].isFinish = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
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
void UpdateEffect(void)
{
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use)
		{

			if (effectWk[i].isEnding){
				effectWk[i].use = FALSE;
				continue;
			}

			//�G�~�b�g���L���ł���΃G�t�F�N�g�쐬�������s��
			if (effectWk[i].isRemoveOnFinish == FALSE)
			{

				//�G�t�F�N�g�쐬���[�g�̑�������
				if (effectWk[i].effectCount < EFFECT_NUM_PARTS)
					effectWk[i].emitCounter++;

				//�o�b�t�@�ɋ󂫂�����A�ǉ��^�C�~���O�����Ă���ΐV���ȃG�t�F�N�g��ǉ�����
				while ((effectWk[i].effectCount < EFFECT_NUM_PARTS) && (effectWk[i].emitCounter > EMISSION_RATE))
				{
					//�S�̒ǉ��t���O��ON�ł���΋󂫗̈�S�ĂɐV���ȃG�t�F�N�g��ǉ�����
					if (EMISSION_FULL)
						effectWk[i].effectCount = EFFECT_NUM_PARTS;
					else
						effectWk[i].effectCount++;

					//�G�t�F�N�g�쐬���[�g�̏�����
					effectWk[i].emitCounter = 0;
				}

				effectWk[i].elapsed++;

				//���Ԓ���
				if ((effectWk[i].duration != -1) && (effectWk[i].duration < effectWk[i].elapsed)){
					effectWk[i].isRemoveOnFinish = TRUE;
				}
			}

			int effectIndex = 0;

			//�G�t�F�N�g�̍X�V����
			while (effectIndex < effectWk[i].effectCount)
			{
				if (effectWk[i].pParticle[effectIndex].liveTime > 0)
				{
					//�����t���[������
					effectWk[i].pParticle[effectIndex].liveTime--;

					//�A�j���p�^�[���i�s
					if (++effectWk[i].pParticle[effectIndex].CountAnim > EFFECT_TIME_ANIMATION) {
						//�A�j���p�^�[�����ő�l�ɒB�����ꍇ�ł��I��
						if (++effectWk[i].pParticle[effectIndex].PatternAnim >= (EFFECT_ANIM_PATTERN_NUM-1))
						{
							effectWk[i].pParticle[effectIndex].PatternAnim = EFFECT_ANIM_PATTERN_NUM - 1;
							effectWk[i].pParticle[effectIndex].liveTime = 0;
						}

						effectWk[i].pParticle[effectIndex].CountAnim = 0;
					}

					effectIndex++;
				}
				else{
					if (effectWk[i].isRemoveOnFinish){
						if (effectWk[i].pParticle[effectIndex].isFinish == 0)
						{
							effectWk[i].pParticle[effectIndex].isFinish = 1;
							effectWk[i].numFinish++;
						}
						//�I������
						if (effectWk[i].numFinish == effectWk[i].effectCount)
						{
							effectWk[i].isEnding = TRUE;
							break;
						}
						effectIndex++;
					}
					else{
						//�o�b�t�@������������
						ResetParticle(i, effectIndex);

						//�����łȂ���΃C���f�b�N�X���l�߂�
						if (effectIndex != (effectWk[i].effectCount - 1))
						{
							effectWk[i].pParticle[effectIndex] = effectWk[i].pParticle[effectWk[i].effectCount - 1];
							ResetParticle(i, (effectWk[i].effectCount - 1));
						}
						effectWk[i].effectCount--;
					}
				}
			}
		}
	}

	UpdateHealEffect();
	UpdateEffect3();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
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

	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == TRUE)		// ���̃G�t�F�N�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			for (int n = 0; n < effectWk[i].effectCount; n++)
			{
				if (effectWk[i].pParticle[n].isFinish == 0) {
					//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = effectWk[i].pParticle[n].pos.x - bg->pos.x;	// �G�t�F�N�g�̕\���ʒuX
					float py = effectWk[i].pParticle[n].pos.y - bg->pos.y;	// �G�t�F�N�g�̕\���ʒuY
					float pw = EFFECT_TEXTURE_SIZE_X;			// �G�t�F�N�g�̕\����
					float ph = EFFECT_TEXTURE_SIZE_Y;			// �G�t�F�N�g�̕\������

					px -= EFFECT_TEXTURE_SIZE_X / 4;
					py -= EFFECT_TEXTURE_SIZE_Y / 4;

					float tw = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
					float th = 1.0f / EFFECT_TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
					float tx = (float)(effectWk[i].pParticle[n].PatternAnim % EFFECT_TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
					float ty = (float)(effectWk[i].pParticle[n].PatternAnim / EFFECT_TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteColorRotation(g_VertexBuffer,
						px, py, pw, ph,
						tx, ty, tw, th,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						0.0f);

					// �|���S���`��
					GetDeviceContext()->Draw(4, 0);
				}
			}
		}
	}

	DrawHealEffect();
	DrawEffect3();
}


//=============================================================================
// �G�t�F�N�g�̃Z�b�g
// 
// int duration		�G�t�F�N�g�������̐�������
//=============================================================================
void SetEffect(float x, float y, int duration)
{
	// �������g�p�̃G�t�F�N�g��������������s���Ȃ�( =����ȏ�\���ł��Ȃ����Ď� )
	for (int i = 0; i < EFFECT_NUM_EFFECTS; i++)
	{
		if (effectWk[i].use == false)		// ���g�p��Ԃ̃G�t�F�N�g��������
		{
			effectWk[i].use = true;
			effectWk[i].isEnding = false;
			effectWk[i].isRemoveOnFinish = FALSE;

			effectWk[i].duration = duration;
			effectWk[i].pos.x = x;			// ���W���Z�b�g
			effectWk[i].pos.y = y;			// ���W���Z�b�g

			effectWk[i].effectCount = 0;
			effectWk[i].elapsed = 0;
			effectWk[i].emitCounter = EMISSION_RATE;
			effectWk[i].numFinish = 0;

			//�p�[�e�B�N���̏�����
			for (int n = 0; n < EFFECT_NUM_PARTS; n++)
			{
				ResetParticle(i, n);
			}

			return;							// 1�Z�b�g�����̂ŏI������
		}
	}
}


void InitHealEffect(void)
{
	g_healeffect.use = FALSE;
	g_healeffect.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_healeffect.countAnim = 0;
	g_healeffect.patternAnim = 0;
}


void UpdateHealEffect(void)
{
	if (g_healeffect.use)
	{
		PLAYER* player = GetPlayer();
		g_healeffect.pos = player[0].pos;

		float waitMax = 1.0f;
		g_healtimeCnt += GetDeltatime();
		if (g_healtimeCnt >= waitMax * 1000.0f)
		{
			g_healtimeCnt = 0.0f;
			g_healeffect.use = FALSE;
		}


		if (g_healeffect.countAnim++ > HEAL_WAIT_MAX)
		{
			g_healeffect.countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_healeffect.patternAnim = (g_healeffect.patternAnim + 1) % HEAL_ANIM_PATTERN_NUM;
		}

	}
}
void DrawHealEffect(void)
{

	BG* bg = GetBG();

	if (g_healeffect.use == TRUE)		// ���̃v���C���[���g���Ă���H
	{									// Yes
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_healeffect.pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
		float py = g_healeffect.pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
		float pw = 150;		// �v���C���[�̕\����
		float ph = 150;		// �v���C���[�̕\������

		// �A�j���[�V�����p
		//float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		//float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		//float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		//float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

		float tw = 1.0f / HEAL_PATTERN_DIVIDE_X;							// �e�N�X�`���̕�	8�p�^�[��
		float th = 1.0f / HEAL_PATTERN_DIVIDE_Y;								// �e�N�X�`���̍���
		float tx = tw * (g_healeffect.patternAnim % HEAL_PATTERN_DIVIDE_X);		// �e�N�X�`���̍���X���W
		float ty = th * (g_healeffect.patternAnim / HEAL_PATTERN_DIVIDE_X);								// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

void SetHealEffect(XMFLOAT3 pos)
{
	if (g_healeffect.use == FALSE)
	{
		g_healeffect.use = TRUE;
		g_healeffect.pos = pos;

	}
	return;
}





void InitEffect3(void)
{

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].use = FALSE;
		g_Effect[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Effect[i].countAnim = 0;
		g_Effect[i].patternAnim = 0;
		g_Effect[i].type = 0;
		g_Effect[i].effectCnt = 0;
		g_Effect[i].dir = RIGHT;
		g_Effect[i].w = 0.0f;
		g_Effect[i].h = 0.0f;
	}
	
}


void UpdateEffect3(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use)
		{
			int patternMax = g_effectAni[g_Effect[i].type].divideX * g_effectAni[g_Effect[i].type].divideY;
			int waitMax = g_effectAni[g_Effect[i].type].animewait;

			// && g_Effect[i].type != GUARD
			if ((g_Effect[i].effectCnt++ > patternMax * waitMax))
			{
				g_Effect[i].effectCnt = 0;
				g_Effect[i].use = FALSE;
			}
			
			if (g_Effect[i].countAnim++ > waitMax)
			{
				g_Effect[i].countAnim = 0;
				// �p�^�[���̐؂�ւ�
				g_Effect[i].patternAnim = (g_Effect[i].patternAnim + 1) % patternMax;
			}

		}
	}

}
void DrawEffect3(void)
{

	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)		// ���̃v���C���[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect[i].type]);

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Effect[i].pos.x - bg->pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_Effect[i].pos.y - bg->pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_effectAni[g_Effect[i].type].w;		// �v���C���[�̕\����
			float ph = g_effectAni[g_Effect[i].type].h;		// �v���C���[�̕\������

			float tw = 1.0f / g_effectAni[g_Effect[i].type].divideX;							// �e�N�X�`���̕�	8�p�^�[��
			float th = 1.0f / g_effectAni[g_Effect[i].type].divideY;								// �e�N�X�`���̍���
			float tx = tw * (g_Effect[i].patternAnim % g_effectAni[g_Effect[i].type].divideX);		// �e�N�X�`���̍���X���W
			float ty = th * (g_Effect[i].patternAnim / g_effectAni[g_Effect[i].type].divideX);								// �e�N�X�`���̍���Y���W

			float Alpha = 1.0f;
			if (g_Effect[i].dir == LEFT)
			{
				tw *= (-1);
			}
			if (g_Effect[i].type == ULT1 ||
				g_Effect[i].type == SKILL05||
				g_Effect[i].type == BOSS_DEFEND||
				g_Effect[i].type == MPUP)
			{
				Alpha = 0.3f;
			}
			if (g_Effect[i].type == ULT2 || g_Effect[i].type == ULT1)
			{
				if (GetMode() != MODE_BOSS)
				{
					px = g_Effect[i].pos.x;
					py = g_Effect[i].pos.y;
					pw = 1920.0f;
					ph = 1080.0f;
				}
				else
				{
					px = 1920.0f / 2;
					py = 1080.0f / 2;
					pw = 1920.0f;
					ph = 1080.0f;
				}
			}
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, Alpha));
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

}

EFFECT3* GetEffect3(void)
{
	return &g_Effect[0];
}

void SetEffect3(XMFLOAT3 pos,int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)
		{
			g_Effect[i].use = TRUE;

			g_Effect[i].pos = pos;
			g_Effect[i].type = type;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
			g_Effect[i].w = g_effectAni[type].w;
			g_Effect[i].h = g_effectAni[type].h;

			return;

		}
	}
}

void SetEffect3(XMFLOAT3 pos, int type,int dir)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)
		{
			g_Effect[i].use = TRUE;

			g_Effect[i].pos = pos;
			g_Effect[i].type = type;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
			g_Effect[i].dir = dir;
			g_Effect[i].w = g_effectAni[type].w;
			g_Effect[i].h = g_effectAni[type].h;

			return;

		}
	}
}

void ResetEffect3(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].type == type)
		{
			g_Effect[i].use = FALSE;
			g_Effect[i].countAnim = 0;
			g_Effect[i].patternAnim = 0;
		}
	}
}

XMFLOAT3 GetEffect3Pos(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)continue;

		if (g_Effect[i].type == type)
		{
			return g_Effect[i].pos;
		}
	}
}

int GetEffect3Dir(int type)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)continue;

		if (g_Effect[i].type == type)
		{
			return g_Effect[i].dir;
		}
	}
}


