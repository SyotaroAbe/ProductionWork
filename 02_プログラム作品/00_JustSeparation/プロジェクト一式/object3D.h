//=========================================================
//
// �I�u�W�F�N�g3D���� [object3D.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECT3D_H_  // ���̃}�N����`������ĂȂ�������
#define _OBJECT3D_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//===============================================
// �I�u�W�F�N�g3D�N���X
//===============================================
class CObject3D : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObject3D();					// �f�t�H���g�R���X�g���N�^
	CObject3D(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObject3D();			// �f�X�g���N�^

	static CObject3D *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawShadow(void);

	void BindTexture(int nIdx);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeZ(void) { return m_fSizeZ; }

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fSizeX, const float fSizeZ);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }
	float CollisionVec(D3DXVECTOR3 pos);

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_rot;							// ����
	float m_fSizeX;								// ��
	float m_fSizeZ;								// ���s��

	D3DXVECTOR3 m_aVtx[4];							// ���_���W
	D3DXVECTOR3 m_nor;							// �@���x�N�g��
	D3DXVECTOR3 m_vec0, m_vec1;

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;	// �g�p����e�N�X�`���̔ԍ�
};

#endif