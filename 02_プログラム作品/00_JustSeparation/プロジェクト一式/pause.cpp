//=========================================================
//
// �|�[�Y���� [pause.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "input.h"
#include "title.h"
#include "sound.h"
#include "pause.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object2D.h"
#include "fade.h"
#include "sound.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CObject2D *CPause::m_apObject2D[TEX_MAX];			// �I�u�W�F�N�g2D�N���X�̃|�C���^
const char *CPause::m_apTextureData[TEX_MAX] = {};	// �e�N�X�`���t�@�C����
int CPause::m_aIdxTexture[TEX_MAX] = {};			// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CPause::CPause()
{
	// �l�̃N���A
	m_pVtxBuff = NULL;
	m_Menu = MENU_CONTINUE;
	m_nPosCursor = 0;
	m_EnterFade = false;
	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSize[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CPause::CPause(int nPriority)
{
	// �l�̃N���A
	m_pVtxBuff = NULL;
	m_Menu = MENU_CONTINUE;
	m_nPosCursor = 0;
	m_EnterFade = false;
	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSize[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CPause::~CPause()
{
	
}

//===============================================
// ��������
//===============================================
CPause *CPause::Create(int nPriority)
{
	CPause *pPause;

	// �w�i�̐���
	pPause = new CPause(5);

	// ����������
	pPause->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	return pPause;
}

//===============================================
// ����������
//===============================================
HRESULT CPause::Init(D3DXVECTOR3 pos)
{
	// �e�N�X�`���t�@�C����
	const char *m_apTextureData[TEX_MAX] =
	{
		{ "data\\TEXTURE\\pause_BG.png" },			// �w�i
		{ "data\\TEXTURE\\pause_Continu.png" },		// �Q�[���ɖ߂�
		{ "data\\TEXTURE\\pause_Retry.png" },		// ���g���C
		{ "data\\TEXTURE\\pause_Quit.png" },		// �^�C�g���ɖ߂�
		{ "data\\TEXTURE\\pause_Pause.png" }		// �uPause�v
	};

	// �e�N�X�`���̈ʒu�ݒ�
	m_aPos[TEX_BG] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_aPos[TEX_CONTINU] = D3DXVECTOR3(635.0f, 290.0f, 0.0f);
	m_aPos[TEX_RETRY] = D3DXVECTOR3(635.0f, 405.0f, 0.0f);
	m_aPos[TEX_QUIT] = D3DXVECTOR3(635.0f, 515.0f, 0.0f);
	m_aPos[TEX_LOGO] = D3DXVECTOR3(635.0f, 175.0f, 0.0f);

	// �e�N�X�`���̃T�C�Y�ݒ�
	m_aSize[TEX_BG] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_aSize[TEX_CONTINU] = D3DXVECTOR3(195.0f, 50.0f, 0.0f);
	m_aSize[TEX_RETRY] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);
	m_aSize[TEX_QUIT] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);
	m_aSize[TEX_LOGO] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);

	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = new CObject2D(6);

		// ��ނ̐ݒ�
		m_apObject2D[nCntObj]->SetType(CObject::TYPE_PAUSE);

		// �T�C�Y�ݒ菈��
		m_apObject2D[nCntObj]->SetSize(m_aSize[nCntObj].x, m_aSize[nCntObj].y);

		// �I�u�W�F�N�g2D�̏���������
		m_apObject2D[nCntObj]->Init(m_aPos[nCntObj]);

		// �e�N�X�`���̐ݒ�
		m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);

		// �e�N�X�`���̊��蓖��
		m_apObject2D[nCntObj]->BindTexture(m_aIdxTexture[nCntObj]);
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPause::Uninit(void)
{
	
}

//===============================================
// �X�V����
//===============================================
void CPause::Update(void)
{
	if (m_EnterFade == false)
	{// ENTER���͂���Ă��Ȃ�
		// �J�[�\���𓮂���
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_UP, 0))
		{
			m_nPosCursor--;

			if (m_nPosCursor < MENU_CONTINUE)
			{// �������
				m_nPosCursor = MENU_QUIT;
			}

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CURSOR);
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_DOWN, 0))
		{
			m_nPosCursor++;

			if (m_nPosCursor >= MENU_MAX)
			{// �������
				m_nPosCursor = MENU_CONTINUE;
			}

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CURSOR);
		}

		for (int nCntPause = 0; nCntPause < MENU_MAX; nCntPause++)
		{
			if (nCntPause == m_nPosCursor)
			{
				// ���_�J���[�̐ݒ�
				m_apObject2D[nCntPause + 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// ���_�J���[�̐ݒ�
				m_apObject2D[nCntPause + 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.3f));
			}
		}

		if (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0))
		{// ����L�[�iENTER�L�[�j�������ꂽ
			if (m_nPosCursor == MENU_CONTINUE)
			{
				CGame::SetEnablePause(false);

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
			}
			else if (m_nPosCursor == MENU_RETRY)
			{
				m_EnterFade = true;

				// ���[�h�ݒ�i�Q�[����ʂɈڍs�j
				CRenderer::GetFade()->Set(CScene::MODE_GAME);

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_ENTER);
			}
			else if (m_nPosCursor == MENU_QUIT)
			{
				m_EnterFade = true;

				// ���[�h�ݒ�i�^�C�g����ʂɈڍs�j
				CRenderer::GetFade()->Set(CScene::MODE_TITLE);

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_ENTER);
			}
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CPause::Draw(void)
{
	
}