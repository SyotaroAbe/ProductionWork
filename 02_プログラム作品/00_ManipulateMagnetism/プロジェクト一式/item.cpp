//=========================================================
//
// �A�C�e������ [item.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "item.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "xfile.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "tutorial.h"
#include "manager.h"
#include "debugproc.h"

//===============================================
// �萔��`
//===============================================
namespace
{
	const float LENTH_PLAYER = 100.0f;		// �A�C�e�����������鋗��
	const float MAGNET_PLAYER = 20.0f;		// �A�C�e�����v���C���[�Ƃ���������
	const float ITEM_SPEED = 135.0f;			// �A�C�e������ԂƂ��̃X�s�[�h
}

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CItem::m_aIdxXFile[MODEL_MAX] = {};	// �g�p����X�t�@�C���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CItem::CItem() : CObjectX(3)
{
	// �l���N���A����
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CItem::CItem(int nPriority) : CObjectX(nPriority)
{
	// �l���N���A����
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// �f�X�g���N�^
//===============================================
CItem::~CItem()
{

}

//===============================================
// ��������
//===============================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CItem *pObjX;

	// �I�u�W�F�N�g�̐���
	pObjX = new CItem(nPriority);

	// ��ނ̐ݒ�
	pObjX->SetType(TYPE_ITEM);

	// ���f���̐ݒ�
	pObjX->SetModel(type);

	// ����������
	pObjX->Init(pos);

	// �����ݒ�
	pObjX->SetRot(rot);

	return pObjX;
}

//===============================================
// ����������
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	//// �e��ݒ�
	//m_nIdxShadow = SetShadow();

	// �ʒu�𔽉f
	m_pos = pos;

	CObjectX::Init(m_pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CItem::Uninit(void)
{
	CObjectX::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CItem::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̑O��̈ʒu
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̌���
	CObject *pObj = nullptr;

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
				pObj = pObject;
			}

			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		if (CGame::GetPlayer()->GetPos() == m_pos)
		{
			m_bMagnet = true;
		}
	}

	if (CollisionModel(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin) == true)
	{
		m_move.y = 0.0f;
	}

	CManager::GetInstance()->GetDebugProc()->Print(" �A�C�e���̈ʒu�F�i%f, %f, %f�j\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print(" �A�C�e���̑O��̈ʒu�F�i%f, %f, %f�j\n", m_posOld.x, m_posOld.y, m_posOld.z);
	CManager::GetInstance()->GetDebugProc()->Print(" �A�C�e���̈ړ��ʁF�i%f, %f, %f�j\n", m_move.x, m_move.y, m_move.z);

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// �����蔻��
	//CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);

	CObjectX::Update();
}

//===============================================
// �`�揈��
//===============================================
void CItem::Draw(void)
{
	int nIdxXFile = CObjectX::GetXFile(CObjectX::MODEL_ITEM);

	CObjectX::DrawXFile(nIdxXFile, CXFile::COL_RED);
}

//===============================================
// �`�揈��
//===============================================
void CItem::DrawXFile(int nIdx, CXFile::COL col)
{
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

	// X�t�@�C���̕`��
	CManager::GetInstance()->GetXFile()->Draw(nIdx, col);
}

//===============================================
// �v���C���[�Ƃ̓����蔻��
//===============================================
bool CItem::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;
	float fLenth = 0.0f;

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

			if (type == CObject::TYPE_ITEM)
			{
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - MAGNET_PLAYER <= pPos->z - vtxMin.z && pos.z + sizeMax.z + MAGNET_PLAYER >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - MAGNET_PLAYER <= pPos->y + vtxMax.y && pos.y + sizeMax.y + MAGNET_PLAYER >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
					{// ����
						if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_A) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) < 0)
						{// ���L�[�������ꂽ
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_DOWN) * ITEM_SPEED, cosf(D3DX_PI * ROT_DOWN) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLX(0) > 0)
						{// �E�L�[�������ꂽ
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP) * ITEM_SPEED, cosf(D3DX_PI * ROT_UP) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_W) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLY(0) < 0)
						{// ��L�[�������ꂽ
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_RIGHT) * ITEM_SPEED, cosf(D3DX_PI * ROT_RIGHT) * ITEM_SPEED));
						}
						else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_S) == true
							|| CManager::GetInstance()->GetInputGamePad()->GetJoyStickLY(0) > 0)
						{// ���L�[�������ꂽ
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_LEFT) * ITEM_SPEED, cosf(D3DX_PI * ROT_LEFT) * ITEM_SPEED));
						}
						else
						{// �v���C���[�̌����Ă������
							if (CManager::GetMode() == CScene::MODE_GAME)
							{
								pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP + (1.0f * CGame::GetPlayer()->GetRot().y)) * ITEM_SPEED, cosf(D3DX_PI * ROT_UP) * ITEM_SPEED));
							}
						}
					}
					else if((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
					{// ������
						pObject->SetPos(D3DXVECTOR3(pPos->x, pPos->y, pPos->z));
					}
				}
				else if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - LENTH_PLAYER <= pPos->z - vtxMin.z && pos.z + sizeMax.z + LENTH_PLAYER >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - LENTH_PLAYER <= pPos->y + vtxMax.y && pos.y + sizeMax.y + LENTH_PLAYER >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					CPlayer::EState state = CPlayer::STATE_NONE;

					if (CManager::GetMode() == CScene::MODE_GAME)
					{
						state = CGame::GetPlayer()->GetState();
					}
					else if (CManager::GetMode() == CScene::MODE_TUTORIAL)
					{
						state = CTutorial::GetPlayer()->GetState();
					}

					if (CManager::GetMode() == CScene::MODE_GAME)
					{
						// �v���C���[�ƃu���b�N�̋�����������v�Z���t�����ɔ�΂�
						D3DXVECTOR3 vecDiff;
						vecDiff.z = pos.z - pPos->z;
						vecDiff.y = pos.y - pPos->y;

						D3DXVec3Normalize(&vecDiff, &vecDiff);
						//vecDiff *= 0.08f;

						// ������ڕW�����ɕ␳
						float fAngleDist = atan2f(vecDiff.y, vecDiff.z);

						// �v���C���[�܂ł̋������v�Z
						D3DXVECTOR3 vecDiffDelete;
						vecDiffDelete.x = pos.x - pPos->x;
						vecDiffDelete.y = pos.y - pPos->y;
						vecDiffDelete.z = pos.z - pPos->z;
						fLenth = D3DXVec3Length(&vecDiffDelete);
						fLenth = 220.0f - fLenth;

						//CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMin.y + sizeMax.y, pPos->z));
						if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
							|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
						{// ����
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_UP + fAngleDist) * fLenth * 0.1f, cosf(D3DX_PI * ROT_UP + fAngleDist) * fLenth * 0.1f));
						}
						else if((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
							|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
						{// ������
							pObject->SetMove(D3DXVECTOR3(pObject->GetMove().x, sinf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f, cosf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f));
						}
					}
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
bool CItem::CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bDeath = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject* pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �g�p����Ă���
			CObject* pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾
			D3DXVECTOR3 pos = pObject->GetPos();			// �ʒu
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// �O��̈ʒu
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// �ő�T�C�Y
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// �ŏ��T�C�Y

			if (type == CObject::TYPE_ITEM)
			{
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					bDeath = true;
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bDeath;
}