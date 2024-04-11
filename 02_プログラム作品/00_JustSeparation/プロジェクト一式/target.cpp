//=========================================================
//
// �^�[�Q�b�g���� [target.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "target.h"
#include "rockon.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectBillboard.h"
#include "texture.h"
#include "camera.h"
#include "player.h"
#include "debugproc.h"
#include "garbage.h"
#include "game.h"
#include "tutorial.h"
#include "uigage.h"
#include "sound.h"
#include "gagefull.h"

//===============================================
// �}�N����`
//===============================================
#define DIFF_SIZE			(32.0f)			// ���i�����j
#define DISTANCE_PLAYER		(-600.0f)		// �v���C���[�Ƃ̋���
#define SPEED_TARGET		(23.0f)			// �ړ����x
#define SPEED_TARGETGAGEMAX	(16.0f)			// �Q�[�W���ő厞�̈ړ����x
#define MAX_LENTH			(350.0f)		// �ő�ʒu
#define ROT_DIFF			(0.5f)			// �Ȃ���p�x�̍���
#define POS_DIFF			(0.1f)			// �ʒu�̒l�̕␳
#define ADD_REVISION		(0.2f)			// �␳���̑����l

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CTarget::m_nIdxTexture = 0;											// �g�p����e�N�X�`���̔ԍ�
int CTarget::m_nCntObjTarget[MAX_TARGET] = {};							// �^�[�Q�b�g���Ă���I�u�W�F�N�g�̔ԍ��ۑ�
CGarbage::SEPARATION CTarget::m_state = CGarbage::SEPARATION_NONE;		// �^�[�Q�b�g�̏��

