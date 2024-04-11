//=========================================================
//
// �}�l�[�W������ [manager.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "texture.h"
#include "garbage.h"
#include "xfile.h"
#include "edit.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "tutorial.h"
#include "meshfield.h"
#include "fileload.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CManager *CManager::m_pManager = NULL;				// �}�l�[�W���N���X�̃|�C���^
CRenderer *CManager::m_pRenderer = NULL;			// �����_���[�N���X�̃|�C���^
CInputKeyboard *CManager::m_pKeyboardInput = NULL;	// �L�[�{�[�h���̓N���X�̃|�C���^
CInputGamePad *CManager::m_pInputGamePad = NULL;	// �Q�[���p�b�h���̓N���X�̃|�C���^
CDebugProc *CManager::m_pDebugProc = NULL;			// �f�o�b�O�\���N���X�̃|�C���^
CSound *CManager::m_pSound = NULL;					// �T�E���h�N���X�̃|�C���^
CCamera *CManager::m_pCamera = NULL;				// �J�����N���X�̃|�C���^
CLight *CManager::m_pLight = NULL;					// ���C�g�N���X�̃|�C���^
CTexture *CManager::m_pTexture = NULL;				// �e�N�X�`���N���X�̃|�C���^
CXFile *CManager::m_pXFile = NULL;					// X�t�@�C���N���X�̃|�C���^
CEdit *CManager::m_pEdit = NULL;					// �G�f�B�b�g�N���X�̃|�C���^
CGarbage *CManager::m_pGarbage = NULL;				// �S�~�N���X�̃|�C���^
CScene * CManager::m_pScene = NULL;					// �V�[���N���X�̃|�C���^
CFileLoad *CManager::m_pFileLoad = NULL;			// ���[�h�N���X�̃|�C���^
HWND CManager::m_hWnd = NULL;						// �E�C���h�E�ۑ��p

CScene::MODE CScene::m_mode = CScene::MODE_TITLE;		// ���݂̉�ʃ��[�h
CScene::MODE CScene::m_modeOld = CScene::MODE_TITLE;	// �O��̉�ʃ��[�h
CScene::MODE CManager::m_mode = CScene::MODE_TITLE;		// ���݂̉�ʃ��[�h

//***********************************************************
// �V�[���N���X
//***********************************************************
//===============================================
// �R���X�g���N�^
//===============================================
CScene::CScene()
{
	
}

//===============================================
// �f�X�g���N�^
//===============================================
CScene::~CScene()
{

}

//===============================================
// ��������
//===============================================
CScene *CScene::Create(HWND hWnd, MODE mode)
{
	CScene *pScene = NULL;

	// �V�[���̐���
	switch (mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		pScene = new CTitle;

		if (m_mode == MODE_RESULT)
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Stop(CSound::LABEL_BGM_RANKING);
			CManager::GetSound()->Stop(CSound::LABEL_BGM_TITLE);
			CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE);

			//if (CManager::GetSound()->GetPlay(CSound::LABEL_BGM_TITLE) == false)
			//{// �^�C�g����BGM���Đ�����Ă��Ȃ�
			//	// �T�E���h�̍Đ�
			//	CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE);
			//}
		}
		else
		{
			// �T�E���h�̒�~
			CManager::GetSound()->Stop();

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE);
		}
		break;

	case MODE_TUTORIAL:	// �`���[�g���A�����
		pScene = new CTutorial;
		break;

	case MODE_GAME:		// �Q�[�����
		pScene = new CGame;
		break;

	case MODE_RESULT:	// ���U���g���
		pScene = new CResult;

		if (m_mode != MODE_TITLE)
		{
			// �T�E���h�̒�~
			CManager::GetSound()->Stop();

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_BGM_RANKING);
		}
		break;
	}

	if (pScene != NULL)
	{// �g�p����Ă���
		// ���[�h�̐ݒ�
		pScene->SetMode(mode);

		// �V�[���̏���������
		pScene->Init(hWnd);
	}

	return pScene;
}

