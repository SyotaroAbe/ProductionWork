//=========================================================
//
// �t�B�[�o�[�`�揈�� [fever.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _FEVER_H_     // ���̃}�N����`������ĂȂ�������
#define _FEVER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �t�B�[�o�[�`��N���X
//===============================================
class CFever : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CFever();						// �f�t�H���g�R���X�g���N�^
	CFever(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CFever();						// �f�X�g���N�^

	static CFever *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetEndAnim(void) { return m_bEndAnim; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	bool m_bEndAnim;					// �A�j���[�V�������I��������
};

#endif