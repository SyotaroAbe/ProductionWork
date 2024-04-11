//=========================================================
//
// ���ݏ��� [garbage.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "garbage.h"
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "objectX.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "debugproc.h"
#include "xfile.h"
#include "target.h"
#include "rockon.h"
#include "dumpster.h"
#include "game.h"
#include "effect.h"
#include "uigage.h"
#include "tutorial.h"
#include "sound.h"
#include "justdust.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CGarbage::m_aIdxXFile[TYPE_MAX] = {};			// �g�p����X�t�@�C���̔ԍ�
int CGarbage::m_nNumAll = 0;						// ����
int CGarbage::m_nRandCounter = 0;					// rand���Z�b�g�p

//===============================================
// �}�N����`
//===============================================
#define MAX_GABAGE		(1024)		// �S�~�̍ő吔
#define MOVE_DIFF		(0.02f)		// �ʒu�̕␳�W���i�����傫�����f�����ς��j
#define JUMP_HEIGHT		(16.85f)	// �W�����v��
#define ADD_SCORE		(500)		// �������̃X�R�A
#define ADD_SOCREJUST	(800)		// JUSTDUST�̃X�R�A
#define SUB_SCORE		(-50)		// �s�������̃X�R�A

#define DISPOSE_LENTHZ			(150.0f)	// �S�~������ł���͈�(Z)
#define DISPOSE_LENTHX			(300.0f)	// �S�~������ł���͈�(X)
#define DISPOSE_DISTANCE		(250.0f)	// �S�~������ł���v���C���[����̑O������
#define DIFF_DISPOSE			(10.0f)		// �S�~������ł���͈͂̌덷
#define TURN_ROT_DIFF			(0.001f)	// �Ȃ���p�x�̍���

#define DIFF_HEIGHT			(100.0f)	// �S�~������̖ړI�̍���
#define GRAVITY_DISPOSE		(10.0f)		// �̂Ă鎞�̏d��
#define WAIT_TIME			(60)		// �̂Ă����ƂɐÎ~���Ă��鎞��

// TUTORIAL
#define DELETE_POSITION			(650.0f)	// ����j������ʒu

