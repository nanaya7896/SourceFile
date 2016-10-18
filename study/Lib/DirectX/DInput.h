#pragma once

#define BUFFER_SIZE			256
//=====Include File=====//
#include"../../Global/Global.h"

//=====�N���X�錾=====//
class CDInput
{
	//private�����o�ϐ�
private:
	//DirectInput�V�X�e��
	LPDIRECTINPUT8			m_pDInput;
	//DIRECTINPUTDEVICE�L�[�{�[�h
	LPDIRECTINPUTDEVICE8	m_pDInputKeyBoard;
	//DIRECTINPUTDEVICE�W���C�X�e�B�b�N
	LPDIRECTINPUTDEVICE8	m_pDInputJoyStick;

	BYTE m_KeyInput[BUFFER_SIZE];
	BYTE m_KeyInputPreview[BUFFER_SIZE];
	//public�����o�ϐ�
public:


	//private�����o�֐�
private:
	void InitDirectInput(HWND);
	void ReleaseDirectInput();

	static BOOL CALLBACK EnumJoyStickCallback(LPCDIDEVICEINSTANCE, LPVOID);

	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE, LPVOID);

	//�񋓂��鎲�͈̔͂�ݒ�

	//public�����o�֐�
public:
	//�R���X�g���N�^
	CDInput(HWND);
	//�f�X�g���N�^
	virtual ~CDInput();

	//�X�V
	HRESULT Update();
	//�L�[�{�[�h�̏�Ԃ̎擾
	//void GetKeyBoardState(LPBYTE);
	//�W���C�X�e�B�b�N�̓��͏��̎擾
	void GetJoyStickState(DIJOYSTATE2*);
	//�l�擾�֐�
	bool KeyState(int code);
	bool KeyStatePreview(int code);
	bool KeyDown(int code);
	bool KeyUp(int code);
	int KeyCount(int code);

	BOOL IsButtonDown(int pos);
	BOOL IsButtonPressed(int pos);
	BOOL LAnalogState(int num);
	BOOL AllControllCheck();
	BOOL WASDKeyUpCheck();
	BOOL WASDKeyDownCheck();
	BOOL RAnalogState(int num);
	DIJOYSTATE2	padData;
	//�p�b�h���i�O�t���[���j
	DIJOYSTATE2 lastpaddata;
	//=====�Q�b�g�֐�=====//
	LPDIRECTINPUT8 GetDInput()
	{
		return m_pDInput;
	}

	LPDIRECTINPUTDEVICE8* GetDInputJoyStick()
	{
		return &m_pDInputJoyStick;
	}
};