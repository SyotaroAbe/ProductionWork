//=======================================================================
//
// ���͏��� [input.cpp]
// Author = �����đ�Y
//
//=======================================================================
#include "input.h"

//=========================================================
// �ÓI�����o�ϐ�
//=========================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//***********************************************************
// ���̓N���X�i�e�j
//***********************************************************
//=========================================================
// �R���X�g���N�^
//=========================================================
CInput::CInput()
{
	// �l�̃N���A
	m_pDevice = NULL;
	m_pInput = NULL;
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CInput::~CInput()
{

}

//=========================================================
// ����������
//=========================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{// ���������g�p����Ă��Ȃ�
		// DirectInput�I�u�W�F�N�g�̏�����
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=========================================================
// �I������
//=========================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();			// �L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// Directinput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//***********************************************************
// �L�[�{�[�h���̓N���X�i�q�j
//***********************************************************
//=========================================================
// �R���X�g���N�^
//=========================================================
CInputKeyboard::CInputKeyboard()
{
	// �l�̃N���A
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;
		m_aKeyStateTrigger[nCntKey] = NULL;
		m_aKeyStateRelease[nCntKey] = NULL;
		m_aKeyStateRepeat[nCntKey] = NULL;
	}
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=========================================================
// ����������
//=========================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͂̏���������
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//=========================================================
// �I������
//=========================================================
void CInputKeyboard::Uninit(void)
{
	// ���̓N���X�̏I������
	CInput::Uninit();
}

//=========================================================
// �X�V����
//=========================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �L�[�{�[�h�̃��s�[�g����ۑ�
			m_aKeyStateRepeat[nCntKey] = (m_aKeyState[nCntKey] & aKeyState[nCntKey]);

			// �L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();				// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=======================================================================
// �v���X�����擾
//=======================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=======================================================================
// �g���K�[�����擾
//=======================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=======================================================================
// ���s�[�g�����擾
//=======================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=======================================================================
// �����[�X�����擾
//=======================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//***********************************************************
// �Q�[���p�b�h���̓N���X�i�q�j
//***********************************************************
//=========================================================
// �R���X�g���N�^
//=========================================================
CInputGamePad::CInputGamePad()
{
	// �l�̃N���A
	for (int nCntKey = 0; nCntKey < NUM_PLAYER; nCntKey++)
	{
		m_aState[nCntKey] = {};
		m_aStateTrigger[nCntKey] = {};
		m_aStateRelease[nCntKey] = {};
		m_aStateRepeat[nCntKey] = {};
	}
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CInputGamePad::~CInputGamePad()
{

}

//=======================================================================
// ����������
//=======================================================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ��錾
	int nCntPad;

	// XInput�̃X�e�[�g(�g�����)
	XInputEnable(true);

	// �������̏�����(�v���C���[��)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&m_aState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//=======================================================================
// �I������
//=======================================================================
void CInputGamePad::Uninit(void)
{
	// XInput�̃X�e�[�g(�g��Ȃ����)
	XInputEnable(false);
}

//=======================================================================
// �X�V����
//=======================================================================
void CInputGamePad::Update(void)
{
	// �ϐ��錾
	int nCntPad;

	XINPUT_STATE aGamePadState;			// �Q�[���p�b�h�̓��͏��

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		// ���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			// �Q�[���p�b�h�̃g���K�[����ۑ�
			m_aStateTrigger[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			// ���s�[�g���
			m_aStateRepeat[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			// �����[�X���
			m_aStateRelease[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			// �Q�[���p�b�h�̃v���X����ۑ�
			m_aState[nCntPad] = aGamePadState;
		}
	}
}

//=======================================================================
// �v���X���擾
//=======================================================================
bool CInputGamePad::GetPress(JOYKEY nKey, int nPlayer)
{
	return (m_aState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// �g���K�[���擾
//=======================================================================
bool CInputGamePad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return (m_aStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// �����[�X���擾
//=======================================================================
bool CInputGamePad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return (m_aStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// ���s�[�g���擾
//=======================================================================
bool CInputGamePad::GetRepeat(JOYKEY nKey, int nPlayer)
{
	return(m_aStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}