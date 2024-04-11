//=========================================================
//
// ���f���`�揈�� [model.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MODEL_H_  // ���̃}�N����`������ĂȂ�������
#define _MODEL_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//===============================================
//�}�N����`
//===============================================
#define ROT_DIFF_PI			(2.0f)	// �p�x�̒l�̕␳
#define ROT_CAMERA			(1.0f)	// �J�����̌����ɍ��킹�ē�����

//===============================================
// ���f���N���X
//===============================================
class CModel
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CModel();						// �f�t�H���g�R���X�g���N�^
	virtual ~CModel();				// �f�X�g���N�^

	static CModel *Create(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	virtual HRESULT Init(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawShadowmtx(void);
	
	void BindX(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat);
	void SetVtxSize(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetPosOri(void) { return m_posOri; }
	void SetParent(CModel *pModel);

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	LPDIRECT3DTEXTURE9 *m_apTexture;			// �e�N�X�`���ւ̃|�C���^
	int *m_apIdxTexture;						// �g�p����e�N�X�`���̔ԍ�
	LPD3DXMESH m_pMesh;							// ���b�V���i���_���j�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;					// �}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;							// �}�e���A���̐�
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOri;	// �����ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_vtxMax;	// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;	// ���f���̍ŏ��l
	CModel *m_pParent;		// �e���f���̃|�C���^
};

#endif