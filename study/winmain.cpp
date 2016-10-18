
//=====IncludeFile=====//
//�f�o�b�N�p���������[�N���o
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include"Lib\Application.h"
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR CmdLine, int CmdShow)
{
	//�������֐��̖{�̕��ň�x���g���Ă��Ȃ��ꍇ�Ɏg�p(WINAPI�̏ꍇ)
	//�����ō�����֐��Ȃ񂩂͌^�����c���ϐ������R�����g�A�E�g����(�g���Ȃ��ӎu�\��)
	UNREFERENCED_PARAMETER(CmdShow); UNREFERENCED_PARAMETER(hPreInstance); UNREFERENCED_PARAMETER(CmdLine);
	//���������[�N���o�p
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

	
	//_CrtSetBreakAlloc(171);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	
	//�C���X�^���X���擾
	CApplication* pApp = CApplication::getInstance();

	//======================================
	//�A�v���P�[�V�����̏�����

	//�A�v���P�[�V������
	LPCSTR WindowTitle = "InnocentGirl_SV";
	//�X�N���[���T�C�Y
	SIZE ScreenSize = {WINDOW_WIDTH,WINDOW_HEIGHT};
	//�X�N���[�����[�h�ݒ�(TRUE:�E�B���h�E FALSE:�t���X�N���[��)
	BOOL bWindowed = TRUE;

	HWND hWnd = pApp->Initialize(hInstance, WindowTitle, ScreenSize, bWindowed);

	if (hWnd == NULL)
	{
		return 0;
	}
	
	//======================================
	return pApp->Run(hWnd);
}