//===============================================
// �R���X�g���N�^
//===============================================
CTarget::CTarget() : CObjectBillboard(3)
{
	// �l�̃N���A
	m_pColObj = NULL;
	m_LocalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bWithinRange = false;
	m_bMaxGage = false;
	m_fSpeed = 0.0f;
	m_fMaltiRivision = 0.0f;
	m_nCntFlameJust = 0;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CTarget::CTarget(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	m_pColObj = NULL;
	m_LocalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bWithinRange = false;
	m_bMaxGage = false;
	m_fSpeed = 0.0f;
	m_fMaltiRivision = 0.0f;
	m_nCntFlameJust = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTarget::~CTarget()
{
	
}

//===============================================
// ��������
//===============================================
CTarget *CTarget::Create(D3DXVECTOR3 pos, CGarbage::SEPARATION state, int nID, int nPriority)
{
	CTarget *pTarget;

	// �e�̐���
	pTarget = new CTarget(nPriority);

	// ��ނ̐ݒ�
	pTarget->SetType(TYPE_TARGET);

	// �T�C�Y�̐ݒ�
	pTarget->SetSize(TARGET_SIZEX, TARGET_SIZEY);

	// ����������
	pTarget->Init(pos);

	// ��Ԃ̐ݒ�
	pTarget->SetState(state);

	// �e�N�X�`���̊��蓖��
	pTarget->BindTexture(m_nIdxTexture);

	return pTarget;
}

//===============================================
// ����������
//===============================================
HRESULT CTarget::Init(D3DXVECTOR3 pos)
{
	// �l�̏�����
	m_nCntFlameJust = -1;
	m_posDest = m_LocalPos;		// �ړI�̈ʒu

	// �r���{�[�h�̏���������
	CObjectBillboard::Init(pos);

	float fPlayerRot = 0.0f;
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;	// �v���C���[�̌������擾
		playerPos = CTutorial::GetPlayer()->GetPos();		// �v���C���[�̈ʒu
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		fPlayerRot = CGame::GetPlayer()->GetRot().y;	// �v���C���[�̌������擾
		playerPos = CGame::GetPlayer()->GetPos();		// �v���C���[�̈ʒu
	}

	// �v���C���[�̑O���ʒu���^�[�Q�b�g�̈ʒu�ɐݒ�
	m_pos.x = playerPos.x + m_LocalPos.x + sinf(fPlayerRot) * DISTANCE_PLAYER;
	m_pos.z = playerPos.z + m_LocalPos.z + cosf(fPlayerRot) * DISTANCE_PLAYER;
	m_pos.y = 0.0f;

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\target000.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTarget::Uninit(void)
{
	// �r���{�[�h�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CTarget::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	float fCameraRot = CManager::GetCamera()->GetRot().y;		// �J�����̌������擾
	D3DXVECTOR3 fPosDiff;										// �ړI�̈ʒu�܂ł̍�����ۑ�
	float fPlayerRot = 0.0f;									// �v���C���[�̌������擾
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̈ʒu
	CPlayer::TURN playerTurn = CPlayer::TURN_NONE;				// �v���C���[�̕����ύX���

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;
		playerPos = CTutorial::GetPlayer()->GetPos();
		playerTurn = CTutorial::GetPlayer()->GetTurnOld();
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		fPlayerRot = CGame::GetPlayer()->GetRot().y;
		playerPos = CGame::GetPlayer()->GetPos();
		playerTurn = CGame::GetPlayer()->GetTurnOld();
	}

	// �v���C���[�̑O���ʒu���^�[�Q�b�g�̈ʒu�ɐݒ�
	m_pos.x = playerPos.x + m_LocalPos.x + sinf(fPlayerRot) * DISTANCE_PLAYER;
	m_pos.z = playerPos.z + m_LocalPos.z + cosf(fPlayerRot) * DISTANCE_PLAYER;
	m_pos.y = 0.0f;

	// �S�~�ƃ^�[�Q�b�g�̓����蔻��
	if (WithinRangeGarbage(true) == true || WithinRangeGarbage(false) == true)
	{// �S�~���^�[�Q�b�g�͈͓̔�
		if ((m_bWithinRange == false && CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax() == false)
			|| (m_bWithinRange == false && CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax() == false))
		{// �ŏ���1��̂ݒʂ�
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_ABLE);
		}
		else if (WithinRangeGarbage(true) == true
			&& ((m_bWithinRange == false && CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax() == true)
			|| (m_bWithinRange == false && CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax() == true)))
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DISABLE);
		}
		m_bWithinRange = true;

		if (m_bMaxGage == false || WithinRangeGarbage(false) == true)
		{
			// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
			CObjectBillboard::SetCol(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));	// ��
		}
		else
		{
			// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
			CObjectBillboard::SetCol(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));	// �D�F

			// �Q�[�W�t���ݒ�
			if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{
				CTutorial::GetGageFull()->Set(true);
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetGageFull()->Set(true);
			}
		}

		// �k������
		CObjectBillboard::ScalingSize(DIFF_SIZE);
	}
	else
	{
		m_bWithinRange = false;

		// �g�又��
		CObjectBillboard::ScalingSize(TARGET_SIZEX);

		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		CObjectBillboard::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// ��

		// �Q�[�W�t���ݒ�
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			CTutorial::GetGageFull()->Set(false);
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetGageFull()->Set(false);
		}
	}

	// �^�[�Q�b�g�ʒu�ύX
	if (CManager::GetKeyboardInput()->GetPress(DIK_A) == true
		|| CManager::GetInputGamePad()->GetJoyStickLX(0) < 0)
	{
		if (CManager::GetKeyboardInput()->GetPress(DIK_W) == true
			|| CManager::GetInputGamePad()->GetJoyStickLY(0) > 0)
		{// W�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_S) == true
				|| CManager::GetInputGamePad()->GetJoyStickLY(0) < 0)
		{// S�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
		else
		{// A�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_D) == true
			|| CManager::GetInputGamePad()->GetJoyStickLX(0) > 0)
	{
		if (CManager::GetKeyboardInput()->GetPress(DIK_W) == true
			|| CManager::GetInputGamePad()->GetJoyStickLY(0) > 0)
		{// W�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_S) == true
				|| CManager::GetInputGamePad()->GetJoyStickLY(0) < 0)
		{// S�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
		else
		{// D�L�[����
			m_LocalPos.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
			m_LocalPos.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		}
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_W) == true
			|| CManager::GetInputGamePad()->GetJoyStickLY(0) > 0)
	{// W�L�[����
		m_LocalPos.x += sinf(D3DX_PI * ROT_UP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		m_LocalPos.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_S) == true
			|| CManager::GetInputGamePad()->GetJoyStickLY(0) < 0)
	{// S�L�[����
		m_LocalPos.x += sinf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
		m_LocalPos.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * fCameraRot)) * m_fSpeed;
	}

	// �^�[�Q�b�g��ޕύX
	if (CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
	{// ���L�[�������ꂽ
		// �R���Ȃ��S�~
		SetState(CGarbage::SEPARATION_NONFLAMMABLE);
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
	{// ��L�[�������ꂽ
		// �����S�~
		SetState(CGarbage::SEPARATION_RECYCLABLE);
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
	{// �E�L�[�������ꂽ
		// �R����S�~
		SetState(CGarbage::SEPARATION_BURN);
	}
	else
	{// ���͂���Ă��Ȃ�
		// �Ȃ�
		SetState(CGarbage::SEPARATION_NONE);
	}

	if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax() == true)
		|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax() == true))
	{// �Q�[�W���ő�
		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		CObjectBillboard::SetCol(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));	// �D�F

		m_fSpeed = SPEED_TARGETGAGEMAX;		// �����������݂�����
	}
	else
	{
		m_fSpeed = SPEED_TARGET;		// �ʏ�̑��x�ɂ���
	}

	if (playerTurn == CPlayer::TURN_NONE)
	{// �^�������i��ł�����
		m_posDest = m_LocalPos;
	}

	// �ړI�̈ʒu�܂ł̍������v�Z
	fPosDiff.x = m_posDest.x - m_LocalPos.x;
	fPosDiff.z = m_posDest.z - m_LocalPos.z;

	// �p�x�̕␳
	m_LocalPos.x += fPosDiff.x * POS_DIFF;
	m_LocalPos.z += fPosDiff.z * POS_DIFF;

	// �ړ��ʒu����
	if (m_LocalPos.x > MAX_LENTH)
	{// X���W�̍ő�l�𒴂���
		m_LocalPos.x = MAX_LENTH;
	}
	else if (m_LocalPos.x < -MAX_LENTH)
	{// X���W�̍ŏ��l�𒴂���
		m_LocalPos.x = -MAX_LENTH;
	}

	if (m_LocalPos.z > MAX_LENTH)
	{// �y���W�̍ő�l�𒴂���
		m_LocalPos.z = MAX_LENTH;
	}
	else if (m_LocalPos.z < -MAX_LENTH)
	{// �y���W�̍ŏ��l�𒴂���
		m_LocalPos.z = -MAX_LENTH;
	}

	// �f�o�b�O�\���\��
	CManager::GetDebugProc()->Print(" �^�[�Q�b�g�����F%d\n", CRockon::GetNumAll());
	CManager::GetDebugProc()->Print(" �^�[�Q�b�g�̈ʒu�F�i%f, %f, %f�j\n", m_pos.x, m_pos.y, m_pos.z);
}

