//=============================================================================
//
// メイン処理 [main.cpp]
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



#pragma comment(lib,"d2d1.lib")//---★追加---
#pragma comment(lib,"dwrite.lib")//---★追加---

#include <d2d1.h>//---★追加---
#include <dwrite.h>//---★追加---
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"勇者の旅路～はじまりの声～"			// ウインドウのキャプション名

#define PAUSE_SECOND_MAX		(2)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif

int	g_Mode = MODE_TITLE;					// 起動時の画面を設定

BOOL g_LoadGame = FALSE;					// NewGame

BOOL g_PauseUpdate = FALSE;					//ゲームを一時的更新処理を停止する
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

//ダイレクトライト
ID2D1Factory* g_pD2DFactory = nullptr;
IDWriteFactory* g_pDWriteFactory = nullptr;
IDWriteTextFormat* g_pTextFormat = nullptr;
ID2D1RenderTarget* g_pRT = nullptr;
ID2D1SolidColorBrush* g_pSolidBrush = nullptr;
IDXGISurface* g_pBackBuffer = nullptr;

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// ウィンドウモードかフルスクリーンモードかの処理
	BOOL mode = TRUE;

	int id = MessageBox(NULL, "Windowモードでプレイしますか？", "起動モード", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// YesならWindowモードで起動
		mode = TRUE;
		break;
	case IDNO:		// Noならフルスクリーンモードで起動
		mode = FALSE;	// 環境によって動かない事がある
		break;
	case IDCANCEL:	// CANCELなら終了
	default:
		return -1;
		break;
	}

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, mode)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
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
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
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

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	//DestroyWindow(hWnd);

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	g_beforetime = timeGetTime();
	g_deltatime = 0;
	g_result = GAMEOVER;
	g_ResultMode = RESULTTYPE_GAME;	// 描画の初期化

	InitRenderer(hInstance, hWnd, bWindow);

	// カメラの初期化
	InitCamera();

	// ライトを有効化
	SetLightEnable(FALSE);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// サウンド処理の初期化
	InitSound(hWnd);

	// フェード処理の初期化
	InitFade();

	//InitDirectWrite();

	// 最初のモードをセット
	SetMode(g_Mode);	// ここはSetModeのままで！

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 終了のモードをセット
	if(GetMode() != MODE_QUIT)SetMode(MODE_MAX);

	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// 入力の終了処理
	UninitInput();

	// カメラの終了処理
	UninitCamera();

	// レンダラーの終了処理
	UninitRenderer();

	//UninitDirectWrite();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	g_deltatime = (float)timeGetTime() -  g_beforetime;
	g_beforetime = timeGetTime();
	
	// 入力の更新処理
	UpdateInput();

	// カメラ更新
	UpdateCamera();

	SetVolumeSound(SOUND_LABEL_SE_hit000, 0.4f);
	SetVolumeSound(SOUND_LABEL_SE_gravityBall, 1.8f);

	// モードによって処理を分ける
	switch (g_Mode)
	{
	case MODE_TITLE:		// タイトル画面の更新

		SetVolumeSound(SOUND_LABEL_BGM_opening, 0.6f);
		UpdateTitle();
		break;

	case MODE_OPENING:		// オープンニング画面の更新


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

	case MODE_GAME:			// ゲーム画面の更新

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

	case MODE_BOSS:			// ゲーム画面の更新

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

	case MODE_RESULT:		// リザルト画面の更新
		UpdateResult();
		break;
	}

	UpdateFade();			// フェードの更新処理
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	SetCamera();

	// 2Dの物を描画する処理
	SetViewPort(TYPE_FULL_SCREEN);

	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);


	// モードによって処理を分ける
	switch (g_Mode)
	{
	case MODE_TITLE:		// タイトル画面の描画
		DrawTitle();
		break;

	case MODE_OPENING:		// オープンニング画面の描画
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
	case MODE_GAME:			// ゲーム画面の描画
		DrawBG();
		DrawField();

		DrawEnemy();
		DrawPlayer();
		DrawSkill002();
		DrawBullet();		// 重なる順番を意識してね
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

	case MODE_BOSS:			// ゲーム画面の描画

		DrawBG();
		DrawField();

		DrawEnemy();
		DrawPlayer();



		DrawSkill002();
		DrawNuma();

		DrawBullet();		// 重なる順番を意識してね

		DrawEffect();
		if (g_PauseUpdate)
		{
			DrawDark();
			DrawMessage();
		}
		DrawHitScore();

		if (!GetPause())DrawUI();
		break;



	case MODE_RESULT:		// リザルト画面の描画
		DrawResult();
		break;
	}


	DrawFade();				// フェード画面の描画


#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
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
// モードの設定
//=============================================================================
void SetMode(int mode)
{
	// モードを変える前に全部メモリを解放しちゃう
	StopSound();			// まず曲を止める

	// モードを変える前に全部メモリを解放しちゃう

	// タイトル画面の終了処理
	UninitTitle();
	//オープンニングの終了処理
	UninitOpening();
	// BGの終了処理
	UninitBG();

	//フィールド終了処理
	UninitField();

	// プレイヤーの終了処理
	UninitPlayer();

	// エネミーの終了処理
	UninitEnemy();

	// バレットの終了処理
	UninitBullet();

	// リザルトの終了処理
	UninitResult();

	// エフェクトの終了処理
	UninitEffect();

	UninitTutorial();

	UninitHitScore();

	UninitUI();

	g_StartCnt = 0;
	g_Start = FALSE;

	g_Mode = mode;	// 次のモードをセットしている

	switch (g_Mode)
	{
	case MODE_TITLE:
		// タイトル画面の初期化
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
		// ゲーム画面の初期化

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
		// ゲーム画面の初期化
		InitTutorial();

		InitBG();
		InitField();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEffect();
		InitHitScore();
		InitUI();

		// ロードゲームだったらすべての初期化が終わった後にセーブデータを読み込む
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ロードしたからフラグをClearする
		}

		PlaySound(SOUND_LABEL_BGM_dungeon);

		break;

	case MODE_BOSS:
		// ゲーム画面の初期化
		InitTutorial();

		InitBG();
		InitField();
		InitPlayer();
		InitEnemy();
		InitBullet();
		InitEffect();
		InitHitScore();
		InitUI();

		// ロードゲームだったらすべての初期化が終わった後にセーブデータを読み込む
		if (g_LoadGame == TRUE)
		{
			LoadData();
			g_LoadGame = FALSE;		// ロードしたからフラグをClearする
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
// モードの取得
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}


//=============================================================================
// ニューゲームかロードゲームかをセットする
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
// ゲームから抜ける
//=============================================================================

void QuitGame(void)
{
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	// 終了処理
	//Uninit();

	if (DestroyWindow(hWnd))
		MessageBox(NULL, TEXT("ウィンドウを削除しました"), TEXT("情報"), MB_OK);
}

//=============================================================================
// ゲームを一時的停止
//=============================================================================
void SetPause(BOOL state)
{//ゲームを更新するかしないか
	g_PauseUpdate = state;
	return;
}

BOOL GetPause(void)
{//ゲーム今更新している？
	return g_PauseUpdate;
}

//=============================================================================
// ゲームカウンター
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