//===============================================
// �V�[���ݒ菈��
//===============================================
void CScene::SetMode(MODE mode)
{
	m_modeOld = m_mode;
	m_mode = mode;
}

//***********************************************************
// �}�l�[�W���N���X
//***********************************************************
//===============================================
// �R���X�g���N�^
//===============================================
CManager::CManager()
{
	// �l�̃N���A
	m_nCountFPS = 0;
	m_bEdit = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CManager::~CManager()
{

}

//===============================================
// ����������
//===============================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �E�C���h�E��ۑ�
	m_hWnd = hWnd;

	// �����_���[�̐���
	m_pRenderer = new CRenderer;

	// �����_���[�̏���������
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// �L�[�{�[�h���͂̐���
	m_pKeyboardInput = new CInputKeyboard;

	// �L�[�{�[�h���͂̏���������
	if (FAILED(m_pKeyboardInput->Init(hInstance, hWnd)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// �Q�[���p�b�h���͂̐���
	m_pInputGamePad = new CInputGamePad;

	// �Q�[���p�b�h���͂̏���������
	if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// �T�E���h�̐���
	m_pSound = new CSound;

	// �T�E���h�̏���������
	if (FAILED(m_pSound->Init(hWnd)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// �J�����̐���
	m_pCamera = new CCamera;

	// �J�����̏���������
	if (FAILED(m_pCamera->Init()))
	{// ���������������s�����ꍇ
		return -1;
	}

	// ���C�g�̐���
	m_pLight = new CLight;

	// ���C�g�̏���������
	if (FAILED(m_pLight->Init()))
	{// ���������������s�����ꍇ
		return -1;
	}

	// �e�N�X�`���̐���
	m_pTexture = new CTexture;

	// �e�N�X�`���̓ǂݍ��ݏ���
	if (FAILED(m_pTexture->Load()))
	{// �ǂݍ��ݏ��������s�����ꍇ
		return -1;
	}

	// ���[�h�̐���
	m_pFileLoad = new CFileLoad;

	if (m_pFileLoad != NULL)
	{// �g�p����Ă���
		// ���[�h�̓ǂݍ��ݏ���
		if (FAILED(m_pFileLoad->Init(hWnd)))
		{// �ǂݍ��ݏ��������s�����ꍇ
			return -1;
		}
	}

	// X�t�@�C���̐���
	m_pXFile = new CXFile;

	// X�t�@�C���̓ǂݍ��ݏ���
	if (FAILED(m_pXFile->Load()))
	{// �ǂݍ��ݏ��������s�����ꍇ
		return -1;
	}

	// ���b�V���t�B�[���h�̐���
	CMeshField::Load(hWnd);

	// �V�[������
	m_pScene = CScene::Create(hWnd, CScene::MODE_TITLE);

	// �f�o�b�O�\���̐���
	m_pDebugProc = new CDebugProc;

	// �f�o�b�O�\���̏���������
	m_pDebugProc->Init();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CManager::Uninit(void)
{
	if (m_pScene != NULL)
	{// �g�p����Ă���
		// �V�[���̏I������
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = NULL;
	}

	if (m_pRenderer != NULL)
	{// ���������g�p����Ă���
		// ���b�V���t�B�[���h�̔j��
		CMeshField::Unload();

		// �L�[�{�[�h���͂̏I������
		m_pKeyboardInput->Uninit();
		delete m_pKeyboardInput;
		m_pKeyboardInput = NULL;

		// �Q�[���p�b�h���͂̏I������
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = NULL;

		// �����_���[�̏I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;

		// �T�E���h�̏I������
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;

		// �J�����̏I������
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;

		// ���C�g�̏I������
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;

		// �e�N�X�`���̔j��
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;

		// �t�@�C���ǂݍ��݂̏I������
		m_pFileLoad->Uninit();
		delete m_pFileLoad;
		m_pFileLoad = NULL;

		// X�t�@�C���̔j��
		m_pXFile->Unload();
		delete m_pXFile;
		m_pXFile = NULL;

		// �f�o�b�O�\���̏I������
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;

		// ���������J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//===============================================
// �X�V����
//===============================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{// ���������g�p����Ă���
		// FPS�̕\��
		m_pDebugProc->Print(" FPS�F%d\n\n", m_nCountFPS);

		// �L�[�{�[�h���͂̍X�V����
		m_pKeyboardInput->Update();
		
		//if (CManager::GetKeyboardInput()->GetTrigger(DIK_R) == true)
		//{// ENTER�L�[����
		//	// ���Z�b�g
		//	CObject::Reset();
		//}
		//else if(CManager::GetKeyboardInput()->GetTrigger(DIK_F2) == true)
		//{// F2�L�[����
		//	// �G�f�B�b�g�؂�ւ�
		//	m_bEdit = m_bEdit ? false : true;
		//	m_pEdit->Set(m_bEdit);
		//}

		// �Q�[���p�b�h���͂̍X�V����
		m_pInputGamePad->Update();

		// �����_���[�̍X�V����
		m_pRenderer->Update();

		if ((CManager::GetMode() == CScene::MODE_GAME && (CGame::GetPauseState() == false || CGame::GetPauseCamera() == true))
			|| CManager::GetMode() == CScene::MODE_TUTORIAL || CManager::GetMode() == CScene::MODE_TITLE)
		{// �|�[�Y���
			// �J�����̍X�V����
			m_pCamera->Update();
		}

		// ���C�g�̍X�V����
		m_pLight->Update();

		// �f�o�b�O�\���̍X�V����
		m_pDebugProc->Update();

		if (m_pScene != NULL)
		{
			// �V�[���̍X�V����
			m_pScene->Update();
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// ���������g�p����Ă���
		// �����_���[�̕`�揈��
		m_pRenderer->Draw();

		if (m_pScene != NULL)
		{
			// �V�[���̕`�揈��
			m_pScene->Draw();
		}
	}
}

//===============================================
// ���Z�b�g����
//===============================================
void CManager::Reset(void)
{
	// �G�̐���
	//m_pEnemy->Create(3)->SetPos(D3DXVECTOR3(700.0f, 250.0f, 0.0f));
	//m_pEnemy->Create(3)->SetPos(D3DXVECTOR3(800.0f, 250.0f, 0.0f));

	// �A�C�e���̐���
	//CItem::Create(4)->Set(CItem::TYPE_TREASURE, D3DXVECTOR3(700.0f, 450.0f, 0.0f));
	//CItem::Create(4)->Set(CItem::TYPE_COIN, D3DXVECTOR3(800.0f, 450.0f, 0.0f));
}

//===============================================
// FPS�ݒ菈��
//===============================================
void CManager::SetFPS(int nCountFPS)
{
	m_nCountFPS = nCountFPS;
}

//===============================================
// �V�[���ݒ菈��
//===============================================
void CManager::SetMode(CScene::MODE mode)
{
	int nScore;
	CScene::MODE modeOld = m_mode;
	m_mode = mode;

	if (mode == CScene::MODE_TUTORIAL || mode == CScene::MODE_GAME)
	{
		// �T�E���h�̒�~
		GetSound()->Stop();
	}

	// �V�[������
	CScene *pScenePrev = m_pScene;

	if (mode == CScene::MODE_RESULT && modeOld != CScene::MODE_TITLE)
	{// ���̃��[�h�����U���g
		nScore = CGame::GetScore()->Get();
	}

	// ���݂̃��[�h�̔j��
	if (pScenePrev != NULL)
	{
		pScenePrev->Uninit();
	}

	// �V�������[�h�̐���
	m_pScene = CScene::Create(m_hWnd, mode);

	if (mode == CScene::MODE_RESULT && modeOld != CScene::MODE_TITLE)
	{// ���̃��[�h�����U���g
		// �����L���O�̐ݒ�
		CResult::GetRanking()->Add(nScore);
	}

	// �������̊J��
	delete pScenePrev;
	pScenePrev = NULL;
}

//===============================================
// �V�[���擾����
//===============================================
CScene::MODE CManager::GetMode(void)
{
	return m_mode;
}