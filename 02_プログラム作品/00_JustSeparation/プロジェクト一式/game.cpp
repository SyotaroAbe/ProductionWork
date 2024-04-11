//=========================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "game.h"
#include "player.h"
#include "score.h"
#include "time.h"
#include "object3D.h"
#include "objectX.h"
#include "garbage.h"
#include "meshfield.h"
#include "target.h"
#include "pause.h"
#include "input.h"
#include "ui.h"
#include "uigage.h"
#include "renderer.h"
#include "fade.h"
#include "uitarget.h"
#include "camera.h"
#include "debugproc.h"
#include "sound.h"
#include "justdust.h"
#include "fever.h"
#include "hint.h"
#include "gagefull.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CPlayer *CGame::m_pPlayer = NULL;						// �v���C���[�N���X�̃|�C���^
CNumber *CGame::m_pNumber = NULL;						// �i���o�[�N���X�̃|�C���^
CTime *CGame::m_pTime = NULL;							// �^�C���N���X�̃|�C���^
CScore *CGame::m_pScore = NULL;							// �X�R�A�N���X�̃|�C���^
CGarbage *CGame::m_pGarbage = NULL;						// �S�~�N���X�̃|�C���^
CMeshField *CGame::m_pMeshField = NULL;					// ���b�V���t�B�[���h�N���X�̃|�C���^
CObject3D *CGame::m_pObject3D = NULL;					// �I�u�W�F�N�g3D�N���X�̃|�C���^
CTarget *CGame::m_pTarget = NULL;						// �^�[�Q�b�g�N���X�̃|�C���^
CDumpster *CGame::m_pDumpster[MAX_DUMPSTER] = {};		// �S�~�X�e�[�V�����N���X�̃|�C���^
CPause *CGame::m_pPause = NULL;							// �|�[�Y�N���X�̃|�C���^
CUi *CGame::m_pUi = NULL;								// UI�N���X�̃|�C���^
CUiGage *CGame::m_pUiGage = NULL;						// �S�~�Q�[�W�N���X�̃|�C���^
CUiTarget *CGame::m_pUiTarget = NULL;					// �^�[�Q�b�gUI�N���X�̃|�C���^
CJustDust *CGame::m_pJustDust = NULL;					// JustDust�\���N���X�̃|�C���^
CFever *CGame::m_pFever = NULL;							// Fever�\���N���X�̃|�C���^
CHint *CGame::m_pHint = NULL;							// �q���g�\���N���X�̃|�C���^
CGageFull *CGame::m_pGageFull = NULL;					// �Q�[�W�t���\���N���X�̃|�C���^

bool CGame::m_bPause = false;				// �|�[�Y���
bool CGame::m_bStateReady = false;			// GAMSESTATE_READY���ǂ���
bool CGame::m_bPauseCamera = false;			// �|�[�Y���̃J��������\��

