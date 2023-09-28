//=============================================================================
//
// BG���� [bg.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"
#include "field.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define TEXTURE_WIDTH				(3840)			// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(1080 * MAP_MAX)			// (SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����


static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg1.png",
	"data/TEXTURE/bg_game.png",
	"data/TEXTURE/bg_boss.png"
};


static BOOL	g_Load = FALSE;		// ���������s�������̃t���O
static BG	g_BG;
static BOOL g_Scroll;

static int  g_BGSFlame;
static int	g_frameCnt;
static float g_MaxX;
static float g_MaxY;

static BOOL	g_shaking;

static int  g_CheckPointNum;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(void)
{
	g_Scroll = FALSE;

	g_BGSFlame = 0;
	g_frameCnt = 0;
	g_MaxX = 0.0f;
	g_MaxY = 0.0f;
	g_CheckPointNum = 0;
	g_shaking = FALSE;
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


	switch (GetMode())
	{
	case MODE_TUTORIAL:
		

		// �ϐ��̏�����
		g_BG.w = 3840.0f;
		g_BG.h = 1080.0f;
		g_BG.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_BG.texNo = 0;

		g_BG.scrl = 0.0f;		// TEX�X�N���[��
		g_BG.scrl2 = 0.0f;		// TEX�X�N���[��

		break;

	case MODE_GAME:
		
		// �ϐ��̏�����
		g_BG.w = 4000.0f;
		g_BG.h = 3500.0f;
		g_BG.pos = XMFLOAT3(0.0f, 80.0f, 0.0f);
		g_BG.texNo = 1;

		g_BG.scrl = 0.0f;		// TEX�X�N���[��
		g_BG.scrl2 = 0.0f;		// TEX�X�N���[��
		break;

	case MODE_BOSS:
		
		// �ϐ��̏�����
		g_BG.w = SCREEN_WIDTH;
		g_BG.h = SCREEN_HEIGHT;
		g_BG.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_BG.texNo = 2;

		g_BG.scrl = 0.0f;		// TEX�X�N���[��
		g_BG.scrl2 = 0.0f;		// TEX�X�N���[��
		break;
	}

	


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
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
void UpdateBG(void)
{
	g_BG.old_pos = g_BG.pos;	// �P�t���O�̏���ۑ�

	switch (GetMode())
	{
	case MODE_TUTORIAL:

		//g_BG.scrl -= 0.005f;
		break;
	case MODE_GAME:

		if (g_Scroll == TRUE)
		{
			BGScrool();
		}

		break;
	case MODE_BOSS:

		break;

	default:
		break;
	}


	//g_BG.scrl -= 0.0f;		// 0.005f;		// �X�N���[��


#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
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

	switch (GetMode())
	{

	case MODE_TUTORIAL:
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo] );

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x,
				0 - g_BG.pos.y,
				g_BG.w,
				g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
		break;

	case MODE_GAME:

		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				0 - g_BG.pos.x,
				0 - g_BG.pos.y,
				g_BG.w,
				g_BG.h,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
		break;

	case MODE_BOSS:

	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_BG.texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			0.0f ,
			0.0f ,
			g_BG.w,
			g_BG.h,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	break;

	}

	


	//// ���`��
	//{
	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	//float	tx = (g_BG.pos.x - g_BG.old_pos.x) * ((float)SCREEN_WIDTH / TEXTURE_WIDTH);
	//	//g_BG.scrl += tx * 0.001f;
	//	g_BG.scrl += 0.001f;

	//	SetSpriteLTColor(g_VertexBuffer,
	//		0.0f, 0.0f, SCREEN_WIDTH, SKY_H,
	//		g_BG.scrl, 0.0f, 1.0f, 1.0f,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//	// �|���S���`��
	//	GetDeviceContext()->Draw(4, 0);
	//}

	//{
	//	// �e�N�X�`���ݒ�
	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	//	float	tx = (g_BG.pos.x - g_BG.old_pos.x) * ((float)SCREEN_WIDTH / TEXTURE_WIDTH);
	//	g_BG.scrl2 += tx * 0.01f;
	//	//g_BG.scrl2 += 0.003f;

	//	SetSpriteLTColor(g_VertexBuffer,
	//		0.0f, SKY_H/2, SCREEN_WIDTH, SKY_H,
	//		g_BG.scrl2, 0.0f, 1.0f, 1.0f,
	//		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	//	// �|���S���`��
	//	GetDeviceContext()->Draw(4, 0);
	//}


}


//=============================================================================
// BG�\���̂̐擪�A�h���X���擾
//=============================================================================
BG* GetBG(void)
{
	return &g_BG;
}

void SetBGSFrame(float MaxX,float MaxY,float MaxFrame)
{
	if (g_Scroll == TRUE)return;

	g_Scroll = TRUE;
	g_MaxX = MaxX;
	g_MaxY = MaxY;
	g_BGSFlame = MaxFrame;
	g_frameCnt = MaxFrame;
	g_CheckPointNum++;

	return;
}
void BGScrool(void)
{
	if (g_Scroll == FALSE)return;

	if (g_frameCnt-- > 0)
	{
		float scrollx = g_MaxX / g_BGSFlame;
		float scrolly = g_MaxY / g_BGSFlame;

		g_BG.pos.x += scrollx;
		g_BG.pos.y += scrolly;
	}
	else
	{
		g_frameCnt = 60;
		g_Scroll = FALSE;
	}

	return;
}

//�`�F�b�N�|�C���g
int GetCheckPoint(void)
{
	return g_CheckPointNum;
}

