//=========================================================
//
// �e�̕`�揈�� [shadow.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _SHADOW_H_  // ���̃}�N����`������ĂȂ�������
#define _SHADOW_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

//===============================================
// �e�N���X
//===============================================
class CShadow : public CObject3D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CShadow();					// �f�t�H���g�R���X�g���N�^
	CShadow(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CShadow();					// �f�X�g���N�^

	static CShadow *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:	// �h���N���X������A�N�Z�X�ł���

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
};

#endif