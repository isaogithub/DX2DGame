/*******************************************************************************
* �^�C�g��:		�����\���v���O����
* �v���O������:	write.cpp
* �쐬��:		GP11A132 29 �є��M
* �쐬��:		2023/08/08
*******************************************************************************/



/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "write.h"
#include "renderer.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define DIRECT_WRITE_MAX	(1)

//=============================================================================
//		�t�H���g��
//=============================================================================

const WCHAR* FontList[FONT_MAX]
{
	L"���C���I",
	L"Arial",
	L"Meiryo UI"
};

/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
DIRECT_WRITE DirectWrite[DIRECT_WRITE_MAX];



void SetFont(const WCHAR* FontName, float fontSize, const D2D1_COLOR_F color)
{
	DirectWrite[0].DWriteFactory[0].CreateTextFormat(FontName, DirectWrite[0].FD[0].fontCollection,
		DirectWrite[0].FD[0].fontWeight, DirectWrite[0].FD[0].fontStyle, DirectWrite[0].FD[0].fontStretch, 
		fontSize, DirectWrite[0].FD[0].localeName, &DirectWrite[0].TextFormat);

	DirectWrite[0].TextFormat[0].SetTextAlignment(DirectWrite[0].FD[0].textAlignment);

	DirectWrite[0].RT[0].CreateSolidColorBrush(color, &DirectWrite[0].SolidBrush);

}





//=============================================================================
//     ������
//=============================================================================
void InitDirectWrite(void)
{
	HRESULT hr = S_OK;

	DIRECT_WRITE* DW = GetDirectWrite();

	// �t�H���g�f�[�^
	DW[0].FD = new FONT_DATA();

	// �f�t�H���g�ݒ�
	DW[0].FD->font = Font::Meiryo;
	DW[0].FD->fontCollection = nullptr;
	DW[0].FD->fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
	DW[0].FD->fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
	DW[0].FD->fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
	DW[0].FD->fontSize = 20;
	DW[0].FD->localeName = L"ja-jp";
	DW[0].FD->textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
	DW[0].FD->Color = D2D1::ColorF(D2D1::ColorF::White);

	// Direct2D,DirectWrite�̏�����
	// ID2D1Factory �C���^�[�t�F�C�X�̍쐬
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &DW[0].D2DFactory);

	if (SUCCEEDED(hr)) {
		// IDWriteFactory�̍쐬
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DW[0].DWriteFactory));
	}

	if (SUCCEEDED(hr)) {
		//�֐�CreateTextFormat()
		//��1�����F�t�H���g���iL"���C���I", L"Arial", L"Meiryo UI"���j
		//��2�����F�t�H���g�R���N�V�����inullptr�j
		//��3�����F�t�H���g�̑����iDWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD���j
		//��4�����F�t�H���g�X�^�C���iDWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC�j
		//��5�����F�t�H���g�̕��iDWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED���j
		//��6�����F�t�H���g�T�C�Y�i20, 30���j
		//��7�����F���P�[�����iL""�j
		//��8�����F�e�L�X�g�t�H�[�}�b�g�i&g_pTextFormat�j
		hr = DW[0].DWriteFactory->CreateTextFormat(FontList[(int)DW[0].FD->font],
			DW[0].FD->fontCollection,
			DW[0].FD->fontWeight,
			DW[0].FD->fontStyle,
			DW[0].FD->fontStretch,
			DW[0].FD->fontSize,
			DW[0].FD->localeName,
			&DW[0].TextFormat);
	}

	if (SUCCEEDED(hr)) {
		//�֐�SetTextAlignment()
		//��1�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, DWRITE_TEXT_ALIGNMENT_TRAILING�F��, DWRITE_TEXT_ALIGNMENT_CENTER�F����,
		//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED�F�s�����ς��j
		hr = DW[0].TextFormat->SetTextAlignment(DW[0].FD->textAlignment);
	}


	// �o�b�N�o�b�t�@�̎擾
	// �^�FIDXGISwapChain
	GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&DW[0].BackBuffer));

	// dpi�̐ݒ�
	FLOAT dpiX;
	FLOAT dpiY;
	DW[0].D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	// �����_�[�^�[�Q�b�g�̍쐬
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,
		D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	if (!DW[0].RT) {
		// �T�[�t�F�X�ɕ`�悷�郌���_�[�^�[�Q�b�g���쐬
		hr = DW[0].D2DFactory->CreateDxgiSurfaceRenderTarget(&DW[0].BackBuffer[0], &props, &DW[0].RT);

		// �A���`�G�C���A�V���O���[�h
		DW[0].RT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

		if (SUCCEEDED(hr))
		{
			//�u���V�쐬�֐�
			//��1�����F�t�H���g�F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w��j
			DW[0].RT->CreateSolidColorBrush(DW[0].FD->Color, &DW[0].SolidBrush);
		}
	}


}

//=============================================================================
//     �I������
//=============================================================================
void UninitDirectWrite(void)
{
	DIRECT_WRITE* DW = GetDirectWrite();
	// �����`��֘A�̃����[�X
	if (DW[0].FD) delete(DW[0].FD);
	if (DW[0].BackBuffer) DW[0].BackBuffer->Release();
	if (DW[0].SolidBrush) DW[0].SolidBrush->Release();
	if (DW[0].RT) DW[0].RT->Release();
	if (DW[0].TextFormat) DW[0].TextFormat->Release();
	if (DW[0].DWriteFactory) DW[0].DWriteFactory->Release();
	if (DW[0].D2DFactory) DW[0].D2DFactory->Release();
	if (DW[0].TextLayout) DW[0].TextLayout->Release();

}


//=============================================================================
// �����`��
// string�F������
// pos�F�`��|�W�V����
// rect�F�̈�w��
// options�F�e�L�X�g�̐��`
//=============================================================================
void DrawString(WCHAR* str, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options)
{
	HRESULT hr = S_OK;
	DIRECT_WRITE* DR = GetDirectWrite();

	// �^�[�Q�b�g�T�C�Y�̎擾
	D2D1_SIZE_F TargetSize = DR[0].RT->GetSize();

	// �e�L�X�g���C�A�E�g���쐬
	hr = DR[0].DWriteFactory->CreateTextLayout(str, (UINT32)wcslen(str), DR[0].TextFormat, TargetSize.width, TargetSize.height, &DR[0].TextLayout);

	if (SUCCEEDED(hr)) {
		// �`��ʒu�̊m��
		D2D1_POINT_2F points;
		points.x = pos.x;
		points.y = pos.y;

		// �`��̊J�n
		DR[0].RT->BeginDraw();

		// �`�揈��
		DR[0].RT->DrawTextLayout(points, DR[0].TextLayout, DR[0].SolidBrush, options);

		// �`��̏I��
		DR[0].RT->EndDraw();
	}
}

void DrawStringRect(WCHAR* str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options)
{
	DIRECT_WRITE* DW = GetDirectWrite();

	// �`��̊J�n
	DW[0].RT->BeginDraw();

	// �`�揈��
	DW[0].RT->DrawText(str, (UINT32)wcslen(str), DW[0].TextFormat, rect, DW[0].SolidBrush, options);

	// �`��̏I��
	DW[0].RT->EndDraw();
}

DIRECT_WRITE* GetDirectWrite(void)
{
	return &DirectWrite[0];
}

