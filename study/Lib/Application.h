#pragma once

//=====includeFile=====
#include"../Global/Global.h"
#include"TimeThread.h"
#include"DirectX\DXDevice.h"

//======�N���X�錾=====
class CApplication
{
private:
	//�C���X�^���X
	static CApplication m_pAppInstance;

	// �ŏ����̗L��
	BOOL					m_bWindowActive;
	
	// �\�����W	
	POINT					m_ptWindowShowPosition;

	//�T�C�Y
	SIZE					m_WindowSize;

	//�Q�[���f�o�C�X
	CDXDevice*				m_pDXDevice;

public:
	static CApplication* getInstance();
	HWND Initialize(HINSTANCE, LPCTSTR, SIZE, BOOL);

	int	Run(HWND);
	virtual ~CApplication();

private:
	CApplication();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	HWND InitWindow(HINSTANCE, LPCTSTR, SIZE, BOOL);
	SIZE getClientSize(HWND);
	SIZE getWindowSize(HWND);
	void WindowMoveToCenter(HWND);
	void setClientSize(HWND, SIZE);


};
