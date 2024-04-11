//=========================================================
//
// ���b�N�I������ [rockon.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _ROCKON_H_     // ���̃}�N����`������ĂȂ�������
#define _ROCKON_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"
#include "objectBillboard.h"
#include "garbage.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_ROCKON		(256)		// �^�[�Q�b�g�̍ő吔

//===============================================
// ���b�N�I���N���X
//===============================================
class CRockon : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CRockon();						// �f�t�H���g�R���X�g���N�^
	CRockon(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CRockon();						// �f�X�g���N�^

	static CRockon *Create(D3DXVECTOR3 pos, CGarbage::SEPARATION separation, int nID, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	void SetSeparation(CGarbage::SEPARATION separation);
	void SetIDObj(int nID);
	static int GetNumAll(void) { return m_nNumAll; }
	static void SetObjRockon(int nCntRockon, int nCntObj);
	static int GetObjRockon(int nCntRockon) { return m_aObjRockon[nCntRockon]; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;				// �g�p����e�N�X�`���̔ԍ�
	static int m_nNumAll;					// �g�p���Ă���^�[�Q�b�g����
	static int m_aObjRockon[MAX_ROCKON];	// ���b�N�I�����Ă���I�u�W�F�N�g�̔ԍ��ۑ�

	D3DXVECTOR3 m_localPos;							// �����ʒu���猩�Ă̋���
	CGarbage::SEPARATION m_separation;				// �^�[�Q�b�g�̏��
	int m_nIDGarbage;								// �S�~�̔ԍ�
};

#endif