//=========================================================
//
// �J�������� [camera.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "object.h"
#include "game.h"
#include "debugproc.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE_CAMERA			(2.0f)		// �ړ���
#define RAD_CAMERA			(60.0f)		// ��{������̉�]�p�x
#define SLANTING			(0.25f)		// �΂߂̊p�x
#define MOVE_X				(0.5f)		// X�����ւ̈ړ�
#define PI_SLASH			(100.0f)	// IP�����鐔
#define LENGTH_CAMERA		(-300.0f)	// ���_�̋���
#define HIGHT_CAMERA		(250.0f)	// �J�����̍���
#define MOVEV_MINUS			(0.3f)		// ���_�̈ړ��ʂ̌���
#define MOVER_MINUS			(0.2f)		// �����_�̈ړ��ʂ̌���
#define POSR_DEST			(-300.0f)	// �����_�̏����O�ɐݒ�
#define CAMERA_ROT_SPEED	(0.7f)		// �J�������v���C���[�̔w��ɉ�荞�ނ܂ł̃X�s�[�h
#define CAMERA_ROT_MOVE		(70)		// �������ς��^�C�~���O
#define POS_DIFF			(0.15f)		// �ʒu�̒l�̕␳
#define MAPVIEW_TIME		(80)		// ���J�����ɂȂ��Ă��鎞��

//===============================================
// �R���X�g���N�^
//===============================================
CCamera::CCamera()
{
	// �l�̃N���A
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nViewMapCounter = 0;
	m_bMapCamera = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CCamera::~CCamera()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CCamera::Init(void)
{
	// �l��������
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		m_posV = D3DXVECTOR3(-2500.0f, 7400.0f, -1800.0f);
		m_posR = D3DXVECTOR3(-2500.0f, 0.0f, -2500.0f);
	}
	else if (CScene::GetMode() == CScene::MODE_TUTORIAL || CScene::GetMode() == CScene::MODE_TITLE)
	{
		m_posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
		m_posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
	}

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_nViewMapCounter = MAPVIEW_TIME;
	m_bMapCamera = true;

	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�����̒Ǐ]
	// �ړI�̎��_�E�����_��ݒ�
	m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
	m_posRDest.y = playerPos.y;
	m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
	m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
	m_posVDest.y = playerPos.y;
	m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CCamera::Uninit(void)
{
	
}

//===============================================
// �X�V����
//===============================================
void CCamera::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̑O��̈ʒu
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̌���

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while(pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾

			if (type == CObject::TYPE_PLAYER)
			{// �v���C���[
				playerPos = pObject->GetPos();			// �ʒu���擾
				playerPosOld = pObject->GetPosOld();	// �O��̈ʒu���擾
				playerRot = pObject->GetRot();			// �������擾
			}

			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetStateReady() == true && CGame::GetPauseState() == false)
		{//	�ҋ@���
			if (m_nViewMapCounter < 0)
			{// �J�E���^���c���Ă���
				D3DXVECTOR3 posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
				D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);

				// �ړI�̈ʒu�܂ł̍������v�Z
				D3DXVECTOR3 fPosVDest = posV - m_posV;
				D3DXVECTOR3 fPosRDest = posR - m_posR;

				// �ʒu�̕␳
				m_posV += fPosVDest* POS_DIFF;
				m_posR += fPosRDest* POS_DIFF;
			}
			else
			{
				m_nViewMapCounter--;	// �J�E���^�����炷
			}
		}
		else
		{//	�ҋ@��Ԃ���Ȃ�
			if (m_bMapCamera == true)
			{// �}�b�v�J����
				D3DXVECTOR3 posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
				D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
				m_bMapCamera = false;
			}

			if (CGame::GetPauseState() == true && CGame::GetPauseCamera() == true)
			{// �|�[�Y���
				// ���_����
				if (CManager::GetKeyboardInput()->GetPress(DIK_Y) == true)
				{
					// ��ړ�
					m_posV.y += MOVE_CAMERA;
				}
				else if (CManager::GetKeyboardInput()->GetPress(DIK_N) == true)
				{
					// ���ړ�
					m_posV.y -= MOVE_CAMERA;
				}

				// �����_����
				if (CManager::GetKeyboardInput()->GetPress(DIK_T) == true)
				{
					// ��ړ�
					m_posR.y += MOVE_CAMERA;
				}
				else if (CManager::GetKeyboardInput()->GetPress(DIK_B) == true)
				{
					// ���ړ�
					m_posR.y -= MOVE_CAMERA;
				}

				// �J�����ʒu���Z�b�g
				if (CManager::GetKeyboardInput()->GetTrigger(DIK_F2) == true)
				{
					m_posV.y = HIGHT_CAMERA;
					m_posR.y = 0.0f;
				}
			}

			if (CGame::GetPauseState() == false)
			{
				// �J�����̒Ǐ]
				// �ړI�̎��_�E�����_��ݒ�
				m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
				m_posRDest.y = playerPos.y;
				m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
				m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
				m_posVDest.y = playerPos.y;
				m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

				// �ړ��ʂ��X�V�i����������j
				m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
				m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
				m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
				m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

				m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

				m_rotDest.y -= m_rot.y;		// �ړI�̌����܂ł̍���

				// �p�x�̒l�̕␳
				if (m_rotDest.y > D3DX_PI)
				{
					m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
				}
				else if (m_rotDest.y < -D3DX_PI)
				{
					m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
				}

				m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// �p�x�̕␳

				// �p�x�̒l�̕␳
				if (m_rot.y > D3DX_PI)
				{
					m_rot.y += -D3DX_PI * ROT_DIFF_PI;
				}
				else if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * ROT_DIFF_PI;
				}
			}
		}
	}
	else if (CScene::GetMode() == CScene::MODE_TUTORIAL)
	{
		// �J�����̒Ǐ]
		// �ړI�̎��_�E�����_��ݒ�
		m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
		m_posRDest.y = playerPos.y;
		m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
		m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
		m_posVDest.y = playerPos.y;
		m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

		// �ړ��ʂ��X�V�i����������j
		m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
		m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
		m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
		m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

		m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

		m_rotDest.y -= m_rot.y;		// �ړI�̌����܂ł̍���

		// �p�x�̒l�̕␳
		if (m_rotDest.y > D3DX_PI)
		{
			m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{
			m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
		}

		m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// �p�x�̕␳

		// �p�x�̒l�̕␳
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * ROT_DIFF_PI;
		}
	}
	if (CScene::GetMode() == CScene::MODE_TITLE)
	{
		// �J�����̒Ǐ]
		// �ړI�̎��_�E�����_��ݒ�
		m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
		m_posRDest.y = playerPos.y;
		m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
		m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
		m_posVDest.y = playerPos.y;
		m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

		// �ړ��ʂ��X�V�i����������j
		m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
		m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
		m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
		m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

		m_rotDest.y = D3DX_PI + (ROT_CAMERA * playerRot.y);

		m_rotDest.y -= m_rot.y;		// �ړI�̌����܂ł̍���

									// �p�x�̒l�̕␳
		if (m_rotDest.y > D3DX_PI)
		{
			m_rotDest.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rotDest.y < -D3DX_PI)
		{
			m_rotDest.y += D3DX_PI * ROT_DIFF_PI;
		}

		m_rot.y += m_rotDest.y * CAMERA_ROT_SPEED;	// �p�x�̕␳

													// �p�x�̒l�̕␳
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += -D3DX_PI * ROT_DIFF_PI;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * ROT_DIFF_PI;
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		8000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,
						&m_posR,
						&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}