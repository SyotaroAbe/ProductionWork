//=========================================================
//
// ���΃u���b�N���� [magnet.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "magnet.h"
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
#include "particle.h"

//===============================================
// �萔��`
//===============================================
namespace
{
	const float LENTH_MAGNET = 80.0f;		// ���΂��������鋗��
}

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CMagnet::m_aIdxXFile[MODEL_MAX] = {};	// �g�p����X�t�@�C���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CMagnet::CMagnet() : CObjectX(3)
{
	// �l���N���A����
	m_state = STATE_NONE;
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
CMagnet::CMagnet(int nPriority) : CObjectX(nPriority)
{
	// �l���N���A����
	m_state = STATE_NONE;
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
CMagnet::~CMagnet()
{

}

//===============================================
// ��������
//===============================================
CMagnet *CMagnet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CMagnet *pObjX;

	// �I�u�W�F�N�g�̐���
	pObjX = new CMagnet(nPriority);

	// ��ނ̐ݒ�
	if (type == MODEL_DAMAGE)
	{
		pObjX->SetType(TYPE_BOXDAMAGE);
	}
	else
	{
		pObjX->SetType(TYPE_BOXNORMAL);
	}

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
HRESULT CMagnet::Init(D3DXVECTOR3 pos)
{
	//// �e��ݒ�
	//m_nIdxShadow = SetShadow();

	// �ʒu�𔽉f
	m_pos = pos;

	m_state = STATE_N;

	CObjectX::Init(m_pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CMagnet::Uninit(void)
{
	CObjectX::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CMagnet::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̑O��̈ʒu
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̌���
	CObject *pObj = nullptr;

	m_posOld = m_pos;

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

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// �����蔻��
	//pPlayer->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// �`�揈��
//===============================================
void CMagnet::Draw(void)
{
	CObjectX::Draw();
}

//===============================================
// �`�揈��
//===============================================
void CMagnet::DrawXFile(int nIdx, CXFile::COL col)
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
bool CMagnet::CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;
	float fLenth = 0.0f;
	float fAngleDist = 0.0f;
	D3DXVECTOR3 vecDiff = {};

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

			if (type == CObject::TYPE_BOXDAMAGE)
			{// �v���C���[
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z - LENTH_MAGNET <= pPos->z - vtxMin.z && pos.z + sizeMax.z + LENTH_MAGNET >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y - LENTH_MAGNET <= pPos->y + vtxMax.y && pos.y + sizeMax.y + LENTH_MAGNET >= pPos->y + vtxMin.y)
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

					// �v���C���[�ƃu���b�N�̋�����������v�Z���t�����ɔ�΂�
					vecDiff.z = pPos->z - pos.z;
					vecDiff.y = pPos->y - pos.y;

					D3DXVec3Normalize(&vecDiff, &vecDiff);
					//vecDiff *= 0.08f;

					// ������ڕW�����ɕ␳
					fAngleDist = atan2f(vecDiff.y, vecDiff.z);

					// �v���C���[�܂ł̋������v�Z
					vecDiff.x = pPos->x - pos.x;
					vecDiff.y = pPos->y - pos.y;
					vecDiff.z = pPos->z - pos.z;
					fLenth = D3DXVec3Length(&vecDiff);
					//fLenth = 200.0f - fLenth;

					if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_N))
					{// ����
						if (pos.z + sizeMin.z <= pPos->z
							&& pos.z + sizeMax.z >= pPos->z)
						{// �㉺����
							pMove->y += vecDiff.y * 0.02f;
						}
						else if (pos.z + sizeMax.z >= pPos->z || pos.z + sizeMin.z <= pPos->z)
						{// �E����������͍�����
							pMove->y += vecDiff.y * 0.04f;
							pMove->z += vecDiff.z * 0.04f;
						}
						CParticle::Create()->Set({ pPos->x, pPos->y, pPos->z }, CParticle::TYPE_ENEMY);
					}
					else if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S)
						|| (CManager::GetMode() == CScene::MODE_TUTORIAL && CTutorial::GetPlayer()->GetMagnet() == CPlayer::EMAGNET_S))
					{// ������
						if ((pos.z + sizeMin.z <= pPos->z && pos.z + sizeMax.z >= pPos->z)
							|| pos.z + sizeMax.z >= pPos->z || pos.z + sizeMin.z <= pPos->z)
						{// �㉺����܂��͉E����������͍�����
							pMove->y = sinf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f;
							pMove->z = cosf(D3DX_PI * ROT_DOWN + fAngleDist) * fLenth * 0.1f;
						}
					}

					// �ړ��ʂ̏���ݒ�
					if (pMove->y >= 25.0f)
					{
						pMove->y = 25.0f;
					}
					if (pMove->z >= 25.0f)
					{
						pMove->z = 25.0f;
					}
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ړ������F%f\n", fLenth);

	return bLand;
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
bool CMagnet::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

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
			{// �v���C���[
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// �ォ��߂荞��
						// ��ɂ̂���
						pPos->y = posOld.y - vtxMin.y + sizeMax.y;
						pMove->y = 0.0f;

						bLand = true;
					}
					else if (posOld.y + sizeMin.y >= pPosOld->y + vtxMax.y
						&& pos.y + sizeMin.y <= pPos->y + vtxMax.y)
					{// ������߂荞��
						// ���ɖ߂�
						pPos->y = posOld.y - vtxMax.y + sizeMin.y;
						//pPlayer->SetMove(D3DXVECTOR3(pPlayer->GetMove().x, 0.0f, pPlayer->GetMove().z));
					}
					else if (posOld.z + sizeMin.z >= pPosOld->z - vtxMin.z
						&& pos.z + sizeMin.z <= pPos->z - vtxMin.z)
					{// ������E�ɂ߂荞��
						// �ʒu��߂�
						pPos->z = posOld.z + vtxMin.z + sizeMin.z;
						pRot->y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
					else if (posOld.z + sizeMax.z <= pPosOld->z + vtxMin.z
						&& pos.z + sizeMax.z >= pPos->z + vtxMin.z)
					{// �E���獶�ɂ߂荞��
						// �ʒu��߂�
						pPos->z = posOld.z - vtxMin.z + sizeMax.z;
						pRot->y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}