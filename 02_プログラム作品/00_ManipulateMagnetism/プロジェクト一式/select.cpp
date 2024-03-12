//=========================================================
//
// �Z���N�g��ʏ��� [select.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "select.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "bg.h"
#include "object2D.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CBg *CSelect::m_pBg = nullptr;						// �w�i�N���X�̃|�C���^
CObject2D* CSelect::m_pObject2D = nullptr;			// �I�u�W�F�N�g2D�N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CSelect::CSelect() : CScene()
{
	// �l�̃N���A
	m_pTutorial = nullptr;
	m_pFirstStage = nullptr;
	m_pSecondStage = nullptr;
	m_pThirdStage = nullptr;
	m_nTime = 0;
	m_nSelectStage = STAGE_TUTORIAL;
	m_bFade = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CSelect::~CSelect()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CSelect::Init(HWND hWnd)
{
	m_nSelectStage = STAGE_TUTORIAL;

	// �J�����̏���������
	CManager::GetInstance()->GetCamera()->Init();

	// �w�i�̐���
	m_pBg = CBg::Create(CBg::TYPE_TITLE);

	// Tutorial��\��
	m_pTutorial = new CObject2D(3);
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->Init({ 300.0f, 150.0f, 0.0f });
		m_pTutorial->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_Tutorial.png"));
		m_pTutorial->SetSize(200.0f, 50.0f);
		m_pTutorial->SetVtx();
	}

	// �X�e�[�W�P
	m_pFirstStage = new CObject2D(3);
	if (m_pFirstStage != nullptr)
	{
		m_pFirstStage->Init({ 500.0f, 300.0f, 0.0f });
		m_pFirstStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_FirstStage.png"));
		m_pFirstStage->SetSize(200.0f, 50.0f);
		m_pFirstStage->SetVtx();
	}

	// �X�e�[�W�Q
	m_pSecondStage = new CObject2D(3);
	if (m_pSecondStage != nullptr)
	{
		m_pSecondStage->Init({ 700.0f, 450.0f, 0.0f });
		m_pSecondStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_SecondStage.png"));
		m_pSecondStage->SetSize(200.0f, 50.0f);
		m_pSecondStage->SetVtx();
	}

	// �X�e�[�W�R
	m_pThirdStage = new CObject2D(3);
	if (m_pThirdStage != nullptr)
	{
		m_pThirdStage->Init({ 900.0f, 600.0f, 0.0f });
		m_pThirdStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_ThirdStage.png"));
		m_pThirdStage->SetSize(200.0f, 50.0f);
		m_pThirdStage->SetVtx();
	}

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CSelect::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CSelect::Update(void)
{
	m_nTime++;	// ���Ԃ��J�E���g

	// �F�����Z�b�g
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetCol({0.0f, 0.0f, 0.0f, 0.0f});
	}
	if (m_pFirstStage != nullptr)
	{
		m_pFirstStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_pSecondStage != nullptr)
	{
		m_pSecondStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_pThirdStage != nullptr)
	{
		m_pThirdStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	// �X�e�[�W�I��
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_S) == true
		|| CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_DOWN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_DOWN, 0) == true)
	{
		m_nSelectStage++;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_W) == true
		|| CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_UP) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_UP, 0) == true)
	{
		m_nSelectStage--;
	}

	if (m_nSelectStage >= STAGE_MAX)
	{
		m_nSelectStage = STAGE_TUTORIAL;
	}
	else if (m_nSelectStage < STAGE_TUTORIAL)
	{
		m_nSelectStage = STAGE_MAX -1;
	}

	switch (m_nSelectStage)
	{
	case STAGE_TUTORIAL:	// �`���[�g���A��
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Brightness(0.05f);
			m_pFirstStage->SetVtx();
			m_pSecondStage->SetVtx();
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_1:			// �X�e�[�W�P
		if (m_pFirstStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->Brightness(0.05f);
			m_pSecondStage->SetVtx();
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_2:			// �X�e�[�W�Q
		if (m_pSecondStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->SetVtx();
			m_pSecondStage->Brightness(0.05f);
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_3:			// �X�e�[�W�R
		if (m_pThirdStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->SetVtx();
			m_pSecondStage->SetVtx();
			m_pThirdStage->Brightness(0.05f);
		}
		break;
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			m_bFade = true;

			// �I�𒆂̃X�e�[�W�ɂ���đJ�ڐ��ς���
			switch (m_nSelectStage)
			{
			case STAGE_TUTORIAL:	// �`���[�g���A��
				CRenderer::GetFade()->Set(CScene::MODE_TUTORIAL);	// �`���[�g���A����ʂֈڍs
				break;

			default:				// �X�e�[�W�P�ȍ~
				CRenderer::GetFade()->Set(CScene::MODE_GAME);	// �Q�[����ʂֈڍs
				break;
			}

			CManager::GetInstance()->SetStage(m_nSelectStage);	// manager�ɃX�e�[�W����ۑ�

			if (CManager::GetMode() == MODE_GAME)
			{
				m_bFade = true;		// ����Ȃ�
			}
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CSelect::Draw(void)
{
	
}