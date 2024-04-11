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
#include "camera.h"
#include "object.h"
#include "particle.h"
#include "model.h"
#include "meshfield.h"
#include "motion.h"
#include "fileload.h"
#include "shadow.h"
#include "object3D.h"
#include "game.h"
#include "tutorial.h"
#include "sound.h"
#include "title.h"
#include "bullet.h"
#include "objectX.h"
#include "magnet.h"
#include "renderer.h"
#include "fade.h"
#include "time.h"
#include "item.h"

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

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CPlayer* CPlayer::m_pPlayer = nullptr;				// �v���C���[�N���X�̃|�C���^
int CPlayer::m_nPriority = 0;

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
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
	m_nInvincibleCounter = 0;
	m_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDisp = true;
	m_magnet = EMAGNET_NONE;
	m_magnetOld = EMAGNET_NONE;
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
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = nullptr;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
	m_nInvincibleCounter = 0;
	m_posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDisp = true;
	m_magnet = EMAGNET_NONE;
	m_magnetOld = EMAGNET_NONE;
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
	m_nPriority = nPriority;

	// �v���C���[�̐���
	pPlayer = new CPlayer(nPriority);

	if (pPlayer != nullptr)
	{
		// ��ނ̐ݒ�
		pPlayer->SetType(CObject::TYPE_PLAYER);

		// ����������
		pPlayer->Init(pos);
	}

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
	m_nNumModel = CManager::GetInstance()->GetLoad()->GetNumModel(CFileLoad::FILE_PLAYER);

	// �ʒu�̐ݒ�
	m_pos = pos;
	m_posShadow = m_pos;

	// �����̐ݒ�
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

	// �ړ����x�̏�����
	m_fSpeed = MOVE_PLAYER;

	// �p�[�e�B�N�������h�~
	m_nParticleCounter = COUNT_PARTICLE;

	const char *apModelFile[MAX_MODEL];		// ���f���t�@�C����

	// ���f���̐����i�S�p�[�c�j
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetInstance()->GetLoad()->GetFileName(CFileLoad::FILE_PLAYER, nCntModel);		// �t�@�C�����擾
		pos = CManager::GetInstance()->GetLoad()->GetPos(CFileLoad::FILE_PLAYER, nCntModel);								// �ʒu�̎擾
		rot = CManager::GetInstance()->GetLoad()->GetRot(CFileLoad::FILE_PLAYER, nCntModel);								// �����̎擾

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// ����
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// ����

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// �e���f���̐ݒ�i�S�p�[�c�j
	m_apModel[0]->SetParent(nullptr);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetInstance()->GetLoad()->GetParent(CFileLoad::FILE_PLAYER, nCntModel);	// �e���擾

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// �l�̏�����
	m_vtxMin = D3DXVECTOR3(900.0f, 900.0f, 900.0f);
	m_vtxMax = D3DXVECTOR3(-900.0f, -900.0f, -900.0f);

	// �S�Ă̒��_���r���ă��f���̍ŏ��l�E�ő�l�𔲂��o��
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// �ŏ��l�̔�r
		if (m_apModel[nCntModel]->GetVtxMin().x <= m_vtxMin.x)
		{
			m_vtxMin.x = m_apModel[nCntModel]->GetVtxMin().x;
		}

		if (m_apModel[nCntModel]->GetVtxMin().y <= m_vtxMin.y)
		{
			m_vtxMin.y = m_apModel[nCntModel]->GetVtxMin().y;
		}

		if (m_apModel[nCntModel]->GetVtxMin().z <= m_vtxMin.z)
		{
			m_vtxMin.z = m_apModel[nCntModel]->GetVtxMin().z;
		}

		// �ő�l�̔�r
		if (m_apModel[nCntModel]->GetVtxMax().x >= m_vtxMax.x)
		{
			m_vtxMax.x = m_apModel[nCntModel]->GetVtxMax().x;
		}

		if (m_apModel[nCntModel]->GetVtxMax().y >= m_vtxMax.y)
		{
			m_vtxMax.y = m_apModel[nCntModel]->GetVtxMax().y;
		}

		if (m_apModel[nCntModel]->GetVtxMax().z >= m_vtxMax.z)
		{
			m_vtxMax.z = m_apModel[nCntModel]->GetVtxMax().z;
		}
	}

	// ���[�V�������̐ݒ�
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// ���[�V���������J��Ԃ�
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetInstance()->GetLoad()->GetInfo(CFileLoad::FILE_PLAYER, nCntMotion));
	}

	// �������[�V�����ݒ�
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// ���[�V�����̍X�V����
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	// ������Ԑݒ�
	m_state = STATE_NORMAL;
	m_fLenthCamera = LENTH_NORMAL;

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
		if (m_apModel[nCntModel] != nullptr)
		{// �g�p����Ă���
			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = nullptr;
		}
	}

	// ���[�V�����ւ̃|�C���^�̔j��
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	this->Release();

	//if (m_pPlayer != nullptr)
	//{
	//	delete m_pPlayer;
	//	m_pPlayer = nullptr;
	//}
}

