//=========================================================
//
// ��������\������ [uioperation.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "uioperation.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "tutorial.h"
#include "uigage.h"
#include "target.h"
#include "dumpster.h"
#include "player.h"

//===============================================
// �}�N����`
//===============================================
#define SIZE_X				(285.0f)		// ���T�C�Y
#define SIZE_Y				(90.0f)			// �c�T�C�Y
#define TIME_PROGRESS		(240)			// �i���ڍs����
#define TIME_PROGRESSTART	(1200)			// �i���ڍs�J�n����

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CUiOperation::m_aIdxTexture[TEX_MAX] = {};				// �g�p����e�N�X�`���̔ԍ�
int CUiOperation::m_nProgress = -1;							// �i��
int CUiOperation::m_nTime = 0;								// �o�ߎ���

//===============================================
// �e�N�X�`���t�@�C����
//===============================================
const char *CUiOperation::m_apTextureData[TEX_MAX] = 
{
	{ "data\\TEXTURE\\tutorial008.png" },	// �ړ�����ł��Ȃ�
	{ "data\\TEXTURE\\tutorial009.png" },	// �^�[�Q�b�g���d�˂�
	{ "data\\TEXTURE\\tutorial001.png" },	// �^�[�Q�b�g�ړ�
	{ "data\\TEXTURE\\tutorial010.png" },	// �F�𑵂��悤
	{ "data\\TEXTURE\\tutorial002.png" },	// �R����S�~
	{ "data\\TEXTURE\\tutorial003.png" },	// �R���Ȃ��S�~
	{ "data\\TEXTURE\\tutorial004.png" },	// �����S�~
	{ "data\\TEXTURE\\tutorial005.png" },	// �Q�[�W
	{ "data\\TEXTURE\\tutorial006.png" },	// �S�~�X�e�[�V����
	{ "data\\TEXTURE\\tutorial007.png" }	// �I���m�F
};

