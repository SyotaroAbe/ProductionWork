//=========================================================
//
// �q���g�`�揈�� [hint.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "hint.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE			"data\\TEXTURE\\hint.png"	// �q���g�̃e�N�X�`��
#define SIZE_X			(285.0f)					// ���T�C�Y
#define SIZE_Y			(50.0f)						// �c�T�C�Y
#define ARRIVAL_TIME	(1200)						// �o������܂ł̎���

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CHint::m_IdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CHint::CHint() : CObject2D(0)
{
	// �l�̃N���A
	m_nTime = 0;
	m_bDisp = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CHint::CHint(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nTime = 0;
	m_bDisp = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CHint::~CHint()
{
	
}

//===============================================
// ��������
//===============================================
CHint *CHint::Create(int nPriority)
{
	CHint *pHint;

	// �q���g�̐���
	pHint = new CHint(nPriority);

	// ��ނ̐ݒ�
	pHint->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pHint->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pHint->Init(D3DXVECTOR3(620.0f, 635.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pHint->BindTexture(m_IdxTexture);

	return pHint;
}

//===============================================
// ����������
//===============================================
HRESULT CHint::Init(D3DXVECTOR3 pos)
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
void CHint::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CHint::Update(void)
{
	m_nTime++;		// ���Ԃ��J�E���g

	if (CManager::GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetKeyboardInput()->GetPress(DIK_Z) == true 
		|| CManager::GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0))
	{// �Ȃ������
		m_nTime = 0;		// ���Ԃ����Z�b�g
	}
}

//===============================================
// �`�揈��
//===============================================
void CHint::Draw(void)
{
	if (m_nTime >= ARRIVAL_TIME)
	{
		// �I�u�W�F�N�g2D�̕`�揈��
		CObject2D::Draw();
	}
}