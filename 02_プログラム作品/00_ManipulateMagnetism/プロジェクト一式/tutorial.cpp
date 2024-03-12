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
#include "meshfield.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "gamebg.h"

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
CPlayer *CTutorial::m_pPlayer = nullptr;			// �v���C���[�N���X�̃|�C���^
CNumber *CTutorial::m_pNumber = nullptr;			// �i���o�[�N���X�̃|�C���^
CMeshField *CTutorial::m_pMeshField = nullptr;		// ���b�V���t�B�[���h�N���X�̃|�C���^
CBg *CTutorial::m_pBg = nullptr;						// �w�i�N���X�̃|�C���^
CGameBg *CTutorial::m_pGameBg = nullptr;						// �w�i�N���X�̃|�C���^

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
	CManager::GetInstance()->GetCamera()->Init();

	// �I�u�W�F�N�gX�t�@�C���̐���
	CObjectX::Load(hWnd);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 510.0f, -350.0f), 4);

	// �w�i�̐���
	m_pGameBg = CGameBg::Create(CManager::GetInstance()->GetCamera()->GetPosR(), CGameBg::TEX_TUTORIAL, 0);

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTutorial::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CTutorial::Update(void)
{
	m_nTime++;	// ���Ԃ��J�E���g

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_SELECT);	// �Q�[����ʂֈڍs
			m_bFade = true;
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CTutorial::Draw(void)
{
	
}