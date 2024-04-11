//=========================================================
//
// �e���� [bullet.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _BULLET_H_     // ���̃}�N����`������ĂȂ�������
#define _BULLET_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"
#include "objectBillboard.h"

//===============================================
// �e�N���X
//===============================================
class CBullet : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CBullet();						// �f�t�H���g�R���X�g���N�^
	CBullet(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CBullet();						// �f�X�g���N�^

	static CBullet *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	bool CollisionEnemy(D3DXVECTOR3 pos);

	static int GetNumAll(void){ return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	int m_nLife;						// �̗�
	static int m_nNumAll;				// �e�̑���
};

#endif