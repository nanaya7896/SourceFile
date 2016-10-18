//=====================================================================
//���ʓǂݍ��݂̍쐬
//		
//=====================================================================
#pragma once
#define _CRT_SECURE_NO_WARNINGS

//=====define�l���`=====//
#define DIRECTINPUT_VERSION 0x0800			// DirectInputVersion�ݒ�(DX8.0�ȍ~)
#define	TotalAdapter		6
#define SAFE_DELETE(p)		{ if(p) { delete (p);		(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define SAFE_DELETE_ARRAY(p){ if(p) { delete [](p);		(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release();	(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define UM_SETCURSOR		WM_USER + 101	// �J�[�\���̐ݒ�
#define VERTEX_FVF	(D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE)

/*�W���C�X�e�B�b�N�̃L�[���蓖��*/
#define		SQUARE_BUTTON		0
#define		X_MARK_BUTTON		1
#define		CIRCLE_BUTTON		2
#define		TRIANGLE_BUTTON		3
#define		L1_BUTTON			4
#define		R1_BUTTON			5

/*���ʂ̃v���C���[�E�A�C�e���̏��*/
#define		STATE_NONE			0
#define		STATE_HAVE_WG		1
#define     STATE_HAVE_BG		2
#define		STATE_HAVE_BB		3
#define     STATE_BG_SHOT		4
#define		STATE_WG_SHOT		5
#define		STATE_BB_SHOT		6
#define     STATE_BG_PICKUP		7
#define     STATE_WG_PICKUP		8
#define     STATE_BB_PICKUP		9
#define     STATE_WATER_WAIT	10
#define     STATE_BALL_END		11

//�f�o�b�O�o�͗p
#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // �����
#endif

//�E�B���h�E�̑傫�����`
#define WINDOW_WIDTH	1600
#define WINDOW_HEIGHT	900

//=====Lib�̓ǂݍ���=====//
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")

#pragma comment(lib, "dinput8.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"xinput.lib ")

//=====C++=====//
#include <Windows.h>
#include <comdef.h>
#include <dsound.h>
#include <mmsystem.h>
#include <tchar.h>
#include <vector>
#include <strsafe.h>
#include<string.h>
#include<XInput.h>
#include<math.h>
#include<map>
#include<string>
#include<list>
#include<iostream>
#include<sstream>
#include <complex>
#include <XInput.h>
#include <algorithm>
#include <set>

//=====Direct3D=====//
#include <d3d9.h>
#include <d3dx9.h>
#include<d3dx9anim.h>
#include <dinput.h>





//====== �^�錾======//
enum METHOD
{
	D3DNONINDEXED,	// ���_�u�����f�B���O			(�ő�)
	D3DINDEXED,		// �C���f�b�N�X�u�����f�B���O	(�ő�)
	SOFTWARE,		// �\�t�g�E�F�A�u�����f�B���O	(�ᑬ)
};

//�V�[���J�ڗp�\����
struct SCENE 
{
	enum
	{
		TITLE,
		OPERATE,
		MAIN,
		GAMEOVER,
		RESULT,
		CREDIT,
		RANKING
	};
};

enum STATE
{
	Land,
	Have,
	Throw,
	Air,
	Shot
};



//���_�p�\����
struct VTX {
	float x, y, z,u,v;
};

