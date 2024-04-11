//=========================================================
//
// �u���b�N���� [block.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "explosion.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CBlock::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�
int CBlock::m_nNumAll = 0;							// ����

//===============================================
// �R���X�g���N�^
//===============================================
CBlock::CBlock() : CObject2D(3)
{
	m_nNumAll++;	// �������C���N�������g
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CBlock::CBlock(int nPriority) : CObject2D(nPriority)
{
	m_nNumAll++;	// �������C���N�������g
}

//===============================================
// �f�X�g���N�^
//===============================================
CBlock::~CBlock()
{
	m_nNumAll--;	// �������f�N�������g
}

//===============================================
// ��������
//===============================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, int nPriority)
{
	CBlock *pBlock;

	// �u���b�N�̐���
	pBlock = new CBlock(3);

	// ��ނ̐ݒ�
	pBlock->SetType(TYPE_BLOCK);

	// �T�C�Y�̐ݒ�
	pBlock->SetSize(BLOCK_SIZEX, BLOCK_SIZEY);

	// ����������
	pBlock->Init(pos);

	// �e�N�X�`���̊��蓖��
	pBlock->BindTexture(m_nIdxTexture);

	return pBlock;
}

//===============================================
// ����������
//===============================================
HRESULT CBlock::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\block000.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CBlock::Update(void)
{
	// �O��̏��̕ۑ�
	m_posOld = m_pos;

	// �ʒu�̍X�V
	UpdatePos(m_pos, BLOCK_SIZEX, BLOCK_SIZEY);
}

//===============================================
// �`�揈��
//===============================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CBlock::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;
}

//===============================================
// �v���C���[�Ƃ̓����蔻��
//===============================================
bool CBlock::CollisionPlayer(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pBlock = CObject::GetTop(nCntPriority);	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^���擾

		while (pBlock != NULL)
		{// �g�p����Ă���
			CObject *pBlockNext = pBlock->GetNext();	// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			TYPE typeBlock = pBlock->GetType();			// ��ނ��擾

			if (typeBlock == TYPE_BLOCK)
			{// ��ނ��u���b�N�̏ꍇ
				for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
				{
					CObject *pObject = CObject::GetTop(nCntPriority);	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^���擾

					while (pObject != NULL)
					{// �g�p����Ă���
						CObject *pObjectNext = pObject->GetNext();	// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
						TYPE type = pObject->GetType();				// ��ނ��擾

						if (type == TYPE_PLAYER)
						{// ��ނ��v���C���[�̏ꍇ
							D3DXVECTOR3 BlockPos = pBlock->GetPos();			// �ʒu���擾
							D3DXVECTOR3 BlockPosOld = pBlock->GetPosOld();		// �O��̈ʒu���擾

							if (pos->x >= BlockPos.x - fSezeX
								&& pos->x <= BlockPos.x + BLOCK_SIZEX + fSezeX
								&& pos->y <= BlockPos.y + BLOCK_SIZEY + fSezeY
								&& pos->y >= BlockPos.y)
							{//	���݂̈ʒu���u���b�N�͈͓̔�

								if (posOld->y <= BlockPosOld.y && pos->y >= BlockPos.y)
								{// �u���b�N�̏ォ��߂荞��
									bLand = true;
									pObject->SetPos(D3DXVECTOR3(pos->x, BlockPos.y, 0.0f));		// �u���b�N�̏�ɂ̂���
									pObject->SetMove(D3DXVECTOR3(move->x, 0.0f, 0.0f));			// �ړ��ʂ�����������
								}
								else if (posOld->y - fSezeY >= BlockPosOld.y + BLOCK_SIZEY
									&& pos->y - fSezeY <= BlockPos.y + BLOCK_SIZEY)
								{//�u���b�N�̉�����߂荞��
									pObject->SetPos(D3DXVECTOR3(pos->x, BlockPos.y + BLOCK_SIZEY + fSezeY, 0.0f));	// �u���b�N�̉��Ɉړ�������
									pObject->SetMove(D3DXVECTOR3(move->x, 0.0f, 0.0f));								// �ړ��ʂ�����������
								}
								else if (posOld->y >= BlockPosOld.y
									|| posOld->y - fSezeY <= BlockPosOld.y + BLOCK_SIZEY)
								{// �u���b�N�̉�����߂荞��

									if (posOld->x - fSezeX * 0.85f >= BlockPosOld.x + BLOCK_SIZEX * 0.85f
										&& pos->x - fSezeX * 0.85f <= BlockPos.x + BLOCK_SIZEX)
									{// �E����߂荞��
										pObject->SetMove(D3DXVECTOR3(0.0f, move->y, 0.0f));
										pObject->SetPos(D3DXVECTOR3(BlockPos.x + BLOCK_SIZEX + fSezeX * 0.85f, pos->y, 0.0f));	// �u���b�N�̉E�Ɉړ�������
									}
									else if (posOld->x + fSezeX * 0.85f <= BlockPosOld.x
										&& pos->x + fSezeX * 0.85f >= BlockPos.x)
									{// ������߂荞��
										pObject->SetMove(D3DXVECTOR3(0.0f, move->y, 0.0f));
										pObject->SetPos(D3DXVECTOR3(BlockPos.x - fSezeX * 0.85f, pos->y, 0.0f));	// �u���b�N�̍��Ɉړ�������
									}
								}
							}
						}

						pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
					}
				}
			}

			pBlock = pBlockNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}

//===============================================
// �u���b�N�Ƃ̓����蔻��
//===============================================
bool CBlock::Collision(const TYPE Objtype, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pBlock = CObject::GetTop(nCntPriority);	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^���擾

		while (pBlock != NULL)
		{// �g�p����Ă���
			CObject *pBlockNext = pBlock->GetNext();	// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			TYPE typeBlock = pBlock->GetType();			// ��ނ��擾

			if (typeBlock == TYPE_BLOCK)
			{// �u���b�N�̏ꍇ
				D3DXVECTOR3 BlockPos = pBlock->GetPos();			// �ʒu���擾
				D3DXVECTOR3 BlockPosOld = pBlock->GetPosOld();		// �O��̈ʒu���擾

				if (pos->x >= BlockPos.x - fSezeX
					&& pos->x <= BlockPos.x + BLOCK_SIZEX + fSezeX
					&& pos->y <= BlockPos.y + BLOCK_SIZEY + fSezeY
					&& pos->y >= BlockPos.y - fSezeY)
				{//���݂̈ʒu���u���b�N�͈͓̔�
					bLand = true;
				}
			}

			pBlock = pBlockNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}