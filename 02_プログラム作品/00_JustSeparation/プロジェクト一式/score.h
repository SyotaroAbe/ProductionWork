//=========================================================
//
// �X�R�A���� [score.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _SCORE_H_     // ���̃}�N����`������ĂȂ�������
#define _SCORE_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "number.h"

//===============================================
// �X�R�A�N���X
//===============================================
class CScore
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CScore();					// �f�t�H���g�R���X�g���N�^
	CScore(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CScore();					// �f�X�g���N�^

	static CScore *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nScore);
	int Get(void) { return m_nScore; }
	void Add(const int nScore);

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CNumber *m_apNumber[MAX_NUMBER];		// �i���o�[�N���X�̃|�C���^
	static int m_nIdxTexture;					// �g�p����e�N�X�`���̔ԍ�
	float m_fSizeX;								// ��
	float m_fSizeY;								// ����
	static int m_nScore;						// �X�R�A�̒l
};

#endif