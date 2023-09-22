//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "ui.h"
#include "bg.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(300)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(300)	// 
#define TEXTURE_MAX					(UI_MAX + 3)
#define GAUGE_WIDTH			(220)
#define GAUGE_HEIGHT		(30)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[TEXTURE_MAX] = {

		"data/TEXTURE/blade1.png",
		"data/TEXTURE/bullet1.png",
		"data/TEXTURE/bullet2.png",
		"data/TEXTURE/guard.png",
		"data/TEXTURE/ult1.png",
		"data/TEXTURE/jump1.png",
		"data/TEXTURE/heroicon.png",
		"data/TEXTURE/bar_white.png",
		"data/TEXTURE/state.png",
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static UI		g_SkillUI[UI_MAX];
static PLAYERICON	g_PlayerIcon;
static XMFLOAT3 g_Clickpos;
static int		g_SkillNum;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitUI(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�O���[�o���ϐ�������
	g_Clickpos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_SkillNum = 0;
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	
	InitSkillUI();
	InitPlayerIconUI();

	g_Load = TRUE;
	return S_OK;
}

void InitSkillUI(void)
{
	for (int i = 0; i < UI_MAX; i++)
	{
		g_SkillUI[i].use = TRUE;			// true:�g���Ă���  false:���g�p
		g_SkillUI[i].texNo = i;			// true:�g���Ă���  false:���g�p
		g_SkillUI[i].w = 200;			// true:�g���Ă���  false:���g�p
		g_SkillUI[i].h = 200;			// true:�g���Ă���  false:���g�p
		g_SkillUI[i].texNo = i;			// true:�g���Ă���  false:���g�p
		g_SkillUI[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_SkillUI[i].time = 0;
		g_SkillUI[i].r = g_SkillUI[i].w * 0.3;
	}

	g_SkillUI[UI_BLADE].pos = XMFLOAT3(1310.0f, 880.0f, 0.0f);
	g_SkillUI[UI_BULLET].pos = XMFLOAT3(1500.0f, 775.0f, 0.0f);
	g_SkillUI[UI_BLACKHOLE].pos = XMFLOAT3(1680.0f, 920.0f, 0.0f);
	g_SkillUI[UI_GUARD].pos = XMFLOAT3(1680.0f, 650.0f, 0.0f);
	g_SkillUI[UI_ULT].pos = XMFLOAT3(1140.0f, 920.0f, 0.0f);
	g_SkillUI[UI_JUMP].pos = XMFLOAT3(1500.0f, 620.0f, 0.0f);

	g_SkillUI[UI_BULLET].w = 320;
	g_SkillUI[UI_BULLET].h = 320;
	g_SkillUI[UI_BULLET].r = 320 * 0.2;

	g_SkillUI[UI_JUMP].w = 150;
	g_SkillUI[UI_JUMP].h = 150;
	g_SkillUI[UI_JUMP].r = 150 * 0.3;
}
void InitPlayerIconUI(void)
{
	g_PlayerIcon.use = FALSE;
	g_PlayerIcon.pos = XMFLOAT3(175.0f, 180.0f, 0.0f);
	g_PlayerIcon.w = 180.0f;
	g_PlayerIcon.h = 180.0f;
	g_PlayerIcon.texNo = 6;
	g_PlayerIcon.theta = 0.0f;

	return;
}


//=============================================================================
// �I������
//=============================================================================
void UninitUI(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < UI_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}
	
	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================

void UpdateUI(void)
{
	UpdateSkillUI();
	UpdatePlayerIconUI();
}


void UpdateSkillUI(void)
{
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();

	{//UI�̍X�V����
		if (player[0].state == PLAYER_STATE_BLADE)SetSkillUIUSE(UI_BLADE, FALSE);
		else SetSkillUIUSE(UI_BLADE, TRUE);

		if (player[0].state == PLAYER_STATE_GUARD)SetSkillUIUSE(UI_GUARD, FALSE);
		else SetSkillUIUSE(UI_GUARD, TRUE);

		if (player[0].jumptimes <= 1)SetSkillUIUSE(UI_JUMP, FALSE);
		else SetSkillUIUSE(UI_JUMP, TRUE);

		if (player[0].mp < 1.0f || player[0].state == PLAYER_STATE_BULLET)SetSkillUIUSE(UI_BULLET, FALSE);
		else SetSkillUIUSE(UI_BULLET, TRUE);

		if (player[0].mp < 35.0f || player[0].state == PLAYER_STATE_BLACKHOLE)SetSkillUIUSE(UI_BLACKHOLE, FALSE);
		else SetSkillUIUSE(UI_BLACKHOLE, TRUE);

		if (player[0].mp < PLAYER_MP_MAX || player[0].state == PLAYER_STATE_ULT)SetSkillUIUSE(UI_ULT, FALSE);
		else SetSkillUIUSE(UI_ULT, TRUE);

	}


	{//�}�E�X����

		g_Clickpos = XMFLOAT3((float)GetMouseX(), (float)GetMouseY(), 0.0f);
		BOOL ans = CollisionBC(g_SkillUI[UI_GUARD].pos, g_Clickpos, g_SkillUI[UI_GUARD].r, 1);
		if (IsMouseLeftPressed() && ans == TRUE)
		{
			g_SkillNum = UI_GUARD;
			return;

		}

		if (!IsMouseLeftPressed() && ans)
		{
			g_SkillNum = -1;
			return;
		}


		if (IsMouseLeftTriggered())
		{
			for (int i = 0; i < UI_MAX; i++)
			{
				if (CollisionBC(g_SkillUI[i].pos, g_Clickpos, g_SkillUI[i].r, 1) && i != UI_GUARD)
				{
					g_SkillNum = i;
					return;
				}
			}
		}

		g_SkillNum = -1;
		//if (IsMouseLeftPressed())
		//{
		//	g_Clickpos = XMFLOAT3((float)GetMouseX(), (float)GetMouseY(), 0.0f);
		//	
		//	if (CollisionBC(g_UI[UI_GUARD].pos, g_Clickpos, g_UI[UI_GUARD].r,0.1))
		//	{
		//		g_SkillNum = UI_GUARD;
		//		return;
		//	}
		//}




	}

}
void UpdatePlayerIconUI(void)
{
	PLAYER* player = GetPlayer();
	if (player[0].hitting == TRUE)g_PlayerIcon.use = TRUE;
	else g_PlayerIcon.use = FALSE;

	if (g_PlayerIcon.use == FALSE)
	{
		g_PlayerIcon.theta = 0.0f;
		return;
	}

	g_PlayerIcon.theta += 1.5f;


}


void DrawUI(void)
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


	DrawSkillUI();
	DrawPlayerState();
	DrawPlayerIconUI();

}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkillUI(void)
{

	for (int i = 0; i < UI_MAX; i++)
	{
		{//�v���C���[�\��
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_SkillUI[i].texNo]);	//0 : STAND, 1:walk 2

			//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_SkillUI[i].pos.x;	// �v���C���[�̕\���ʒuX
			float py = g_SkillUI[i].pos.y;	// �v���C���[�̕\���ʒuY
			float pw = g_SkillUI[i].w;		// �v���C���[�̕\����
			float ph = g_SkillUI[i].h;		// �v���C���[�̕\������

			
			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = 0.0f;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			float Alpha = 1.0f;

			if (g_SkillUI[i].use == FALSE) Alpha = 0.6;
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0, 1.0, 1.0, Alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
			
			//�U���\��
			//DrawAttackEffect();
	}

	//DrawUISkill();
}

