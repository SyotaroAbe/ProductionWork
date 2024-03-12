//=========================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "game.h"

//=============================
// �ÓI�����o�ϐ� 
//=============================
CObject *CObject::m_apTop[PRIORITY_MAX] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_apCur[PRIORITY_MAX] = {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll = 0;						// �I�u�W�F�N�g�̑���

//===============================================
// �R���X�g���N�^�i�f�t�H���g�j
//===============================================
CObject::CObject()
{
	m_nPriority = 3;	// �`�揇��ݒ�

	// �l���N���A
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_type = TYPE_NONE;
	m_pShadow = nullptr;
	m_bDeath = false;

	if (m_apTop[m_nPriority] == nullptr)
	{// �擪�̃I�u�W�F�N�g�����݂��Ȃ�
		m_apTop[m_nPriority] = this;		// �擪�̃I�u�W�F�N�g�Ɏ�������
	}

	if (m_apCur[m_nPriority] != nullptr)
	{// �Ō���̃I�u�W�F�N�g���ۑ�����Ă���
		m_pPrev = m_apCur[m_nPriority];			// �O��̍Ō���̃I�u�W�F�N�g��O�̃I�u�W�F�N�g�̃|�C���^�ɕۑ�
		m_pPrev->m_pNext = this;				// �O��̍Ō���̎��̃I�u�W�F�N�g�Ɏ��g��ۑ�
	}
	m_apCur[m_nPriority] = this;				// �Ō���̃I�u�W�F�N�g�Ɏ�������

	m_nNumAll++;		// �������J�E���g�A�b�v
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;	// �`�揇��ݒ�

	// �l���N���A
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_type = TYPE_NONE;
	m_pShadow = nullptr;
	m_bDeath = false;

	if (m_apTop[m_nPriority] == nullptr)
	{// �擪�̃I�u�W�F�N�g�����݂��Ȃ�
		m_apTop[m_nPriority] = this;		// �擪�̃I�u�W�F�N�g�Ɏ�������
	}

	if (m_apCur[m_nPriority] != nullptr)
	{// �Ō���̃I�u�W�F�N�g���ۑ�����Ă���
		m_pPrev = m_apCur[m_nPriority];			// �O��̍Ō���̃I�u�W�F�N�g��O�̃I�u�W�F�N�g�̃|�C���^�ɕۑ�
		m_pPrev->m_pNext = this;				// �O��̍Ō���̎��̃I�u�W�F�N�g�Ɏ��g��ۑ�
	}
	m_apCur[m_nPriority] = this;				// �Ō���̃I�u�W�F�N�g�Ɏ�������

	m_nNumAll++;		// �������J�E���g�A�b�v
}

//===============================================
// �f�X�g���N�^
//===============================================
CObject::~CObject()
{

}

//===============================================
// �S�ẴI�u�W�F�N�g�̔j��
//===============================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			// �I������
			pObject->Uninit();

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			if (pObject->m_bDeath == true)
			{// ���S�t���O�������Ă���
				// ���X�g����폜���Ĕj������
				if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�ł��Ō���ł��Ȃ�
					pObject->m_pPrev->m_pNext = pObject->m_pNext;		// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�̃|�C���^�Ɏ��̃I�u�W�F�N�g��ۑ�����
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;		// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g�̃|�C���^�ɑO�̃I�u�W�F�N�g��ۑ�����
				}
				else if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] == pObject)
				{// ���g���擪�ł͂Ȃ����Ō���ł���
					m_apCur[nCntPriority] = pObject->m_pPrev;		// �Ō����O�̃I�u�W�F�N�g�ɐݒ肷��
				}
				else if (m_apTop[nCntPriority] == pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�����Ō���ł͂Ȃ�
					m_apTop[nCntPriority] = pObject->m_pNext;		// �擪�����̃I�u�W�F�N�g�ɐݒ肷��
				}

				if (pObject == m_apTop[nCntPriority])
				{// �擪�ƈ�v
					m_apTop[nCntPriority] = nullptr;
				}
				if (pObject == m_apCur[nCntPriority])
				{// �Ō���ƈ�v
					m_apCur[nCntPriority] = nullptr;
				}

				// ���������J��
				delete pObject;
				pObject = nullptr;
			}

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}
}

