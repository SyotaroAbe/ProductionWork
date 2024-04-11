//=========================================================
//
// ���b�N�I������ [rockon.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "rockon.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectBillboard.h"
#include "texture.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"
#include "sound.h"

//===============================================
// �}�N����`
//===============================================
#define ROCKON_SIZEX		(300.0f)		// ���i�����j
#define ROCKON_SIZEY		(300.0f)		// �����i�����j
#define DIFF_SIZE			(37.0f)			// �ړI�̃T�C�Y

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CRockon::m_nIdxTexture = 0;					// �g�p����e�N�X�`���̔ԍ�
int CRockon::m_nNumAll = 0;						// �g�p���Ă���^�[�Q�b�g����
int CRockon::m_aObjRockon[MAX_ROCKON] = {};		// �^�[�Q�b�g���Ă���I�u�W�F�N�g�̔ԍ��ۑ�

//===============================================
// �R���X�g���N�^
//===============================================
CRockon::CRockon() : CObjectBillboard(3)
{
	// �l�̃N���A
	m_localPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_separation = CGarbage::SEPARATION_NONE;
	m_nIDGarbage = 0;

	m_nNumAll++;	// �������J�E���g�A�b�v
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CRockon::CRockon(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_localPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_separation = CGarbage::SEPARATION_NONE;
	m_nIDGarbage = 0;

	m_nNumAll++;	// �������J�E���g�A�b�v
}

//===============================================
// �f�X�g���N�^
//===============================================
CRockon::~CRockon()
{
	m_nNumAll--;	// �������J�E���g�_�E��
}

//===============================================
// ��������
//===============================================
CRockon *CRockon::Create(D3DXVECTOR3 pos, CGarbage::SEPARATION separation, int nID, int nPriority)
{
	CRockon *pRockon = NULL;

	if (MAX_ROCKON > m_nNumAll)
	{// ���b�N�I���̑����ɒB���Ă��Ȃ�
		// ���b�N�I���̐���
		pRockon = new CRockon(nPriority);

		// ��ނ̐ݒ�
		pRockon->SetType(TYPE_ROCKON);

		// �T�C�Y�̐ݒ�
		pRockon->SetSize(ROCKON_SIZEX, ROCKON_SIZEY);

		// ����������
		pRockon->Init(pos);

		// ��Ԃ̐ݒ�
		pRockon->SetSeparation(separation);

		// �e�N�X�`���̊��蓖��
		pRockon->BindTexture(m_nIdxTexture);
	}

	return pRockon;
}

//===============================================
// ����������
//===============================================
HRESULT CRockon::Init(D3DXVECTOR3 pos)
{
	// �r���{�[�h�̏���������
	CObjectBillboard::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\target000.png");

	// �T�E���h�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_ROCKON);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRockon::Uninit(void)
{
	// �r���{�[�h�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CRockon::Update(void)
{
	// �k������
	CObjectBillboard::ScalingSize(DIFF_SIZE);

	// �̂Ă��Ƃ��Ƀ��b�N�I�����O��
	if ((m_separation == CGarbage::SEPARATION_BURN && (CManager::GetKeyboardInput()->GetRelease(DIK_RIGHT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_B, 0) == true))
		|| (m_separation == CGarbage::SEPARATION_NONFLAMMABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_LEFT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_X, 0) == true))
		|| (m_separation == CGarbage::SEPARATION_RECYCLABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_UP) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_Y, 0) == true)))
	{// SPACE�L�[�������ꂽ
		// �I������
		Uninit();
	}
}

//===============================================
// �`�揈��
//===============================================
void CRockon::Draw(void)
{
	// �r���{�[�h�̕`�揈��
	CObjectBillboard::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CRockon::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;
}

//===============================================
// ��Ԃ̐ݒ菈��
//===============================================
void CRockon::SetSeparation(CGarbage::SEPARATION separation)
{
	m_separation = separation;

	// ���ʂ��ƂɐF��ݒ肷��
	switch (m_separation)
	{
	case CGarbage::SEPARATION_BURN:			// �R����S�~
		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		CObjectBillboard::SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));	// ��
		break;

	case CGarbage::SEPARATION_NONFLAMMABLE:	// �R���Ȃ��S�~
		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		CObjectBillboard::SetCol(D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));	// ��
		break;

	case CGarbage::SEPARATION_RECYCLABLE:		// �����S�~
		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		CObjectBillboard::SetCol(D3DXCOLOR(0.9f, 0.75f, 0.2f, 1.0f));	// ���F
		break;
	}
}

//===============================================
// �I�u�W�F�N�g�̔ԍ��ݒ菈��
//===============================================
void CRockon::SetIDObj(int nID)
{
	m_nIDGarbage = nID;
}

//===============================================
// ���b�N�I�������I�u�W�F�N�g�̐ݒ菈��
//===============================================
void CRockon::SetObjRockon(int nCntRockon, int nCntObj)
{
	m_aObjRockon[nCntRockon] = nCntObj;
}