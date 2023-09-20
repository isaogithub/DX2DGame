/*******************************************************************************
* �^�C�g��:		���C���v���O����
* �v���O������:	main.cpp
* �쐬��:		GP11A132 99 �O������
* �쐬��:		2022/04/18
*******************************************************************************/



/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "tutorial.h"
#include "title.h"
#include "bg.h"
#include "input.h"
#include "fade.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "write.h"
#include "sound.h"
#include "result.h"
/*******************************************************************************
* �}�N����`
*******************************************************************************/


#define TEXTURE_MAX					(TUTORIAL_MAX + 2)				// �e�N�X�`���̐�


#define TEXTURE_WIDTH			(1920)			// ���S�T�C�Y
#define TEXTURE_HEIGHT			(1080)			// 
	
#define MESSAGE_TEXTURE_WIDTH	(1100)
#define MESSAGE_TEXTURE_HEIGHT	(234)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static ID3D11ShaderResourceView* g_Message[MESSAGE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TextureName[TEXTURE_MAX] = {
	
	"data/TUTORIAL/tutorial_move.png",//TUTORIAL_MOVE
	"data/TUTORIAL/tutorial_jump.png",//TUTORIAL_JUMP
	"data/TUTORIAL/tutorial_bullet01.png",
	"data/TUTORIAL/tutorial_blade.png",
	"data/TUTORIAL/tutorial_guard.png",
	"data/TUTORIAL/tutorial_blackhole.png",
	"data/TUTORIAL/tutorial_ult.png",
	"data/TUTORIAL/tutorial_last.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/PRESS.png",
};

static char* g_MessageName[] = {

	"data/Message/message_door1.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_door2.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_door3.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_boss1.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_hiroine001.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_hiroine002.png",	//���b�Z�[�W�E�C���h�E
	"data/Message/message_hiroine003.png",	//���b�Z�[�W�E�C���h�E

};

PAUSE g_pause[TUTORIAL_MAX];

MESSAGEPOS g_Tmessage[TUTORIAL_MAX];


MESSAGE g_Messagegame[MESSAGE_MAX];



static int g_pauseNum;					//�扽�Ԗڂ̒�~

static float g_clearCnt;
static float g_clearPlus;

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

static int g_MessageNum;
static BOOL g_MessageDisplay;

static int  g_stopFrame;

