//=============================================================================
//
// ���C������ [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "debugproc.h"
#include "input.h"

#include "title.h"
#include "opening.h"

#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "slime.h"
#include "bullet.h"
#include "hitscore.h"

#include "result.h"

#include "sound.h"
#include "fade.h"

#include "file.h"

#include "effect.h"
#include "field.h"
#include "tutorial.h"
#include "ui.h"



#pragma comment(lib,"d2d1.lib")//---���ǉ�---
#pragma comment(lib,"dwrite.lib")//---���ǉ�---

#include <d2d1.h>//---���ǉ�---
#include <dwrite.h>//---���ǉ�---
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�E�҂̗��H�`�͂��܂�̐��`"			// �E�C���h�E�̃L���v�V������

#define PAUSE_SECOND_MAX		(2)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif

int	g_Mode = MODE_TITLE;					// �N�����̉�ʂ�ݒ�

BOOL g_LoadGame = FALSE;					// NewGame

BOOL g_PauseUpdate = FALSE;					//�Q�[�����ꎞ�I�X�V�������~����
BOOL g_Start = FALSE;
BOOL g_BossBT = FALSE;
int  g_StartCnt;
int  g_PauseCnt;

float g_beforetime;
float g_deltatime;
static int	g_result;
static int	g_ResultMode;

WNDCLASSEX	wcex;
HWND		hWnd;

