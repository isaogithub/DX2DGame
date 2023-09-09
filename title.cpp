//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : 
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "main.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(1920)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(1080)	// 
#define TEXTURE_MAX					(TITLE_PARTS_MAX + 5)				// �e�N�X�`���̐�


#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

#define TEXTURE_BUTTON_WIDTH		(400)		// ���S�T�C�Y
#define TEXTURE_BUTTON_HEIGHT		(100)		// 

#define TEXTURE_HHAIR_WIDTH			(695)
#define TEXTURE_HHAIR_HEIGHT		(801)

#define TEXTURE_HBODY_WIDTH			(705)
#define TEXTURE_HBODY_HEIGHT		(1080)

#define TEXTURE_HAIR_WIDTH			(900)
#define TEXTURE_HAIR_HEIGHT			(665)

#define TEXTURE_BODY_WIDTH			(1065)
#define TEXTURE_BODY_HEIGHT			(1080)

#define	TITLE_WIDTH					(480)
#define	TITLE_HEIGHT				(80)
#define	TITLE_SPLIT					(20.0f)


#define PI							(3.14159265)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �^�C�g���e�N�X�`�����
static ID3D11ShaderResourceView *g_Button [BUTTON_MAX]  = { NULL };	// BUTTON�e�N�X�`�����


static char *g_TexturName[TEXTURE_MAX] = {
	"data/MenuButtons/start.png",
	"data/MenuButtons/loadgame.png",
	"data/MenuButtons/exit.png",
	"data/TITLE/title_bg.png",
	"data/TITLE/tree1.png",
	"data/TITLE/hbody1.png",
	"data/TITLE/hbody2.png",
	"data/TITLE/left_Oppai.png",
	"data/TITLE/right_oppai.png",
	"data/TITLE/tree2.png",
	"data/TITLE/hair.png",
	"data/TITLE/tree3.png",
	"data/TITLE/body.png",
	"data/TITLE/hair1.png",
	"data/TITLE/title_effect1.png",
	"data/TITLE/title_effect2.png",
	"data/TITLE/title_effectlogo.png",
	"data/TITLE/title_logo.png",
	"data/TEXTURE/PRESS.png",
	"data/TEXTURE/select.png",

	//"data/TEXTURE/effect000.jpg",
};

static char* g_ButtonName[BUTTON_MAX] = {

	"data/MenuButtons/button_unselect.png",
	"data/MenuButtons/button_unselect.png",
	"data/MenuButtons/button_select.png",

};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
static TITLE_PART				g_title[TITLE_PARTS_MAX];

static TITLE_PART				g_MoveOBJ[MOVEOBJ_MAX];
static float clearCnt;
static BOOL	clear;

float	alpha;
BOOL	flag_alpha;

MENU_BUTTON g_menuButton[BUTTON_MAX];
MENU_BUTTON g_moji[BUTTON_MAX];

int g_buttonSelected;

static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;
static float			r;			//���~�̔��a
static float			r2;			//���~�̔��a
//�^�C�g���ϐ�

