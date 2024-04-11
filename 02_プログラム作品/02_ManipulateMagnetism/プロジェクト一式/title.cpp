//=========================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "title.h"
#include "bg.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"
#include "meshfield.h"
#include "manager.h"
#include "camera.h"
#include "logo.h"
#include "pressenter.h"

//===============================================
// �}�N����`
//===============================================
#define TIME_FADERANKING		(800)			// �����t�F�[�h����

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CBg *CTitle::m_pBg = nullptr;						// �w�i�N���X�̃|�C���^
CLogo *CTitle::m_pLogo = nullptr;					// �^�C�g�����S�`��N���X�̃|�C���^
CPressEnter *CTitle::m_pPressEnter = nullptr;		// PressEnter�`��N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CTitle::CTitle() : CScene()
{
	// �l�̃N���A
	m_nTimeFade = 0;
	m_bFade = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTitle::~CTitle()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CTitle::Init(HWND hWnd)
{
	// �J�����̏���������
	CManager::GetInstance()->GetCamera()->Init();

	// �w�i�̐���
	m_pBg = CBg::Create(CBg::TYPE_TITLE, 4);

	// �^�C�g�����S�̐���
	m_pLogo = CLogo::Create();

	// PressEnter�̐���
	m_pPressEnter = CPressEnter::Create();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTitle::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CTitle::Update(void)
{
	m_nTimeFade++;		// ���Ԃ��J�E���g

	if (m_pLogo->GetEndAnim() == true && (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true))
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_SELECT);	// �Z���N�g��ʂֈڍs
			m_bFade = true;

			// �_�Őݒ�
			m_pPressEnter->SetBrightnessFade(true);

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_TITLE_ENTER);
		}
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_Z) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInstance()->GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LEFT, 0)
		|| CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0) || CManager::GetInstance()->GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RIGHT, 0))
	{// �v���C���[�i�J�����j�𑀍쒆
		m_nTimeFade = 0;		//	�J�E���g�����Z�b�g
	}
	else if(m_nTimeFade > TIME_FADERANKING)
	{// ��莞�Ԍo��
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_RESULT);	// �����L���O��ʂֈڍs
			m_bFade = true;
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CTitle::Draw(void)
{
	
}