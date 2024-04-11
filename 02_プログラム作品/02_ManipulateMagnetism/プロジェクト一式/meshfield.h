//=========================================================
//
// ���b�V���t�B�[���h�`�揈�� [meshfield.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MESHFIELD_H_  // ���̃}�N����`������ĂȂ�������
#define _MESHFIELD_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectMesh.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_FIELD				(36)		// �ő台��

//===============================================
// ���b�V���t�B�[���h�N���X
//===============================================
class CMeshField : CObjectMesh
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMeshField();					// �f�t�H���g�R���X�g���N�^
	CMeshField(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CMeshField();					// �f�X�g���N�^

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void TurnInfo(float fRot);

	int GetNumAll(void) { return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;					// �g�p����e�N�X�`���̔ԍ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	static TYPE m_aType[MAX_FIELD];						// ���̎��
	static int m_nNumAll;								// ����

	D3DXVECTOR3 m_PosMin;				// �ʒu�̍ŏ��l
	D3DXVECTOR3 m_PosMax;				// �ʒu�̍ő�l

	int m_nTimeFever;				// �t�B�[�o�[��Ԃ̎���
	int m_nRandFever;				// �t�B�[�o�[��Ԃ̃����_��
};

#endif