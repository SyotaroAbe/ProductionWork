//=========================================================
//
// �u���b�N���� [bullet.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _BLOCK_H_     // ���̃}�N����`������ĂȂ�������
#define _BLOCK_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �}�N����`
//===============================================
#define BLOCK_SIZEX		(60)				// ���i�����j
#define BLOCK_SIZEY		(60)				// �����i�����j

//===============================================
// �u���b�N�N���X
//===============================================
class CBlock : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CBlock();						// �f�t�H���g�R���X�g���N�^
	CBlock(int nPriority = 3);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CBlock();						// �f�X�g���N�^

	static CBlock *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	static bool CollisionPlayer(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY);
	static bool Collision(const TYPE Objtype, const D3DXVECTOR3 *pos, const D3DXVECTOR3 *posOld, const D3DXVECTOR3 *move, const float fSezeX, const float fSezeY);

	static int GetNumAll(void) { return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
	static int m_nNumAll;		// �u���b�N�̑���
};

#endif