static int   g_TitleNum;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
{
	// �ϐ��̏�����
	g_buttonSelected = 0;

	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	r = 1.0f;
	r2 = 1.0f;
	g_TitleNum = 0;

	clearCnt = 0.0f;
	clear = FALSE;

	//

	ID3D11Device *pDevice = GetDevice();

	//�^�C�g���e�N�X�`������
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
	//BUTTON�e�N�X�`������
	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_Button[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_ButtonName[i],
			NULL,
			NULL,
			&g_Button[i],
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


	for (int i = 0; i < TITLE_PARTS_MAX; i++)
	{
		g_title[i].pos = XMFLOAT3(0.0f,0.0f,0.0f);
		g_title[i].use = TRUE;
		g_title[i].w = 1920;
		g_title[i].h = 1080;
		g_title[i].texNo = i + 4;
		g_title[i].r = 1.0f;
		g_title[i].r2 = 1.0f;
		g_title[i].theta = 0.0f;
		g_title[i].flipx = FALSE;
		g_title[i].flipy = FALSE;
	}

	g_title[TITLEHHAIR].w = 695;
	g_title[TITLEHHAIR].h = 800;

	g_title[TITLEHAIR].w = TEXTURE_HAIR_WIDTH;
	g_title[TITLEHAIR].h = TEXTURE_HAIR_HEIGHT;

	for (int i = 0; i < MOVEOBJ_MAX; i++)
	{
		g_MoveOBJ[i].use = TRUE;
		g_MoveOBJ[i].pos = XMFLOAT3(325.0f, 428.0f, 0.0f);
		g_MoveOBJ[i].w = 200;
		g_MoveOBJ[i].h = 200;
		g_MoveOBJ[i].texNo = 10;
		g_MoveOBJ[i].r = 1.0f;
		g_MoveOBJ[i].r2 = 1.0f;
		g_MoveOBJ[i].theta = 0.0f;
		g_MoveOBJ[i].flipx = FALSE;
		g_MoveOBJ[i].flipy = FALSE;
	}

	g_MoveOBJ[BODY].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_MoveOBJ[BODY].w = 705;
	g_MoveOBJ[BODY].h =	1080;
	g_MoveOBJ[BODY].texNo = 3 + TITLEHBODY1;
	g_MoveOBJ[BODY].theta = 0.0f;

	g_MoveOBJ[LEFTOPPAI].pos = XMFLOAT3(325.0f, 428.0f, 0.0f);
	g_MoveOBJ[LEFTOPPAI].w = 200;
	g_MoveOBJ[LEFTOPPAI].h = 200;
	g_MoveOBJ[LEFTOPPAI].texNo = 3 + TITLELEFTOPPAI;
	g_MoveOBJ[LEFTOPPAI].theta = 0.0f;

	g_MoveOBJ[RIGHTOPPAI].pos = XMFLOAT3(430.0f, 465.0f, 0.0f);
	g_MoveOBJ[RIGHTOPPAI].w = 200;
	g_MoveOBJ[RIGHTOPPAI].h = 200;
	g_MoveOBJ[RIGHTOPPAI].texNo = 3 + TITLERIGHTOPPAI;
	g_MoveOBJ[RIGHTOPPAI].theta = 0.0f;

	g_MoveOBJ[EFFECT2].pos = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_MoveOBJ[EFFECT2].w = TEXTURE_WIDTH;
	g_MoveOBJ[EFFECT2].h = TEXTURE_HEIGHT;
	g_MoveOBJ[EFFECT2].texNo = 3 + TITLEEFFECT2;
	g_MoveOBJ[EFFECT2].theta = 0.0f;



	for (int i = 0; i < BUTTON_MAX; i++)
	{
		g_menuButton[i].use = TRUE;
		g_menuButton[i].pos = XMFLOAT3((SCREEN_WIDTH / 2)  + (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2 + 100  + (i * 100.0f), 0.0f);	// ���S�_����\��
		g_menuButton[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_menuButton[i].w = TEXTURE_BUTTON_WIDTH;
		g_menuButton[i].h = TEXTURE_BUTTON_HEIGHT;
		g_menuButton[i].texNo = i;
		g_menuButton[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_moji[i].use = TRUE;
		g_moji[i].pos = XMFLOAT3((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4), SCREEN_HEIGHT / 2 + 100 + (i * 100.0f), 0.0f);	// ���S�_����\��
		g_moji[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_moji[i].w = TEXTURE_BUTTON_WIDTH;
		g_moji[i].h = TEXTURE_BUTTON_HEIGHT;
		g_moji[i].texNo = i;
		g_moji[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	}
	g_menuButton[0].texNo = 2;



	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
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

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		if (g_Button[i])
		{
			g_Button[i]->Release();
			g_Button[i] = NULL;
		}
	}


	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitle(void)
{

	UpdatePress();

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{// Enter��������A�X�e�[�W��؂�ւ���
		PlaySound(SOUND_LABEL_SE_selected);
		switch (g_TitleNum)
		{
		case TITLE:

			g_TitleNum++;
			break;
		case SELECT:
			OnClickButton(g_buttonSelected);
			break;
		}
	}

	switch (g_TitleNum)
	{
	case TITLE:

		break;
		
	case SELECT:

		if (GetKeyboardTrigger(DIK_DOWN) || 
			GetKeyboardTrigger(DIK_S)||
			IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SOUND_LABEL_SE_select);
			g_buttonSelected = (g_buttonSelected + 1) % BUTTON_MAX;

		}
		else if (GetKeyboardTrigger(DIK_UP) ||
			GetKeyboardTrigger(DIK_W) ||
			IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_select);
			g_buttonSelected = (--g_buttonSelected < 0) ? (BUTTON_MAX - 1) : g_buttonSelected;
		}

		for (int i = 0; i < BUTTON_MAX; i++)
		{
			if (g_buttonSelected == i)
			{
				g_menuButton[i].texNo = 2;
				g_moji[i].color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
				//g_menuButton[i].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				g_menuButton[i].texNo = 0;
				g_moji[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				//g_menuButton[i].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		break;
	}
	// �{�^���̑I��
	// ��̃{�^��
	

	// �Z�[�u�f�[�^�����[�h����H
	if (GetKeyboardTrigger(DIK_L))
	{
		SetLoadGame(TRUE);
		SetFade(FADE_OUT, MODE_GAME);
	}


	//// �e�X�g�ŃG�t�F�N�g�̔����ꏊ���ړ�������
	//float speed = 4.0f;

	//if (GetKeyboardPress(DIK_DOWN))
	//{
	//	effect_dy += speed;
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{
	//	effect_dy -= speed;
	//}

	//if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	effect_dx += speed;
	//}
	//else if (GetKeyboardPress(DIK_LEFT))
	//{
	//	effect_dx -= speed;
	//}

	//for (int i = 0; i < TITLE_PARTS_MAX; i++)
	//{
	//	if (i == TITLEHHAIR)
	//	{
			//g_title[i].theta += 0.01f;
			//if (g_title[i].r >= TEXTURE_WIDTH / 10)g_title[i].flipx = TRUE;
			//else if (g_title[i].r <= 1.0f)g_title[i].flipx = FALSE;

			//if (g_title[i].r2 >= TEXTURE_HEIGHT / 50)g_title[i].flipy = TRUE;
			//else if (g_title[i].r <= 1.0f)g_title[i].flipy = FALSE;

			//if (g_title[i].flipx)r += 0.2f;
			//else r -= 0.2f;

			//if (g_title[i].flipy)r2 += 0.2f;
			//else r2 -= 0.2f;
	//	}

	//}

		//HHAIR
		g_title[TITLEHHAIR].theta += 0.03f;

		if (g_title[TITLEHHAIR].r >= TEXTURE_HHAIR_WIDTH / 100)g_title[TITLEHHAIR].flipx = TRUE;
		else if (g_title[TITLEHHAIR].r <= 1.0f)g_title[TITLEHHAIR].flipx = FALSE;

		if (g_title[TITLEHHAIR].flipx == FALSE)g_title[TITLEHHAIR].r += 0.015f;
		else g_title[TITLEHHAIR].r -= 0.015f;



		for (int i = 0; i < MOVEOBJ_MAX; i++)
		{
			g_MoveOBJ[i].theta += 0.02f;
		}


		//HBODY
		g_title[TITLEHBODY2].theta += 0.02f;

		if (g_title[TITLEHBODY2].r >= TEXTURE_HBODY_WIDTH / 100)g_title[TITLEHBODY2].flipx = TRUE;
		else if (g_title[TITLEHBODY2].r <= 1.0f)g_title[TITLEHBODY2].flipx = FALSE;

		if (g_title[TITLEHBODY2].r2 >= TEXTURE_HBODY_HEIGHT / 350)g_title[TITLEHBODY2].flipy = TRUE;
		else if (g_title[TITLEHBODY2].r <= 1.0f)g_title[TITLEHBODY2].flipy = FALSE;

		if (!g_title[TITLEHBODY2].flipx)g_title[TITLEHBODY2].r += 0.15f;
		else g_title[TITLEHBODY2].r -= 0.15f;

		if (!g_title[TITLEHBODY2].flipy)g_title[TITLEHBODY2].r2 += 0.15f;
		else g_title[TITLEHBODY2].r2 -= 0.15f;

		//��l��HAIR


		g_title[TITLEHAIR].theta += 0.015f;

		if (g_title[TITLEHAIR].r >= TEXTURE_HAIR_WIDTH / 100)g_title[TITLEHAIR].flipx = TRUE;
		else if (g_title[TITLEHAIR].r <= 1.0f)g_title[TITLEHAIR].flipx = FALSE;

		if (g_title[TITLEHAIR].flipx == FALSE)g_title[TITLEHAIR].r += 0.015f;
		else g_title[TITLEHAIR].r -= 0.015f;

		//HBODY
		g_title[TITLEBODY].theta += 0.01f;

		if (g_title[TITLEBODY].r >= TEXTURE_HBODY_WIDTH / 100)g_title[TITLEBODY].flipx = TRUE;
		else if (g_title[TITLEBODY].r <= 1.0f)g_title[TITLEBODY].flipx = FALSE;

		if (g_title[TITLEBODY].r2 >= TEXTURE_HBODY_HEIGHT / 350)g_title[TITLEBODY].flipy = TRUE;
		else if (g_title[TITLEBODY].r <= 1.0f)g_title[TITLEBODY].flipy = FALSE;

		if (!g_title[TITLEBODY].flipx)g_title[TITLEBODY].r += 0.2f;
		else g_title[TITLEBODY].r -= 0.2f;

		if (!g_title[TITLEBODY].flipy)g_title[TITLEBODY].r2 += 0.20f;
		else g_title[TITLEBODY].r2 -= 0.20f;

		//HBODY

		//g_title[TITLEEFFECT2].theta += 0.01f;

		//if (g_title[TITLEEFFECT2].r >= TEXTURE_WIDTH / 100)g_title[TITLEEFFECT2].flipx = TRUE;
		//else if (g_title[TITLEEFFECT2].r <= 1.0f)g_title[TITLEEFFECT2].flipx = FALSE;

		//if (g_title[TITLEEFFECT2].r2 >= TEXTURE_HEIGHT / 350)g_title[TITLEEFFECT2].flipy = TRUE;
		//else if (g_title[TITLEEFFECT2].r <= 1.0f)g_title[TITLEEFFECT2].flipy = FALSE;

		//if (!g_title[TITLEEFFECT2].flipx)g_title[TITLEEFFECT2].r += 0.5f;
		//else g_title[TITLEEFFECT2].r -= 0.5f;

		//if (!g_title[TITLEEFFECT2].flipy)g_title[TITLEEFFECT2].r2 += 0.10f;
		//else g_title[TITLEEFFECT2].r2 -= 0.10f;



		for (int i = 0; i < MOVEOBJ_MAX; i++)
		{
			g_MoveOBJ[i].theta += 0.01f;
		}

	//logo_theta += 0.01;

	//if (r <= TEXTURE_WIDTH / 10)r += 0.2f;

	//if (r2 <= TEXTURE_HEIGHT / 50)r2 += 0.4f;



#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
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

	// �^�C�g���̔w�i��`��

	
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEBG]);//�w�i

	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLETREE1]);//�؁@��

	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);



	DrawHeroineBody();
	
	DrawMoveOBJ();

	
	DrawTreeM();
	
	DrawHeroineHair();
	

	{//��3�̕`�揈��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLETREE3]);//�؁@��
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}

	DrawHeroBody();
	DrawHeroHair();

	{//�G�t�F�N�g1�̕`�揈��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEEFFECT1]);//�؁@��
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}


	DrawTitleEffect2();

	DrawLogoEffect();

	{//���S�`�揈��
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLELOGO]);
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		GetDeviceContext()->Draw(4, 0);
	}



	switch (g_TitleNum)
	{
	case TITLE:
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
	
		//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		////// �|���S���`��
		//GetDeviceContext()->Draw(4, 0);
		
		break;

	case SELECT:
		
		


		// �����Z�̃e�X�g
		//SetBlendState(BLEND_MODE_ADD);		// ���Z����
	//	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

		// �e�N�X�`���ݒ�
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);
		//
		//for (int i = 0; i < 30; i++)
		//{
		//	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//	float dx = effect_dx;
		//	float dy = effect_dy;
		//	float sx = (float)(rand() % 100);
		//	float sy = (float)(rand() % 100);
		//
		//	SetSpriteColor(g_VertexBuffer, dx + sx, dy + sy, 50, 50, 0.0f, 0.0f, 1.0f, 1.0f,
		//		XMFLOAT4(1.0f, 0.3f, 1.0f, 0.5f));
		//
		//	// �|���S���`��
		//	GetDeviceContext()->Draw(4, 0);
		//}
		//SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		//// �|���S���`��
		//GetDeviceContext()->Draw(4, 0);

		//// �^�C�g����`��
		//{
		//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

		//	for (float y = 400; y < 500; y += TITLE_SPLIT)
		//	{
		//		for (float x = 280; x < 500; x += TITLE_SPLIT)
		//		{
		//			// �E�B���h�E
		//			//float tx = x + r * sinf(logo_theta + 2 * 3.14 * y / TEXTURE_HEIGHT) -SCREEN_WIDTH / 3;
		//			float tx = x ;
		//			float ty = y;
		//			ty = y + r2 * sinf(logo_theta + 2 * 3.14 * x / TEXTURE_WIDTH);
		//			//float ty = y + SCREEN_HEIGHT / 3;
		//			float tw = TITLE_SPLIT;
		//			float th = TITLE_SPLIT;

		//			//UV
		//			float sw = x / TEXTURE_WIDTH;
		//			float sh = y / TEXTURE_HEIGHT;
		//			float sx = 2.0f / TEXTURE_WIDTH;
		//			float sy = 2.0f / TEXTURE_HEIGHT;

		//			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//			SetSprite(g_VertexBuffer,
		//				tx, ty, tw, th,
		//				sw, sh, sx, sy);
		//			//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		//			// �|���S���`��
		//			GetDeviceContext()->Draw(4, 0);
		//		}
		//	}

		//}


		
		

		DrawMenuButton();
		DrawMenuMoji();

		break;
	}
	DrawPress();

}