//===============================================
// �`�揈��
//===============================================
void CTarget::Draw(void)
{
	// �r���{�[�h�̕`�揈��
	CObjectBillboard::Draw();
}

//===============================================
// �S�~�ƃ^�[�Q�b�g�̓����蔻�菈��
//===============================================
bool CTarget::WithinRangeGarbage(bool bGarbage)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObj = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObj != NULL)
		{// �g�p����Ă���
			CObject::TYPE type = pObj->GetType();	// ��ނ��擾
			CObject *pObjectNext = pObj->GetNext();	// ���̃I�u�W�F�N�g��ۑ�

			if ((bGarbage == true && type == TYPE_GARBAGE) || (bGarbage == false && type == TYPE_DUMPSTER))
			{// �S�~�E�S�~�X�e�[�V����
				if (pObj->GetPos().x <= m_pos.x + TARGET_SIZEX + TARGET_SIZEX && pObj->GetPos().x >= m_pos.x - TARGET_SIZEX - TARGET_SIZEX
					&& pObj->GetPos().z <= m_pos.z + TARGET_SIZEY + TARGET_SIZEY && pObj->GetPos().z >= m_pos.z - TARGET_SIZEY - TARGET_SIZEY)
				{// �^�[�Q�b�g�͈͓̔��ɓ�����
					if (m_pColObj != pObj)
					{// �␳�����I�u�W�F�N�g�ƕʂ̃I�u�W�F�N�g
						m_fMaltiRivision = 0.0f;		// �␳�l�����Z�b�g
					}

					// �ʒu�␳����
					CObjectBillboard::RevisionPos(pObj->GetPos(), m_fMaltiRivision);

					if (bGarbage == true && m_pos == pObj->GetPos())
					{// �S�~�ƃ^�[�Q�b�g���d�Ȃ���
						if (m_nCntFlameJust < FLAME_JUSTDUST)
						{// �W���X�g�^�C�~���O
							m_nCntFlameJust++;	// �t���[�������J�E���g
						}
					}

					m_fMaltiRivision += ADD_REVISION;		// �␳�l���グ��
					m_pColObj = pObj;						// �����蔻��̂Ƃ�I�u�W�F�N�g����
					return true;
				}
			}
			pObj = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}
	m_nCntFlameJust = -1;			// �J�E���g���Z�b�g
	m_fMaltiRivision = 0.0f;		// �␳�l���Z�b�g
	return false;
}

