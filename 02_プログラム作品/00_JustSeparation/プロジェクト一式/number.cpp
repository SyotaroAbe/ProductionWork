//=========================================================
//
// �i���o�[���� [number.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "score.h"
#include "time.h"
#include "object.h"

//===============================================
// �R���X�g���N�^
//===============================================
CNumber::CNumber()
{
	// �l���N���A����
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CNumber::CNumber(int nPriority)
{
	// �l���N���A����
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CNumber::~CNumber()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	m_pObject2D = new CObject2D(nPriority);

	m_pObject2D->SetSize(fSizeX, fSizeY);

	// �I�u�W�F�N�g2D�̏���������
	m_pObject2D->Init(pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CNumber::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	m_pObject2D->Uninit();
	m_pObject2D = NULL;
}

//===============================================
// �X�V����
//===============================================
void CNumber::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CNumber::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	//m_pObject2D->Draw();
}

//===============================================
// �e�N�X�`���̊��蓖��
//===============================================
void CNumber::BindTexture(int nIdx)
{
	m_pObject2D->BindTexture(nIdx);
}

//===============================================
// �i���o�[�̎�ނ̐ݒ�
//===============================================
void CNumber::SetType(const TYPE type)
{
	m_type = type;
}

//===============================================
// �ݒ�
//===============================================
void CNumber::Set(const int nNumber)
{
	m_pObject2D->SetNumber(nNumber);
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CNumber::SetPos(const D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	m_pObject2D->UpdatePos(pos, fSizeX, fSizeY);
}

//===============================================
// �F�̐ݒ�
//===============================================
void CNumber::SetCol(const D3DXCOLOR col)
{
	// �F�̐ݒ菈��
	m_pObject2D->SetCol(col);
}