//=========================================================
//
// �t�F�[�h���� [fade.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "fade.h"
#include "renderer.h"

//===============================================
// �}�N����`
//===============================================
#define STATE_UPDATE	(0.03f)		// �t�F�[�h�X�s�[�h

//===============================================
// �R���X�g���N�^
//===============================================
CFade::CFade()
{
	// �l�̃N���A
	m_pVtxBuff = NULL;
	m_state = STATE_NONE;
	m_modeNext = CScene::MODE_TITLE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===============================================
// �f�X�g���N�^
//===============================================
CFade::~CFade()
{

}

//===============================================
// ��������
//===============================================
CFade *CFade::Create(CScene::MODE modeNext, int nPriority)
{
	CFade *pUi;

	// �w�i�̐���
	pUi = new CFade;

	// ����������
	pUi->Init(modeNext);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CFade::Init(CScene::MODE modeNext)
{
	m_state = STATE_IN;								// �t�F�[�h�C����Ԃ�
	m_modeNext = modeNext;							// ���̉�ʁi���[�h�j��ݒ�
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �����|���S���i�s�����j�ɂ��Ă���

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D * pVtx;   // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CFade::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//===============================================
// �X�V����
//===============================================
void CFade::Update(void)
{
	VERTEX_2D * pVtx;   // ���_���ւ̃|�C���^

	if (m_state != STATE_NONE)
	{
		if (m_state == STATE_IN)
		{// �t�F�[�h�C�����
			m_color.a -= STATE_UPDATE;  // �|���S���𓧖��ɂ��Ă���

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_state = STATE_NONE;   // �������Ă��Ȃ���Ԃ�
			}
		}
		else if (m_state == STATE_OUT)
		{// �t�F�[�h�A�E�g���
			m_color.a += STATE_UPDATE;  // �|���S����s�����ɂ��Ă���

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_state = STATE_IN;

				// ���[�h�̐ݒ�i���̉�ʂɈڍs�j
				CManager::SetMode(m_modeNext);
			}
		}

		// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//===============================================
// �`�揈��
//===============================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// �ݒ菈��
//===============================================
void CFade::Set(CScene::MODE modeNext)
{
	if (m_state != STATE_OUT)
	{
		m_state = STATE_OUT;							// �t�F�[�h�A�E�g��Ԃ�
		m_modeNext = modeNext;							// ���̉�ʁi���[�h�j��ݒ�
		m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �����|���S���i�����j�ɂ��Ă���
	}
}