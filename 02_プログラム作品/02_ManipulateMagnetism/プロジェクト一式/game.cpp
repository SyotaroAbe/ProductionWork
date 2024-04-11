//=========================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "game.h"
#include "object3D.h"
#include "objectX.h"
#include "meshfield.h"
#include "pause.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "camera.h"
#include "debugproc.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "gamebg.h"
#include "time.h"
#include "clear.h"
#include "select.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CPlayer *CGame::m_pPlayer = nullptr;						// �v���C���[�N���X�̃|�C���^
CMeshField *CGame::m_pMeshField = nullptr;					// ���b�V���t�B�[���h�N���X�̃|�C���^
CObject3D *CGame::m_pObject3D = nullptr;					// �I�u�W�F�N�g3D�N���X�̃|�C���^
CPause *CGame::m_pPause = nullptr;							// �|�[�Y�N���X�̃|�C���^
CEnemy *CGame::m_pEnemy = nullptr;							// �G�N���X�̃|�C���^
CGameBg *CGame::m_pGameBg = nullptr;						// �w�i�N���X�̃|�C���^
CTime *CGame::m_pTime = nullptr;							// �^�C���N���X�̃|�C���^
CItem* CGame::m_pItem = nullptr;							// �A�C�e���N���X�̃|�C���^
CClear* CGame::m_pClear = nullptr;						// �N���A�\���N���X�̃|�C���^

bool CGame::m_bPause = false;				// �|�[�Y���
bool CGame::m_bStateReady = false;			// GAMSESTATE_READY���ǂ���
bool CGame::m_bPauseCamera = false;			// �|�[�Y���̃J��������\��
bool CGame::m_bClear = false;			// �N���A���Ă��邩

//===============================================
// �R���X�g���N�^
//===============================================
CGame::CGame() : CScene()
{
	// �l�̃N���A
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_hWnd = nullptr;
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
	m_bClear = false;

	m_hWnd = hWnd;		// HWND�ۑ�
	int nStage = CManager::GetInstance()->GetStage();	// �X�e�[�W���擾

	// �J�����̏���������
	CManager::GetInstance()->GetCamera()->Init();

	// �I�u�W�F�N�gX�t�@�C���̐���
	CObjectX::Load(m_hWnd);

	// �v���C���[�̐���
	switch (nStage)
	{
	case CSelect::STAGE_1:		// �X�e�[�W�P
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 110.0f, -600.0f), 4);
		break;

	case CSelect::STAGE_2:		// �X�e�[�W�Q
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 510.0f, -600.0f), 4);
		break;
	
	case CSelect::STAGE_3:		// �X�e�[�W�R
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 110.0f, -500.0f), 4);
		break;

	default:
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);
		break;
	}
	
	// �w�i�̐���
	m_pGameBg = CGameBg::Create(D3DXVECTOR3(m_pPlayer->GetPos().x, 201.0f, CManager::GetInstance()->GetCamera()->GetPosR().z), CGameBg::TEX_GAME, 0);

	// �G�̐���
	CEnemy::Load(m_hWnd);

	// �^�C���̐���
	m_pTime = CTime::Create(5);

	// �N���A�\���̐���
	m_pClear = CClear::Create();

	// �|�[�Y�̐���
	m_pPause = CPause::Create(6);

	// �ʏ��Ԃɐݒ�
	m_state = STATE_NORMAL;
	m_nCounterState = 0;

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGame::Uninit(void)
{
	m_bPause = false;
	m_bStateReady = true;		// �ҋ@��Ԃɂ���
	m_bPauseCamera = false;
	m_bClear = false;

	// �^�C���̏I������
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	// �|�[�Y�̏I������
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CGame::Update(void)
{
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_P) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// �|�[�Y����
		m_bPause = m_bPause ? false : true;		// �|�[�Y��Ԑ؂�ւ�

		if (m_bPause == true)
		{
			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE);
		}
		else
		{
			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
		}
	}

#if _DEBUG
	if (m_bPause == true && CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// �|�[�Y���J��������
		m_bPauseCamera = m_bPauseCamera ? false : true;		// �|�[�Y��Ԑ؂�ւ�
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
		SetTime(m_pTime->Get());		// ���Ԃ̐ݒ�
	}
#endif

	if (m_pPlayer->GetPos().z >= 900.0f)
	{// BackSpace
		m_pPlayer->SetState(CPlayer::STATE_CLEAR);
		SetClear(true);
		//CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
		SetTime(m_pTime->Get());		// ���Ԃ̐ݒ�
	}

	if (m_pPlayer->GetPos().y < -400.0f)
	{// ������
		m_pPlayer->Death();
	}

	if (m_bStateReady == false)
	{// �ҋ@��Ԃ���Ȃ�
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			// �^�C���̍X�V����
			if (m_pTime != nullptr)
			{
				if (m_bClear == false)
				{// �N���A���Ă��Ȃ�
					m_pTime->Update();
				}
			}
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
		if (m_pPause != nullptr)
		{
			m_pPause->Update();
		}
	}

	switch (m_state)
	{
	case STATE_NORMAL:  // �ʏ���
		m_nCounterState--;
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
	CManager::GetInstance()->GetDebugProc()->Print(" ���݂̏�ԁF%d\n", m_state);
}

//===============================================
// �`�揈��
//===============================================
void CGame::Draw(void)
{
	// �^�C���̕`�揈��
	if (m_pTime != nullptr)
	{
		m_pTime->Draw();
	}
}

//===============================================
// ���Z�b�g����
//===============================================
void CGame::Reset(void)
{
	//CRenderer::GetFade()->Set(CScene::MODE_GAME, false);

	////if (CRenderer::GetFade()->Get() == CFade::STATE_IN)
	////{
	//	// �T�E���h�̒�~
	//	CManager::GetInstance()->GetSound()->Stop(CSound::LABEL_BGM_GAME);

	//	// �|�[�Y�̏I������
	//	m_pPause->Uninit();
	//	delete m_pPause;
	//	m_pPause = nullptr;

	//	// �I�u�W�F�N�g�̃��Z�b�g
	//	CObject::Reset();

	//	m_bPause = false;
	//	m_bStateReady = true;		// �ҋ@��Ԃɂ���
	//	m_bPauseCamera = false;

	//	// �J�����̏���������
	//	CManager::GetInstance()->GetCamera()->Init();

	//	//// �I�u�W�F�N�gX�t�@�C���̐���
	//	//CObjectX::Load(m_hWnd);

	//	// �v���C���[�̏�����
	//	m_pPlayer->Init(D3DXVECTOR3(0.0f, 210.0f, -350.0f));

	//	// �w�i�̏�����
	//	m_pGameBg->Init(D3DXVECTOR3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, CManager::GetInstance()->GetCamera()->GetPosR().z));

	//	// �G�̐���
	//	CEnemy::Load(m_hWnd);

	//	// �|�[�Y�̐���
	//	m_pPause = CPause::Create(6);

	//	// �ʏ��Ԃɐݒ�
	//	m_state = STATE_NORMAL;
	//	m_nCounterState = 0;

	//	// �T�E���h�̍Đ�
	//	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
	////}
}

//===============================================
// �|�[�Y��Ԃ̐ݒ�
//===============================================
void CGame::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}