//=========================================================
//
// �t�B�[�o�[�`�揈�� [fever.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "fever.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE		"data\\TEXTURE\\fever000.png"	// UI�̃e�N�X�`��
#define SPEED_MOVE	(0.08f)							// �ړ����x
#define SIZE_X		(287.0f)						// ���T�C�Y
#define SIZE_Y		(61.0f)							// �c�T�C�Y

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CFever::m_IdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CFever::CFever() : CObject2D(0)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CFever::CFever(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CFever::~CFever()
{
	
}

//===============================================
// ��������
//===============================================
CFever *CFever::Create(int nPriority)
{
	CFever *pUi;

	// �w�i�̐���
	pUi = new CFever(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pUi->Init(D3DXVECTOR3(-SIZE_X, 250.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CFever::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_IdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CFever::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CFever::Update(void)
{
	// �ʒu�␳����
	if (m_bEndAnim == false && CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f), SPEED_MOVE, true) == true)
	{// �ʒu�␳�I��
		m_bEndAnim = true;		// �A�j���[�V�����I��
	}
	else if (m_bEndAnim == true)
	{
		CObject2D::RevisionPos(D3DXVECTOR3(SCREEN_WIDTH + SIZE_X, 250.0f, 0.0f), SPEED_MOVE, true);
	}
}

//===============================================
// �`�揈��
//===============================================
void CFever::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}