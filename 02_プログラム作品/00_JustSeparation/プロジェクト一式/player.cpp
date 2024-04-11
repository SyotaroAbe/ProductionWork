//=========================================================
//
// �v���C���[���� [player.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "bullet.h"
#include "camera.h"
#include "object.h"
#include "particle.h"
#include "model.h"
#include "meshfield.h"
#include "motion.h"
#include "fileload.h"
#include "shadow.h"
#include "object3D.h"
#include "garbage.h"
#include "game.h"
#include "tutorial.h"
#include "time.h"
#include "target.h"
#include "sound.h"
#include "title.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE_PLAYER			(0.45f)		// �������x
#define JUMP_PLAYER			(16.85f)	// �W�����v��
#define MOVE_GRAVITY		(0.75f)		// �d��
#define LAND_POS			(0.0f)		// �n��

#define MOVE_MINUS			(0.07f)		// �ړ��ʂ̌���
#define TURN_TIME			(1)			// �Ȃ��鎞��
#define COUNT_PARTICLE		(25)		// �p�[�e�B�N����������

// �t�B�[�o�[
#define MOVE_FEVER			(0.6f)		// �ړ����x�i�t�B�[�o�[�j

//===============================================
// �R���X�g���N�^
//===============================================
CPlayer::CPlayer() : CObject(4)
{
	// �l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_speed = 0.0f;
	m_bJump = true;
	m_fRotBullet = 0.0f;
	m_turnType = TURN_NONE;
	m_turnTypeOld = TURN_NONE;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_speed = 0.0f;
	m_bJump = true;
	m_fRotBullet = 0.0f;
	m_turnType = TURN_NONE;
	m_turnTypeOld = TURN_NONE;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_nParticleCounter = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// ��������
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int nPriority)
{
	CPlayer *pPlayer;

	// �v���C���[�̐���
	pPlayer = new CPlayer(nPriority);

	// ��ނ̐ݒ�
	pPlayer->SetType(CObject::TYPE_PLAYER);
	
	// ����������
	pPlayer->Init(pos);

	return pPlayer;
}

//===============================================
// ����������
//===============================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// ���[�V�����̐���
	m_pMotion = new CMotion;

	// ���[�V�����̏�����
	m_pMotion->Init();

	// ���f���̑���
	m_nNumModel = CManager::GetLoad()->GetNumModel();

	// �ʒu�̐ݒ�
	m_pos = pos;

	// �ړ����x�̏�����
	m_speed = MOVE_PLAYER;

	// �p�[�e�B�N�������h�~
	m_nParticleCounter = COUNT_PARTICLE;

	const char *apModelFile[MAX_MODEL];		// ���f���t�@�C����

	// ���f���̐����i�S�p�[�c�j
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetLoad()->GetFileName(nCntModel);		// �t�@�C�����擾
		pos = CManager::GetLoad()->GetPos(nCntModel);								// �ʒu�̎擾
		rot = CManager::GetLoad()->GetRot(nCntModel);								// �����̎擾

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// ����
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// ����

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// �e���f���̐ݒ�i�S�p�[�c�j
	m_apModel[0]->SetParent(NULL);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetLoad()->GetParent(nCntModel);	// �e���擾

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// ���[�V�������̐ݒ�
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// ���[�V���������J��Ԃ�
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetLoad()->GetInfo(nCntMotion));
	}

	// �������[�V�����ݒ�
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
{
	// ���f���̔j��
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// �g�p����Ă���
			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// ���[�V�����ւ̃|�C���^�̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CPlayer::Update(void)
{
#if _DEBUG
	// �S�~��z�u
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_0) == true)
	{// ENTER�L�[�������ꂽ
		CGarbage::Create(m_pos, 3);
		CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f,
			m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f), 3);
		CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f,
			m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * 200.0f), 3);
	}

	// �ړ����x�ύX
	if (CManager::GetKeyboardInput()->GetPress(DIK_O) == true || CManager::GetInputGamePad()->GetJoyStickRY(0) > 0)
	{// O�L�[�������ꂽ
		m_speed += 0.01f;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_L) == true || CManager::GetInputGamePad()->GetJoyStickRY(0) < 0)
	{// L�L�[�������ꂽ
		m_speed -= 0.01f;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_8) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LS, 0))
	{// L�L�[�������ꂽ
		m_speed = MOVE_PLAYER;
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_9) == true || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RS, 0))
	{// L�L�[�������ꂽ
		m_speed = MOVE_FEVER;
	}
