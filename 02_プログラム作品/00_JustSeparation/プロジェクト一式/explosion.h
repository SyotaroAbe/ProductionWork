//=========================================================
//
// �������� [explosion.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _EXPLOSION_H_     // ���̃}�N����`������ĂȂ�������
#define _EXPLOSION_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �����N���X
//===============================================
class CExplosion : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CExplosion();						// �f�t�H���g�R���X�g���N�^
	CExplosion(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CExplosion();						// �f�X�g���N�^

	static CExplosion *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
	int m_nCounterAnim;			// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;			// �A�j���[�V�����p�^�[��No.
};

#endif