//===============================================
// �R���X�g���N�^
//===============================================
CGarbage::CGarbage() : CObjectX(4)
{
	// �l�̃N���A
	particleType = CParticle::TYPE_BURN;
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nextposDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_BAG;
	m_size = MODELSIZE_NORMAL;
	m_state = STATE_NONE;
	m_separation = SEPARATION_NONE;
	m_rockonType = SEPARATION_NONE;
	m_bLand = false;
	m_bJustDust = false;
	m_fMoveDiff = 0.0f;
	m_nIdxDumpster = 0;

	m_nNumAll++;	// �������J�E���g�A�b�v
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CGarbage::CGarbage(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	particleType = CParticle::TYPE_BURN;
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nextposDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_BAG;
	m_size = MODELSIZE_NORMAL;
	m_state = STATE_NONE;
	m_separation = SEPARATION_NONE;
	m_rockonType = SEPARATION_NONE;
	m_bLand = false;
	m_bJustDust = false;
	m_fMoveDiff = 0.0f;
	m_nIdxDumpster = 0;

	m_nNumAll++;		// �������J�E���g�A�b�v
}

//===============================================
// �f�X�g���N�^
//===============================================
CGarbage::~CGarbage()
{
	m_nNumAll--;		// �������J�E���g�_�E��
}

//===============================================
// ���f���̐����i�ǂݍ��݁j
//===============================================
HRESULT CGarbage::Load(void)
{
	// �w�t�@�C���ɓǂݍ���
	m_aIdxXFile[TYPE_BOOKOLD] = CManager::GetXFile()->Regist("data\\MODEL\\BookOld.x");
	m_aIdxXFile[TYPE_BOOT] = CManager::GetXFile()->Regist("data\\MODEL\\boot.x");
	m_aIdxXFile[TYPE_BAG] = CManager::GetXFile()->Regist("data\\MODEL\\bag001.x");
	m_aIdxXFile[TYPE_LIGHT] = CManager::GetXFile()->Regist("data\\MODEL\\Stage_Illumination.x");
	m_aIdxXFile[TYPE_CAMERA] = CManager::GetXFile()->Regist("data\\MODEL\\Camera001.x");
	m_aIdxXFile[TYPE_BUCKET] = CManager::GetXFile()->Regist("data\\MODEL\\bucket.x");
	m_aIdxXFile[TYPE_BIN] = CManager::GetXFile()->Regist("data\\MODEL\\bin.x");
	m_aIdxXFile[TYPE_CAN] = CManager::GetXFile()->Regist("data\\MODEL\\can.x");
	m_aIdxXFile[TYPE_DANBORU] = CManager::GetXFile()->Regist("data\\MODEL\\danboru.x");

	return S_OK;
}

//===============================================
// �e�N�X�`���̔j��
//===============================================
void CGarbage::Unload(void)
{
	
}

//===============================================
// ��������
//===============================================
CGarbage *CGarbage::Create(D3DXVECTOR3 pos, int nPriority)
{
	CGarbage *pGarbage = NULL;
	bool bMatchPos = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾

			if (type == CObject::TYPE_GARBAGE)
			{// �S�~
				if (pObject->GetPos().x <= pos.x + 3.0f && pObject->GetPos().x >= pos.x - 3.0f
					&& pObject->GetPos().y <= pos.y + 3.0f && pObject->GetPos().y >= pos.y - 3.0f
					&& pObject->GetPos().z <= pos.z + 3.0f && pObject->GetPos().z >= pos.z - 3.0f)
				{// ���ɔz�u���ꂽ�S�~�ƈʒu����v
					bMatchPos = true;
				}
			}

			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	if (bMatchPos == false)
	{
		if (MAX_GABAGE > m_nNumAll)
		{// �����ɒB���Ă��Ȃ�
			// �S�~�̐���
			pGarbage = new CGarbage(nPriority);

			// ��ނ̐ݒ�
			pGarbage->SetType(CObject::TYPE_GARBAGE);

			// ����������
			pGarbage->Init(pos);

			// �p�[�e�B�N���̐���
			CParticle::Create(1)->Set(pos, CParticle::TYPE_ENEMY);		// �p�[�e�B�N���̐���
		}
		else
		{
			m_nNumAll = MAX_GABAGE;		// ������߂�
		}
	}

	return pGarbage;
}

//===============================================
// ����������
//===============================================
HRESULT CGarbage::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init(pos);

	// ��ނ̐ݒ�
	srand(m_nRandCounter);							// rand�̒l�����Z�b�g
	m_type = (TYPE)(rand() % TYPE_MAX);				// rand�̒l���擾(���)
	m_size = (MODELSIZE)(rand() % MODELSIZE_MAX);	// rand�̒l���擾(�T�C�Y)

	// ��Ԃ̐ݒ�
	m_state = STATE_NORMAL;

	// ���ʂ̎��
	if (m_type == TYPE_BOOKOLD || m_type == TYPE_BOOT || m_type == TYPE_BAG)
	{// �Ö{�E���C�E��
		m_separation = SEPARATION_BURN;				// �R����S�~
	}
	else if (m_type == TYPE_LIGHT || m_type == TYPE_CAMERA || m_type == TYPE_BUCKET)
	{// ���C�g�E�J�����E�o�P�c
		m_separation = SEPARATION_NONFLAMMABLE;		// �R���Ȃ��S�~
	}
	else if (m_type == TYPE_BIN || m_type == TYPE_CAN || m_type == TYPE_DANBORU)
	{// �r�E�ʁE�i�{�[��
		m_separation = SEPARATION_RECYCLABLE;		// �����S�~
	}

	// ���f���T�C�Y�ݒ�
	if (m_type == TYPE_BOOKOLD || m_type == TYPE_LIGHT || m_type == TYPE_BIN)
	{// �Ö{�E���C�g�E�r
		m_size = MODELSIZE_NORMAL;					// �ʏ�
	}
	else if (m_type == TYPE_BOOT || m_type == TYPE_CAMERA || m_type == TYPE_CAN)
	{// ���C�E�J�����E��
		m_size = MODELSIZE_SMALL;					// ������
	}
	else if (m_type == TYPE_BAG || m_type == TYPE_BUCKET || m_type == TYPE_DANBORU)
	{// ���E�o�P�c�E�i�{�[��
		m_size = MODELSIZE_BIG;						// �傫��
	}

	m_nRandCounter++;	// rand���l�ύX

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGarbage::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CGarbage::Update(void)
{
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 PlayerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̌������擾

	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		PlayerPos = CTutorial::GetPlayer()->GetPos();			// �v���C���[�̈ʒu���擾
		PlayerRot = CTutorial::GetPlayer()->GetRot();			// �v���C���[�̌������擾
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{
		PlayerPos = CGame::GetPlayer()->GetPos();			// �v���C���[�̈ʒu���擾
		PlayerRot = CGame::GetPlayer()->GetRot();			// �v���C���[�̌������擾
	}

	//�d�͏���
	m_move.y -= MOVE_GRAVITY;

	if (m_pos.y < LAND_POS)
	{// ���n����
		// �ʒu��߂�
		m_pos.y = LAND_POS;
		m_move.y = 0.0f;
	}

	// ��ԕʂɏ�������
	switch (m_state)
	{
	case STATE_NORMAL:			// �ʏ���
		if (CManager::GetMode() == CScene::MODE_TUTORIAL &&
			(m_pos.x <= CTutorial::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX && m_pos.x >= CTutorial::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			&& m_pos.z <= CTutorial::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY && m_pos.z >= CTutorial::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY)
			|| CManager::GetMode() == CScene::MODE_GAME &&
			(m_pos.x <= CGame::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX && m_pos.x >= CGame::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
				&& m_pos.z <= CGame::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY && m_pos.z >= CGame::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY))
		{// �^�[�Q�b�g�͈͓̔��ɓ�����
			if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax(m_size) == true)
				|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax(m_size) == true))
			{
				if (CManager::GetMode() == CScene::MODE_TUTORIAL)
				{// �`���[�g���A��
					CTutorial::GetTarget()->SetMaxGage(true);
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{// �Q�[��
					CGame::GetTarget()->SetMaxGage(true);
				}
			}
			else
			{
				if (CManager::GetMode() == CScene::MODE_TUTORIAL)
				{// �`���[�g���A��
					CTutorial::GetTarget()->SetMaxGage(false);
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{// �Q�[��
					CGame::GetTarget()->SetMaxGage(false);
				}
			}

			if (CManager::GetMode() == CScene::MODE_TUTORIAL &&(CTutorial::GetTarget()->GetState() != SEPARATION_NONE && CTutorial::GetTarget()->GetWithinRange() == true)
				|| CManager::GetMode() == CScene::MODE_GAME && (CGame::GetTarget()->GetState() != SEPARATION_NONE && CGame::GetTarget()->GetWithinRange() == true))
			{// ���ʂ̐ݒ肪����Ă���
				CRockon::Create(m_pos, CTarget::GetState(), 0, 5);		// ���̈ʒu�Ƀ^�[�Q�b�g��ݒ�
				m_rockonType = CTarget::GetState();

				if (m_rockonType != SEPARATION_NONE)
				{
					m_state = STATE_SEPARATION;								// ���ʏ�Ԃ�
				}
			}
		}
		break;

	case STATE_ROCKON:			// ���b�N�I���\
		// �S�~����
		if (CManager::GetKeyboardInput()->GetPress(DIK_LEFT) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_X, 0) == true)
		{// ���L�[�������ꂽ
			// �R����S�~
			CRockon::Create(m_pos, SEPARATION_BURN, 0, 5);			// ���̈ʒu�Ƀ^�[�Q�b�g��ݒ�
			m_rockonType = SEPARATION_BURN;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_UP) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_Y, 0) == true)
		{// ��L�[�������ꂽ
			// �R���Ȃ��S�~
			CRockon::Create(m_pos, SEPARATION_NONFLAMMABLE, 0, 5);	// ���̈ʒu�Ƀ^�[�Q�b�g��ݒ�
			m_rockonType = SEPARATION_NONFLAMMABLE;
		}
		else if (CManager::GetKeyboardInput()->GetPress(DIK_RIGHT) == true
			|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_B, 0) == true)
		{// �E�L�[�������ꂽ
			// �����S�~
			CRockon::Create(m_pos, SEPARATION_RECYCLABLE, 0, 5);		// ���̈ʒu�Ƀ^�[�Q�b�g��ݒ�
			m_rockonType = SEPARATION_RECYCLABLE;
		}

		if (CManager::GetMode() == CScene::MODE_TUTORIAL &&
			(m_pos.x >= CTutorial::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX || m_pos.x <= CTutorial::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			|| m_pos.z >= CTutorial::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY || m_pos.z <= CTutorial::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY)
			|| CManager::GetMode() == CScene::MODE_GAME &&
			(m_pos.x >= CGame::GetTarget()->GetPos().x + TARGET_SIZEX + TARGET_SIZEX || m_pos.x <= CGame::GetTarget()->GetPos().x - TARGET_SIZEX - TARGET_SIZEX
			|| m_pos.z >= CGame::GetTarget()->GetPos().z + TARGET_SIZEY + TARGET_SIZEY || m_pos.z <= CGame::GetTarget()->GetPos().z - TARGET_SIZEY - TARGET_SIZEY))
		{// �^�[�Q�b�g�͈̔͊O�֊O�ꂽ
			m_state = STATE_NORMAL;		// �ʏ��Ԃɂ���
		}
		break;

	case STATE_SEPARATION:		// ����
		if ((m_rockonType == SEPARATION_BURN && (CManager::GetKeyboardInput()->GetRelease(DIK_RIGHT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_B, 0) == true))
			|| (m_rockonType == SEPARATION_NONFLAMMABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_LEFT) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_X, 0) == true))
			|| (m_rockonType == SEPARATION_RECYCLABLE && (CManager::GetKeyboardInput()->GetRelease(DIK_UP) == true || CManager::GetInputGamePad()->GetRelease(CInputGamePad::BUTTON_Y, 0) == true)))
		{// �L�[�𗣂���
			if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetUiGage()->GetMax(m_size) == false)
				|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetUiGage()->GetMax(m_size) == false))
			{// �S�~�Q�[�W���ő傶��Ȃ�
				if ((PlayerRot.y <= D3DX_PI * ROT_UP + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_UP - TURN_ROT_DIFF && m_pos.z < PlayerPos.z) 
					|| (PlayerRot.y <= D3DX_PI * ROT_DOWN + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_DOWN - TURN_ROT_DIFF && m_pos.z > PlayerPos.z)
					|| (PlayerRot.y <= -D3DX_PI * ROT_DOWN + TURN_ROT_DIFF && PlayerRot.y >= -D3DX_PI * ROT_DOWN - TURN_ROT_DIFF && m_pos.z > PlayerPos.z)
					|| (PlayerRot.y <= D3DX_PI * ROT_RIGHT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_RIGHT - TURN_ROT_DIFF && m_pos.x < PlayerPos.x)
					|| (PlayerRot.y <= D3DX_PI * ROT_LEFT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_LEFT - TURN_ROT_DIFF && m_pos.x > PlayerPos.x))
				{// ���b�N�I�������S�~����ʓ��ɂ���
					if ((CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetTarget()->GetFlameJust() < FLAME_JUSTDUST && CTutorial::GetTarget()->GetFlameJust() >= 0)
						|| (CManager::GetMode() == CScene::MODE_GAME && CGame::GetTarget()->GetFlameJust() < FLAME_JUSTDUST && CGame::GetTarget()->GetFlameJust() >= 0))
					{// �W���X�g�^�C�~���O
						m_bJustDust = true;		// JustDust�ɐݒ�
					}

					if (CManager::GetMode() == CScene::MODE_TUTORIAL)
					{// �`���[�g���A��
						CTutorial::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);		// �v���C���[���[�V������ݒ�
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{// �Q�[��
						CGame::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);			// �v���C���[���[�V������ݒ�
					}

					m_posDest.y = DIFF_HEIGHT;		// �ړI�̍�����ݒ�
					m_state = STATE_DISPOSE;		// �̂Ă�
				}
				else
				{
					m_state = STATE_NORMAL;		// �ʏ��Ԃ֖߂�
				}
			}
			else
			{// �Q�[�W���ő�𒴂��Ă��܂�
				m_state = STATE_NORMAL;		// �ʏ��Ԃ֖߂�

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_DISABLE);
			}
		}
		else if (m_rockonType == SEPARATION_NONE)
		{// ���ʂ̐ݒ肪����Ă��Ȃ�
			m_state = STATE_NORMAL;		// �ʏ��Ԃɂ���
		}
		//// �v���C���[�̏�ň�莞�ԐÎ~������
		//m_pos = PlayerPos;
		//m_pos.y = DIFF_HEIGHT;

		//m_nWaitCounter--;	// ���Ԃ��f�N�������g

		//if (m_nWaitCounter <= 0)
		//{// �ݒ肵�����ԂɒB����
		//	m_state = STATE_COLLECT;		// ���ʏ�Ԃ�
		//}
		break;

	case STATE_DISPOSE:			// �̂Ă�
		m_fMoveDiff += MOVE_DIFF;	// �␳�W�������Z����

		// �ړI�̈ʒu��ݒ�
		m_posDest.x = PlayerPos.x;
		m_posDest.z = PlayerPos.z;

		m_posDiff = m_posDest - m_pos;	// �ړI�̈ʒu�܂ł̍���

		m_pos += m_posDiff * m_fMoveDiff;	// �ʒu�̕␳

		if (m_rockonType == m_separation)
		{// ���ʐ���
			if (m_bJustDust == false)
			{
				// �G�t�F�N�g�̐���
				CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
			}
			else
			{
				// �G�t�F�N�g�̐���
				CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.9f, 0.5f, 0.2f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
			}
		}
		else
		{// ���ʎ��s
			// �G�t�F�N�g�̐���
			CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);
		}

		if (m_pos.x <= m_posDest.x + DIFF_DISPOSE
			&& m_pos.x >= m_posDest.x - DIFF_DISPOSE
			&& m_pos.z <= m_posDest.z + DIFF_DISPOSE
			&& m_pos.z >= m_posDest.z - DIFF_DISPOSE)
		{// �ړI�̈ʒu�ɂȂ���
			m_state = STATE_COLLECT;		// ���
			m_nWaitCounter = WAIT_TIME;		// �Î~���Ă��鎞�Ԃ�ݒ�
			m_fMoveDiff = 0.0f;				// �␳�W����������
		}
		break;

	case STATE_COLLECT:			// ���
		m_pos.y -= GRAVITY_DISPOSE;		// �d�͂�t�^

		if (m_pos.y <= LAND_POS + 50.0f)
		{// �v���C���[�Ɋђ�
			// �p�[�e�B�N���̐���
			CParticle::Create(3)->Set(D3DXVECTOR3(m_pos.x - sinf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f), m_pos.y, 
				m_pos.z - cosf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f)), particleType);

			// �ݒ菈��
			if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{
				// ���f���̃T�C�Y�ʂɃQ�[�W�㏸�ʂ�ύX
				switch (m_size)
				{
				case MODELSIZE_NORMAL:		// �ʏ�
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_NORMAL);		// �Q�[�W�㏸
					break;

				case MODELSIZE_SMALL:		// ������
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_SMALL);		// �Q�[�W�㏸
					break;

				case MODELSIZE_BIG:			// �傫��
					CTutorial::GetUiGage()->Set(AMOUNT_OF_RISE_BIG);		// �Q�[�W�㏸
					break;
				}
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				// ���f���̃T�C�Y�ʂɃQ�[�W�㏸�ʂ�ύX
				switch (m_size)
				{
				case MODELSIZE_NORMAL:		// �ʏ�
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_NORMAL);		// �Q�[�W�㏸
					break;

				case MODELSIZE_SMALL:		// ������
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_SMALL);		// �Q�[�W�㏸
					break;

				case MODELSIZE_BIG:			// �傫��
					CGame::GetUiGage()->Set(AMOUNT_OF_RISE_BIG);		// �Q�[�W�㏸
					break;
				}
			}

			Separation(m_rockonType);		// ���ʏ����i�X�R�A�ɉ��Z�����j

			m_state = STATE_INPLAYER;		// �`�悵�Ȃ���Ԃɂ���
		}
		break;

	case STATE_INPLAYER:		// �`�悵�Ȃ�
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			m_pos = CTutorial::GetPlayer()->GetPos();	// �S�~�̈ʒu���v���C���[�Ɠ���������
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			m_pos = CGame::GetPlayer()->GetPos();	// �S�~�̈ʒu���v���C���[�Ɠ���������
		}

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

			while (pObject != NULL)
			{// �g�p����Ă���
				CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
				CObject::TYPE type = pObject->GetType();		// ��ނ��擾

				if (type == CObject::TYPE_DUMPSTER)
				{// �S�~�X�e�[�V����
					if ((CManager::GetMode() == CScene::MODE_TUTORIAL
						&& pObject->GetPos().x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
						&& pObject->GetPos().z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEY && pObject->GetPos().z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEY)
						|| (CManager::GetMode() == CScene::MODE_GAME
						&& pObject->GetPos().x <= CGame::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && pObject->GetPos().x >= CGame::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
						&& pObject->GetPos().z <= CGame::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEY && pObject->GetPos().z >= CGame::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEY))
					{// �^�[�Q�b�g���S�~�X�e�[�V�����𗘗p�ł���͈͓��ɓ�����
						if (CManager::GetKeyboardInput()->GetTrigger(DIK_SPACE) == true
							|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
						{// SPACE�L�[�������ꂽ
							if (CManager::GetMode() == CScene::MODE_TUTORIAL)
							{// �`���[�g���A��
								CTutorial::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);		// �v���C���[���[�V������ݒ�
							}
							else if (CManager::GetMode() == CScene::MODE_GAME)
							{// �Q�[��
								CGame::GetPlayer()->SetMotion(CPlayer::MOTIONTYPE_ACTION);			// �v���C���[���[�V������ݒ�
							}

							m_posDest.y = DIFF_HEIGHT;			// �ړI�̍�����ݒ�
							m_nextposDest = pObject->GetPos();	// ���̖ړI�̈ʒu��ۑ�

							m_state = STATE_DUMPSTER;		// �S�~�X�e�[�V����
						}
					}
				}
				pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
			}
		}
		break;

	case STATE_DUMPSTER:		// �S�~�X�e�[�V����
		m_fMoveDiff += MOVE_DIFF;	// �␳�W�������Z����

		// �ړI�̈ʒu��ݒ�
		m_posDest.x = m_nextposDest.x;
		m_posDest.z = m_nextposDest.z;

		m_posDiff = m_posDest - m_pos;		// �ړI�̈ʒu�܂ł̍���
		m_pos += m_posDiff * m_fMoveDiff;	// �ʒu�̕␳

		// �G�t�F�N�g�̐���
		CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.2f, 1.0f), CEffect::TYPE_NORMAL, 20, 25, 3);

		if (m_pos.x <= m_posDest.x + DIFF_DISPOSE
			&& m_pos.x >= m_posDest.x - DIFF_DISPOSE
			&& m_pos.z <= m_posDest.z + DIFF_DISPOSE
			&& m_pos.z >= m_posDest.z - DIFF_DISPOSE)
		{// �ړI�̈ʒu�ɂȂ���
			m_state = STATE_DELETE;			// �j��

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DUMPSTER);
		}
		break;

	case STATE_DELETE:			// �j��
		m_pos.y -= GRAVITY_DISPOSE;		// �d�͂�t�^

		if (m_pos.y <= LAND_POS + 50.0f)
		{// �v���C���[�Ɋђ�
			// �p�[�e�B�N���̐���
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - sinf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f), m_pos.y,
				m_pos.z - cosf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f)), particleType);

			if (CManager::GetMode() == CScene::MODE_GAME)
			{// �Q�[�����[�h
				CGame::GetScore()->Add(ADD_SCORE);		// �X�R�A���Z

				// �ݒ菈��
				CGame::GetUiGage()->Set(-MAX_GARBAGE);
			}
			else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{// �`���[�g���A��
				CTutorial::GetScore()->Add(ADD_SCORE);		// �X�R�A���Z

				// �ݒ菈��
				CTutorial::GetUiGage()->Set(-MAX_GARBAGE);
			}

			 // �I������
			Uninit(); 
		}
		break;
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (PlayerPos.z < m_pos.z - DELETE_POSITION)
		{// ������ʊO�ւ�����
			// �I������
			Uninit();
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CGarbage::Draw(void)
{
	if (m_state != STATE_INPLAYER)
	{// �`�悵�Ȃ���Ԃ���Ȃ�
		if (m_state == STATE_ROCKON)
		{// ���b�N�I���\���
			// �I�u�W�F�N�gX�̕`�揈���i�ԁj
			CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_RED);
		}
		else
		{// ����ȊO
			// ���ʂ̎�ޕʂɐF��ς���
			switch (m_separation)
			{
			case SEPARATION_BURN:			// �R����S�~
				// �I�u�W�F�N�gX�̕`�揈���i�ԁj
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_RED);
				break;

			case SEPARATION_NONFLAMMABLE:	// �R���Ȃ��S�~
				// �I�u�W�F�N�gX�̕`�揈���i�j
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_BLUE);
				break;

			case SEPARATION_RECYCLABLE:		// �����S�~
				// �I�u�W�F�N�gX�̕`�揈���i���F�j
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_YELLOW);
				break;

			default:						// ����ȊO
				// �I�u�W�F�N�gX�̕`�揈��
				CObjectX::DrawXFile(m_aIdxXFile[m_type], CXFile::COL_NORMAL);
				break;
			}
		}
	}
}

