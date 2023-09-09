//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "collision.h"
#include "score.h"
#include "field.h"
#include "slime.h" 

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(128)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(128)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X * TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {

	"data/CHARA/test_slime.png",
};


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static ENEMY	g_Slime[SLIME_MAX];	// �G�l�~�[�̍\����
static float g_gravityCnt = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSlime(void)
{
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

	

	

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < SLIME_MAX; i++)
	{

		g_Slime[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);			// �|���S���̍��W
		g_Slime[i].opos = g_Slime[i].pos;
		g_Slime[i].nextpos = XMFLOAT3(0.0f, 0.0f, 0.0f);	//�v���C���[�̈ʒu

		g_Slime[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);;			// �|���S���̉�]��
		g_Slime[i].scl = XMFLOAT3(0.0f, 0.0f, 0.0f);;			// �|���S���̊g��k��
		g_Slime[i].use = TRUE;			// true:�g���Ă���  false:���g�p
		g_Slime[i].w = TEXTURE_WIDTH;			// ���ƍ���
		g_Slime[i].h = TEXTURE_HEIGHT;			// ���ƍ���

		g_Slime[i].countAnim  = 0;		// �A�j���[�V�����J�E���g
		g_Slime[i].patternAnim = 0;	// �A�j���[�V�����p�^�[���i���o�[
		g_Slime[i].texNo = 0;			// �e�N�X�`���ԍ�

		g_Slime[i].hp = 100;				// �G�l�~�[��HP
		g_Slime[i].hitCnt = 0;			// �����蔻��J�E���^
		g_Slime[i].hit = FALSE;			// ���Ă�ꂽ
		g_Slime[i].hitting = FALSE;		// ���Ă�ꂽ�r��
		g_Slime[i].clear = FALSE;			// �����t���O
		g_Slime[i].onGround = FALSE;

		g_Slime[i].str = 5;			// �U����
		g_Slime[i].dir = ENEMY_DIR_RIGHT;			//�G�l�~�[����
		g_Slime[i].move = XMFLOAT3( 3.0 , 0.0 , 0.0 );			// �ړ����x
		g_Slime[i].amove = XMFLOAT3(3.0 , 0.0 , 0.0);			// �v���C���[�����������x

		g_Slime[i].time = 0;			// ���`��ԗp
		g_Slime[i].tblNo = 0;			// �s���f�[�^�̃e�[�u���ԍ�
		g_Slime[i].tblMax = 0;			// ���̃e�[�u���̃f�[�^��

	}
	
	g_Slime[0].pos = XMFLOAT3( 600.0f, 400.0f, 0.0f);
	g_Slime[1].pos = XMFLOAT3(1200.0f, 400.0f, 0.0f);
	g_Slime[2].pos = XMFLOAT3(1800.0f, 400.0f, 0.0f);
	g_Slime[3].pos = XMFLOAT3(2400.0f, 400.0f, 0.0f);
	g_Slime[4].pos = XMFLOAT3(3000.0f, 400.0f, 0.0f);

	

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSlime(void)
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

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSlime(void)
{
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// �����Ă�G�l�~�[�̂�������������
		if (g_Slime[i].use == TRUE)
		{
			// �n�`�Ƃ̓����蔻��p�ɍ��W�̃o�b�N�A�b�v������Ă���
			g_Slime[i].opos = g_Slime[i].pos;


			// �A�j���[�V����  
			g_Slime[i].countAnim += 1.0f;
			if (g_Slime[i].countAnim > ANIM_WAIT)
			{
				g_Slime[i].countAnim = 0.0f;
				// �p�^�[���̐؂�ւ�
				g_Slime[i].patternAnim = (g_Slime[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			g_Slime[i].rot.z -= 0.0f;
			
			float speed;


			BG* bg = GetBG();
			{
				float speed = g_Slime[i].move.x;
				//g_Slime[i].pos.x += speed;

				//MAP�̉E�[�܂ŗ����H
				if (g_Slime[i].pos.x > MAP_WIDTH)
				{
					g_Slime[i].pos.x = MAP_WIDTH;
					g_Slime[i].move.x = 0 - g_Slime[i].move.x;
				}


				//MAP�̍��[�܂ŗ����H
				if (g_Slime[i].pos.x  < 0.0f)
				{
					g_Slime[i].pos.x = 0.0f;
					g_Slime[i].move.x *= (-1);

				}
			}

			{
		
				// MAP�O�`�F�b�N
				//BG* bg = GetBG();

				if (g_Slime[i].pos.x < 0.0f)
				{
					g_Slime[i].pos.x = 0.0f;
				}

				if (g_Slime[i].pos.x > MAP_WIDTH)
				{
					g_Slime[i].pos.x = MAP_WIDTH;
				}

				if (g_Slime[i].pos.y < 0.0f)
				{
					g_Slime[i].pos.y = 0.0f;
				}

				if (g_Slime[i].pos.y > MAP_HEIGHT)
				{
					g_Slime[i].pos.y = MAP_HEIGHT;
				}

				// �ړ����I�������o���b�g�Ƃ̓����蔻��
				
				PLAYER* player = GetPlayer();
				for (int j = 0; j < PLAYER_MAX; j++)
				{
					if (player[j].use == TRUE)
					{//�����Ă���G�l�~�[�Ƃ�����͂�Ă��������Ȃ�

						BOOL ans = CollisionBC(
							g_Slime[i].pos,
							player[j].pos,
							g_Slime[i].w / 2,
							player[j].w / 2);
						if (ans == TRUE)
						{
							g_Slime[i].use = FALSE;
							
							break;
						}
					}
				}
				//�d��
				if (g_Slime[i].onGround == FALSE)
				{
					g_gravityCnt += GRAVITY;
					float acc = g_gravityCnt * 5;

					g_Slime[i].pos.y += g_gravityCnt * (acc * acc);
				}
				else
				{
					g_gravityCnt = 0;
				}

			}
		}
	}

#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSlime(void)
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

	for (int i = 0; i < SLIME_MAX; i++)
	{
		if (g_Slime[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Slime[i].pos.x - bg->pos.x;	// �G�l�~�[�̕\���ʒuX
			float py = g_Slime[i].pos.y - bg->pos.y;	// �G�l�~�[�̕\���ʒuY
			float pw = g_Slime[i].w;		// �G�l�~�[�̕\����
			float ph = g_Slime[i].h;		// �G�l�~�[�̕\������

			 //�A�j���[�V�����p
			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Slime[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Slime[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Slime[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}


}


//=============================================================================
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetSlime(void)
{
	return &g_Slime[0];
}