//===============================================
// ���Z�b�g����
//===============================================
void CObject::Reset()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�
			TYPE type = pObject->GetType();				// �I�u�W�F�N�g�̎�ނ��擾

			if (type == TYPE_ENEMY || type == TYPE_BOXNORMAL || type == TYPE_BOXDAMAGE)
			{// �G�A�{�X
				// �I������
				pObject->Uninit();
			}

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			if (pObject->m_bDeath == true)
			{// ���S�t���O�������Ă���
				// ���X�g����폜���Ĕj������
				if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�ł��Ō���ł��Ȃ�
					pObject->m_pPrev->m_pNext = pObject->m_pNext;		// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�̃|�C���^�Ɏ��̃I�u�W�F�N�g��ۑ�����
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;		// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g�̃|�C���^�ɑO�̃I�u�W�F�N�g��ۑ�����
				}
				else if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] == pObject)
				{// ���g���擪�ł͂Ȃ����Ō���ł���
					m_apCur[nCntPriority] = pObject->m_pPrev;		// �Ō����O�̃I�u�W�F�N�g�ɐݒ肷��
				}
				else if (m_apTop[nCntPriority] == pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�����Ō���ł͂Ȃ�
					m_apTop[nCntPriority] = pObject->m_pNext;		// �擪�����̃I�u�W�F�N�g�ɐݒ肷��
				}

				if (pObject == m_apTop[nCntPriority])
				{// �擪�ƈ�v
					m_apTop[nCntPriority] = nullptr;
				}
				if (pObject == m_apCur[nCntPriority])
				{// �Ō���ƈ�v
					m_apCur[nCntPriority] = nullptr;
				}

				// ���������J��
				delete pObject;
				pObject = nullptr;
			}

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}
}

//===============================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//===============================================
void CObject::UpdateAll()
{
	if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetStateReady() == false) 
		|| (CManager::GetMode() != CScene::MODE_GAME))
	{// �ҋ@���
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

			while (pObject != nullptr)
			{// �|�C���^���g�p����Ă���
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

				// �X�V����
				pObject->Update();

				pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
			}
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			if (pObject->m_bDeath == true)
			{// ���S�t���O�������Ă���
				// ���X�g����폜���Ĕj������
				if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] == pObject)
				{// ���g���擪�ł͂Ȃ����Ō���ł���
					m_apCur[nCntPriority] = pObject->m_pPrev;		// �Ō����O�̃I�u�W�F�N�g�ɐݒ肷��
					pObject->m_pPrev->m_pNext = nullptr;				// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���폜
				}
				else if (m_apTop[nCntPriority] == pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�����Ō���ł͂Ȃ�
					m_apTop[nCntPriority] = pObject->m_pNext;		// �擪�����̃I�u�W�F�N�g�ɐݒ肷��
					pObject->m_pNext->m_pPrev = nullptr;				// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g���폜
				}
				else if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] != pObject)
				{// ���g���擪�ł��Ō���ł��Ȃ�
					pObject->m_pPrev->m_pNext = pObject->m_pNext;		// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g�̃|�C���^�Ɏ��̃I�u�W�F�N�g��ۑ�����
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;		// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g�̃|�C���^�ɑO�̃I�u�W�F�N�g��ۑ�����
				}

				if (pObject == m_apTop[nCntPriority])
				{// �擪�ƈ�v
					m_apTop[nCntPriority] = nullptr;
				}
				if(pObject == m_apCur[nCntPriority])
				{// �Ō���ƈ�v
					m_apCur[nCntPriority] = nullptr;
				}

				// ���������J��
				delete pObject;
				pObject = nullptr;
			}

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}
}

//===============================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//===============================================
void CObject::DrawAll()
{
	CCamera *m_pCamera = CManager::GetInstance()->GetCamera();	// �J�����ւ̃|�C���^���擾

	if (m_pCamera != nullptr)
	{// �J�������g�p����Ă���
		// �J�����̐ݒ菈��
		m_pCamera->Set();
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// �擪�̃I�u�W�F�N�g����

		while (pObject != nullptr)
		{// �|�C���^���g�p����Ă���
			CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			if (pObject->m_type != TYPE_PAUSE || CGame::GetPauseState() == true && CGame::GetPauseCamera() == false)
			{// �I�u�W�F�N�g�̎�ނ��|�[�Y����Ȃ� �������̓|�[�Y��Ԃ̂Ƃ�
				// �`�揈��
				pObject->Draw();
			}

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}
}

//===============================================
// �I�u�W�F�N�g�̔j��
//===============================================
void CObject::Release()
{
	m_bDeath = true;	// �����ɔj�������Ɏ��S�t���O�𗧂Ă�

	m_nNumAll--;	//�������J�E���g�_�E��
}

//===============================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//===============================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}