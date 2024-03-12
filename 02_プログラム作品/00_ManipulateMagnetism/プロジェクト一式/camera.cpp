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
#define TRUE_LENTH			(3.0f)		// �␳�����l

//===============================================
// �R���X�g���N�^
//===============================================
CCamera::CCamera()
{
	// �l�̃N���A
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_nViewMapCounter = 0;
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
	m_posV = D3DXVECTOR3(1000.0f, 600.0f, 100.0f);
	m_posR = D3DXVECTOR3(0.0f, 350.0f, 100.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_fLength = LENTH_NORMAL;
	m_nViewMapCounter = MAPVIEW_TIME;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{// �`���[�g���A���p�J����
		m_posV = D3DXVECTOR3(666.6f, 1100.0f, 0.0f);
		m_posR = D3DXVECTOR3(0.0f, 750.0f, 0.0f);
	}

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

		while (pObject != nullptr)
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

	if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_Y) == true)
	{
		// ��ړ�
		m_posV.y += MOVE_CAMERA;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_N) == true)
	{
		// ���ړ�
		m_posV.y -= MOVE_CAMERA;
	}

	// �����_����
	if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_T) == true)
	{
		// ��ړ�
		m_posR.y += MOVE_CAMERA;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_B) == true)
	{
		// ���ړ�
		m_posR.y -= MOVE_CAMERA;
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_Y) == true)
	{
		// ��ړ�
		m_posV.y += MOVE_CAMERA;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_N) == true)
	{
		// ���ړ�
		m_posV.y -= MOVE_CAMERA;
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_Q) == true)
	{
		// ��ړ�
		m_rot.y += MOVE_CAMERA;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_E) == true)
	{
		// ���ړ�
		m_rot.y -= MOVE_CAMERA;
	}

	// �J�����ʒu���Z�b�g
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_F2) == true)
	{
		m_posV.y = HIGHT_CAMERA;
		m_posR.y = 0.0f;
	}

	//if (CManager::GetMode() != CScene::MODE_TUTORIAL)
	//{
	//	// �J�����̒Ǐ]
	//	// �ړI�̎��_�E�����_��ݒ�
	//	m_posRDest.x = playerPos.x;
	//	m_posRDest.y = playerPos.y + 400.0f;
	//	m_posRDest.z = playerPos.z;
	//	m_posVDest.x = playerPos.x;
	//	m_posVDest.y = playerPos.y + 400.0f;
	//	m_posVDest.z = playerPos.z;

	//	// �ړ��ʂ��X�V�i����������j
	//	m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
	//	m_posV.y += (m_posVDest.y - m_posV.y) * MOVEV_MINUS;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
	//	m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
	//	m_posR.y += (m_posRDest.y - m_posV.y) * MOVER_MINUS;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;
	//}
	//else
	//{
	//	// �J�����̒Ǐ]
	//	// �ړI�̎��_�E�����_��ݒ�
	//	m_posVDest.x = playerPos.x;
	//	m_posVDest.z = playerPos.z;

	//	// �ړ��ʂ��X�V�i����������j
	//	m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
	//}

	////�����_�ƈꏏ�Ɏ��_��������
	//m_posV.x = m_posV.x + m_fLength;
	//m_posV.z = m_posV.z;

	if (m_posR.z <= 0.0f)
	{
		m_posV.z = 0.0f;
		m_posR.z = 0.0f;
	}

	if (m_posR.z >= 100.0f)
	{
		m_posV.z = 100.0f;
		m_posR.z = 100.0f;
	}
}

//===============================================
// �g�k����
//===============================================
bool CCamera::ScalingLenth(const float fLenth, float fMalti)
{
	// �ړI�̃T�C�Y�܂ł̍������v�Z
	float fLenthDiff = fLenth - m_fLength;

	// �T�C�Y�̕␳
	m_fLength += fLenthDiff * fMalti;

	if (m_fLength <= fLenth + TRUE_LENTH && m_fLength >= fLenth - TRUE_LENTH)
	{
		return true;
	}

	return false;
}

//===============================================
// �ݒ菈��
//===============================================
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	// ���s���e
	if (CManager::GetMode() != CScene::MODE_TUTORIAL)
	{
		D3DXMatrixOrthoLH(&m_mtxProjection,
			(float)SCREEN_WIDTH * 1.2f,
			(float)SCREEN_HEIGHT * 1.2f,
			10.0f,
			10000.0f);
	}
	else /*if(CManager::GetMode() == CScene::MODE_GAME && CGame::GetPauseCamera() == false)*/
	{
		D3DXMatrixOrthoLH(&m_mtxProjection,
			(float)SCREEN_WIDTH,
			(float)SCREEN_HEIGHT,
			10.0f,
			10000.0f);
	}
	//else
	//{
	//	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
	//		D3DXToRadian(45.0f),
	//		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	//		10.0f,
	//		8000.0f);
	//}

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