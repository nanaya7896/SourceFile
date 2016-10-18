#include "DXFont.h"


//====================================
//�֐���	:CDXFont
//����		:
//�߂�l	:
//���e		:
//====================================
CDXFont::CDXFont(LPDIRECT3DDEVICE9 pDevice)
{
	m_pFont = NULL;
	CreateFont(pDevice);
}

//====================================
//�֐���	:~CDXFont
//����		:
//�߂�l	:
//���e		:
//====================================
CDXFont::~CDXFont()
{
	SAFE_RELEASE(m_pFont);
}

//====================================
//�֐���	:CreateFont
//����		:pDevice:�`��f�o�C�X
//�߂�l	:
//���e		:�t�H���g�̍쐬
//====================================
void CDXFont::CreateFont(LPDIRECT3DDEVICE9 pDevice)
{
	//�t�H���g�X�^�C���̍\����
	D3DXFONT_DESC font;
	//�\���̂̏�����
	ZeroMemory(&font, sizeof(D3DXFONT_DESC));
	//�����̍���
	font.Height = 32;
	//�����̕�
	font.Width	= 32;
	//�E�F�C�g�l
	font.Weight = FW_NORMAL;
	//�~�b�v�}�b�v���x��
	font.MipLevels = D3DX_DEFAULT;
	//�Α�
	font.Italic = FALSE;
	//�����Z�b�g
	font.CharSet = SHIFTJIS_CHARSET;
	//�o�͂̐��x
	font.OutputPrecision = OUT_DEFAULT_PRECIS;
	//�o�͂̕i��
	font.Quality = PROOF_QUALITY;
	//�s�b�`�ƃt�@�~��
	font.PitchAndFamily = VARIABLE_PITCH;
	//�t�H���g��
	::lstrcpy(font.FaceName, "�l�r ����");
	
	//�t�H���g�̍쐬
	D3DXCreateFontIndirect(
		pDevice,	//�`��f�o�C�X
		&font,		//�ݒ肵���X�^�C���̍\���̂̃A�h���X
		&m_pFont	//�󂯎��LPD3DXFONT�̃A�h���X
		);


}

//====================================
//�֐���	:DrawText
//����		:x		:�����W
//			 y		:�����W
//			 pText	:������̍ŏ��̃A�h���X
//			 color	:�F
//�߂�l	:
//���e		:�t�H���g�̕`��
//====================================
void CDXFont::DrawText(int x, int y, const char * pText, D3DCOLOR color,...)
{
	//��`
	RECT rc;
	rc.left = x;
	rc.top = y;
	rc.right = CW_USEDEFAULT;
	rc.bottom = CW_USEDEFAULT;

	//�e�L�X�g����`�ɕ`��
	m_pFont->DrawTextA(
		NULL,				//�X�v���C�g�ւ̃A�h���X
		pText,				//������̐擪�A�h���X
		-1,					//�`�悷�镶������v�Z(-1�Ŏ����v�Z)
		&rc,				//�`��ʒu���̓������\����
		DT_LEFT | DT_NOCLIP,//�\���`�����w��(���悹�A�N���b�s���O�Ȃ�)
		color				//�e�L�X�g�̐F���w��
		);
}