//�_�C���N�g���C�g
ID2D1Factory* g_pD2DFactory = nullptr;
IDWriteFactory* g_pDWriteFactory = nullptr;
IDWriteTextFormat* g_pTextFormat = nullptr;
ID2D1RenderTarget* g_pRT = nullptr;
ID2D1SolidColorBrush* g_pSolidBrush = nullptr;
IDXGISurface* g_pBackBuffer = nullptr;

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	g_PauseCnt = 0;

	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// �E�B���h�E���[�h���t���X�N���[�����[�h���̏���
	BOOL mode = TRUE;

	int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
		mode = TRUE;
		break;
	case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
		mode = FALSE;	// ���ɂ���ē����Ȃ���������
		break;
	case IDCANCEL:	// CANCEL�Ȃ�I��
	default:
		return -1;
		break;
	}

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
				if (g_PauseUpdate == TRUE)
				{
					/*if (g_PauseCnt++ >= PAUSE_SECOND_MAX)
					{
						SetPause(FALSE);
						g_PauseCnt = 0;
					}*/
				}
				
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				PLAYER* player = GetPlayer();
				ENEMY* enemy = GetEnemy();
				BG* bg = GetBG();
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], "MX:%d MY:%d PX:%d PY:%d PonG:%d PS:%d  PJ:%d PAt:%d Pani:%d PHP:%d PMING : %d  PFL : %d  PFEC : %d BLOCK:%d	ESTOP:%d EANI:%d Estate:%d EHIT:%d EHITG:%d EHCNT:%d EonG:%d EATT:%d",
					GetMousePosX(), GetMousePosY(),
					(int)player[0].pos.x, (int)player[0].pos.y, player[0].onGround,player[0].state,player[0].jump,player[0].attack,player[0].patternAnim,(int)player[0].hp, (int)player[0].moving, (int)player[0].flash, player[0].flashExeCnt,
					GetFieldData( (int)( GetMousePosY() / 32), (int)(GetMousePosX() / 32) ) ,
					enemy[ENEMY_MAX - 1].stop, enemy[ENEMY_MAX - 1].patternAnim, enemy[ENEMY_MAX - 1].state, enemy[ENEMY_MAX - 1].hit,enemy[ENEMY_MAX - 1].hitting,enemy[ENEMY_MAX - 1].hitCnt,enemy[ENEMY_MAX - 1].onGround, enemy[ENEMY_MAX - 1].attack
				) ;
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	//DestroyWindow(hWnd);

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	g_beforetime = timeGetTime();
	g_deltatime = 0;
	g_result = GAMEOVER;
	g_ResultMode = RESULTTYPE_GAME;	// �`��̏�����

	InitRenderer(hInstance, hWnd, bWindow);

	// �J�����̏�����
	InitCamera();

	// ���C�g��L����
	SetLightEnable(FALSE);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �T�E���h�����̏�����
	InitSound(hWnd);

	// �t�F�[�h�����̏�����
	InitFade();

	//InitDirectWrite();

	// �ŏ��̃��[�h���Z�b�g
	SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �I���̃��[�h���Z�b�g
	if(GetMode() != MODE_QUIT)SetMode(MODE_MAX);

	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

	// ���͂̏I������
	UninitInput();

	// �J�����̏I������
	UninitCamera();

	// �����_���[�̏I������
	UninitRenderer();

	//UninitDirectWrite();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	g_deltatime = (float)timeGetTime() -  g_beforetime;
	g_beforetime = timeGetTime();
	
	// ���͂̍X�V����
	UpdateInput();

	// �J�����X�V
	UpdateCamera();

	SetVolumeSound(SOUND_LABEL_SE_hit000, 0.4f);
	SetVolumeSound(SOUND_LABEL_SE_gravityBall, 1.8f);

	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̍X�V

		SetVolumeSound(SOUND_LABEL_BGM_opening, 0.6f);
		UpdateTitle();
		break;

	case MODE_OPENING:		// �I�[�v���j���O��ʂ̍X�V


		UpdateOpening();
		
		break;

	case MODE_TUTORIAL:

		SetVolumeSound(SOUND_LABEL_BGM_tutorial, 0.3f);

		g_StartCnt++;
		if (g_StartCnt >= 120)g_Start = TRUE;
		if (g_Start)
		{
			UpdateTutorial();
		}
		if (!g_PauseUpdate)
		{

			UpdateBG();
			UpdateField();
			UpdatePlayer();
			UpdateEnemy();
			UpdateBullet();
			UpdateEffect();
			UpdateHitScore();
		}
		UpdateUI();
		break;

	case MODE_GAME:			// �Q�[����ʂ̍X�V

		SetVolumeSound(SOUND_LABEL_BGM_dungeon, 0.1f);

		UpdateTutorial();
		UpdateUI();
		if (!g_PauseUpdate )
		{
			UpdateBG();
			UpdateField();
			UpdatePlayer();
			UpdateEnemy();
			UpdateBullet(); 

			UpdateEffect();
			UpdateHitScore();
		}

		break;

	case MODE_BOSS:			// �Q�[����ʂ̍X�V

		SetVolumeSound(SOUND_LABEL_BGM_boss, 0.2f);

		UpdateTutorial();
		UpdateUI();
		if (!g_PauseUpdate)
		{
			UpdateBG();
			UpdateField();
			UpdatePlayer();
			UpdateEnemy();
			UpdateBullet();
			UpdateNuma();

			UpdateEffect();
			UpdateHitScore();
		}

		break;

	case MODE_RESULT:		// ���U���g��ʂ̍X�V
		UpdateResult();
		break;
	}

	UpdateFade();			// �t�F�[�h�̍X�V����
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();

	// 2D�̕���`�悷�鏈��
	SetViewPort(TYPE_FULL_SCREEN);

	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̕`��
		DrawTitle();
		break;

	case MODE_OPENING:		// �I�[�v���j���O��ʂ̕`��
		DrawOpening();
		break;

	case MODE_TUTORIAL:
		

		DrawBG();
		DrawField();

		if (g_PauseUpdate)
		{
			DrawDark();
		}
		DrawTutorial();

		DrawPlayer();
		DrawEnemy();
		DrawBullet();
		DrawEffect();
		DrawHitScore();
		if(!GetPause())DrawUI();

		break;
	case MODE_GAME:			// �Q�[����ʂ̕`��
		DrawBG();
		DrawField();

		DrawEnemy();
		DrawPlayer();
		DrawSkill002();
		DrawBullet();		// �d�Ȃ鏇�Ԃ��ӎ����Ă�
#ifndef _DEBUG
		DrawEyeDark();
#endif // DEBUG

		DrawEffect();
		if (g_PauseUpdate)
		{
			DrawDark();
			DrawMessage();
		}
		DrawHitScore();
		if (!GetPause())DrawUI();

		break;

	case MODE_BOSS:			// �Q�[����ʂ̕`��

		DrawBG();
		DrawField();

		DrawEnemy();
		DrawPlayer();



		DrawSkill002();
		DrawNuma();

		DrawBullet();		// �d�Ȃ鏇�Ԃ��ӎ����Ă�

		DrawEffect();
		if (g_PauseUpdate)
		{
			DrawDark();
			DrawMessage();
		}
		DrawHitScore();

		if (!GetPause())DrawUI();
		break;



	case MODE_RESULT:		// ���U���g��ʂ̕`��
		DrawResult();
		break;
	}


	DrawFade();				// �t�F�[�h��ʂ̕`��


