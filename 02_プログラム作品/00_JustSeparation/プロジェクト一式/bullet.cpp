//=========================================================
//
// �e���� [bullet.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "block.h"
#include "objectBillboard.h"
#include "texture.h"
#include "game.h"

//===============================================
// �}�N����`
//===============================================
#define BULLET_SIZEX		(8.0f)			// ���i�����j
#define BULLET_SIZEY		(8.0f)			// �����i�����j
#define MOVE_BULLET			(5.0f)			// �������x
#define BULLET_LIFE			(150)			// ���C�t

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CBullet::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�
int CBullet::m_nNumAll = 0;							// ����

//===============================================
// �R���X�g���N�^
//===============================================
CBullet::CBullet() : CObjectBillboard(2)
{
	// �l���N���A
	m_nLife = 0;

	m_nNumAll++;	// �������C���N�������g
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CBullet::CBullet(int nPriority) : CObjectBillboard(nPriority)
{
	// �l���N���A
	m_nLife = 0;

	m_nNumAll++;	// �������C���N�������g
}

//===============================================
// �f�X�g���N�^
//===============================================
CBullet::~CBullet()
{
	m_nNumAll--;	// �������f�N�������g
}

//===============================================
// ��������
//===============================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, int nPriority)
{
	CBullet *pBullet;

	// �e�̐���
	pBullet = new CBullet(2);

	// ��ނ̐ݒ�
	pBullet->SetType(TYPE_BULLET);

	// �T�C�Y�̐ݒ�
	pBullet->SetSize(BULLET_SIZEX, BULLET_SIZEY);

	// ����������
	pBullet->Init(pos);

	// �e�N�X�`���̊��蓖��
	pBullet->BindTexture(m_nIdxTexture);

	return pBullet;
}

//===============================================
// ����������
//===============================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObjectBillboard::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\bullet001.png");

	// �̗͂̐ݒ�
	m_nLife = BULLET_LIFE;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CBullet::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObjectBillboard::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CBullet::Update(void)
{
	////�G�Ƃ̓����蔻��
	//if (CollisionEnemy(m_pos) == false)
	//{//�e���������ĂȂ�
	//	if (CManager::GetBlock()->Collision(GetType(), &m_pos, &m_posOld, &m_move, BULLET_SIZEX, BULLET_SIZEX) == false)
	//	{// �u���b�N�ɓ������Ă��Ȃ�
			if (m_nLife <= 0)
			{//�e�̈ʒu����ʊO�ɏo���ꍇ
				//CExplosion::Create(m_pos, 3)->SetPos(m_pos);	//�����̐���
				Uninit();				//�����̏I������

				////�T�E���h�̍Đ�
				//CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
			}
			else
			{
				// �O��̈ʒu��ۑ�
				m_posOld = m_pos;

				//�ʒu���X�V
				m_pos.x += m_move.x;
				m_pos.z += m_move.z;

				m_nLife--;	//�̗͂����炷
			}
		//}
		//else
		//{// �e����������
		//	CExplosion::Create(3)->SetPos(m_pos);	//�����̐���
		//	Uninit();				//�����̏I������
	//	}
	//}
}

//===============================================
// �`�揈��
//===============================================
void CBullet::Draw(void)
{
	CObjectBillboard::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CBullet::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;

	//// �T�E���h�̍Đ�
	//CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
bool CBullet::CollisionEnemy(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while(pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g
			TYPE type = pObject->GetType();					// ��ނ��擾

			if (type == TYPE_ENEMY)
			{// �G
				D3DXVECTOR3 EnemyPos = pObject->GetPos();	// �ʒu���擾

				if (pos.x >= EnemyPos.x - ENEMY_SIZEX - BULLET_SIZEX
					&& pos.x <= EnemyPos.x + ENEMY_SIZEX + BULLET_SIZEX
					&& pos.y <= EnemyPos.y + ENEMY_SIZEX + BULLET_SIZEX
					&& pos.y >= EnemyPos.y - ENEMY_SIZEX - BULLET_SIZEX)
				{// �G�ƒe����������
					CParticle::Create(1)->Set(pObject->GetPos(), CParticle::TYPE_ENEMY);	// �p�[�e�B�N���̐���
					pObject->Uninit();														// �G�̏I������
					CGame::GetScore()->Add(ENEMY_SCORE);									// �X�R�A���Z
					Uninit();																// �����̏I������

					//// �T�E���h�̍Đ�
					//CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION_HIT);
					return true;
				}
			}

			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}
	return false;
}