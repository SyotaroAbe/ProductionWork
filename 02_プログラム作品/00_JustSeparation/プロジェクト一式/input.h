//=======================================================================
//
// ���͏��� [input.h]
// Author = �����đ�Y
//
//=======================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

//===============================================
// ���C�u�����̃����N
//===============================================
#pragma comment(lib,"xinput.lib")

//=========================================================
// �}�N����`
//=========================================================
#define NUM_KEY_MAX			(256)			// �L�[�̍ő吔
#define NUM_PLAYER			(4)				// �v���C���[��

//=========================================================
// ���̓N���X
//=========================================================
class CInput
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CInput();				// �f�t�H���g�R���X�g���N�^
	virtual ~CInput();		// �f�X�g���N�^

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	static LPDIRECTINPUT8 m_pInput;		
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//=========================================================
// �L�[�{�[�h���̓N���X
//=========================================================
class CInputKeyboard : public CInput
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CInputKeyboard(); 		   // �f�t�H���g�R���X�g���N�^
	~CInputKeyboard();		   // �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	BYTE m_aKeyState[NUM_KEY_MAX];			// �v���X�̏��
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �g���K�[�̏��
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// �����[�X�̏��
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];	// ���s�[�g�̏��
};

//=========================================================
// �Q�[���p�b�h���̓N���X
//=========================================================
class CInputGamePad : public CInput
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CInputGamePad();			   // �f�t�H���g�R���X�g���N�^
	~CInputGamePad();			   // �f�X�g���N�^

	// �����ꂽ�{�^���̎��
	typedef enum
	{
		BUTTON_UP = 0,				// �\���� 
		BUTTON_DOWN,				// �\����
		BUTTON_LEFT,				// �\����
		BUTTON_RIGHT,				// �\����
		BUTTON_START,				// start
		BUTTON_BACK,				// back
		BUTTON_LS,					// ���X�e�B�b�N��������
		BUTTON_RS,					// �E�X�e�B�b�N��������
		BUTTON_LB,					// LB/5
		BUTTON_RB,					// RB/6
		BUTTON_10,					// ?
		BUTTON_11,					// ?
		BUTTON_A,					// A/3 
		BUTTON_B,					// B/4
		BUTTON_X,					// X/1
		BUTTON_Y,					// Y/2
		BUTTON_16,					// ?
		BUTTON_17,					// ?
		BUTTON_18,					// ?
		BUTTON_19,					// ?
		BUTTON_20,					// ?
		BUTTON_MAX
	} JOYKEY;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(JOYKEY nKey, int nPlayer);
	bool GetTrigger(JOYKEY nKey, int nPlayer);
	bool GetRelease(JOYKEY nKey, int nPlayer);
	bool GetRepeat(JOYKEY nKey, int nPlayer);
	SHORT GetJoyStickLX(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbLX; }
	SHORT GetJoyStickLY(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbLY; }
	SHORT GetJoyStickRX(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbRX; }
	SHORT GetJoyStickRY(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbRY; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	XINPUT_STATE m_aState[NUM_PLAYER];			// �v���X�̏��
	XINPUT_STATE m_aStateTrigger[NUM_PLAYER];	// �g���K�[�̏��
	XINPUT_STATE m_aStateRelease[NUM_PLAYER];	// �����[�X�̏��
	XINPUT_STATE m_aStateRepeat[NUM_PLAYER];	// ���s�[�g�̏��
};

////�}�E�X
//HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
//void UninitMouse(void);
//void UpdateMouse(void);
////bool GetMousePress(int nKey);
//void GetMouseDB(void);

#endif