void UpdatePress(void)
{

	//clearCnt++ > 1.0f ? clear = !(clear) : clear = clear;

	if (clear == TRUE)clearCnt -= 0.01;
	else clearCnt += 0.01;

	if (clearCnt >= 1.0f || clearCnt <= 0.0f)
	{
		clear = !(clear);
	}

}
void DrawPress(void)
{
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

	if (g_TitleNum == 0)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_MAX - 2]);
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, 950.0f, 750.0f, 100.0f,
			0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, clearCnt));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
	else
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TEXTURE_MAX - 1]);
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 50, 950.0f, 1500.0f, 100.0f,
			0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, clearCnt));
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	

}

//=============================================================================
// Menu Button ��`��
//=============================================================================
void DrawMenuButton(void)
{

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Button[g_menuButton[i].texNo]);

		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_menuButton[i].pos.x;			// �{�^���̕\���ʒuX
		float py = g_menuButton[i].pos.y;			// �{�^���̕\���ʒuY
		float pw = g_menuButton[i].w;		// �{�^���̕\����
		float ph = g_menuButton[i].h;		// �{�^���̕\������


		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W
		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_menuButton[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}

void DrawMenuMoji(void)
{

	for (int i = 0; i < BUTTON_MAX; i++)
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_moji[i].texNo]);

		//�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_moji[i].pos.x;			// �{�^���̕\���ʒuXw
		float py = g_moji[i].pos.y;			// �{�^���̕\���ʒuY
		float pw = g_moji[i].w;		// �{�^���̕\����
		float ph = g_moji[i].h;		// �{�^���̕\������


		float tx = 0.0f;	// �e�N�X�`���̍���X���W
		float ty = 0.0f;	// �e�N�X�`���̍���Y���W
		float tw = 1.0f;	// �e�N�X�`���̕�
		float th = 1.0f;


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			g_moji[i].color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}