HRESULT InitTutorial(void)
{
	ID3D11Device* pDevice = GetDevice();

	g_clearCnt = 0;
	g_clearPlus = 0.005f;
	
	g_MessageDisplay = FALSE;

	g_stopFrame = 0;
	g_pauseNum = 0;

	//�e�N�X�`������

	switch (GetMode())
	{
	case MODE_TUTORIAL:
		g_MessageNum = 0;
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

		g_pause[0].pos = XMFLOAT3(100.0f, 540.0f, 0.0f);
		g_pause[1].pos = XMFLOAT3(500.0f, 540.0f, 0.0f);
		//g_pause[2].pos = XMFLOAT3(1550.0f, 540.0f, 0.0f);
		//g_pause[3].pos = XMFLOAT3(2240.0f, 540.0f, 0.0f);
		//g_pause[4].pos = XMFLOAT3(2940.0f, 540.0f, 0.0f);
		//g_pause[5].pos = XMFLOAT3(3450.0f, 540.0f, 0.0f);

		for (int i = 0; i < TUTORIAL_MAX; i++)
		{
			g_pause[i].use = TRUE;

			g_Tmessage[i].use = TRUE;
			g_Tmessage[i].w = MESSAGE_TEXTURE_WIDTH * 0.8;
			g_Tmessage[i].h = MESSAGE_TEXTURE_HEIGHT * 0.8;
			if (i % 2 == 0)
			{
				g_Tmessage[i].pos = XMFLOAT3(500.0f + i * g_Tmessage[i].w * 0.5, 100.0f, 0.0f);
			}
			else g_Tmessage[i].pos = XMFLOAT3(500.0f + i * g_Tmessage[i].w * 0.5, 100.0f+ g_Tmessage[i].h, 0.0f);

			g_Tmessage[i].texNo = i;
		}
		

		break;

	case MODE_GAME:
		g_MessageNum = -1;
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

	

		for (int i = 0; i <MESSAGE_MAX; i++)
		{
			g_Message[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_MessageName[i],
				NULL,
				NULL,
				&g_Message[i],
				NULL);
		}
		//g_pause[0].pos = XMFLOAT3(100.0f, 540.0f, 0.0f);
		//g_pause[1].pos = XMFLOAT3(550.0f, 540.0f, 0.0f);
		//g_pause[2].pos = XMFLOAT3(2000.0f, 540.0f, 0.0f);

		

		for (int i = 0; i < MESSAGE_MAX; i++)
		{
			g_Messagegame[i].use = FALSE;
			g_Messagegame[i].display = FALSE;
			g_Messagegame[i].texNo = i;
			g_Messagegame[i].w = MESSAGE_TEXTURE_WIDTH;
			g_Messagegame[i].h = MESSAGE_TEXTURE_HEIGHT;
		}
		g_Messagegame[0].use = TRUE;

		break;
	case MODE_BOSS:
		g_MessageNum = -1;

		for (int i = 0; i < MESSAGE_MAX; i++)
		{
			g_Message[i] = NULL;
			D3DX11CreateShaderResourceViewFromFile(GetDevice(),
				g_MessageName[i],
				NULL,
				NULL,
				&g_Message[i],
				NULL);
		}



		break;

	default:
		break;
	}

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
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_Texture[i])
			{
				g_Texture[i]->Release();
				g_Texture[i] = NULL;
			}
		}
		break;
	case MODE_GAME:

		for (int i = 0; i < TEXTURE_MAX; i++)
		{
			if (g_Texture[i])
			{
				g_Texture[i]->Release();
				g_Texture[i] = NULL;
			}
		}

		for (int i = 0; i < MESSAGE_MAX; i++)
		{
			if (g_Message[i])
			{
				g_Message[i]->Release();
				g_Message[i] = NULL;
			}
		}


		break;
	case MODE_BOSS:

		for (int i = 0; i < MESSAGE_MAX; i++)
		{
			if (g_Message[i])
			{
				g_Message[i]->Release();
				g_Message[i] = NULL;
			}
		}


		break;


	}


	g_Load = FALSE;
}
void UpdateTutorial(void)
{ 
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	INTERACT_OB* door = GetDoor();
	INTERACT_OB* heroine = GetHeroine();


	g_clearCnt += g_clearPlus;
	g_clearPlus = ((g_clearCnt += g_clearPlus) >= 1.0f) ? g_clearPlus *= (-1) : g_clearPlus;
	g_clearPlus = ((g_clearCnt += g_clearPlus) < 0.0f) ? g_clearPlus *= (-1) : g_clearPlus;


	switch (GetMode())
	{
	case MODE_TUTORIAL:

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
		{// Enter��������A�X�V���n�߂�
			if (g_pauseNum++ >= TUTORIAL_MAX - 1)
			{
				g_pauseNum = 0;
				SetPause(FALSE);
			}
		}

		for (int i = 0; i < PLAYER_MAX; i++)
		{
			if (CollisionBB(player[i].pos, player[i].w, player[i].h, g_pause[TUTORIAL_MOVE].pos, 2000.0f, 500.0f) == TRUE)
			{
				if (g_pause[TUTORIAL_MOVE].use == TRUE)
				{
					g_MessageDisplay = TRUE;
					g_pause[TUTORIAL_MOVE].use = FALSE;
					SetPause(TRUE);
				}
		
			}
		}

	break;
	case MODE_GAME:
	
		switch (g_MessageNum)
		{
		case MESSAGE_DOOR01:

			SetPause(TRUE);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{// Enter��������A�X�V���n�߂�
					SetPause(FALSE);
					g_MessageNum = -1;
			}
			break;

		case MESSAGE_DOOR02:
			SetPause(TRUE);
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{// Enter��������A�X�V���n�߂�

				g_MessageNum++;

			}
			break;

		case MESSAGE_DOOR03:
			
			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
			{// Enter��������A�X�V���n�߂�

				SetPause(FALSE);
				door->use = FALSE;
				g_MessageNum = -1;
			}
			break;
		}
		

	
		break;		
		case MODE_BOSS:

			switch (g_MessageNum)
			{
			case MESSAGE_HIROINE0:

				SetPause(TRUE);

				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
				{// Enter��������A�X�V���n�߂�
					SetPause(FALSE);
					g_MessageNum = -1;
				}
				break;

			case MESSAGE_HIROINE1:
			case MESSAGE_HIROINE2:
				SetPause(TRUE);
				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
				{// Enter��������A�X�V���n�߂�

					g_MessageNum++;

				}
				break;

				

			case MESSAGE_HIROINE3:

				if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
				{// Enter��������A�X�V���n�߂�

					SetPause(FALSE);
					heroine->use = FALSE;
					g_MessageNum = -1;
					SetResult(GAMECLEAR,RESULTTYPE_BOSS);
					SetFade(FADE_OUT, MODE_RESULT);
				}
				break;
			}

			break;


	}
	
	
	
}

