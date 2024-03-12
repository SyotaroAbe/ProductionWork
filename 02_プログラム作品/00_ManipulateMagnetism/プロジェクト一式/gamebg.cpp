//=========================================================
//
// ���C�����[�h�w�i���� [gamebg.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "gamebg.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectBillboard.h"
#include "texture.h"
#include "camera.h"

//===============================================
// �}�N����`
//===============================================
#define GAMEBG_SIZEX		(8.0f)			// ���i�����j
#define GAMEBG_SIZEY		(8.0f)			// �����i�����j

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CGameBg::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CGameBg::CGameBg() : CObjectBillboard(2)
{
	// �l���N���A
	m_tex = TEX_GAME;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CGameBg::CGameBg(int nPriority) : CObjectBillboard(nPriority)
{
	// �l���N���A
	m_tex = TEX_GAME;
}

//===============================================
// �f�X�g���N�^
//===============================================
CGameBg::~CGameBg()
{
	
}

//===============================================
// ��������
//===============================================
CGameBg *CGameBg::Create(D3DXVECTOR3 pos, ETex tex, int nPriority)
{
	CGameBg *pGameBg;

	// �e�̐���
	pGameBg = new CGameBg(2);

	// ��ނ̐ݒ�
	pGameBg->SetType(TYPE_BG);

	// ����������
	pGameBg->Init(pos, tex);

	// �e�N�X�`���̊��蓖��
	pGameBg->BindTexture(m_nIdxTexture);

	return pGameBg;
}

//===============================================
// ����������
//===============================================
HRESULT CGameBg::Init(D3DXVECTOR3 pos, ETex tex)
{
	m_tex = tex;

	if (m_tex == TEX_TUTORIAL)
	{
		m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial000.png");			// �e�N�X�`���̐ݒ�
		SetSize(SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.45f);														// �T�C�Y�̐ݒ�

		// �I�u�W�F�N�g2D�̏���������
		CObjectBillboard::Init(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
	}
	else
	{
		m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\game000.png");				// �e�N�X�`���̐ݒ�
		SetSize(SCREEN_WIDTH * 1.0f, SCREEN_HEIGHT * 1.0f);														// �T�C�Y�̐ݒ�
	
		// �I�u�W�F�N�g2D�̏���������
		CObjectBillboard::Init(D3DXVECTOR3(m_pos.x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z));
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGameBg::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CGameBg::Update(void)
{
	// �ʒu���J�����̒����_�ɐݒ�
	if (m_tex != TEX_TUTORIAL)
	{
		CObjectBillboard::SetPos(D3DXVECTOR3(m_pos.x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z));
	}
}

//===============================================
// �`�揈��
//===============================================
void CGameBg::Draw(void)
{
	CObjectBillboard::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CGameBg::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;

	//// �T�E���h�̍Đ�
	//CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
}