//=========================================================
//
// �w�i���� [bg.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "multibg.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE_TITLE	"data\\TEXTURE\\title000.png"	// �^�C�g���̃e�N�X�`��
#define TEXTURE_RESULT	"data\\TEXTURE\\ranking000.png"	// ���U���g�̃e�N�X�`��
#define TEX_MOVE	(0.00005f)							// �e�N�X�`���̈ړ���

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CBg::m_aIdxTexture[TYPE_MAX] = {};				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CBg::CBg() : CObject2D(0)
{
	// �l�̃N���A
	m_bMove = true;
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;

	m_nIdx++;		// �w�i�����C���N�������g
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CBg::CBg(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_bMove = true;
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;

	m_nIdx++;		// �w�i�����C���N�������g
}

//===============================================
// �f�X�g���N�^
//===============================================
CBg::~CBg()
{
	m_nIdx--;		// �w�i�����f�N�������g
}

//===============================================
// ��������
//===============================================
CBg *CBg::Create(int nIdx, int nPriority)
{
	CBg *pBg;

	// �w�i�̐���
	pBg = new CBg(nPriority);

	// ��ނ̐ݒ�
	pBg->SetType(TYPE_BG);

	// �T�C�Y�̐ݒ�
	pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	// ����������
	pBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// �e�N�X�`���̊��蓖��
	pBg->BindTexture(m_aIdxTexture[nIdx]);

	return pBg;
}

//===============================================
// ����������
//===============================================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_aIdxTexture[TYPE_TITLE] = CManager::GetTexture()->Regist(TEXTURE_TITLE);
	m_aIdxTexture[TYPE_RESULT] = CManager::GetTexture()->Regist(TEXTURE_RESULT);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CBg::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CBg::Update(void)
{
	if (m_bMove == true)
	{//�e�N�X�`�����W�𓮂���
		// �e�N�X�`�����W�̊J�n�ʒu�̍X�V
		m_fTexU -= TEX_MOVE * m_nIdx;
		//m_fTexV -= TEX_MOVE * m_nIdx;

		if (m_fTexU < 0.0f)
		{// 0���������
			m_fTexU += 1.0f;	// 1�֖߂�
		}

		if (m_fTexV < 0.0f)
		{// 0���������
			m_fTexV += 1.0f;	// 1�֖߂�
		}

		//�e�N�X�`�����W�̍X�V����
		//CObject2D::UpdateTex(m_fTexU, m_fTexV);
	}
}

//===============================================
// �`�揈��
//===============================================
void CBg::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CBg::Set(const D3DXVECTOR3 pos, const bool bMove)
{
	m_bMove = bMove;

	// �ʒu�ݒ�
	SetPos(pos);
}