//=========================================================
//
// �^�[�Q�b�gUI���� [uitarget.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "uitarget.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectBillboard.h"
#include "game.h"
#include "input.h"
#include "target.h"
#include "player.h"
#include "tutorial.h"

//===============================================
// �}�N����`
//===============================================
#define SIZE_X			(38.0f)						// ���T�C�Y
#define SIZE_Y			(38.0f)						// �c�T�C�Y
#define DISTANCE_CENTER	(80.0f)						// ���S����̋���
#define MOVE_SPEED		(0.4f)						// �ړ��X�s�[�h

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CObjectBillboard *CUiTarget::m_apObjectBillboard[TYPE_MAX];	// �I�u�W�F�N�g�r���{�[�h�N���X�̃|�C���^
const char *CUiTarget::m_apTextureData[TYPE_MAX] = {};		// �e�N�X�`���t�@�C����
int CUiTarget::m_aIdxTexture[TYPE_MAX] = {};				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CUiTarget::CUiTarget()
{
	// �l�̃N���A
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CUiTarget::CUiTarget(int nPriority)
{
	// �l�̃N���A
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CUiTarget::~CUiTarget()
{
	
}

//===============================================
// ��������
//===============================================
CUiTarget *CUiTarget::Create(int nIdx, int nPriority)
{
	CUiTarget *pUi;

	// �w�i�̐���
	pUi = new CUiTarget(nPriority);

	// ����������
	pUi->Init(D3DXVECTOR3(600.0f, 300.0f, 0.0f), nPriority);

	return pUi;
}

//===============================================
// ����������
//===============================================
HRESULT CUiTarget::Init(D3DXVECTOR3 pos, int nPriority)
{
	// �e�N�X�`���t�@�C����
	const char *m_apTextureData[TYPE_MAX] =
	{
		{ "data\\TEXTURE\\ui005.png" },	// �R����S�~
		{ "data\\TEXTURE\\ui006.png" },	// �R���Ȃ��S�~
		{ "data\\TEXTURE\\ui007.png" }	// �����S�~
	};

	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �^�[�Q�b�g�̈ʒu���擾
	float fPlayerRot = 0.0f;								// �v���C���[�̌������擾

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		TargetPos = CTutorial::GetTarget()->GetPos();
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetTarget()->GetPos();
		fPlayerRot = CGame::GetPlayer()->GetRot().y;
	}

	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObjectBillboard[nCntObj] = new CObjectBillboard(nPriority);

		// �T�C�Y�ݒ菈��
		m_apObjectBillboard[nCntObj]->SetSize(SIZE_X, SIZE_Y);

		// �I�u�W�F�N�g�r���{�[�h�̏���������
		m_apObjectBillboard[nCntObj]->Init(TargetPos);

		// �e�N�X�`���̐ݒ�
		m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);

		// �e�N�X�`���̊��蓖��
		m_apObjectBillboard[nCntObj]->BindTexture(m_aIdxTexture[nCntObj]);
	}

	// ���[�J���ʒu��ݒ�
	m_aPos[TYPE_BURN] = D3DXVECTOR3(TargetPos.x + cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z - sinf(fPlayerRot) * DISTANCE_CENTER);
	m_aPos[TYPE_NONFLAMMABLE] = D3DXVECTOR3(TargetPos.x, TargetPos.y + DISTANCE_CENTER, TargetPos.z);
	m_aPos[TYPE_RECYCLABLE] = D3DXVECTOR3(TargetPos.x - cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z + sinf(fPlayerRot) * DISTANCE_CENTER);

	// �I�u�W�F�N�g�r���{�[�h�̏���������
	m_apObjectBillboard[TYPE_BURN]->SetPos(m_aPos[TYPE_BURN]);
	m_apObjectBillboard[TYPE_NONFLAMMABLE]->SetPos(m_aPos[TYPE_NONFLAMMABLE]);
	m_apObjectBillboard[TYPE_RECYCLABLE]->SetPos(m_aPos[TYPE_RECYCLABLE]);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CUiTarget::Uninit(void)
{
	
}

//===============================================
// �X�V����
//===============================================
void CUiTarget::Update(void)
{
	bool bKeyBurn = false;			// �R����S�~�̃L�[���͂��ꂽ��
	bool bKeyNonflammable = false;	// �R���Ȃ��S�~�̃L�[���͂��ꂽ��
	bool bKeyPlastic = false;		// �����S�~�̃L�[���͂��ꂽ��

	D3DXVECTOR3 TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �^�[�Q�b�g�̈ʒu���擾
	float fPlayerRot = 0.0f;								// �v���C���[�̌������擾

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		TargetPos = CTutorial::GetTarget()->GetPos();		// �^�[�Q�b�g�̈ʒu���擾
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;	// �v���C���[�̌������擾
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		TargetPos = CGame::GetTarget()->GetPos();		// �^�[�Q�b�g�̈ʒu���擾
		fPlayerRot = CGame::GetPlayer()->GetRot().y;	// �v���C���[�̌������擾
	}

	// �ʒu���^�[�Q�b�g�̎���ɌŒ�
	m_apObjectBillboard[TYPE_BURN]->SetPos(m_aPos[TYPE_BURN]);
	m_apObjectBillboard[TYPE_NONFLAMMABLE]->SetPos(m_aPos[TYPE_NONFLAMMABLE]);
	m_apObjectBillboard[TYPE_RECYCLABLE]->SetPos(m_aPos[TYPE_RECYCLABLE]);
	
	if (CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
	{// ���L�[�������ꂽ
		// �R����S�~
		// �ړI�̈ʒu�܂ł̍������v�Z
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_BURN];

		// �ʒu�̕␳
		m_aPos[TYPE_BURN].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_BURN].y += posDest.y * MOVE_SPEED;
		m_aPos[TYPE_BURN].z += posDest.z * MOVE_SPEED;

		bKeyBurn = true;	// �R����S�~�̃L�[���͂��ꂽ
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
	{// ��L�[�������ꂽ
		// �R���Ȃ��S�~
		// �ړI�̈ʒu�܂ł̍������v�Z
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_NONFLAMMABLE];

		// �ʒu�̕␳
		m_aPos[TYPE_NONFLAMMABLE].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_NONFLAMMABLE].z += posDest.z * MOVE_SPEED;
		m_aPos[TYPE_NONFLAMMABLE].y += posDest.y * MOVE_SPEED;

		bKeyNonflammable = true;	// �R���Ȃ��S�~�̃L�[���͂��ꂽ
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
	{// �E�L�[�������ꂽ
		// �����S�~
		// �ړI�̈ʒu�܂ł̍������v�Z
		D3DXVECTOR3 posDest = TargetPos - m_aPos[TYPE_RECYCLABLE];

		// �ʒu�̕␳
		m_aPos[TYPE_RECYCLABLE].x += posDest.x * MOVE_SPEED;
		m_aPos[TYPE_RECYCLABLE].y += posDest.y * MOVE_SPEED;
		m_aPos[TYPE_RECYCLABLE].z += posDest.z * MOVE_SPEED;

		bKeyPlastic = true;		// �����S�~�̃L�[���͂��ꂽ
	}

	if (bKeyBurn == false)
	{// �^�[�Q�b�g���R����S�~�̏�Ԃ���Ȃ�
		// �ʒu�����ɖ߂�
		m_aPos[TYPE_BURN] = D3DXVECTOR3(TargetPos.x + cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z - sinf(fPlayerRot) * DISTANCE_CENTER);
	}
	if (bKeyNonflammable == false
	|| (bKeyNonflammable == true && bKeyBurn == true))
	{// �^�[�Q�b�g���R���Ȃ��S�~�̏�Ԃ���Ȃ�
		// �ʒu�����ɖ߂�
		m_aPos[TYPE_NONFLAMMABLE] = D3DXVECTOR3(TargetPos.x, TargetPos.y + DISTANCE_CENTER, TargetPos.z);
	}
	if (bKeyPlastic == false
		|| (bKeyPlastic == true && (bKeyBurn == true || bKeyNonflammable == true)))
	{// �^�[�Q�b�g�������S�~�̏�Ԃ���Ȃ�
		// �ʒu�����ɖ߂�
		m_aPos[TYPE_RECYCLABLE] = D3DXVECTOR3(TargetPos.x - cosf(fPlayerRot) * DISTANCE_CENTER, TargetPos.y, TargetPos.z + sinf(fPlayerRot) * DISTANCE_CENTER);
	}
}

//===============================================
// �`�揈��
//===============================================
void CUiTarget::Draw(void)
{
	
}