#pragma once
//=====Include File=====//
#include"../../Global/Global.h"

//=====�N���X�錾=====//
class CDXFont
{
	//private�����o�ϐ�
private:
	//�t�H���g
	LPD3DXFONT m_pFont;


	//public�����o�ϐ�
public:


	//private�����o�֐�
private:


	//public�����o�֐�
public:
	//�R���X�g���N�^
	CDXFont(LPDIRECT3DDEVICE9);
	//�f�X�g���N�^
	~CDXFont();
	void CreateFont(LPDIRECT3DDEVICE9);
	void DrawText(int, int, const char*, D3DCOLOR,...);
	//=====�Q�b�g�֐�====//
	LPD3DXFONT getDXFont()
	{
		return m_pFont;
	}
};