#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(int mode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	StopSound();			// �܂��Ȃ��~�߂�

	// ���[�h��ς���O�ɑS������������������Ⴄ

	// �^�C�g����ʂ̏I������
	UninitTitle();
	//�I�[�v���j���O�̏I������
	UninitOpening();
	// BG�̏I������
	UninitBG();

	//�t�B�[���h�I������
	UninitField();

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �o���b�g�̏I������
	UninitBullet();

	// ���U���g�̏I������
	UninitResult();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	UninitTutorial();

	UninitHitScore();

	UninitUI();

	g_StartCnt = 0;
	g_Start = FALSE;

	g_Mode = mode;	// ���̃��[�h���Z�b�g���Ă���

	switch (g_Mode)
	{
	case MODE_TITLE:
		// �^�C�g����ʂ̏�����
		InitTitle();

		//SetVolumeSound(1.0f);

		//SetFrequencyRatio(2.0f);
		SetVolumeSound(SOUND_LABEL_BGM_opening, 0.4);
		PlaySound(SOUND_LABEL_BGM_opening);
		break;
	case MODE_OPENING:
		InitOpening();

		PlaySound(SOUND_LABEL_BGM_opening2);
		break;

	case MODE_TUTORIAL:
		// �Q�[����ʂ̏�����

		InitTutorial();

		InitBG();
		InitField();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEffect();
		InitHitScore();
		InitUI();

		SetVolumeSound(SOUND_LABEL_BGM_tutorial, 0.3f);		
		PlaySound(SOUND_LABEL_BGM_tutorial);

		break;
	
	case MODE_GAME:
		// �Q�[����ʂ̏�����
		InitTutorial();

		InitBG();
		InitField();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEffect();
		InitHitScore();
		InitUI();

		// ���[�h�Q�[���������炷�ׂĂ̏��������I�������ɃZ�[�u�f�[�^��ǂݍ���
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ���[�h��������t���O��Clear����
		}

		PlaySound(SOUND_LABEL_BGM_dungeon);

		break;

	case MODE_BOSS:
		// �Q�[����ʂ̏�����
		InitTutorial();

		InitBG();
		InitField();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEffect();
		InitHitScore();
		InitUI();

		// ���[�h�Q�[���������炷�ׂĂ̏��������I�������ɃZ�[�u�f�[�^��ǂݍ���
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ���[�h��������t���O��Clear����
		}
		SetVolumeSound(SOUND_LABEL_BGM_boss,0.5);
		PlaySound(SOUND_LABEL_BGM_boss);

		break;


	case MODE_RESULT:
		InitResult();

		if (GetResult() == GAMECLEAR) PlaySound(SOUND_LABEL_BGM_gameclear);
		else PlaySound(SOUND_LABEL_BGM_gameover);
		
		break;

	case MODE_QUIT:

		QuitGame();

		break;
	case MODE_MAX:
		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}


//=============================================================================
// �j���[�Q�[�������[�h�Q�[�������Z�b�g����
//=============================================================================
void SetLoadGame(BOOL flg)
{
	g_LoadGame = flg;
}

XMFLOAT3* XMFLOAT3ADD(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	float x = pos1.x + pos2.x;
	float y = pos1.y + pos2.y;
	float z = pos1.z + pos2.z;
	XMFLOAT3 pos = XMFLOAT3(x, y, z);

	return &pos;
}
//=============================================================================
// �Q�[�����甲����
//=============================================================================

void QuitGame(void)
{
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	// �I������
	//Uninit();

	if (DestroyWindow(hWnd))
		MessageBox(NULL, TEXT("�E�B���h�E���폜���܂���"), TEXT("���"), MB_OK);
}

//=============================================================================
// �Q�[�����ꎞ�I��~
//=============================================================================
void SetPause(BOOL state)
{//�Q�[�����X�V���邩���Ȃ���
	g_PauseUpdate = state;
	return;
}

BOOL GetPause(void)
{//�Q�[�����X�V���Ă���H
	return g_PauseUpdate;
}

//=============================================================================
// �Q�[���J�E���^�[
//=============================================================================
void SetStart(BOOL start)
{
	g_Start = start;
}

BOOL GetStart(void)
{
	return g_Start;
}

float GetDeltatime(void)
{
	return g_deltatime;
}

void SetBossBGM(BOOL inbossbt)
{
	g_BossBT = inbossbt;
}

void SetResult(BOOL result, int mode)
{
	g_result = result;
	g_ResultMode = mode;
	return;
}

int GetResult(void)
{
	return g_result;
}

int GetResultMODE(void)
{
	return g_ResultMode;
}