//=============================================================================
// OnClick Button ��`��
//=============================================================================
void OnClickButton(int button)
{
	switch (button)
	{
	case BUTTON_START_GAME:
		SetFade(FADE_OUT, MODE_OPENING);
		break;

	case BUTTON_SETTING_GAME:
		SetFade(FADE_OUT, MODE_BOSS);
		break;


	case BUTTON_QUIT_GAME:
		//SetFade(FADE_OUT, MODE_GAME);
		SetMode(MODE_QUIT);
		break;
	}
}

int GetButtonSelected(void)
{
	return g_buttonSelected;
}


void DrawHeroineBody(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEHBODY1]);//��
	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_HBODY_WIDTH, TEXTURE_HBODY_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

	for (float y = 0; y < 1080; y += TITLE_SPLIT)
	{
		for (float x = 0; x <= 705; x += TITLE_SPLIT)
		{
			// �E�B���h�E
			//float tx = x + r * sinf(logo_theta + 2 * 3.14 * y / TEXTURE_HEIGHT) -SCREEN_WIDTH / 3;
			float tx = x;
			float ty = y;

			tx = x;
			ty = y + g_title[TITLEHBODY2].r2 * 0.5 * sinf(g_title[TITLEHBODY2].theta + 2 * PI * x / TEXTURE_HBODY_WIDTH);

			float tw = TITLE_SPLIT;
			float th = TITLE_SPLIT;

			//UV
			float sw = x / TEXTURE_HBODY_WIDTH;
			float sh = y / TEXTURE_HBODY_HEIGHT;
			float sx = TITLE_SPLIT / TEXTURE_HBODY_WIDTH;
			float sy = TITLE_SPLIT / TEXTURE_HBODY_HEIGHT;

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer,
				tx, ty, tw, th,
				sw, sh, sx, sy);
			//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}
void DrawMoveOBJ(void)
{
	for (int i = 1; i < EFFECT2; i++)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MoveOBJ[i].texNo]);

		float tx = g_MoveOBJ[i].pos.x;
		float ty = g_MoveOBJ[i].pos.y;
		float tw = g_MoveOBJ[i].w;
		float th = g_MoveOBJ[i].h;

		float u = 0.0f;
		float v = 0.0f;
		float uw = 1.0f;
		float vh = 1.0f;

		tx += 1 * sin(g_MoveOBJ[i].theta + i * PI);
		ty += 5 * cos(g_MoveOBJ[i].theta + i * PI);
		tw = g_MoveOBJ[i].w * (1 + 0.025 * sin(g_MoveOBJ[i].theta +  PI));
		th = g_MoveOBJ[i].h * (1 + 0.025 * sin(g_MoveOBJ[i].theta +  PI));

		SetSpriteRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, 0.0f);
		//// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}
}