#endif

	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	// �����]��
	switch (m_turnType)
	{
	case TURN_NONE:		// �Ȃ���Ȃ�
		m_move.x += sinf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rot.y = D3DX_PI + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		// �ʒu�␳����
		if (m_turnTypeOld != TURN_NONE)
		{
			if (CManager::GetMode() == CScene::MODE_TUTORIAL)
			{
				CTutorial::GetTarget()->RevisionPos();
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetTarget()->RevisionPos();
				m_nTurnCounter++;

				if (m_nTurnCounter >= 20)
				{
					m_turnTypeOld = TURN_NONE;
				}
			}
			else if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				//CTitle::GetTarget()->RevisionPos();
				m_nTurnCounter++;

				if (m_nTurnCounter >= 20)
				{
					m_turnTypeOld = TURN_NONE;
				}
			}
		}

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{// �^�C�g������Ȃ�
			// �G�t�F�N�g�̐����i���j
			CParticle::Create(1)->Set(m_pos, CParticle::TYPE_MOVE);
		}

		if (m_pMotion->IsFinish() == true || m_pMotion->GetType() == MOTIONTYPE_NEUTRAL)
		{
			// ���[�V�����ݒ�
			m_pMotion->Set(MOTIONTYPE_MOVE);
		}
		break;

	case TURN_RIGHT:	// �E�֋Ȃ���
		m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// �p�[�e�B�N������������
		m_turnTypeOld = m_turnType;	// �O��̏���ۑ�
		m_nTurnCounter = 0;			// �J�E���^�����Z�b�g
		m_turnType = TURN_NONE;		// �Ȃ���Ȃ��ݒ�ɂ���

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;

	case TURN_LEFT:		// ���֋Ȃ���
		m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// �p�[�e�B�N������������
		m_turnTypeOld = m_turnType;	// �O��̏���ۑ�
		m_nTurnCounter = 0;			// �J�E���^�����Z�b�g
		m_turnType = TURN_NONE;		// �Ȃ���Ȃ��ݒ�ɂ���

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;

	case TURN_BACK:		// �܂�Ԃ�
		m_move.x += sinf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_move.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_speed;
		m_rotDest.y = D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);

		m_nParticleCounter = 0;		// �p�[�e�B�N������������
		m_turnTypeOld = m_turnType;	// �O��̏���ۑ�
		m_nTurnCounter = 0;			// �J�E���^�����Z�b�g
		m_turnType = TURN_NONE;		// �Ȃ���Ȃ��ݒ�ɂ���

		if (CManager::GetMode() != CScene::MODE_TITLE)
		{
			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_CURVE);
		}
		break;
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����[�h
		if (CGame::GetTime()->Get() == TIME_FEVER)
		{// �t�B�[�o�[�^�C��
			m_speed = MOVE_FEVER;		// ���x���グ��
		}
	}

	if (m_speed == MOVE_FEVER)
	{// �t�B�[�o�[
		m_nParticleCounter = 0;		// �p�[�e�B�N������������
	}

	if (m_nParticleCounter < COUNT_PARTICLE && CManager::GetMode() != CScene::MODE_TITLE)
	{// �p�[�e�B�N���������ԂɒB���Ă��Ȃ�
		// �p�[�e�B�N���̐���
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z + sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		m_nParticleCounter++;											// �������Ԃ��J�E���g�A�b�v
	}

	m_fRotDiff = m_rotDest.y - m_rot.y;	// �ړI�̌����܂ł̍���

	// �p�x�̒l�̕␳
	if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * ROT_DIFF_PI;
	}

	m_rot.y += m_fRotDiff * ROT_CAMERA_DIFF;	// �p�x�̕␳

	// �p�x�̒l�̕␳
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * ROT_DIFF_PI;
	}

	// �d�͏���
	m_move.y -= MOVE_GRAVITY;

	// �ʒu���X�V
	m_pos += m_move;

	// 3D�|���S���Ƃ̓����蔻��
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// �ړ��ʂ��X�V�i����������j
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	if (m_pos.y < LAND_POS)
	{// ���n����
		m_pos.y = LAND_POS;
		m_move.y = 0.0f;
	}

	// ���[�V�����̍X�V����
	m_pMotion->Update();

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print(" [Q E]�F�ړ������ύX\n");
	CManager::GetDebugProc()->Print(" [SPACE]�F�̂Ă�\n");
	CManager::GetDebugProc()->Print(" [�� �� ��]�F����\n");
	CManager::GetDebugProc()->Print(" [A D]�F�^�[�Q�b�g�ړ�\n\n");

	CManager::GetDebugProc()->Print(" [ENTER]�F�S�~�̐���\n");
	CManager::GetDebugProc()->Print(" [O L]�F�ړ����x�ύX\n\n");

	CManager::GetDebugProc()->Print(" �v���C���[�̈ʒu�F�i%f, %f, %f�j\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetDebugProc()->Print(" �v���C���[�̈ړ����x�F%f\n", m_speed);
	CManager::GetDebugProc()->Print(" �v���C���[�̌����F%f\n\n", m_rot.y);
	
	CManager::GetDebugProc()->Print(" �J�����̌����F%f\n", CManager::GetCamera()->GetRot().y);
	CManager::GetDebugProc()->Print(" ���_�F�i%f, %f, %f�j\n", CManager::GetCamera()->GetPosV().x, CManager::GetCamera()->GetPosV().y, CManager::GetCamera()->GetPosV().z);
	CManager::GetDebugProc()->Print(" �����_�F�i%f, %f, %f�j\n", CManager::GetCamera()->GetPosR().x, CManager::GetCamera()->GetPosR().y, CManager::GetCamera()->GetPosR().z);
	CManager::GetDebugProc()->Print(" �S�~�̑����F%d\n", CManager::GetGarbage()->GetNumAll());
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CManager::GetDebugProc()->Print(" ���̑����F%d\n", CGame::GetMeshField()->GetNumAll());
	}
}

//===============================================
// �`�揈��
//===============================================
void CPlayer::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_TITLE)
	{// �^�C�g������Ȃ�
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
		D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ���f���̕`��i�S�p�[�c�j
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			// ���f���̕`�揈��
			m_apModel[nCntModel]->Draw();

			// ���f���̉e�̕`�揈��
			m_apModel[nCntModel]->DrawShadowmtx();
		}
	}
}

//===============================================
// �ʒu�̐ݒ�
//===============================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʂ̐ݒ�
//===============================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �W�����v�������̐ݒ�
//===============================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//===============================================
// �J�[�u�̐ݒ�
//===============================================
void CPlayer::SetTurn(TURN turn)
{
	m_turnType = turn;
}

//===============================================
// ���[�V�����̐ݒ�
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}