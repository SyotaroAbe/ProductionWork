//=========================================================
//
// �`���[�g���A����ʏ��� [tutorial.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "score.h"
#include "time.h"
#include "garbage.h"
#include "meshfield.h"
#include "target.h"
#include "uigage.h"
#include "uitarget.h"
#include "dumpster.h"
#include "uioperation.h"
#include "justdust.h"
#include "sound.h"
#include "gagefull.h"

//===============================================
// �}�N����`
//===============================================
#define FIRSTSET_FIELD			(10)			// ���̏����z�u��
#define DISTANCE_FIELD			(-1000.0f)		// ���̔z�u�Ԋu

#define TIME_CREATEFIELD		(116)			// ���̐������ԊԊu
#define TIME_CREATEDUMPSTER		(1300)			// �S�~�X�e�[�V�����̐������ԊԊu
#define POS_CREATEOBJ			(-9120.0f)		// �����ʒu�Ԋu

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CPlayer *CTutorial::m_pPlayer = NULL;			// �v���C���[�N���X�̃|�C���^
CNumber *CTutorial::m_pNumber = NULL;			// �i���o�[�N���X�̃|�C���^
CScore *CTutorial::m_pScore = NULL;				// �X�R�A�N���X�̃|�C���^
CGarbage *CTutorial::m_pGarbage = NULL;			// �S�~�N���X�̃|�C���^
CMeshField *CTutorial::m_pMeshField = NULL;		// ���b�V���t�B�[���h�N���X�̃|�C���^
CTarget *CTutorial::m_pTarget = NULL;			// �^�[�Q�b�g�N���X�̃|�C���^
CDumpster *CTutorial::m_pDumpster = NULL;		// �S�~�X�e�[�V�����N���X�̃|�C���^
CUiGage *CTutorial::m_pUiGage = NULL;			// �S�~�Q�[�W�N���X�̃|�C���^
CUiTarget *CTutorial::m_pUiTarget = NULL;		// �^�[�Q�b�gUI�N���X�̃|�C���^
CUiOperation *CTutorial::m_pUiOperation = NULL;	// ������@�\���N���X�̃|�C���^
CJustDust *CTutorial::m_pJustDust = NULL;		// JustDust�\���N���X�̃|�C���^
CGageFull *CTutorial::m_pGageFull = NULL;		// �Q�[�W�t���\���N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CTutorial::CTutorial() : CScene()
{
	// �l�̃N���A
	m_nTime = 0;
	m_bFade = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTutorial::~CTutorial()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CTutorial::Init(HWND hWnd)
{
	// �J�����̏���������
	CManager::GetCamera()->Init();

	// ���b�V���t�B�[���h�̐���
	//CMeshField::load(hWnd);
	for (int nCntField = 0; nCntField < FIRSTSET_FIELD; nCntField++)
	{
		CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f + nCntField * DISTANCE_FIELD), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, CMeshField::TYPE_STRAIGHT_H, 3);
	}

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

	// ���݂̐���
	CGarbage::Load();

	// �S�~�X�e�[�V�����̓ǂݍ��݂Ɛ���
	CDumpster::Load(hWnd);
	m_pDumpster = CDumpster::Create(D3DXVECTOR3(300.0f, 0.0f, POS_CREATEOBJ), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 2);

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

	// �S�~�Q�[�W�̐���
	m_pUiGage = CUiGage::Create(6);

	// ������@�\���̐���
	for (int nCntOperation = 0; nCntOperation < CUiOperation::TEX_MAX; nCntOperation++)
	{
		m_pUiOperation = CUiOperation::Create(nCntOperation, 6);
	}

	// �T�E���h�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTutorial::Uninit(void)
{
	if (m_pScore != NULL)
	{
		// �X�R�A�̏I������
		m_pScore->Uninit();
		delete m_pScore;
		m_pScore = NULL;
	}

	if (m_pUiGage != NULL)
	{
		// �S�~�Q�[�W�̏I������
		delete m_pUiGage;
		m_pUiGage = NULL;
	}

	if (m_pUiTarget != NULL)
	{
		// �^�[�Q�b�gUI�̏I������
		delete m_pUiTarget;
		m_pUiTarget = NULL;
	}

	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CTutorial::Update(void)
{
	m_nTime++;	// ���Ԃ��J�E���g

	if ((m_nTime % TIME_CREATEFIELD) == 0)
	{
		D3DXVECTOR3 playerPos = GetPlayer()->GetPos();		// �v���C���[�̈ʒu

		// ���b�V���t�B�[���h�̐���
		CMeshField::Create(D3DXVECTOR3(playerPos.x, playerPos.y, playerPos.z + POS_CREATEOBJ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, CMeshField::TYPE_STRAIGHT_H, 2);
	}
	if ((m_nTime % TIME_CREATEDUMPSTER) == 0)
	{
		D3DXVECTOR3 playerPos = GetPlayer()->GetPos();		// �v���C���[�̈ʒu

		m_pDumpster = CDumpster::Create(D3DXVECTOR3(300.0f, playerPos.y, playerPos.z + POS_CREATEOBJ), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 2);
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_GAME);	// �Q�[����ʂֈڍs
			m_bFade = true;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_TUTORIAL);	// �`���[�g���A����ʂֈڍs
			m_bFade = true;
		}
	}


	if (m_pScore != NULL)
	{
		// �X�R�A�̍X�V����
		m_pScore->Update();
	}

	if (m_pUiGage != NULL)
	{
		// �S�~�Q�[�W�̍X�V����
		m_pUiGage->Update();
	}

	if (m_pUiTarget != NULL)
	{
		// �^�[�Q�b�gUI�̍X�V����
		m_pUiTarget->Update();
	}
}

//===============================================
// �`�揈��
//===============================================
void CTutorial::Draw(void)
{
	if (m_pScore != NULL)
	{
		// �X�R�A�̕`�揈��
		m_pScore->Draw();
	}

	if (m_pUiGage != NULL)
	{
		// �S�~�Q�[�W�̕`�揈��
		m_pUiGage->Draw();
	}
}