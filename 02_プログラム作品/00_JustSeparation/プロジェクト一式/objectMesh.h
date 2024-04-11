//=========================================================
//
// �I�u�W�F�N�g���b�V������ [objectMesh.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECTMESH_H_  // ���̃}�N����`������ĂȂ�������
#define _OBJECTMESH_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//===============================================
// �I�u�W�F�N�g���b�V���N���X
//===============================================
class CObjectMesh : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObjectMesh();					// �f�t�H���g�R���X�g���N�^
	CObjectMesh(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectMesh();			// �f�X�g���N�^

	// �F�̎��
	enum COL
	{
		COL_NORMAL = 0,		// �ʏ�
		COL_RED,			// ��
		COL_BLUE,			// ��
		COL_GREEN,			// ��
		COL_MAX
	};

	static CObjectMesh *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(int nIdx);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeZ(void) { return m_fSizeZ; }

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fSizeX, const float fSizeZ);
	void SetCol(const COL col);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }

protected:	// �h���N���X������A�N�Z�X�ł���
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pldxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_rot;							// ����
	int m_nCntMeshX;							// ���_���i���j
	int m_nCntMeshY;							// ���_���i�c�j
	float m_fSizeX;								// ��
	float m_fSizeZ;								// ���s��

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
};

#endif