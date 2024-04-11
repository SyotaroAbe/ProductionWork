//=========================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECT_H_     // ���̃}�N����`������ĂȂ�������
#define _OBJECT_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"

//===============================================
// �O���錾
//===============================================
class CShadow;

//===============================================
// �}�N����`
//===============================================
#define MAX_OBJECT		(2048)		// �ő�I�u�W�F�N�g��
#define PRIORITY_MAX	(8)			// �D�揇�ʂ̑���

//===============================================
// �I�u�W�F�N�g�N���X
//===============================================
class CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObject();						// �f�t�H���g�R���X�g���N�^
	CObject(int nPriority = 3);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObject();				// �f�X�g���N�^

	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		TYPE_NONE = 0,	// �Ȃ�
		TYPE_PLAYER,	// �v���C���[
		TYPE_ENEMY,		// �G
		TYPE_BLOCK,		// �u���b�N
		TYPE_BULLET,	// �e
		TYPE_EXPLOSION,	// ����
		TYPE_ITEM,		// �A�C�e��
		TYPE_BG,		// �w�i
		TYPE_EFFECT,	// �G�t�F�N�g
		TYPE_FIELD,		// ��
		TYPE_WALL,		// ��
		TYPE_BILLBOARD,	// �r���{�[�h
		TYPE_TARGET,	// �^�[�Q�b�g
		TYPE_ROCKON,	// ���b�N�I��
		TYPE_XFAIL,		// X�t�@�C��
		TYPE_GARBAGE,	// ����
		TYPE_DUMPSTER,	// �S�~�X�e�[�V����
		TYPE_SHADOW,	// �e
		TYPE_SCENE,		// �V�[��
		TYPE_PAUSE,		// �|�[�Y
		TYPE_RANKIN,	// �����N�C��
		TYPE_MAX
	}TYPE;

	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void Reset(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static CObject *GetTop(int nPriority) { return m_apTop[nPriority]; }
	CObject *GetNext(void) { return m_pNext; }
	void SetType(const TYPE type);
	TYPE GetType(void) { return m_type; }

	virtual void SetPos(const D3DXVECTOR3 pos) = 0;
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetPosOld(void) = 0;
	virtual void SetMove(const D3DXVECTOR3 move) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual D3DXVECTOR3 GetSize(void) = 0;

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	void Release(void);

	CShadow *m_pShadow;		// �e�ւ̃|�C���^

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CObject *m_apTop[PRIORITY_MAX];			// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_apCur[PRIORITY_MAX];			// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pPrev;								// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;								// ���̃I�u�W�F�N�g�ւ̃|�C���^

	TYPE m_type;				// �I�u�W�F�N�g�̎��
	static int m_nNumAll;		// �I�u�W�F�N�g�̑���
	int m_nPriority;			// �D�揇�ʂ̈ʒu
	bool m_bDeath;				// ���S�t���O
};

#endif