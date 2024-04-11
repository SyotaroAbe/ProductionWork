//=========================================================
//
// �A�C�e������ [item.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "explosion.h"
#include "player.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "texture.h"
#include "game.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CItem::m_aIdxTexture[CItem::TYPE_MAX] = {};						// �g�p����e�N�X�`���̔ԍ�
const char *CItem::m_apTextureData[CItem::TYPE_MAX] = {};			// �e�N�X�`���t�@�C����
int CItem::m_nNumAll = 0;											// ����

//===============================================
// �}�N����`
//===============================================
#define ITEM_SIZEX		(35)				// ���i�����j
#define ITEM_SIZEY		(35)				// �����i�����j
#define TREASURE_SCORE	(1000)				// �󔠂̃X�R�A
#define COIN_SCORE		(500)				// �R�C���̃X�R�A

//===============================================
// �R���X�g���N�^
//===============================================
CItem::CItem() : CObject2D(4)
{
	// �l���N���A����
	m_type = TYPE_TREASURE;

	m_nNumAll++;		// �������C���N�������g
}

//===============================================
// �f�X�g���N�^
//===============================================
CItem::~CItem()
{
	m_nNumAll--;		// �������f�N�������g
}

//===============================================
// ��������
//===============================================
CItem *CItem::Create(D3DXVECTOR3 pos, int nPriority)
{
	CItem *pItem;

	// �A�C�e���̐���
	pItem = new CItem;

	// ��ނ̐ݒ�
	pItem->SetType(TYPE_ITEM);

	// �T�C�Y�̐ݒ�
	pItem->SetSize(ITEM_SIZEX, ITEM_SIZEY);

	// ����������
	pItem->Init(pos);

	return pItem;
}

//===============================================
// ����������
//===============================================
HRESULT CItem::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���t�@�C����
	const char *m_apTextureData[TYPE_MAX] =
	{
		{ "data\\TEXTURE\\item000.png" },	// ��
		{ "data\\TEXTURE\\item001.png" }	// ����
	};

	// �e�N�X�`���̐ݒ�
	for (int nCntItem = 0; nCntItem < TYPE_MAX; nCntItem++)
	{
		m_aIdxTexture[TYPE_TREASURE] = CManager::GetTexture()->Regist(m_apTextureData[nCntItem]);
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CItem::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CItem::Update(void)
{
	// �v���C���[�Ƃ̓����蔻��
	if (CollisionEnemy(m_pos) == false)
	{// �A�C�e�����������ĂȂ�
		if (m_pos.x < 0.0f || m_pos.x > SCREEN_WIDTH || m_pos.y < 0.0f || m_pos.y > SCREEN_HEIGHT)
		{// �A�C�e���̈ʒu����ʊO�ɏo���ꍇ
			CExplosion::Create(m_pos, 3)->SetPos(m_pos);	// �����̐���
			Uninit();				// �����̏I������
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CItem::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CItem::Set(const TYPE type, const D3DXVECTOR3 pos)
{
	// �ݒ肳�ꂽ�l����
	m_type = type;
	m_pos = pos;

	// �e�N�X�`���̊��蓖��
	CObject2D::BindTexture(m_aIdxTexture[m_type]);

	// �ʒu�̍X�V����
	CObject2D::UpdatePos(m_pos, ITEM_SIZEX, ITEM_SIZEY);
}

//===============================================
// �v���C���[�Ƃ̓����蔻��
//===============================================
bool CItem::CollisionEnemy(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObj = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while(pObj != NULL)
		{// �g�p����Ă���			// ��ނ��擾

			CObject::TYPE type = pObj->GetType();	// ��ނ��擾

			if (type == TYPE_PLAYER)
			{// �v���C���[
				D3DXVECTOR3 PlayerPos = pObj->GetPos();	// �ʒu���擾

				if (pos.x >= PlayerPos.x - PLAYER_SIZEX - ITEM_SIZEX
					&& pos.x <= PlayerPos.x + PLAYER_SIZEX + ITEM_SIZEX
					&& pos.y <= PlayerPos.y + PLAYER_SIZEX + ITEM_SIZEX
					&& pos.y >= PlayerPos.y - PLAYER_SIZEX - ITEM_SIZEX)
				{// �v���C���[�ƃA�C�e������������
					//CParticle::Create(1)->Set(m_pos, CParticle::TYPE_ITEM);		// �p�[�e�B�N���̐���

					// �X�R�A���Z
					switch (m_type)
					{
					case TYPE_TREASURE:		// ��
						CGame::GetScore()->Add(TREASURE_SCORE);
						break;

					case TYPE_COIN:			// �R�C��
						CGame::GetScore()->Add(COIN_SCORE);
						break;
					}

					this->Uninit();			// �����̏I������

					//// �T�E���h�̍Đ�
					//CManager::GetSound()->Play(CSound::LABEL_SE_ITEM);
					return true;
				}
			}
		}
	}
	return false;
}