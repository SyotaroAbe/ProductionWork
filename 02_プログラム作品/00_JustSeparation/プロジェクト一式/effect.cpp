//=========================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_EFFECT		(1024)		// �G�t�F�N�g�̍ő吔
#define SUB_RADIOS		(0.5f)		// ���a�̌��Z

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CEffect::m_aIdxTexture[TYPE_MAX] = {};		// �g�p����e�N�X�`���̔ԍ�
int CEffect::m_nNumAll = 0;						// ����

//===============================================
// �R���X�g���N�^
//===============================================
CEffect::CEffect() : CObjectBillboard(1)
{
	// �l���N���A����
	m_type = TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nRadius = 0.0f;
	m_nLife = 0;

	m_nNumAll++;	// �������J�E���g�A�b�v
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CEffect::CEffect(int nPriority) : CObjectBillboard(nPriority)
{
	// �l���N���A����
	m_type = TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nRadius = 0.0f;
	m_nLife = 0;

	m_nNumAll++;	// �������J�E���g�A�b�v
}

//===============================================
// �f�X�g���N�^
//===============================================
CEffect::~CEffect()
{
	m_nNumAll--;	// �������J�E���g�_�E��
}

//===============================================
// ��������
//===============================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type, float fRadios, int nLife, int nPriority)
{
	CEffect *pEffect = NULL;	// �G�t�F�N�g�N���X�̃|�C���^

	if (MAX_EFFECT > m_nNumAll)
	{// �����ɒB���Ă��Ȃ�
		// �G�t�F�N�g�̐���
		pEffect = new CEffect(nPriority);

		// ��ނ̐ݒ�
		pEffect->SetType(TYPE_EFFECT);

		// ����������
		pEffect->Init(pos);

		// �ݒ菈��
		pEffect->Set(pos, move, col, fRadios, nLife);

		// �e�N�X�`���̊��蓖��
		pEffect->BindTexture(m_aIdxTexture[TYPE_NORMAL]);
		pEffect->BindTexture(m_aIdxTexture[TYPE_STEAM]);
		pEffect->BindTexture(m_aIdxTexture[TYPE_FIRE]);
	}

	return pEffect;
}

//===============================================
// ����������
//===============================================
HRESULT CEffect::Init(D3DXVECTOR3 pos)
{
	// �T�C�Y�̐ݒ�
	SetSize(15.0f, 15.0f);

	// �I�u�W�F�N�g�r���{�[�h�̃T�C�Y
	CObjectBillboard::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_aIdxTexture[TYPE_NORMAL] = CManager::GetTexture()->Regist("data\\TEXTURE\\effect000.jpg");
	m_aIdxTexture[TYPE_STEAM] = CManager::GetTexture()->Regist("data\\TEXTURE\\eff_steam_000.png");
	m_aIdxTexture[TYPE_FIRE] = CManager::GetTexture()->Regist("data\\TEXTURE\\eff_Glow_001.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CEffect::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CEffect::Update(void)
{
	if (m_nLife <= 0 || m_nRadius <= 0.0f)
	{// �e�̈ʒu����ʊO�ɏo���ꍇ�܂��͎������s����
		Uninit();				// �����̏I������
	}
	else
	{
		// �ʒu���X�V
		m_pos += m_move;

		// ���_���W�̍X�V����
		CObjectBillboard::UpdatePos(m_pos, m_nRadius, m_nRadius);

		// �̗͂����炷
		m_nLife--;
		m_nRadius -= SUB_RADIOS;
	}
}

//===============================================
// �`�揈��
//===============================================
void CEffect::Draw(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̕`�揈��
	CObjectBillboard::DrawEffect();
}

//===============================================
// �ݒ菈��
//===============================================
void CEffect::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadios, const int nLife)
{
	// �l����
	m_pos = pos;
	m_move = move;
	m_col = col;
	m_nRadius = fRadios;
	m_nLife = nLife;

	// �F�̐ݒ菈��
	CObjectBillboard::SetCol(m_col);
}