//===============================================
// �X�V����
//===============================================
void CPlayer::Update(void)
{
	float fGravity = MOVE_GRAVITY;

	// �J�E���^���X�V
	m_nStateCounter--;
	m_nInvincibleCounter--;

	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;
	m_magnetOld = m_magnet;

	if (m_state != STATE_CLEAR)
	{
		if (m_nParticleCounter < COUNT_PARTICLE && CManager::GetMode() != CScene::MODE_TITLE)
		{// �p�[�e�B�N���������ԂɒB���Ă��Ȃ�
			// �p�[�e�B�N���̐���
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z + sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
			m_nParticleCounter++;											// �������Ԃ��J�E���g�A�b�v
		}

		if (m_state == STATE_NORMAL)
		{
			if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_A) == true
				|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) < 0)
			{//���L�[�������ꂽ
				m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
				if (m_bJump == false)
				{
					m_pMotion->Set(MOTIONTYPE_MOVE);				// �ړ����[�V�����ݒ�
				}
			}
			else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true
				|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) > 0)
			{//�E�L�[�������ꂽ
				m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
				m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
				if (m_bJump == false)
				{
					m_pMotion->Set(MOTIONTYPE_MOVE);				// �ړ����[�V�����ݒ�
				}
			}
			else if (m_bJump == false)
			{
				m_pMotion->Set(MOTIONTYPE_NEUTRAL);				// �ҋ@���[�V�����ݒ�
			}

			if ((CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_SPACE) == true 
				|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0)) && m_bJump == false)
			{// SPACE�L�[�������ꂽ
				m_pMotion->Set(MOTIONTYPE_JUMP);				// �W�����v���[�V����

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);

				m_move.y = JUMP_PLAYER;
				m_bJump = true;
			}
		}
	}

	if (m_state == STATE_NORMAL || m_state == STATE_DAMAGE)
	{
		// �J�������������ɐݒ�
		m_fLenthCamera = LENTH_NORMAL;
		CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.15f);
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		m_fSpeed = MOVE_PLAYER;
		break;

	case STATE_DAMAGE:		// �_���[�W
		if (m_nStateCounter < 0)
		{
			m_state = STATE_NORMAL;
			m_bInvincible = true;
			m_nInvincibleCounter = 120;				// ��ԃJ�E���^�[��ݒ�
		}
		break;

	case STATE_INVINCIBLE:	// ���G
		break;

	case STATE_DEATH:		// ���S
		break;
	}

	if (m_bInvincible == true)
	{
		if ((m_nInvincibleCounter % 10) == 0)
		{
			m_bDisp = m_bDisp ? false : true;
		}

		if (m_nInvincibleCounter < 0)
		{
			m_bInvincible = false;
			m_bDisp = true;
		}
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
	m_move.y -= fGravity;

	// �ʒu���X�V
	m_pos += m_move;

	// 3D�|���S���Ƃ̓����蔻��
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// �ړ��ʂ��X�V�i����������j
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	//if (m_pos.y < LAND_POS && CManager::GetMode() == CScene::MODE_TUTORIAL)
	//{// ���n����
	//	m_pos.y = LAND_POS;
	//	m_move.y = 0.0f;
	//	m_bJump = false;
	//	m_bAirJump = false;
	//}

	// �`���[�g���A����ʂ̉�ʔ͈͐ݒ�
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		if (m_pos.z < -650.0f)
		{
			m_pos.z = -650.0f;
		}
		if (m_pos.z > 900.0f)
		{
			m_pos.z = 900.0f;
		}
	}
	else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (m_pos.z < -500.0f)
		{
			m_pos.z = -500.0f;
		}
		if (m_pos.z > 600.0f)
		{
			m_pos.z = 600.0f;
		}
	}

	// ���[�V�����̍X�V����
	if (m_pMotion != nullptr)
	{
		m_pMotion->Update();
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_LB, 0)
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_RB, 0))
	{
		m_magnet++;

		if (m_magnet >= EMAGNET_MAX)
		{
			m_magnet = EMAGNET_NONE;
		}
	}

	// �n�`�Ƃ̓����蔻��
	CItem::CollisionItem(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);
	if (CObjectX::CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin) == true)
	{// ���n���Ă���
		SetState(STATE_NORMAL);
		SetJump(false);			// �W�����v�t���O�����Z�b�g
	}
	else
	{
		m_bJump = true;
	}
	CMagnet::CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);

	// �e�̈ʒu�̐ݒ�
	SetPosShadow();

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(" �ړ�          �FA D\n");
	CManager::GetInstance()->GetDebugProc()->Print(" �W�����v      �FSPACE\n");
	CManager::GetInstance()->GetDebugProc()->Print(" ���͐؂�ւ�  �FENTER\n");
	CManager::GetInstance()->GetDebugProc()->Print(" ���U���g��    �FBACKSPACE\n\n");

	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ʒu�F�i%f, %f, %f�j\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ړ��ʁF�i%f, %f, %f�j\n", m_move.x, m_move.y, m_move.z);
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ړ����x�F%f\n", m_fSpeed);
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̌����F%f\n\n", m_rot.y);
	
	CManager::GetInstance()->GetDebugProc()->Print(" �J�����̌����F%f\n", CManager::GetInstance()->GetCamera()->GetRot().y);
	CManager::GetInstance()->GetDebugProc()->Print(" ���_�F�i%f, %f, %f�j\n", CManager::GetInstance()->GetCamera()->GetPosV().x, CManager::GetInstance()->GetCamera()->GetPosV().y, CManager::GetInstance()->GetCamera()->GetPosV().z);
	CManager::GetInstance()->GetDebugProc()->Print(" �����_�F�i%f, %f, %f�j\n\n", CManager::GetInstance()->GetCamera()->GetPosR().x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z);
}

