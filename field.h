/*******************************************************************************
* タイトル:
* プログラム名:	????.h
* 作成者:		GP11A132 99 外岡高明
* 作成日:		2021/04/26
*******************************************************************************/

#pragma once	// インクルードガード（多重インクルード防止）

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define		MAP_MAX							(3)
#define		MAP_WIDTH						(SCREEN_WIDTH * 2)	//マップ
#define		MAP_HEIGHT						(SCREEN_HEIGHT * MAP_MAX)

#define		GRAVITY			(0.02f)

enum TUTORIAL_FIELD_ITEM_TEXTURE
{
	TUTORIAL_MAPCHIP,
	TUTORIAL_ARROW,
	TUTORIAL_CAVE,
};

enum IOB_MESSAGE
{
	NOKEY,
	DOOR,
	BOSSNODEAD,
	HEROINE,
};
enum INGAMEBGM
{
	BGM_DUNGEON,
	BGM_BOSS,
	BGM_MAX,
};

enum BUS_TYPE
{
	LR_BUS,
	UD_BUS,
};
/*******************************************************************************
* 構造体定義
*******************************************************************************/

struct ROPE
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	float		theta;			//ロープの回転角度
	float		x;
	float		y;
	float		r;				//CollisionBC用半径
};

struct ITEM
{
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	BOOL		clear;			// 透明
	float		w, h;			// 幅と高さ
	int			type;
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	

	float		r;				//CollisionBC用半径
	XMFLOAT3	move;
};

struct ITEM2
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
};

struct TRAPWALL
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号

	XMFLOAT3	move;

};


struct DROP_ITEM
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			type;
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	

};

struct TOGE
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
};


struct TRAP
{
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	BOOL		trig;

	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
	float		gravityCnt;
};

struct FIELD
{

	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	scl;			// ポリゴンの拡大縮小
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号

};

struct PAUSE
{
	BOOL use;
	XMFLOAT3 pos;
};


struct INTERACT_OB
{
	BOOL		use;
	BOOL		col;
	BOOL		display;
	XMFLOAT3	pos;
	int			texNo;					// 何番目のテクスチャーを使用するのか
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
	float		w, h;	// 幅と高さ
};

struct MESSAGE
{
	BOOL		use;
	BOOL		display;
	int			texNo;					// 何番目のテクスチャーを使用するのか
	float		w, h;	// 幅と高さ
};

struct MESSAGEPOS
{
	BOOL		use;
	BOOL		display;
	XMFLOAT3	pos;
	int			texNo;					// 何番目のテクスチャーを使用するのか
	float		w, h;	// 幅と高さ
};


struct HINT
{
	BOOL		use;
	int			texNo;					// 何番目のテクスチャーを使用するのか
	float		w, h;	// 幅と高さ
	XMFLOAT3	pos;
};

struct SKILL
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
	XMFLOAT3	move;
	int			frameCnt;
	int			delayCnt;
	BOOL		startFlag;
};

struct SKILL3
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
	int			frameCnt;
	int			delayCnt;
	BOOL		clear;
	float		gravityCnt;

};

struct SKILL4
{
	XMFLOAT3	pos;			// ポリゴンの座標
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int			texNo;			// テクスチャ番号
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
	int			frameCnt;
	int			delayCnt;
	BOOL		clear;

};



struct ANI
{
	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー	
};

struct ANIMATION
{
	int		divideX;
	int		divideY;
	int		animewait;
	float		w;
	float		h;
};
/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

void InitIOB(void);
void UninitIOB(void);
void UpdateIOB(void);
void DrawIOB(void);

INTERACT_OB* GetIOB(void);
INTERACT_OB* GetDoor(void);
INTERACT_OB* GetHeroine(void);


float GetScrollX(XMFLOAT3 pos);

float GetScrollY(XMFLOAT3 pos);

//引数の位置を中心に全マップの座標を参照し、、今画面の大きさによるXの初期位置を求める
float GetStartCollusionX(XMFLOAT3 pos);

//引数の位置を中心に全マップの座標を参照し、、今画面の大きさによるYの初期位置を求める
float GetStartCollusionY(XMFLOAT3 pos);

float GetStartDownCollusionY(XMFLOAT3 pos);

float GetStartWaistCollusionY(XMFLOAT3 pos);

//フィールドのアドレスを返す
int* GetFieldPtr(void);

//指定されたＸＹ座標のフィールドデータを返す
int GetFieldData(int y, int x);

//posの座標はマップチップの当たり判定BLOCKと同じ？
BOOL FieldCollision(XMFLOAT3 pos, float width, float height);
BOOL DownFieldCollision(XMFLOAT3 pos, float width, float height);
BOOL WaistFieldCollision(XMFLOAT3 pos, float width, float height);

BOOL SkillMouse(ITEM skill);

//マップロード
void LoadMap(void);

void Arcmove(void);
//重力位置計算
XMFLOAT3 GravityPosCal(XMFLOAT3 pos, float gravityCnt);


void InitTrap(void);
void UpdateTrap(void);
void DrawTrap(void);
void DrawFaketrap(void);

void InitBus(void);
void UpdateBus(void);
void DrawBus(void);

void InitRope(void);
void InitHint(void);
void UpdateHint(void);
void DrawHint(void);

void InitKey(void);
void UpdateKey(void);
void DrawKey(void);


void InitToge(void);
void UpdateToge(void);
void DrawToge(void);

void InitDropItem(void);

//鍵をゲットした？
BOOL isGetKey(void);

//プレイヤーはゲームの中にどの段階にいる
int GetBGMmode(void);


void InitDropItem(void);
void UpdateDropItem(void);
void DrawDropItem(void);
void SetDropItem(XMFLOAT3 pos);

void InitTrapWall(void);
void UpdateTrapWall(void);
void DrawTrapWall(void);