void DrawTitleEffect2(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEEFFECT2]);

	float tx = g_MoveOBJ[EFFECT2].pos.x;
	float ty = g_MoveOBJ[EFFECT2].pos.y;
	float tw = g_MoveOBJ[EFFECT2].w;
	float th = g_MoveOBJ[EFFECT2].h;

	float u = 0.0f;
	float v = 0.0f;
	float uw = 1.0f;
	float vh = 1.0f;

	tx += 5 * sin(g_MoveOBJ[EFFECT2].theta);
	ty += 5 * cos(g_MoveOBJ[EFFECT2].theta);
	tw = g_MoveOBJ[EFFECT2].w * (1 + 0.025 * sin(g_MoveOBJ[EFFECT2].theta + PI));
	th = g_MoveOBJ[EFFECT2].h * (1 + 0.025 * sin(g_MoveOBJ[EFFECT2].theta + PI));

	SetSpriteRotation(g_VertexBuffer, tx, ty, tw, th, u, v, uw, vh, 0.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);


}




void DrawTreeM(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLETREE2]);//�؁@����

	SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	//// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}

void DrawHeroineHair(void)
{
	//���̖�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEHHAIR]);

	for (float y = 48; y < 625; y += TITLE_SPLIT)
	{
		for (float x = 67; x < 650; x += TITLE_SPLIT)
		{
			// �E�B���h�E
			//float tx = x;
			float tx;
			float ty;

			if (y >= 273 && x >= 65)
			{

				tx = x + g_title[TITLEHHAIR].r * sinf(g_title[TITLEHHAIR].theta + 2 * PI * y / TEXTURE_HHAIR_HEIGHT);
				ty = y;
			}
			else
			{
				tx = x + g_title[TITLEHHAIR].r * 0.7 * sinf(g_title[TITLEHHAIR].theta + 2 * PI * y / TEXTURE_HHAIR_HEIGHT);
				ty = y;
			}
			float tw = TITLE_SPLIT;
			float th = TITLE_SPLIT;

			//UV
			float sw = x / TEXTURE_HHAIR_WIDTH;  // �e�N�X�`���̕�
			float sh = y / TEXTURE_HHAIR_HEIGHT; // �e�N�X�`���̍���
			float sx = TITLE_SPLIT / TEXTURE_HHAIR_WIDTH;		// �e�N�X�`���̍���X���W
			float sy = TITLE_SPLIT / TEXTURE_HHAIR_HEIGHT;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer,
				tx, ty, tw, th,
				sw, sh, sx, sy);
			//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}

void DrawHeroBody(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEBODY]);//��


	for (float y = 0; y < TEXTURE_BODY_HEIGHT; y += TITLE_SPLIT)
	{
		for (float x = 181; x <= TEXTURE_BODY_WIDTH; x += TITLE_SPLIT)
		{
			// �E�B���h�E
			//float tx = x + r * sinf(logo_theta + 2 * 3.14 * y / TEXTURE_HEIGHT) -SCREEN_WIDTH / 3;
			float tx = x;
			float ty = y;

			tx = x;
			ty = y + g_title[TITLEBODY].r2 * 0.5 * sinf(g_title[TITLEBODY].theta + 2 * PI * x / TEXTURE_BODY_WIDTH);

			float tw = TITLE_SPLIT;
			float th = TITLE_SPLIT;

			//UV
			float sw = x / TEXTURE_BODY_WIDTH;
			float sh = y / TEXTURE_BODY_HEIGHT;
			float sx = TITLE_SPLIT / TEXTURE_BODY_WIDTH;
			float sy = TITLE_SPLIT / TEXTURE_BODY_HEIGHT;

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer,
				tx, ty, tw, th,
				sw, sh, sx, sy);
			//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


void DrawHeroHair(void)
{
	//���̖�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLEHAIR]);

	for (float y = 264; y < 664; y += TITLE_SPLIT)
	{
		for (float x = 595; x < 890; x += TITLE_SPLIT)
		{
			// �E�B���h�E
			//float tx = x;
			float tx;
			float ty;

			if (y >= 273 && x >= 65)
			{

				tx = x + g_title[TITLEHAIR].r * sinf(g_title[TITLEHAIR].theta + 2 * PI * y / TEXTURE_HAIR_HEIGHT);
				ty = y;
			}
			else
			{
				tx = x + g_title[TITLEHAIR].r * 0.7 * sinf(g_title[TITLEHAIR].theta + 2 * PI * y / TEXTURE_HAIR_HEIGHT);
				ty = y;
			}
			float tw = TITLE_SPLIT;
			float th = TITLE_SPLIT;

			//UV
			float sw = x / TEXTURE_HAIR_WIDTH;  // �e�N�X�`���̕�
			float sh = y / TEXTURE_HAIR_HEIGHT; // �e�N�X�`���̍���
			float sx = TITLE_SPLIT / TEXTURE_HAIR_WIDTH;		// �e�N�X�`���̍���X���W
			float sy = TITLE_SPLIT / TEXTURE_HAIR_HEIGHT;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer,
				tx, ty, tw, th,
				sw, sh, sx, sy);
			//SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TITLE_WIDTH, TITLE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}
}


void DrawLogoEffect(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3 + TITLE_LOGOEFFECT]);
	SetSpriteLTColor(g_VertexBuffer, -5.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, clearCnt));
	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}