//=========================================================
//
// �G���� [enemy.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _ENEMY_H_     // ���̃}�N����`������ĂȂ�������
#define _ENEMY_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �}�N����`
//===============================================
#define ENEMY_SIZEX		(30)								// ���i�����j
#define ENEMY_SIZEY		(30)								// �����i�����j
#define ENEMY_SCORE		(500)								// �G�̃X�R�A

//===============================================
// �G�N���X
//===============================================
class CEnemy : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CEnemy();						// �f�t�H���g�R���X�g���N�^
	CEnemy(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEnemy();						// �f�X�g���N�^

	static CEnemy *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
};

#endif