void  DrawPlayerIconUI(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_PlayerIcon.texNo]);

	float tx = g_PlayerIcon.pos.x;
	float ty = g_PlayerIcon.pos.y;
	float tw = g_PlayerIcon.w;
	float th = g_PlayerIcon.h;

	float u = 0.0f;
	float v = 0.0f;
	float uw = 1.0f;
	float vh = 1.0f;

	float R = 1.0f;
	float G = 1.0f;
	float B = 1.0f;
	float Alpha = 1.0f;

	if (g_PlayerIcon.use == TRUE)
	{
		G = 0.0f, B = 0.0f, Alpha = 0.7f;

		tx += 10 * sin(g_PlayerIcon.theta);
		/*	ty += 5 * cos(g_PlayerIcon.theta);*/
		tw = g_PlayerIcon.w * (1 + 0.025 * sin(g_PlayerIcon.theta));
		th = g_PlayerIcon.h * (1 + 0.025 * sin(
			g_PlayerIcon.theta));
	}
	XMFLOAT4 color = XMFLOAT4(R, G, B, Alpha);

	SetSpriteColorRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, color, 0.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);


}

//�v���C���[��ԕ\��
void DrawPlayerState(void)
{
	DrawPlayerHP();
	DrawPlayerMP();
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = 60.0f;		// �Q�[�W�̕\���ʒuX
		float py = -10.0f;		// �Q�[�W�̕\���ʒuY
		float pw = 480.0f;	// �Q�[�W�̕\����
		float ph = 405.0f;		// �Q�[�W�̕\������

		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;	// �e�N�X�`���̍���
		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}

//�v���C���[��HP�\��
void DrawPlayerHP(void)
{
	// HP�̕\��
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = 263.0f;		// �Q�[�W�̕\���ʒuX
		float py = 135.0f;		// �Q�[�W�̕\���ʒuY
		float pw = 220;	// �Q�[�W�̕\����
		float ph = 30.0f;		// �Q�[�W�̕\������

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
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		pw = pw * ((float)player[0].hp / PLAYER_HP_MAX);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


	}

}

//�v���C���[��MP�\��
void DrawPlayerMP(void)
{
	// HP�̕\��
	BG* bg = GetBG();
	PLAYER* player = GetPlayer();
	{
		// ���~���̃Q�[�W�i�g�I�ȕ��j
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = 263.0f;		// �Q�[�W�̕\���ʒuX
		float py = 200.0f;		// �Q�[�W�̕\���ʒuY
		float pw = GAUGE_WIDTH;	// �Q�[�W�̕\����
		float ph = GAUGE_HEIGHT;		// �Q�[�W�̕\������

		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;	// �e�N�X�`���̍���
		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W

		float R = 0.0f;
		float G = 0.3f;
		float B = 0.8f;
		float Alpha = 1.0f;

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

		//�Q�[�W�̈ʒu��e�N�X�`���[���W�𔽉f
		float temp = ((float)player[0].mp / PLAYER_MP_MAX);
		pw = pw * temp;
		if (pw >= GAUGE_WIDTH)
		{
			R = 0.11765f;
			G = 0.941f;
			B = 1.0f;
		}
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(R, G, B, Alpha));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);


	}

}



UI* GetSkillUI(void)
{
	return &g_SkillUI[0];
}

void SetSkillUIUSE(int type,BOOL tf)
{
	g_SkillUI[type].use = tf;
	return;
}

int	GetSkillUIClicked()
{

	if(g_SkillNum != UI_GUARD)return g_SkillNum;
	/*if (type == g_SkillNum)return TRUE;
	return FALSE;*/
}

int GetSkillUIPressed()
{
	return g_SkillNum;
}

BOOL isPressGuard(void)
{
	if(g_SkillNum == UI_GUARD)return TRUE;
	return FALSE;
}





