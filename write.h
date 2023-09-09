/*******************************************************************************
* �^�C�g��:		���C�g�w�b�_�[
* �v���O������:	write.h
* �쐬��:		GP11A132 30 ���ѕ��F
* �쐬��:		2023/06/14
* 
*******************************************************************************/

#pragma once	// �C���N���[�h�K�[�h�i���d�C���N���[�h�h�~�j


#pragma warning(push)
#pragma warning(disable:4005)

#include "main.h"

#include <d2d1.h>
#include <dwrite.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


/*******************************************************************************
* �}�N����`
*******************************************************************************/

//=============================================================================
//		�t�H���g���X�g
//=============================================================================
enum Font
{
	Meiryo,
	Arial,
	MeiryoUI,

	FONT_MAX
};



/*******************************************************************************
* �\���̒�`
*******************************************************************************/
// �t�H���g�f�[�^�\����
struct FONT_DATA
{
	Font font;								// �t�H���g��
	IDWriteFontCollection* fontCollection;	// �t�H���g�R���N�V����
	DWRITE_FONT_WEIGHT fontWeight;			// �t�H���g�̑���
	DWRITE_FONT_STYLE fontStyle;			// �t�H���g�X�^�C��
	DWRITE_FONT_STRETCH fontStretch;		// �t�H���g�̕�
	FLOAT fontSize;							// �t�H���g�T�C�Y
	WCHAR const* localeName;				// ���P�[����
	DWRITE_TEXT_ALIGNMENT textAlignment;	// �e�L�X�g�̔z�u
	D2D1_COLOR_F Color;						// �t�H���g�̐F

};

// DIRECT_WRITE�\����
struct DIRECT_WRITE
{
	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWriteFactory;
	IDWriteTextFormat* TextFormat;
	IDWriteTextLayout* TextLayout;
	ID2D1RenderTarget* RT;
	ID2D1SolidColorBrush* SolidBrush;
	IDXGISurface* BackBuffer;

	// �t�H���g�f�[�^
	FONT_DATA* FD;

};

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

// DIRECT_WRITE OBJ
void InitDirectWrite(void);		// DIRECT_WRITE�̏���������
void UninitDirectWrite(void);	// DIRECT_WRITE�̏I������


// �����`�揈��
// WCHAR�F������
// pos�F�`��|�W�V����
// options�F�e�L�X�g�̐��`
void DrawString(WCHAR* FontList, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options);

// �����`�揈��
// string�F������
// rect�F�̈�w��
// options�F�e�L�X�g�̐��`
void DrawStringRect(WCHAR* FontList, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options);


// �t�H���g�ݒ�
// ��1�����F�t�H���g�f�[�^�\����
void SetFont(const WCHAR* FontName, float fontSize, const D2D1_COLOR_F color);


DIRECT_WRITE* GetDirectWrite(void);

