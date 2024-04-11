//=========================================================
//
// JustDust�\������ [justdust.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "justdust.h"
#include "manager.h"
#include "texture.h"
#include "tutorial.h"
#include "game.h"
#include "target.h"
#include "player.h"

//===============================================
// �}�N����`
//===============================================
#define SIZE_X				(72.0f)		// ���T�C�Y
#define SIZE_Y				(60.0f)		// �c�T�C�Y
#define DIFF_SIZE			(500.0f)	// �T�C�Y
#define TIME_SCALLSIZE		(30)		// �T�C�Y�ύX����
#define SCALL_DIFF			(0.3f)		// �T�C�Y�̒l�̕␳
#define TURN_ROT_DIFF		(0.001f)	// �Ȃ���p�x�̍���

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CJustDust::m_nIdxTexture = 0;				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CJustDust::CJustDust() : CObjectBillboard(5)
{
	// �l�̃N���A
	m_nTime = 0;
	m_bScall = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CJustDust::CJustDust(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_nTime = 0;
	m_bScall = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CJustDust::~CJustDust()
{
	
}

//===============================================
// ��������
//===============================================
CJustDust *CJustDust::Create(int nPriority)
{
	CJustDust *pUi;

	// �w�i�̐���
	pUi = new CJustDust(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BULLET);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(DIFF_SIZE, DIFF_SIZE);

	// ����������
	pUi->Init(D3DXVECTOR3(650.0f, 600.0f, 0.0f), nPriority);

	// �e�N�X�`���̊��蓖��
	pUi->BindTexture(m_nIdxTexture);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CJustDust::Init(D3DXVECTOR3 pos, int nPriority)
{
	// �I�u�W�F�N�g2D�̏���������
	CObjectBillboard::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\JustDust.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CJustDust::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CJustDust::Update(void)
{
	D3DXVECTOR3 posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �^�[�Q�b�g�̈ʒu
	D3DXVECTOR3 rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̌���

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{// �`���[�g���A��
		posTarget = CTutorial::GetTarget()->GetPos();
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����
		posTarget = CGame::GetTarget()->GetPos();
		rotPlayer = CGame::GetPlayer()->GetRot();
	}

	// �I�u�W�F�N�g2D�̈ʒu�ݒ菈��
	if (rotPlayer.y <= TURN_ROT_DIFF && rotPlayer.y >= -TURN_ROT_DIFF)
	{// �����ʒu��������
		CObjectBillboard::SetPos(D3DXVECTOR3(posTarget.x - 150.0f, posTarget.y + 50.0f, posTarget.z));
	}
	else if (rotPlayer.y <= D3DX_PI * ROT_LEFT + TURN_ROT_DIFF && rotPlayer.y >= D3DX_PI * ROT_LEFT - TURN_ROT_DIFF)
	{// �����ʒu���獶����
		CObjectBillboard::SetPos(D3DXVECTOR3(posTarget.x, posTarget.y + 50.0f, posTarget.z - 150.0f));
	}
	else if (rotPlayer.y <= D3DX_PI * ROT_RIGHT + TURN_ROT_DIFF && rotPlayer.y >= D3DX_PI * ROT_RIGHT - TURN_ROT_DIFF)
	{// �����ʒu����E����
		CObjectBillboard::SetPos(D3DXVECTOR3(posTarget.x, posTarget.y + 50.0f, posTarget.z + 150.0f));
	}
	else if (rotPlayer.y <= D3DX_PI + TURN_ROT_DIFF && rotPlayer.y >= D3DX_PI - TURN_ROT_DIFF)
	{// �����ʒu���牺����
		CObjectBillboard::SetPos(D3DXVECTOR3(posTarget.x + 150.0f, posTarget.y + 50.0f, posTarget.z));
	}

	if (m_nTime > TIME_SCALLSIZE)
	{
		m_bScall = m_bScall ? false : true;		// �T�C�Y�؂�ւ�
		m_nTime = 0;							// �l��������
	}

	if (m_bScall == false)
	{
		// �g�又��
		CObjectBillboard::ScalingSize(DIFF_SIZE, 1.0f);
	}
	else
	{
		m_nTime++;		// ���Ԃ��J�E���g

		// �k������
		CObjectBillboard::ScalingSize(SIZE_X, SCALL_DIFF);
	}

	// �I�u�W�F�N�g2D�̍X�V����
	CObjectBillboard::Update();
}

//===============================================
// �`�揈��
//===============================================
void CJustDust::Draw(void)
{
	if (m_bScall == true)
	{
		// �I�u�W�F�N�g2D�̕`�揈��
		CObjectBillboard::Draw();
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CJustDust::Set(bool bScall)
{
	m_bScall = bScall;
}