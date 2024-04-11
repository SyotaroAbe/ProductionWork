//=========================================================
//
// �����N�C���\�� [rankIn.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "rankIn.h"
#include "manager.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define RANKIN_SIZEX	(90)		// �����N�C���̕��i�����j
#define RANKIN_SIZEY	(30)		// �����N�C���̍����i�����j
#define RANKIN_DISTANCE	(50.0f)		// �����N�ʂɂ��炷�ʒu

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CRankIn::m_pIdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CRankIn::CRankIn() : CObject2D(0)
{
	// �l�̃N���A
	m_fBrightness = 0.0f;
	m_bFlash = false;
	m_nRank = 0;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CRankIn::CRankIn(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fBrightness = 0.0f;
	m_bFlash = false;
	m_nRank = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CRankIn::~CRankIn()
{

}

//===============================================
// ��������
//===============================================
CRankIn *CRankIn::Create(int nPriority)
{
	CRankIn *pRankIn;

	// �����N�C���̐���
	pRankIn = new CRankIn(nPriority);

	// ��ނ̐ݒ�
	pRankIn->SetType(TYPE_RANKIN);

	// �T�C�Y�̐ݒ�
	pRankIn->SetSize(RANKIN_SIZEX, RANKIN_SIZEY);

	// ����������
	pRankIn->Init(D3DXVECTOR3(120.0f, 130.0f, 0.0f));

	// �F�̐ݒ�
	pRankIn->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pRankIn->BindTexture(m_pIdxTexture);

	return pRankIn;
}

//===============================================
// ����������
//===============================================
HRESULT CRankIn::Init(D3DXVECTOR3 pos)
{
	// �l�̏�����
	m_nRank = -1;

	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_pIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\rankIn.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRankIn::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CRankIn::Update(void)
{
	if (m_nRank > - 1)
	{// �����N�C������
		if (m_fBrightness <= MIN_COLA)
		{// ������x�Â��Ȃ���
			m_bFlash = false;		// ���邭����
		}
		else if (m_fBrightness >= MAX_COLA)
		{// ������x���邭�Ȃ���
			m_bFlash = true;		// �Â�����
		}

		if (m_bFlash == true)
		{// �Â�����
			m_fBrightness += COL_DARK;
		}
		else if (m_bFlash == false)
		{// ���邭����
			m_fBrightness += COL_LIGHT;
		}
	}

	// �I�u�W�F�N�g2D�̍X�V����
	CObject2D::Update();

	// �F�̐ݒ�
	CObject2D::SetCol(D3DXCOLOR(m_fBrightness, m_fBrightness, 0.3f, 1.0f));
}

//===============================================
// �`�揈��
//===============================================
void CRankIn::Draw(void)
{
	if (m_nRank > - 1)
	{// �����N�C������
		// �I�u�W�F�N�g2D�̕`�揈��
		CObject2D::Draw();
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CRankIn::Set(const int nRank)
{
	m_nRank = nRank;	// �����N����ʒu��ݒ�

	// �ʒu�̐ݒ菈��
	CObject2D::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + RANKIN_DISTANCE * (m_nRank * 2), 0.0f));
}