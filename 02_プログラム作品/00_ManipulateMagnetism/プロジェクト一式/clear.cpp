//=========================================================
//
// �N���A�\������ [clear.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "clear.h"
#include "manager.h"
#include "texture.h"
#include "game.h"
#include "renderer.h"
#include "fade.h"
#include "time.h"

//===============================================
//�}�N����`
//===============================================
#define SPEED_MOVE	(0.08f)							// �ړ����x
#define SIZE_X		(350.0f)						// ���T�C�Y
#define SIZE_Y		(100.0f)						// �c�T�C�Y

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CClear::m_IdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CClear::CClear() : CObject2D(0)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CClear::CClear(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CClear::~CClear()
{

}

//===============================================
// ��������
//===============================================
CClear *CClear::Create(int nPriority)
{
	CClear *pUi;

	// �w�i�̐���
	pUi = new CClear(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pUi->Init(D3DXVECTOR3(-SIZE_X, 350.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CClear::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_IdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\GameClear.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CClear::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CClear::Update(void)
{
	if (CGame::GetBClear() == true)
	{
		// �ʒu�␳����
		if (m_bEndAnim == false && CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 350.0f, 0.0f), SPEED_MOVE, true) == true)
		{// �ʒu�␳�I��
			m_bEndAnim = true;		// �A�j���[�V�����I��
			CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
			CScene::SetTime(CGame::GetTime()->Get());		// ���Ԃ̐ݒ�
		}
		else if (m_bEndAnim == true)
		{
			CObject2D::RevisionPos(D3DXVECTOR3(SCREEN_WIDTH + SIZE_X, 350.0f, 0.0f), SPEED_MOVE, true);
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CClear::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}