//===============================================
// �`�揈��
//===============================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{// �^�C�g������Ȃ�
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
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
			switch (m_magnet)
			{
			case EMAGNET_NONE:
				m_apModel[nCntModel]->Draw();
				break;

			case EMAGNET_N:
				m_apModel[nCntModel]->SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
				break;

			case EMAGNET_S:
				m_apModel[nCntModel]->SetCol(D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f));
				break;
			}

			// ���f���̉e�̕`�揈��
			m_apModel[nCntModel]->DrawShadowmtx(m_posShadow.y);
		}
	}
}

//===============================================
// ���S����̏���
//===============================================
void CPlayer::Death(void)
{
	SetState(CPlayer::STATE_DEATH);
	CManager::GetInstance()->AddCountDeath(CManager::GetMode());			// ���S�񐔂��J�E���g
	CRenderer::GetFade()->Set(CScene::MODE_GAME);							// ���U���g��ʂֈړ�
	//Reset();	// ���Z�b�g
	m_bDisp = false;

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CParticle::Create()->Set(m_pos, CParticle::TYPE_BULLET);
		int nTime = CGame::GetTime()->Get();
		CGame::SetTime(nTime);		// ���Ԃ̐ݒ�
	}
}

//===============================================
// �I�u�W�F�N�gX�Ƃ̓����蔻��
//===============================================
void CPlayer::CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPos->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPos->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z && pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z
		&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y && pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
	{// �͈͓��ɂ���
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// �ォ��߂荞��
			// ��ɂ̂���
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;

			if (m_state != STATE_DAMAGE)
			{// �q�b�v�h���b�v��
				SetState(STATE_NORMAL);
			}
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// ������߂荞��
			// ���ɖ߂�
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z + vtxMin.z >= m_posOld.z - m_vtxMin.z
			&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z)
		{// ������E�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z + m_vtxMin.z + vtxMin.z;
		}
		else if (pPosOld->z + vtxMax.z <= m_posOld.z + m_vtxMin.z
			&& pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z)
		{// �E���獶�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z - m_vtxMin.z + vtxMax.z;
		}
	}
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
void CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPosOld->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPosOld->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPosOld->z + vtxMin.z - m_vtxMax.z <= m_pos.z && pPosOld->z + vtxMax.z - m_vtxMin.z >= m_pos.z
		&& pPosOld->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPosOld->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// �͈͓��ɂ���
		if (m_state != STATE_DAMAGE && m_bInvincible == false)
		{// �G�ɓ�������
			Death();
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
// �T�C�Y�̐ݒ�
//===============================================
void CPlayer::SetSize(D3DXVECTOR3 size)
{
	m_vtxMax = size;
}

//===============================================
// �T�C�Y�̐ݒ�
//===============================================
void CPlayer::SetSizeMin(D3DXVECTOR3 size)
{
	m_vtxMin = size;
}

//===============================================
// ��Ԃ̐ݒ�
//===============================================
void CPlayer::SetState(EState state)
{
	if (state != STATE_DAMAGE && m_state == STATE_NORMAL)
	{// �q�b�v�h���b�v���ȊO�Œn�`�֒��n
		m_move.y = 0.0f;
		SetJump(false);
		m_state = state;
	}

	if (m_state != STATE_DAMAGE && state == STATE_DAMAGE && m_bInvincible == false)
	{// �G�ɓ�������
		m_pMotion->Set(MOTIONTYPE_DAMAGE);
		m_nStateCounter = 30;
		m_move.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * m_rot.y)) * 10.0f;
		m_state = state;
	}

	if (state != STATE_DAMAGE)
	{
		m_state = state;
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		break;

	case STATE_DAMAGE:		// �_���[�W
		break;

	case STATE_INVINCIBLE:	// ���G
		break;

	case STATE_DEATH:		// ���S
		break;
	}
}

