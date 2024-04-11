//=========================================================
//
// �I�u�W�F�N�gX�t�@�C������ [objectX.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECTX_H_  // ���̃}�N����`������ĂȂ�������
#define _OBJECTX_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"
#include "xfile.h"

//===============================================
// �I�u�W�F�N�gX�t�@�C���N���X
//===============================================
class CObjectX : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObjectX();						// �f�t�H���g�R���X�g���N�^
	CObjectX(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectX();			// �f�X�g���N�^

	// ���f���̎��
	typedef enum
	{
		MODEL_NONE = 0,			// �Ȃ�
		MODEL_HOUSE00,			// �ƂP
		MODEL_HOUSE01,			// �ƂQ
		MODEL_MAX
	}MODEL;

	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);
	static void Load(HWND hWnd);
	static void Script(FILE *pFile);
	static void ModelSet(FILE *pFile);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetSize(void);
	void SetModel(MODEL type);

protected:	// �h���N���X������A�N�Z�X�ł���
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxXFile[MODEL_MAX];			// �g�p����X�t�@�C���̔ԍ�
	
	MODEL m_modelType;							// ���f���̎��
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nIdxShadow;							// �g�p����e�̔ԍ�
};

#endif