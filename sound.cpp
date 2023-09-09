//=============================================================================
//
// サウンド処理 [sound.cpp]
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDPARAM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

// 各音素材のパラメータ
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ (char*)"data/BGM/bgm_opening.wav", -1 },	// BGM
	{ (char*)"data/BGM/bgm_tutorial.wav", -1 },	// BGM0
	{ (char*)"data/BGM/bgm_dungeon.wav", -1 },	// BGM0
	{ (char*)"data/BGM/bgm_boss.wav", -1 },		// BGM0
	{ (char*)"data/BGM/gameover.wav", -1 },			// BGM Maou
	{ (char*)"data/BGM/gameclear.wav", -1 },			// BGM Maou
	{ (char*)"data/BGM/opening.wav", -1 },			// BGM Maou

	{ (char*)"data/SE/bomb000.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/defend000.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/defend001.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/hit000.wav", 0 },			// 弾発射音
	{ (char*)"data/SE/laser000.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/lockon000.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/shot000.wav", 0 },		// 弾発射音
	{ (char*)"data/SE/shot001.wav", 0 },		// ヒット音
	{ (char*)"data/SE/ken.wav", 0 },
	{ (char*)"data/SE/ken002.wav", 0 },
	{ (char*)"data/SE/jump1.wav", 0 },
	{ (char*)"data/SE/skill001.wav", 0 },
	{ (char*)"data/SE/enemy_hit.wav", 0 },
	{ (char*)"data/SE/slime_hit.wav", 0 },
	{ (char*)"data/SE/slime_dead.wav", 0 },
	{ (char*)"data/SE/trap_fall.wav", 0 },
	{ (char*)"data/SE/hit_Player.wav", 0 },
	{ (char*)"data/SE/enemy_skill01.wav", 0 },
	{ (char*)"data/SE/slime_attack.wav", 0 },
	{ (char*)"data/SE/boss_walk.wav", 0 },
	{ (char*)"data/SE/enemy_skill02.wav", 0 },
	{ (char*)"data/SE/key.wav", 0 },
	{ (char*)"data/SE/bullet_hp.wav", 0 },
	{ (char*)"data/SE/heal001.wav", 0 },
	{ (char*)"data/SE/skill03_dead.wav", 0 },
	{ (char*)"data/SE/skill03_explosion.wav", 0 },
	{ (char*)"data/SE/guard.wav", 0 },
	{ (char*)"data/SE/ult_start.wav", 0 },
	{ (char*)"data/SE/ult_hit.wav", 0 },
	{ (char*)"data/SE/gravityBall.wav", 0 },
	{ (char*)"data/SE/warp.wav", 0 },
	{ (char*)"data/SE/electricShock.wav", 0 },
	{ (char*)"data/SE/shield.wav", 0 },
	{ (char*)"data/SE/hitshield.wav", 0 },
	{ (char*)"data/SE/itemdrop.wav", 0 },
	{ (char*)"data/SE/slime_move.wav", 0 },
	{ (char*)"data/SE/select1.wav", 0 },
	{ (char*)"data/SE/selected.wav", 0 },
	{ (char*)"data/SE/opening_selected.wav", 0 },
	{ (char*)"data/SE/bossbomb.wav", 0 },
	{ (char*)"data/SE/mpalerm.wav", 0 },
	{ (char*)"data/SE/mpUp.wav", 0 },


};

//=============================================================================
// 初期化処理
//=============================================================================
bool InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return false;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}
	
		// WAVEファイルのチェック RIFF
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}
		if(dwFiletype != 'EVAW')//WAVE
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return false;
		}
	
		// フォーマットチェック  fmt
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}

		// オーディオデータ読み込み data
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return false;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return false;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return false;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		g_apSourceVoice[nCntSound]->SetVolume(0.0f);

	}

	return true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
void PlaySound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}


//=============================================================================
// 音量設定(全て)
//=============================================================================
void SetVolumeSound(float volume)//振幅の調整
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 音量設定
			g_apSourceVoice[nCntSound]->SetVolume(volume);
		}
	}
}


//=============================================================================
// 音量設定(ラベル指定)
//=============================================================================
void SetVolumeSound(int label, float volume)//振幅の調整
{
	if (g_apSourceVoice[label])
	{
		// 音量設定
		g_apSourceVoice[label]->SetVolume(volume);
	}
}

//=============================================================================
// ピッチ設定(全て)
//=============================================================================
void SetFrequencyRatio(float pitch)//
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// 音量設定
			g_apSourceVoice[nCntSound]->SetFrequencyRatio(pitch);
		}
	}
}

//=============================================================================
// ピッチ設定(ラベル指定)
//=============================================================================
void  SetFrequencyRatio(int label, float pitch)//振幅の調整
{
	if (g_apSourceVoice[label])
	{
		// 音量設定
		g_apSourceVoice[label]-> SetFrequencyRatio(pitch);
	}
}

//=============================================================================
// 回饗
//=============================================================================
float g_Volume [2] = {};
float g_Volume2[4] = {};
void  SetOutputMatrixSound(int label, float left,float right)
{
	int InChannels;
	int OutChannels;
	XAUDIO2_VOICE_DETAILS details;

	if (g_apSourceVoice[label])
	{
		
		g_apSourceVoice[label]->GetVoiceDetails(&details);
		InChannels = details.InputChannels;

		g_pMasteringVoice->GetVoiceDetails(&details);
		OutChannels = details.InputChannels;

		if (InChannels == 1)
		{
			g_Volume[0] = left;
			g_Volume[1] = right;
			
			g_apSourceVoice[label]->SetOutputMatrix(NULL, InChannels, OutChannels, g_Volume);
		}
		else if (InChannels == 2)
		{
			g_Volume2[0] = left;
			g_Volume2[1] = left;
			g_Volume2[2] = right;
			g_Volume2[3] = right;

			// 音量設定
			g_apSourceVoice[label]->SetOutputMatrix(NULL, InChannels, OutChannels, g_Volume2);
		}
		
	}
}


//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