//===============================================
// ���[�V�����̐ݒ�
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}

//===============================================
// �e�̈ʒu�̐ݒ�
//===============================================
void CPlayer::SetPosShadow(void)
{
	float fPos = -500.0f;
	m_posShadow.y = fPos;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾
			D3DXVECTOR3 pos = pObject->GetPos();			// �ʒu
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// �O��̈ʒu
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// �ő�T�C�Y
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// �ŏ��T�C�Y

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// �n�`
				if (pos.x + sizeMin.x - m_vtxMax.x <= m_pos.x && pos.x + sizeMax.x - m_vtxMin.x >= m_pos.x
					&& pos.z + sizeMin.z <= m_pos.z - m_vtxMin.z && pos.z + sizeMax.z >= m_pos.z + m_vtxMin.z)
				{// �͈͓��ɂ���
					if (m_pos.y > pos.y)
					{// �v���C���[��艺�̈ʒu
						if (fPos <= pos.y)
						{
							fPos = pos.y;
							m_posShadow.y = pos.y + sizeMax.y + 10.0f;
						}
					}
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}
}

//===============================================
// �V���O���g��
//===============================================
CPlayer* CPlayer::GetInstance(void)
{
	if (m_pPlayer == nullptr)
	{
		return m_pPlayer = new CPlayer(m_nPriority);
	}
	else
	{
		return m_pPlayer;
	}
}