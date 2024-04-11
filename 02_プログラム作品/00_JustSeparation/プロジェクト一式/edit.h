//=========================================================
//
// �G�f�B�b�g���� [editor.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _EDITOR_H_  // ���̃}�N����`������ĂȂ�������
#define _EDITOR_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "objectX.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_MODELEDIT		(256)		// �ő�ݒu���f��

//===============================================
// �G�f�B�b�g�N���X
//===============================================
class CEdit
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CEdit();						// �f�t�H���g�R���X�g���N�^
	~CEdit();						// �f�X�g���N�^;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Get(void) { return m_bEdit; }
	void Set(bool bEdit);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_rot;										// ����
	int m_nType;											// �S�~�̎��
	int m_nID;												// ���݂̃I�u�W�F�N�g
	bool m_bEdit;											// �G�f�B�b�g�؂�ւ�
	static CObjectX *m_apObjectX[MAX_MODELEDIT];			// �I�u�W�F�N�gX�N���X�̃|�C���^
};

#endif