//=========================================================
//
// �}�l�[�W������ [manager.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MANAGER_H_     // ���̃}�N����`������ĂȂ�������
#define _MANAGER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"

//===============================================
// �O���錾
//===============================================
class CRenderer;
class CInputKeyboard;
class CInputGamePad;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CObjectX;
class CXFile;
class CFade;
class CFileLoad;

//===============================================
// �V�[���N���X
//===============================================
class CScene
{
public:		// �N�ł��A�N�Z�X�\[�A�N�Z�X�w��q]
	
	// ��ʃ��[�h�̎��
	enum MODE
	{
		MODE_TITLE,		// �^�C�g�����
		MODE_SELECT,	// �Z���N�g���
		MODE_TUTORIAL,	// �`���[�g���A�����
		MODE_GAME,		// �Q�[�����
		MODE_RESULT,	// ���U���g���
		MODE_MAX
	};

	CScene();				// �f�t�H���g�R���X�g���N�^
	virtual ~CScene();		// �f�X�g���N�^
	
	static CScene *Create(HWND hWnd, MODE mode);
	virtual HRESULT Init(HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void SetMode(MODE mode);
	//static MODE GetMode(void) { return m_mode; }
	static MODE GetModeOld(void) { return m_modeOld; }
	static void SetTime(int nTime);

private:	// �����̂݃A�N�Z�X�\[�A�N�Z�X�w��q]
	static MODE m_modeOld;	// �O��̉�ʃ��[�h
	static int m_nTime;						// �X�R�A
};

//===============================================
// �}�l�[�W���N���X
//===============================================
class CManager
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CManager();		// �f�t�H���g�R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);
	void SetFPS(int nCountFPS);
	HWND GetHWND(void) { return m_hWnd; }
	void AddCountDeath(CScene::MODE mode);
	void SetStage(int stage) { m_nStage = stage; }
	int GetStage(void) { return m_nStage; }

	static CManager* GetInstance(void);
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void);

	CRenderer *GetRenderer() { return m_pRenderer; }
	CInputKeyboard *GetKeyboardInput() { return m_pKeyboardInput; }
	CInputGamePad *GetInputGamePad() { return m_pInputGamePad; }
	CDebugProc *GetDebugProc() { return m_pDebugProc; }
	CSound *GetSound() { return m_pSound; }
	CCamera *GetCamera() { return m_pCamera; }
	CLight *GetLight() { return m_pLight; }
	CTexture *GetTexture() { return m_pTexture; }
	CXFile *GetXFile() { return m_pXFile; }
	CFileLoad *GetLoad() { return m_pFileLoad; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CManager *m_pManager;					// �}�l�[�W���N���X�̃|�C���^
	static CRenderer *m_pRenderer;					// �����_���[�N���X�̃|�C���^
	static CInputKeyboard *m_pKeyboardInput;		// �L�[�{�[�h���̓N���X�̃|�C���^
	static CInputGamePad *m_pInputGamePad;			// �Q�[���p�b�h���̓N���X�̃|�C���^
	static CDebugProc *m_pDebugProc;				// �f�o�b�O�\���N���X�̃|�C���^
	static CSound *m_pSound;						// �T�E���h�N���X�̃|�C���^
	static CCamera *m_pCamera;						// �J�����N���X�̃|�C���^
	static CLight *m_pLight;						// ���C�g�N���X�̃|�C���^
	static CTexture *m_pTexture;					// �e�N�X�`���N���X�̃|�C���^
	static CXFile *m_pXFile;						// X�t�@�C���N���X�̃|�C���^
	static CFileLoad *m_pFileLoad;					// �t�@�C�����[�h�N���X�̃|�C���^
	static CScene *m_pScene;						// �V�[���N���X�̃|�C���^

	static HWND m_hWnd;						// �E�C���h�E�ۑ��p
	static CScene::MODE m_mode;				// ���݂̉�ʃ��[�h

	int m_nCountFPS;						// FPS���J�E���g
	int m_nCntDeathGame;					// �Q�[�����[�h���S���J�E���g
	int m_nStage;							// �I�������X�e�[�W
	bool m_bEdit;							// �G�f�B�b�g�؂�ւ�
};

#endif