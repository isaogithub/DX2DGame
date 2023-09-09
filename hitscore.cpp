//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "enemy.h"
#include "player.h"
#include "hitscore.h"
#include "bg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(60)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(60)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define SCORE_MAX					(20)

#define NUM_PATTERN_DIVIDE_X					(12)
#define NUM_PATTERN_DIVIDE_Y					(1)
#define NUM_ANIM_PATTERN_NUM	( NUM_PATTERN_DIVIDE_X *  NUM_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

#define WAIT_FRAME_MAX				(40)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/number.png",
	"data/TEXTURE/heal_number.png",
};


static SCORE	g_HitScore[SCORE_MAX];
static float	g_Scoretimer;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHitScore(void)
{
	g_Scoretimer = 0.0f;


	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
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


	// �v���C���[�̏�����
	for (int i = 0; i < SCORE_MAX; i++)
	{
		g_HitScore[i].use = FALSE;
		g_HitScore[i].w = TEXTURE_WIDTH;
		g_HitScore[i].h = TEXTURE_HEIGHT;
		g_HitScore[i].pos = { 0.0f, 0.0f, 0.0f };
		g_HitScore[i].scl = { 1.0f, 1.0f, 1.0f };
		g_HitScore[i].texNo = 0;
		g_HitScore[i].score = 0;	// �X�R�A�̏�����
		g_HitScore[i].g_ScoreframeCnt = 0;	// �X�R�A�̏�����
		g_HitScore[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// �X�R�A�̏�����
	}



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHitScore(void)
{
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
void UpdateHitScore(void)
{


	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (g_HitScore[i].use)
		{
			if (g_HitScore[i].g_ScoreframeCnt++ >= WAIT_FRAME_MAX)
			{
				g_HitScore[i].g_ScoreframeCnt = 0;
				g_HitScore[i].use = FALSE;
			}
			float Alpha = 1.0f - (float)g_HitScore[i].g_ScoreframeCnt / WAIT_FRAME_MAX;
			g_HitScore[i].color.w = Alpha;
			

		}
	}

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHitScore(void)
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

	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (g_HitScore[i].use)
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			ENEMY* enemy = GetEnemy();
			PLAYER* player = GetPlayer();
			BG* bg = GetBG();
			// ��������������

			int number = g_HitScore[i].score;

			{
				// ����\�����錅�̐���

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_HitScore[i].pos.x - bg->pos.x - g_HitScore[i].w * 3;	// �X�R�A�̕\���ʒuX
				float py = g_HitScore[i].pos.y - bg->pos.y - g_HitScore[i].g_ScoreframeCnt * 5;			// �X�R�A�̕\���ʒuY
				float pw = g_HitScore[i].w * g_HitScore[i].scl.x;				// �X�R�A�̕\����
				float ph = g_HitScore[i].h * g_HitScore[i].scl.y;				// �X�R�A�̕\������

				float tw = 1.0f / NUM_PATTERN_DIVIDE_X;		// �e�N�X�`���̕�
				float th = 1.0f / NUM_PATTERN_DIVIDE_Y;		// �e�N�X�`���̍���
				float tx;
				if (number <= 0) tx = tw * 11;			// �e�N�X�`���̍���X���W
				else tx = tw * 10;
				float ty = 0.0f;			// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_HitScore[i].color);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);

				// ���̌���
			}


			number = abs(number);

			for (int j = 0; j < HITSCORE_DIGIT; j++)
			{

				// ����\�����錅�̐���
				float x = (float)(number % 10);

				// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = g_HitScore[i].pos.x - bg->pos.x - g_HitScore[i].w * j;	// �X�R�A�̕\���ʒuX
				float py = g_HitScore[i].pos.y - bg->pos.y - g_HitScore[i].g_ScoreframeCnt * 5;			// �X�R�A�̕\���ʒuY
				float pw = g_HitScore[i].w * g_HitScore[i].scl.x;				// �X�R�A�̕\����
				float ph = g_HitScore[i].h * g_HitScore[i].scl.y;				// �X�R�A�̕\������

				float tw = 1.0f / NUM_PATTERN_DIVIDE_X;		// �e�N�X�`���̕�
				float th = 1.0f / NUM_PATTERN_DIVIDE_Y;		// �e�N�X�`���̍���
				float tx = x * tw;			// �e�N�X�`���̍���X���W
				float ty = 0.0f;			// �e�N�X�`���̍���Y���W

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				float Alpha = 1.0f - (float)g_HitScore[i].g_ScoreframeCnt / WAIT_FRAME_MAX;
				SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
					g_HitScore[i].color);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);

				// ���̌���
				number /= 10;
			}

			
		}
		
	}
	

		
	
	
}


void SetHitScore(int score,XMFLOAT3 pos,int type)
{
	for (int i = 0; i < SCORE_MAX; i++)
	{
		if (!g_HitScore[i].use)
		{
			g_HitScore[i].use = TRUE;
			g_HitScore[i].score = score;
			g_HitScore[i].pos = pos;
			g_HitScore[i].type = type;

			if (g_HitScore[i].type == HEAL)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);


			}
			else if (g_HitScore[i].type == MP1)
			{
				g_HitScore[i].scl = XMFLOAT3(0.7f, 0.7f, 0.7f);
				g_HitScore[i].color = XMFLOAT4(0.1f, 0.8f, 1.0f, 1.0f);
			}
			else if (g_HitScore[i].type == DAMAGE_ENEMY)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.66f, 0.35f, 0.66f, 1.0f);
			}
				

			else if (g_HitScore[i].type == DAMAGE_PLAYER)
			{
				g_HitScore[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(0.95f, 0.1f, 0.0f, 1.0f);
			}

			else if (g_HitScore[i].type == DAMAGE_CRITICAL)
			{
				g_HitScore[i].scl = XMFLOAT3(1.5f, 1.5f, 1.0f);
				g_HitScore[i].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

			}

			return;
		}
	}

}


