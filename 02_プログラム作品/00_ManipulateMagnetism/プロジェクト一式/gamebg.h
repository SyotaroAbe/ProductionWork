//=========================================================
//
// ���C�����[�h�w�i���� [gamebg.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GAMEBG_H_     // ���̃}�N����`������ĂȂ�������
#define _GAMEBG_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "objectBillboard.h"

//===============================================
// �w�i�N���X
//===============================================
class CGameBg : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CGameBg();						// �f�t�H���g�R���X�g���N�^
	CGameBg(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CGameBg();						// �f�X�g���N�^

	// �e�N�X�`���̎��
	enum ETex
	{
		TEX_GAME = 0,	// �Q�[��
		TEX_TUTORIAL,	// �`���[�g���A��
		TEX_MAX
	};

	static CGameBg *Create(D3DXVECTOR3 pos, ETex tex, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, ETex tex);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	ETex m_tex;							// �e�N�X�`���̎��
};

#endif