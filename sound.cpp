//=============================================================================
//
// �T�E���h���� [sound.cpp]
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
} SOUNDPARAM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

// �e���f�ނ̃p�����[�^
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ (char*)"data/BGM/bgm_opening.wav", -1 },	// BGM
	{ (char*)"data/BGM/bgm_tutorial.wav", -1 },	// BGM0
	{ (char*)"data/BGM/bgm_dungeon.wav", -1 },	// BGM0
	{ (char*)"data/BGM/bgm_boss.wav", -1 },		// BGM0
	{ (char*)"data/BGM/gameover.wav", -1 },			// BGM Maou
	{ (char*)"data/BGM/gameclear.wav", -1 },			// BGM Maou
	{ (char*)"data/BGM/opening.wav", -1 },			// BGM Maou

	{ (char*)"data/SE/bomb000.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/defend000.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/defend001.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/hit000.wav", 0 },			// �e���ˉ�
	{ (char*)"data/SE/laser000.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/lockon000.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/shot000.wav", 0 },		// �e���ˉ�
	{ (char*)"data/SE/shot001.wav", 0 },		// �q�b�g��
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
// ����������
//=============================================================================
bool InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return false;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return false;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}
	
		// WAVE�t�@�C���̃`�F�b�N RIFF
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}
		if(dwFiletype != 'EVAW')//WAVE
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return false;
		}
	
		// �t�H�[�}�b�g�`�F�b�N  fmt
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ��� data
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return false;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return false;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return false;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		g_apSourceVoice[nCntSound]->SetVolume(0.0f);

	}

	return true;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
void PlaySound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}


//=============================================================================
// ���ʐݒ�(�S��)
//=============================================================================
void SetVolumeSound(float volume)//�U���̒���
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// ���ʐݒ�
			g_apSourceVoice[nCntSound]->SetVolume(volume);
		}
	}
}


//=============================================================================
// ���ʐݒ�(���x���w��)
//=============================================================================
void SetVolumeSound(int label, float volume)//�U���̒���
{
	if (g_apSourceVoice[label])
	{
		// ���ʐݒ�
		g_apSourceVoice[label]->SetVolume(volume);
	}
}

//=============================================================================
// �s�b�`�ݒ�(�S��)
//=============================================================================
void SetFrequencyRatio(float pitch)//
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound])
		{
			// ���ʐݒ�
			g_apSourceVoice[nCntSound]->SetFrequencyRatio(pitch);
		}
	}
}

//=============================================================================
// �s�b�`�ݒ�(���x���w��)
//=============================================================================
void  SetFrequencyRatio(int label, float pitch)//�U���̒���
{
	if (g_apSourceVoice[label])
	{
		// ���ʐݒ�
		g_apSourceVoice[label]-> SetFrequencyRatio(pitch);
	}
}

//=============================================================================
// ���
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

			// ���ʐݒ�
			g_apSourceVoice[label]->SetOutputMatrix(NULL, InChannels, OutChannels, g_Volume2);
		}
		
	}
}


//=============================================================================
// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

