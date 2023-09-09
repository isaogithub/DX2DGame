//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum
{
	SOUND_LABEL_BGM_opening,	// BGM0
	SOUND_LABEL_BGM_tutorial ,	// BGM0
	SOUND_LABEL_BGM_dungeon,	// BGM0
	SOUND_LABEL_BGM_boss,	// BGM0
	SOUND_LABEL_BGM_gameover,
	SOUND_LABEL_BGM_gameclear,
	SOUND_LABEL_BGM_opening2,		// BGM Maou

	SOUND_LABEL_SE_bomb000,		// 爆発音
	SOUND_LABEL_SE_defend000,	// 
	SOUND_LABEL_SE_defend001,	// 
	SOUND_LABEL_SE_hit000,		// 
	SOUND_LABEL_SE_laser000,	// 
	SOUND_LABEL_SE_lockon000,	// 
	SOUND_LABEL_SE_shot000,		// 
	SOUND_LABEL_SE_shot001,		// 
	SOUND_LABEL_SE_ken001,
	SOUND_LABEL_SE_ken002,
	SOUND_LABEL_SE_jump,
	SOUND_LABEL_SE_skill001,
	SOUND_LABEL_SE_enemyhit,
	SOUND_LABEL_SE_slimehit,
	SOUND_LABEL_SE_slimedead,
	SOUND_LABEL_SE_trapfall,
	SOUND_LABEL_SE_hitplayer,
	SOUND_LABEL_SE_enemyskill01,
	SOUND_LABEL_SE_slimeattack,
	SOUND_LABEL_SE_bosswalk,
	SOUND_LABEL_SE_enemyskill02,
	SOUND_LABEL_SE_key,
	SOUND_LABEL_SE_bullethp,
	SOUND_LABEL_SE_heal001,
	SOUND_LABEL_SE_enemyskill03_dead,
	SOUND_LABEL_SE_enemyskill03_explosion,
	SOUND_LABEL_SE_guard,
	SOUND_LABEL_SE_ultstart,
	SOUND_LABEL_SE_ulthit,
	SOUND_LABEL_SE_gravityBall,
	SOUND_LABEL_SE_warp,
	SOUND_LABEL_SE_eShock,
	SOUND_LABEL_SE_shield,
	SOUND_LABEL_SE_hitshield,

	SOUND_LABEL_SE_itemdrop,
	SOUND_LABEL_SE_slimemove,
	SOUND_LABEL_SE_select,
	SOUND_LABEL_SE_selected,
	SOUND_LABEL_SE_openingselected,
	SOUND_LABEL_SE_bossbomb1,
	SOUND_LABEL_SE_mp,
	SOUND_LABEL_SE_mpUP,



	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

void SetVolumeSound(float volume);
void SetVolumeSound(int label, float volume);
void SetFrequencyRatio(float pitch);
void SetFrequencyRatio(int label, float pitch);
void SetOutputMatrixSound(int label, float left, float right);