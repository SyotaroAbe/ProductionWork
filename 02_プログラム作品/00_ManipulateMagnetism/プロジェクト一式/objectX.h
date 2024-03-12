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
// �}�N����`
//===============================================
#define MAX_NAME	(256)	// �ő啶����

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
		MODEL_NORMAL,			// �ʏ폰
		MODEL_NORMALWIDE,		// �ʏ폰
		MODEL_DAMAGE,			// �_���[�W��
		MODEL_ITEM,				// �A�C�e��
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
	static bool CollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void);
	void SetSizeMin(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSizeMin(void);
	void SetModel(MODEL type);
	int GetXFile(int nModel) { return m_aIdxXFile[nModel]; }

protected:	// �h���N���X������A�N�Z�X�ł���
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxXFile[MODEL_MAX];			// �g�p����X�t�@�C���̔ԍ�
	
	MODEL m_modelType;							// ���f���̎��
	D3DXVECTOR3 m_vtxMax;						// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;						// ���f���̍ŏ��l
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nIdxShadow;							// �g�p����e�̔ԍ�
};

#endif