//===============================================
// �ʒu�␳����
//===============================================
void CTarget::RevisionPos(void)
{
	float fPlayerRot = 0.0f;									// �v���C���[�̌������擾
	CPlayer::TURN playerTurn = CPlayer::TURN_NONE;				// �v���C���[�̕����ύX���

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		fPlayerRot = CTutorial::GetPlayer()->GetRot().y;
		playerTurn = CTutorial::GetPlayer()->GetTurnOld();
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		fPlayerRot = CGame::GetPlayer()->GetRot().y;
		playerTurn = CGame::GetPlayer()->GetTurnOld();
	}

	// �^�[�Q�b�g�ʒu�̕␳
	if (fPlayerRot <= D3DX_PI * ROT_LEFT + ROT_DIFF && fPlayerRot >= D3DX_PI * ROT_LEFT - ROT_DIFF)
	{// �v���C���[�������ʒu���猩�č�����
		if (playerTurn != CPlayer::TURN_BACK)
		{// �v���C���[���܂�Ԃ����Ȃ�
			 // X���W�̈ʒu������������
			 m_posDest.x = 0.0f;
		}
	}
	else if (fPlayerRot <= D3DX_PI * ROT_RIGHT + ROT_DIFF && fPlayerRot >= D3DX_PI * ROT_RIGHT - ROT_DIFF)
	{// �v���C���[�������ʒu���猩�ĉE����
		if (playerTurn != CPlayer::TURN_BACK)
		{// �v���C���[���܂�Ԃ����Ȃ�
			 // X���W�̈ʒu������������
			m_posDest.x = 0.0f;
		}
	}
	else if ((fPlayerRot <= D3DX_PI + ROT_DIFF && fPlayerRot >= D3DX_PI - ROT_DIFF)
		|| (fPlayerRot <= -D3DX_PI + ROT_DIFF && fPlayerRot >= -D3DX_PI - ROT_DIFF))
	{// �v���C���[�������ʒu���猩�ĉ�����
		if (playerTurn != CPlayer::TURN_BACK)
		{// �v���C���[���܂�Ԃ����Ȃ�
			 // Z���W�̈ʒu������������
			m_posDest.z = 0.0f;
		}
	}
	else if (fPlayerRot <= ROT_DIFF && fPlayerRot >= -ROT_DIFF)
	{// �v���C���[�������ʒu���猩�ď����
		if (playerTurn != CPlayer::TURN_BACK)
		{// �v���C���[���܂�Ԃ����Ȃ�
			 // Z���W�̈ʒu������������
			m_posDest.z = 0.0f;
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CTarget::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;
}

//===============================================
// ��Ԃ̐ݒ菈��
//===============================================
void CTarget::SetState(CGarbage::SEPARATION state)
{
	if (state != CGarbage::SEPARATION_NONE && m_state != state)
	{// ��Ԃ�؂�ւ����u�Ԃ̂ݖ炷
		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_SEPARATION);
	}

	m_state = state;

	// ���ʂ��ƂɐF��ݒ肷��
	switch (m_state)
	{
	case CGarbage::SEPARATION_NONE:			// �Ȃ�
		// �I�u�W�F�N�g�r���{�[�h�̐F�ݒ�
		//CObjectBillboard::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �Ȃ�
		break;

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
// �Q�[�W���ő剻�̐ݒ菈��
//===============================================
void CTarget::SetMaxGage(bool bMaxGage)
{
	m_bMaxGage = bMaxGage;
}