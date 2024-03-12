//=========================================================
//
// �i���o�[���� [number.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _NUMBER_H_     // ���̃}�N����`������ĂȂ�������
#define _NUMBER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"

//===============================================
// �O���錾
//===============================================
class CObject2D;

//===============================================
// �}�N����`
//===============================================
#define MAX_NUMBER		(1024)		// �ő�i���o�[��

#define PATTERNANIM_X		(8)									// �A�j�����p�^�[����
#define PATTERNANIM_Y		(1)									// �A�j���c�p�^�[����
#define PATTERNANIM_ALL		(PATTERNANIM_X * PATTERNANIM_Y)		// �A�j���c�p�^�[����
#define SPEED_ANIM			(10)								// �A�j���[�V�����X�s�[�h

//===============================================
// �i���o�[�N���X
//===============================================
class CNumber
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CNumber();					// �f�t�H���g�R���X�g���N�^
	CNumber(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CNumber();			// �f�X�g���N�^

	// �i���o�[�̎��
	typedef enum
	{
		TYPE_NONE = 0,	// �Ȃ�
		TYPE_TIME,		// �^�C��
		TYPE_SCORE,		// �X�R�A
		TYPE_MAX
	}TYPE;

	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetType(const TYPE type);
	TYPE GetType(void) { return m_type; }

	void BindTexture(int nIdx);
	void Set(const int nNumber);
	void SetPos(const D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetCol(const D3DXCOLOR col);

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	D3DXVECTOR3 m_pos;					// �ʒu
	int m_nCounterAnim;					// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;					// �A�j���[�V�����p�^�[��No.

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObject2D *m_pObject2D;			// �I�u�W�F�N�g2D�N���X�̃|�C���^
	TYPE m_type;					// �i���o�[�̎��
};

#endif