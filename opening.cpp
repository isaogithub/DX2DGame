//=============================================================================
//
// BG���� [bg.cpp]
// Author : 
//
//=============================================================================

#include "input.h"
#include "fade.h"
#include "main.h"
#include "opening.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


#define TEXTURE_WIDTH				(1920)			// (SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(1080)			// (SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(OPENING_MAX + 2)				// �e�N�X�`���̐�
#define FADE_RATE					(0.02f);

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���

//static ID3D11ShaderResourceView* g_Tutorial[TEXTURE_TUTORIAL_MAX] = { NULL };	// �e�N�X�`�����

static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

//static char* g_TutorialName[TEXTURE_TUTORIAL_MAX] = {
//	"data/TEXTURE/BG_mori001.png",
//};


static char *g_TexturName[] = {
	"data/Opening/opening001.PNG",
	"data/Opening/opening002.PNG",
	"data/Opening/opening003.PNG",
	"data/Opening/opening004.PNG",
	"data/Opening/opening_white.png",
	"data/TEXTURE/PRESS.png",

};


static BOOL	 g_Load = FALSE;		// ���������s�������̃t���O
static int   g_OpeningNum;
static int   g_OpFade;
static float g_OpAlpha;
static float g_keyAlpha;
static BOOL	 g_Clear;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitOpening(void)
{
	//�O���[�o���ϐ�������
	g_OpeningNum = OPENING_NUM001;
	g_OpFade = FADE_IN;
	g_OpAlpha = 0.0f;
	g_keyAlpha = 0.0f;
	g_Clear = FALSE;
	//
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

		

		// �ϐ��̏�����
		

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitOpening(void)
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
void UpdateOpening(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{// Enter��������A�X�e�[�W��؂�ւ���

		PlaySound(SOUND_LABEL_SE_openingselected);
		if (g_OpFade == FADE_NONE)
		{

			g_OpeningNum++;
			g_OpAlpha = 0.0f;
			//g_OpFade = FADE_IN;
			if (g_OpeningNum == OPENING_MAX)
			{
				SetFade(FADE_OUT, MODE_TUTORIAL);

			}
			else SetOpening(g_OpeningNum);		

			g_keyAlpha = 0.0f;
			g_Clear = FALSE;
		}
	}

	if (g_OpFade != FADE_NONE)
	{// �t�F�[�h������
		if (g_OpFade == FADE_OUT)
		{// �t�F�[�h�A�E�g����
			g_OpAlpha -= FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���

			
			if (g_OpAlpha <= 0.0f)
				{
					// �t�F�[�h�����I��
					g_OpAlpha = 0.0f;
				SetOpFade(FADE_IN);
				//SetOpening(g_OpeningNum);
				

			}

		}
		else if (g_OpFade == FADE_IN)
		{// �t�F�[�h�C������
			g_OpAlpha += FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
		
			if (g_OpAlpha >= 1.0f)
			{
				// ���Ă���Ȃ�S���~�߂�
				// �t�F�[�h�C�������ɐ؂�ւ�
				g_OpAlpha = 1.0f;
				SetOpFade(FADE_NONE);
				
			}

		}
	}
	if (g_OpFade == FADE_NONE)
	{
		//�G���^�[�L�[
		int temp = 0.01f;
		if (g_keyAlpha > 1.0f)		g_Clear = TRUE;
		else if (g_keyAlpha < 0.0f)	g_Clear = FALSE;
		
		if (g_Clear)
		{
			g_keyAlpha -= 0.01f;
		}
		else g_keyAlpha += 0.01f;
		
	}
	//float speed = g_Enemy[i].move.x;
				//g_Enemy[i].pos.x += speed;

				//MAP�̉E�[�܂ŗ����H
				//if (g_Enemy[i].pos.x > bg->w)
				//{
				//	g_Enemy[i].pos.x = bg->w;
				//	g_Enemy[i].move.x = 0 - g_Enemy[i].move.x;
				//}


				////MAP�̍��[�܂ŗ����H
				//if (g_Enemy[i].pos.x  < 0.0f)
				//{
				//	g_Enemy[i].pos.x = 0.0f;
				//	g_Enemy[i].move.x *= (-1);

				//}



	/*switch (GetMode())
	{
	case MODE_TUTORIAL:

		g_BG.scrl -= 0.005f;
		break;
	case MODE_GAME:
		break;

	default:
		break;
	}*/


	//g_BG.scrl -= 0.0f;		// 0.005f;		// �X�N���[��


#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawOpening(void)
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

	{//�w�i
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_MAX]);
		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			0,
			0,
			TEXTURE_WIDTH,
			TEXTURE_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	{
		float tw = 500.0f,th;
		switch (g_OpeningNum)
		{// �e�N�X�`���ݒ�
		case OPENING_NUM001:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM001]);
			tw = 540.0f;
			break;

		case OPENING_NUM002:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM002]);
			tw = 991.0f;
			break;

		case OPENING_NUM003:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM003]);
			tw = 591.0f;
			break;

		case OPENING_NUM004:
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[OPENING_NUM004]);
			tw = 700.0f;
			break;

		}
		th = 54.0f;

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			960,
			540,
			tw,
			th,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, g_OpAlpha));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
	if (g_OpFade == FADE_NONE)
	{
		{//�G���^�[�L�[
		// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer,
				SCREEN_CENTER_X + 300,
				900,
				600,
				80,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(0.0f, 0.0f, 0.0f, g_keyAlpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
	

}


//=============================================================================
// �I�[�v���j���O�ԍ��ݒ�
//=============================================================================
void SetOpening(int num)
{
	g_OpeningNum = num;
	g_OpFade = FADE_OUT;

}

void SetOpFade(int fade)
{
	g_OpFade = fade;
}




