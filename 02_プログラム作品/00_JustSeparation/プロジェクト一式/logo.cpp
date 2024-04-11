//=========================================================
//
// �^�C�g�����S�`�揈�� [logo.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE	"data\\TEXTURE\\logo001.png"	// UI�̃e�N�X�`��
#define SIZE_X		(358.0f)					// ���T�C�Y
#define SIZE_Y		(76.0f)						// �c�T�C�Y

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CLogo::m_IdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CLogo::CLogo() : CObject2D(0)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CLogo::CLogo(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_bEndAnim = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CLogo::~CLogo()
{
	
}

//===============================================
// ��������
//===============================================
CLogo *CLogo::Create(int nPriority)
{
	CLogo *pUi;

	// �w�i�̐���
	pUi = new CLogo(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pUi->Init(D3DXVECTOR3(620.0f, 180.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CLogo::Init(D3DXVECTOR3 pos)
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
void CLogo::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CLogo::Update(void)
{
	// �ʒu�␳����
	if (CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f), 0.02f) == true)
	{// �ʒu�␳�I��
		m_bEndAnim = true;		// �A�j���[�V�����I��
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
	{// ENTER����
		// �ʒu�ݒ菈��
		CObject2D::SetPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f));
	}
}

//===============================================
// �`�揈��
//===============================================
void CLogo::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}