//===============================================
// �R���X�g���N�^
//===============================================
CUiOperation::CUiOperation() : CObject2D(5)
{
	// �l�̃N���A
	m_nTypeTex = 0;
	m_nGarbage = 0;
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		m_bPressInput[nCntButton] = false;
	}
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CUiOperation::CUiOperation(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nTypeTex = 0;
	m_nGarbage = 0;
	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		m_bPressInput[nCntButton] = false;
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CUiOperation::~CUiOperation()
{
	
}

//===============================================
// ��������
//===============================================
CUiOperation *CUiOperation::Create(int nTex, int nPriority)
{
	CUiOperation *pUi;

	// �w�i�̐���
	pUi = new CUiOperation(nPriority);

	// ��ނ̐ݒ�
	pUi->SetType(TYPE_BULLET);

	// �T�C�Y�̐ݒ�
	pUi->SetSize(SIZE_X, SIZE_Y);

	// ����������
	pUi->Init(D3DXVECTOR3(650.0f, 600.0f, 0.0f), nTex, nPriority);

	// �ݒ菈��
	pUi->Set(nTex);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CUiOperation::Init(D3DXVECTOR3 pos, int nTex, int nPriority)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CUiOperation::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CUiOperation::Update(void)
{
	if (m_nProgress != TEX_GAGE)
	{// �Q�[�W��������Ȃ��Ƃ�
		m_nGarbage = CTutorial::GetUiGage()->Get();		// �S�~�̗ʂ�ۑ�
	}

	//if (m_nProgress >= TEX_MATCHCOL)
	//{
	//	// �ݒ菈��
	//	CTutorial::GetUiGage()->Set(-MAX_GARBAGE);
	//}

	if (m_nTypeTex == m_nProgress)
	{// �i���ƃe�N�X�`���̎�ނ�����
		switch (m_nProgress)
		{
		case TEX_NOT_OPERATION:	// �ړ�����ł��Ȃ�
			m_nTime++;		// ���Ԃ��J�E���g�A�b�v

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// �J�E���g�����Z�b�g
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_COLLECT:		// �^�[�Q�b�g���d�˂�
			m_nTime++;		// ���Ԃ��J�E���g�A�b�v

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// �J�E���g�����Z�b�g
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_TARGET:		// �^�[�Q�b�g
			if (CManager::GetKeyboardInput()->GetPress(DIK_A) == true || CManager::GetInputGamePad()->GetJoyStickLX(0) < 0)
			{
				m_bPressInput[BUTTON_A] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_W) == true || CManager::GetInputGamePad()->GetJoyStickLY(0) > 0)
			{
				m_bPressInput[BUTTON_W] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_S) == true || CManager::GetInputGamePad()->GetJoyStickLY(0) < 0)
			{
				m_bPressInput[BUTTON_S] = true;
			}
			else if (CManager::GetKeyboardInput()->GetPress(DIK_D) == true || CManager::GetInputGamePad()->GetJoyStickLX(0) > 0)
			{
				m_bPressInput[BUTTON_D] = true;
			}

			if(m_bPressInput[BUTTON_A] == true && m_bPressInput[BUTTON_W] == true && m_bPressInput[BUTTON_S] == true && m_bPressInput[BUTTON_D] == true)
			{// 
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_MATCHCOL:		// �F�𑵂��悤
			m_nTime++;		// ���Ԃ��J�E���g�A�b�v

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// �J�E���g�����Z�b�g
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_BURN:			// �R����S�~
			if ((CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_NONFLAMMABLE:	// �R���Ȃ��S�~
			if ((CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_RECYCLABLE:	// �����S�~
			if ((CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
				|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
				&& CTutorial::GetTarget()->GetWithinRange() == true)
			{// 
				m_nProgress++;		// �i����i�߂�

				// �I������
				Uninit();
			}
			break;

		case TEX_GAGE:			// �Q�[�W
			m_nTime++;		// ���Ԃ��J�E���g�A�b�v

			//// �S�~�Q�[�W�̐ݒ菈��
			//CTutorial::GetUiGage()->Set(0.01f);

			if (m_nTime > TIME_PROGRESS)
			{// 
				m_nTime = 0;		// �J�E���g�����Z�b�g
				m_nProgress++;		// �i����i�߂�

				//// �S�~�̗ʂ����ɖ߂�
				//CTutorial::GetUiGage()->Set(-1.0f);
				//CTutorial::GetUiGage()->Set(m_nGarbage);

				// �I������
				Uninit();
			}
			break;

		case TEX_DUMPSTER:		// �S�~�X�e�[�V����
			for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
			{
				CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

				while (pObject != NULL)
				{// �g�p����Ă���
					CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
					CObject::TYPE type = pObject->GetType();		// ��ނ��擾

					if (type == CObject::TYPE_DUMPSTER)
					{// �S�~�X�e�[�V����
						if ((CManager::GetKeyboardInput()->GetTrigger(DIK_SPACE) == true
							|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
							&& pObject->GetPos().x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
							&& pObject->GetPos().z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX)
						{// �S�~�X�e�[�V�������g����
							m_nProgress++;		// �i����i�߂�

							// �I������
							Uninit();
						}
					}

					pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
				}
			}
			break;

		case TEX_END:			// �I���m�F
			break;
		}
	}
	else if (m_nProgress == -1)
	{
		m_nTime++;		// ���Ԃ��J�E���g�A�b�v

		if (m_nTime > TIME_PROGRESSTART)
		{// ��莞�Ԍo��
			m_nTime = 0;		// �J�E���g�����Z�b�g
			m_nProgress++;		// �i����i�߂�
		}
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true
		|| CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// ��ʑJ�ڂ���
		m_nTime = 0;			// �J�E���g�����Z�b�g
		m_nProgress = -1;		// �i�������Z�b�g
	}

	// �I�u�W�F�N�g2D�̍X�V����
	CObject2D::Update();
}

//===============================================
// �`�揈��
//===============================================
void CUiOperation::Draw(void)
{
	if (m_nTypeTex == m_nProgress)
	{// �i���ƃe�N�X�`���̎�ނ�����
		// �I�u�W�F�N�g2D�̕`�揈��
		CObject2D::Draw();
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CUiOperation::Set(const int nType)
{
	m_nTypeTex = nType;

	// �e�N�X�`���̐ݒ�
	m_aIdxTexture[m_nTypeTex] = CManager::GetTexture()->Regist(m_apTextureData[m_nTypeTex]);

	// �e�N�X�`���̊��蓖��
	CObject2D::BindTexture(m_aIdxTexture[m_nTypeTex]);
}