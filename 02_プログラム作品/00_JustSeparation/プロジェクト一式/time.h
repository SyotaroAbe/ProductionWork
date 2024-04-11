//=========================================================
//
// �^�C������ [time.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _TIME_H_     // ���̃}�N����`������ĂȂ�������
#define _TIME_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "number.h"

//===============================================
// �^�C���N���X
//===============================================
class CTime
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CTime();		// �f�t�H���g�R���X�g���N�^
	~CTime();		// �f�X�g���N�^

	static CTime *Create(int nPriority = 3);

	HRESULT Init(int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nTime);
	int Get(void) { return m_nTime; }

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CNumber *m_apNumber[MAX_NUMBER];		// �i���o�[�N���X�̃|�C���^
	static int m_nIdxTexture;					// �g�p����e�N�X�`���̔ԍ�
	float m_fSizeX;								// ��
	float m_fSizeY;								// ����
	static int m_nTime;							// �^�C���̒l
	DWORD m_dwTime;								// �^�C�����擾
	DWORD m_dwtime;								// �^�C�����擾

	bool m_bFade;								// �t�F�[�h�ɓ�������
};

#endif