//=========================================================
//
// PressEnter�`�揈�� [pressenter.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "pressenter.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "title.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE			"data\\TEXTURE\\PRESS_ENTER_OR_A.png"	// UI�̃e�N�X�`��
#define BRIGHTNESS		(0.02f)									// ���邳�̒l
#define FADE_BRIGHTNESS	(0.4f)									// �t�F�[�h���̓_��
#define SIZE_X			(325.0f)								// ���T�C�Y
#define SIZE_Y			(50.0f)									// �c�T�C�Y

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CPressEnter::m_IdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CPressEnter::CPressEnter() : CObject2D(0)
{
	// �l�̃N���A
	m_fBrightness = 0.0f;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CPressEnter::CPressEnter(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fBrightness = 0.0f;
}

//===============================================
// �f�X�g���N�^
//===============================================
CPressEnter::~CPressEnter()
{
	
}

//===============================================
// ��������
//===============================================
CPressEnter *CPressEnter::Create(int nPriority)
{
	CPressEnter *pUi;

	// �w�i�̐���
	pUi = new CPressEnter(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pUi->Init(D3DXVECTOR3(620.0f, 550.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CPressEnter::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_IdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	// �l�̏�����
	m_fBrightness = BRIGHTNESS;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPressEnter::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CPressEnter::Update(void)
{
	// �_�ŏ���
	CObject2D::Brightness(m_fBrightness);
}

//===============================================
// �`�揈��
//===============================================
void CPressEnter::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}

//===============================================
// �`�揈��
//===============================================
void CPressEnter::SetBrightnessFade(bool bFade)
{
	if (bFade == false)
	{
		m_fBrightness = BRIGHTNESS;			// �������_��
	}
	else
	{
		m_fBrightness = FADE_BRIGHTNESS;	// �f�����_��
	}
}