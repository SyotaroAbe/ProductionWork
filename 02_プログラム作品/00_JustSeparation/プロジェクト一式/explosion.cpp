//=========================================================
//
// �������� [explosion.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CExplosion::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �}�N����`
//===============================================
#define EXPLOSION_SIZEX		(25)								// ���i�����j
#define EXPLOSION_SIZEY		(25)								// �����i�����j

#define PATTERNANIM_X		(8)									// �A�j�����p�^�[����
#define PATTERNANIM_Y		(1)									// �A�j���c�p�^�[����
#define PATTERNANIM_ALL		(PATTERNANIM_X * PATTERNANIM_Y)		// �A�j���c�p�^�[����
#define SPEED_ANIM			(10)								// �A�j���[�V�����X�s�[�h

//===============================================
// �R���X�g���N�^
//===============================================
CExplosion::CExplosion() : CObject2D(3)
{
	// �l���N���A����
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CExplosion::CExplosion(int nPriority) : CObject2D(nPriority)
{
	// �l���N���A����
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CExplosion::~CExplosion()
{

}

//===============================================
// ��������
//===============================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, int nPriority)
{
	CExplosion *pExplosion;

	// �����̐���
	pExplosion = new CExplosion(nPriority);

	// ��ނ̐ݒ�
	pExplosion->SetType(TYPE_EXPLOSION);

	// �ݒ�̃T�C�Y
	pExplosion->SetSize(EXPLOSION_SIZEX, EXPLOSION_SIZEY);

	// ����������
	pExplosion->Init(pos);

	// �e�N�X�`���̊��蓖��
	pExplosion->BindTexture(m_nIdxTexture);

	return pExplosion;
}

//===============================================
// ����������
//===============================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\enemy000.png");

	// �A�j���[�V�����J�E���^�[�̐ݒ�
	m_nCounterAnim = SPEED_ANIM;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CExplosion::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CExplosion::Update(void)
{
	if (m_nPatternAnim <= PATTERNANIM_ALL)
	{// ���p�^�[�����𒴂��ĂȂ�
		if ((m_nCounterAnim % SPEED_ANIM) == 0)
		{// �J�E���^�[���ݒ肵�������̐��l�ɒB����
			m_nCounterAnim = 0;		// �J�E���^�[�������l�ɖ߂�

			VERTEX_2D *pVtx = 0;	// ���_���ւ̃|�C���^

			// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * m_nPatternAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * m_nPatternAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * m_nPatternAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * m_nPatternAnim, 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			m_nPatternAnim++;     // �p�^�[��No.���X�V����
		}

		m_nCounterAnim++;   // �J�E���^�[�����Z
	}
	else
	{// ���p�^�[�����𒴂���
		Uninit();
	}
}

//===============================================
// �`�揈��
//===============================================
void CExplosion::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}