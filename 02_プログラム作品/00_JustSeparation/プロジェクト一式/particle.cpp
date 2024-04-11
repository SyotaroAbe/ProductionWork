//=========================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "particle.h"
#include "effect.h"

//===============================================
// �}�N����`
//===============================================
#define PARTICLE_SET	(15)		// ��x�ɌĂԗ�

//===============================================
// �R���X�g���N�^
//===============================================
CParticle::CParticle() : CObjectBillboard(2)
{
	// �l���N���A����
	m_tex = CEffect::TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nMaxApeear = 0;
	m_nRadius = 0;
	m_nLife = 0;
	m_nCntAppear = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CParticle::~CParticle()
{

}

//===============================================
// ��������
//===============================================
CParticle *CParticle::Create(int nPriority)
{
	CParticle *pParticle;

	// �p�[�e�B�N���̐���
	pParticle = new CParticle;

	// ����������
	pParticle->Init();

	return pParticle;
}

//===============================================
// ����������
//===============================================
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//===============================================
// �I������
//===============================================
void CParticle::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CParticle::Update(void)
{
	if(m_nCntAppear < m_nMaxApeear)
	{// ��x�ɕ��o���鐔��
		// �G�t�F�N�g�̐���
		CEffect::Create(m_pos, D3DXVECTOR3(m_move.x += sinf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f)),
											m_move.y += cosf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f)), 
											m_move.z += cosf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f))),
											m_col, m_tex, (float)m_nRadius, m_nLife, 4);

		m_nCntAppear++;		// �񐔂��J�E���g�A�b�v
	}

	if (m_nLife <= 0)
	{// �������s����
		Uninit();   // �����̏I������
	}
	else
	{
		// �̗͂����炷
		m_nLife--;
	}
}

//===============================================
// �`�揈��
//===============================================
void CParticle::Draw(void)
{
	
}

//===============================================
// �ݒ菈��
//===============================================
void CParticle::Set(D3DXVECTOR3 pos, TYPE type)
{
	m_pos = pos;

	switch (type)
	{
	case TYPE_PLAYER:		// �v���C���[�̔���
		break;

	case TYPE_ENEMY:		// �G�̔���
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(0.2f, 0.3f, 1.0f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 30;
		m_nLife = 300;
		break;

	case TYPE_BURN:			// �R����S�~
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(1.0f, 0.3f, 0.2f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;
			
	case TYPE_NONFLAMMABLE:	// �R���Ȃ��S�~
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;

	case TYPE_RECYCLABLE:		// �����S�~
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;

	case TYPE_MOVE:			// �ړ�
		m_tex = CEffect::TYPE_STEAM;
		m_col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.05f);
		m_nMaxApeear = 1;
		m_nRadius = 40;
		m_nLife = 18;
		break;

	case TYPE_CURVE:		// �J�[�u
		m_tex = CEffect::TYPE_FIRE;
		m_col = D3DXCOLOR(1.0f, 0.3f, 0.2f, 1.0f);
		m_nMaxApeear = 12;
		m_nRadius = 7;
		m_nLife = 30;
		break;
	}
}