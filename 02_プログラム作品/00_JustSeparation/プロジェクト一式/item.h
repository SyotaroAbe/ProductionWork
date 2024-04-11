//=========================================================
//
// �A�C�e������ [item.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _ITEM_H_     // ���̃}�N����`������ĂȂ�������
#define _ITEM_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �A�C�e���N���X
//===============================================
class CItem : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CItem();		// �f�t�H���g�R���X�g���N�^
	~CItem();		// �f�X�g���N�^

	static CItem *Create(D3DXVECTOR3 pos, int nPriority = 3);

	// �A�C�e���̎��
	typedef enum
	{
		TYPE_TREASURE = 0,	// ��
		TYPE_COIN,			// �R�C��
		TYPE_MAX
	}TYPE;

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const TYPE type, const D3DXVECTOR3 pos);

	bool CollisionEnemy(D3DXVECTOR3 pos);

	static int GetNumAll(void) { return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxTexture[TYPE_MAX];						// �g�p����e�N�X�`���̔ԍ�
	static const char *m_apTextureData[TYPE_MAX];			// �e�N�X�`���t�@�C����
	static int m_nNumAll;									// �A�C�e���̑���
	TYPE m_type;											// ���
};

#endif