//=========================================================
//
// �Q�[�W�t���`�揈�� [gagefull.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "gagefull.h"
#include "manager.h"
#include "texture.h"
#include "tutorial.h"
#include "game.h"
#include "target.h"
#include "uigage.h"

//===============================================
// �}�N����`
//===============================================
#define TEXTURE			"data\\TEXTURE\\gagefull.png"	// �q���g�̃e�N�X�`��
#define SIZE_X			(40.0f)							// ���T�C�Y
#define SIZE_Y			(40.0f)							// �c�T�C�Y
#define SCALING_SIZE	(30.0f)							// �k����̃T�C�Y
#define ARRIVAL_TIME	(1200)							// �o������܂ł̎���

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CGageFull::m_nIdxTexture = 0;											// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CGageFull::CGageFull() : CObjectBillboard(3)
{
	// �l�̃N���A
	m_bMaxScaling = false;
	m_bMaxGage = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CGageFull::CGageFull(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_bMaxScaling = false;
	m_bMaxGage = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CGageFull::~CGageFull()
{
	
}

//===============================================
// ��������
//===============================================
CGageFull *CGageFull::Create(int nPriority)
{
	CGageFull *pGageFull;

	// �I�u�W�F�N�g�̐���
	pGageFull = new CGageFull(nPriority);

	//// ��ނ̐ݒ�
	//pGageFull->SetType(TYPE_TARGET);

	// �T�C�Y�̐ݒ�
	pGageFull->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pGageFull->Init();

	// �e�N�X�`���̊��蓖��
	pGageFull->BindTexture(m_nIdxTexture);

	return pGageFull;
}

//===============================================
// ����������
//===============================================
HRESULT CGageFull::Init()
{
	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		m_rot.y = CTutorial::GetTarget()->GetRot().y;	// �^�[�Q�b�g�̌������擾
		m_pos = CTutorial::GetTarget()->GetPos();		// �^�[�Q�b�g�̈ʒu
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		m_rot.y = CGame::GetTarget()->GetRot().y;	// �^�[�Q�b�g�̌������擾
		m_pos = CGame::GetTarget()->GetPos();		// �^�[�Q�b�g�̈ʒu
	}

	// �r���{�[�h�̏���������
	CObjectBillboard::Init(m_pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGageFull::Uninit(void)
{
	// �r���{�[�h�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CGageFull::Update(void)
{
	float fScalingSize = 0.0f;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		m_pos = CTutorial::GetTarget()->GetPos();
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		m_pos = CGame::GetTarget()->GetPos();
	}

	if (m_bMaxScaling == true)
	{
		fScalingSize = SCALING_SIZE;
	}
	else
	{
		fScalingSize = SIZE_X;
	}

	//// �g��k������
	//if (CObjectBillboard::ScalingSize(fScalingSize, 0.2f) == true)
	//{
	//	m_bMaxScaling = m_bMaxScaling ? false : true;		// �T�C�Y�؂�ւ�
	//}

	//// �_�ŏ���
	//CObjectBillboard::Brightness(0.02f);
}

//===============================================
// �`�揈��
//===============================================
void CGageFull::Draw(void)
{
	if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax() == true)
		|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax() == true)
		|| m_bMaxGage == true)
	{
		// �r���{�[�h�̕`�揈��
		CObjectBillboard::Draw();
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CGageFull::Set(const bool bMax)
{
	m_bMaxGage = bMax;
}