//===============================================
// ���ʏ���
//===============================================
void CGarbage::Separation(SEPARATION separation)
{
	m_rockonType = separation;	// ���b�N�I�����̕��ʂ̎��

	if (m_separation == separation)
	{// �ݒ肳�ꂽ���ʂ̎�ނƓ��͂������ʂ̎�ނ���v
		CManager::GetDebugProc()->Print(" ����");
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			if (m_bJustDust == true)
			{// JustDust�������ꍇ
				CTutorial::GetScore()->Add(ADD_SOCREJUST);	// �X�R�A���Z
				CTutorial::GetJustDust()->Set(true);		// JustDust�ݒ�

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_JUSTDUST);
			}
			else
			{
				CTutorial::GetScore()->Add(ADD_SCORE);		// �X�R�A���Z

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_SUCCESS);
			}
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			if (m_bJustDust == true)
			{// JustDust�������ꍇ
				CGame::GetScore()->Add(ADD_SOCREJUST);	// �X�R�A���Z
				CGame::GetJustDust()->Set(true);		// JustDust�ݒ�

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_JUSTDUST);
			}
			else
			{
				CGame::GetScore()->Add(ADD_SCORE);		// �X�R�A���Z

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_SUCCESS);
			}
		}
	}
	else
	{// �s��v
		CManager::GetDebugProc()->Print(" �s����");
		if (CManager::GetMode() == CScene::MODE_TUTORIAL)
		{
			CTutorial::GetScore()->Add(SUB_SCORE);		// �X�R�A���Z
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetScore()->Add(SUB_SCORE);		// �X�R�A���Z
		}

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_FAILURE);
	}
}