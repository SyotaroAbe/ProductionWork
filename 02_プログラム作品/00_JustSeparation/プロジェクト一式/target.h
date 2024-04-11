//=========================================================
//
// �^�[�Q�b�g���� [target.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _TARGET_H_     // ���̃}�N����`������ĂȂ�������
#define _TARGET_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "objectBillboard.h"
#include "garbage.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_TARGET		(256)			// �^�[�Q�b�g�̍ő吔
#define TARGET_SIZEX	(45.0f)			// ���i�����j
#define TARGET_SIZEY	(45.0f)			// �����i�����j
#define FLAME_JUSTDUST	(7)				// JUSTDUST�͈̔�

//===============================================
// �^�[�Q�b�g�N���X
//===============================================
class CTarget : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CTarget();						// �f�t�H���g�R���X�g���N�^
	CTarget(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CTarget();						// �f�X�g���N�^

	// �^�[�Q�b�g�̏��
	typedef enum
	{
		STATE_NONE = 0,		// �Ȃ�
		STATE_ROCKON,		// ���b�N�I��
		STATE_MAX
	}STATE;

	static CTarget *Create(D3DXVECTOR3 pos, CGarbage::SEPARATION state, int nID, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool WithinRangeGarbage(bool bGarbage = true);
	void RevisionPos(void);

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);
	void SetState(CGarbage::SEPARATION state);
	static CGarbage::SEPARATION GetState(void) { return m_state; }
	void SetMaxGage(bool bMaxGage);
	bool GetWithinRange(void) { return m_bWithinRange; }
	int GetFlameJust(void) { return m_nCntFlameJust; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;					// �g�p����e�N�X�`���̔ԍ�
	static int m_nCntObjTarget[MAX_TARGET];		// �^�[�Q�b�g���Ă���I�u�W�F�N�g�̔ԍ��ۑ�
	static CGarbage::SEPARATION m_state;		// �^�[�Q�b�g�̏��

	CObject *m_pColObj;					// �����蔻����Ƃ�I�u�W�F�N�g
	D3DXVECTOR3 m_LocalPos;				// �����ʒu���猩�Ă̋���
	D3DXVECTOR3 m_posDest;				// �ړI�̈ʒu
	bool m_bWithinRange;				// �S�~���^�[�Q�b�g�͈͓̔�
	bool m_bMaxGage;					// �Q�[�W���ő傩
	float m_fMaltiRivision;				// �ʒu�␳�l
	float m_fSpeed;						// �ړ����x
	int m_nCntFlameJust;				// �W���X�g�^�C�~���O�t���[���J�E���g
};

#endif