//�`���[�g���A���\��
void DrawTutorial(void)
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

	PLAYER* player = GetPlayer();

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_pauseNum]);


	if (GetPause())
	{

		switch (GetMode())
		{
		case MODE_TUTORIAL:

			// �e�N�X�`���ݒ�
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer,
				960.0f,
				750.0f,
				MESSAGE_TEXTURE_WIDTH,
				MESSAGE_TEXTURE_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			break;
		case MODE_GAME:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MessageNum]);

			SetSpriteColor(g_VertexBuffer,
				960.0f,
				750.0f,
				MESSAGE_TEXTURE_WIDTH,
				MESSAGE_TEXTURE_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
			break;

		case MODE_BOSS:

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_MessageNum]);

			SetSpriteColor(g_VertexBuffer,
				960.0f,
				750.0f,
				MESSAGE_TEXTURE_WIDTH,
				MESSAGE_TEXTURE_HEIGHT,
				0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
			break;

		}
	}


	
	if (g_MessageDisplay && !GetPause())
	{
		for (int i = 0; i < TUTORIAL_MAX; i++)
		{
			BG* bg = GetBG();
			if (g_Tmessage[i].use == FALSE)continue;

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Tmessage[i].texNo]);
			{
				// �e�N�X�`���ݒ�

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer,

					g_Tmessage[i].pos.x - bg->pos.x,
					g_Tmessage[i].pos.y,
					g_Tmessage[i].w,
					g_Tmessage[i].h,

					0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f));

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}
		}
	}


	if (GetPause())//��~���Ă���
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			SCREEN_CENTER_X,
			900.0f ,
			750.0f,
			100.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, g_clearCnt));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}

void DrawDark(void)
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


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TUTORIAL_MAX]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLTColor(g_VertexBuffer,
			0.0f,
			0.0f,
			TEXTURE_WIDTH,
			TEXTURE_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f));

		// �|���S���`��

		GetDeviceContext()->Draw(4, 0);
	// �e�N�X�`���ݒ�
}

void DrawMessage(void)
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

	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Message[g_MessageNum]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer,
			960.0f,
			850.0f,
			MESSAGE_TEXTURE_WIDTH,
			MESSAGE_TEXTURE_HEIGHT,
			0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
	
	// �e�N�X�`���ݒ�

}

void SetMessage(int type)
{
	switch (type)
	{
	case NOKEY:

		g_MessageNum = 0;
		break;
	case DOOR:

		g_MessageNum = 1;
		break;
	case BOSSNODEAD:

		g_MessageNum = 3;
		break;

	case HEROINE:

		g_MessageNum = 4;
		break;

	}
}