//===============================================
// �R���X�g���N�^
//===============================================
CGame::CGame() : CScene()
{
	// �l�̃N���A
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_nTimeFever = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CGame::~CGame()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CGame::Init(HWND hWnd)
{
	m_bPause = false;
	m_bStateReady = true;		// �ҋ@��Ԃɂ���
	m_bPauseCamera = false;

	// �J�����̏���������
	CManager::GetCamera()->Init();

	// ���b�V���t�B�[���h�̐���
	CMeshField::RandArrange();

	// �I�u�W�F�N�gX�t�@�C���̐���
	CObjectX::Load(hWnd);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

	// ���݂̐���
	CGarbage::Load();

	// �S�~�X�e�[�V�����̓ǂݍ��݂Ɛ���
	CDumpster::Load(hWnd);

	// �^�[�Q�b�g�̐���
	m_pTarget = CTarget::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CGarbage::SEPARATION_NONE, -1, 5);

	// �^�[�Q�b�gUI�̐���
	m_pUiTarget = CUiTarget::Create(5);

	// �Q�[�W�t���̐���
	m_pGageFull = CGageFull::Create(6);

	// JustDust�̐���
	m_pJustDust = CJustDust::Create(5);

	// �X�R�A�̐���
	m_pScore = CScore::Create(6);

	// �^�C���̐���
	m_pTime = CTime::Create(6);

	// �S�~�Q�[�W�̐���
	m_pUiGage = CUiGage::Create(6);

	// �q���g�\���̐���
	m_pHint = CHint::Create();

	// �|�[�Y�̐���
	m_pPause = CPause::Create(6);

	// �ʏ��Ԃɐݒ�
	m_state = STATE_NORMAL;
	m_nCounterState = 0;

	// �T�E���h�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGame::Uninit(void)
{
	// �^�C���̏I������
	m_pTime->Uninit();
	delete m_pTime;
	m_pTime = NULL;

	// �X�R�A�̏I������
	m_pScore->Uninit();
	delete m_pScore;
	m_pScore = NULL;

	// �S�~�Q�[�W�̏I������
	delete m_pUiGage;
	m_pUiGage = NULL;

	// �^�[�Q�b�gUI�̏I������
	delete m_pUiTarget;
	m_pUiTarget = NULL;

	// �|�[�Y�̏I������
	m_pPause->Uninit();
	delete m_pPause;
	m_pPause = NULL;

	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CGame::Update(void)
{
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_P) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// �|�[�Y����
		m_bPause = m_bPause ? false : true;		// �|�[�Y��Ԑ؂�ւ�

		if (m_bPause == true)
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE);
		}
		else
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
		}
	}

#if _DEBUG
	if (m_bPause == true && CManager::GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// �|�[�Y���J��������
		m_bPauseCamera = m_bPauseCamera ? false : true;		// �|�[�Y��Ԑ؂�ւ�
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
	}
#endif

	if (m_bStateReady == false)
	{// �ҋ@��Ԃ���Ȃ�
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			// �^�C���̍X�V����
			m_pTime->Update();

			// �X�R�A�̍X�V����
			m_pScore->Update();

			// �S�~�Q�[�W�̍X�V����
			m_pUiGage->Update();

			// �^�[�Q�b�gUI�̍X�V����
			m_pUiTarget->Update();
		}
	}
	else if (m_bStateReady == true)
	{// �ҋ@��Ԃ̃t���O�������Ă���
		if (m_state == STATE_NORMAL)
		{
			// �ҋ@��Ԃ֐؂�ւ���
			m_state = STATE_READY;
			m_nCounterState = TIME_STATEREADY;
		}
	}

	if (m_bPause == true && m_bPauseCamera == false)
	{// �|�[�Y���
		// �|�[�Y�̍X�V����
		m_pPause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:  // �ʏ���
		m_nCounterState--;

		if (m_pTime->Get() == TIME_FEVER)
		{
			m_state = STATE_FEVER;				// �t�B�[�o�[��Ԃɐݒ�

			// Fever�̐���
			m_pFever = CFever::Create();

			// �T�E���h�̒�~
			CManager::GetSound()->Stop(CSound::LABEL_BGM_GAME);

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_BGM_FEVER);
		}
		break;

	case STATE_READY:	// �J�n�ҋ@���
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NORMAL;				// �ʏ��Ԃɐݒ�
				m_nCounterState = TIME_STATEFEVER;	// ���̏�Ԃւ̎��Ԃ�ݒ�
				m_bStateReady = false;
			}
		}
		break;

	case STATE_FEVER:	// �t�B�[�o�[
		m_nTimeFever++;	// ���Ԃ��J�E���g
		break;

	case STATE_END:     // �I�����
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NONE;  // �������Ă��Ȃ���Ԃɐݒ�
			}
		}
		break;
	}

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print(" ���݂̏�ԁF%d\n", m_state);
}

//===============================================
// �`�揈��
//===============================================
void CGame::Draw(void)
{
	// �^�C���̕`�揈��
	m_pTime->Draw();

	// �X�R�A�̕`�揈��
	m_pScore->Draw();

	// �S�~�Q�[�W�̕`�揈��
	m_pUiGage->Draw();
}

//===============================================
// �|�[�Y��Ԃ̐ݒ�
//===============================================
void CGame::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}