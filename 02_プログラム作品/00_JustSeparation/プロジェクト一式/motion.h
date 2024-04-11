//=========================================================
//
// ���[�V�������� [motion.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MOTION_H_     // ���̃}�N����`������ĂȂ�������
#define _MOTION_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_FILE	(20)	// �ő�t�@�C����
#define MAX_KEY		(5)		// �L�[�̍ő吔
#define MAX_MOTION	(6)		// ���[�V�����ő吔

//===============================================
// �O���錾
//===============================================
class CModel;

//===============================================
// ���[�V�����N���X
//===============================================
class CMotion
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMotion();						// �f�t�H���g�R���X�g���N�^
	~CMotion();						// �f�X�g���N�^

	//�L�[�̍\����
	typedef struct
	{
		float fPosX;		// �ʒuX
		float fPosY;		// �ʒuY
		float fPosZ;		// �ʒuZ
		float fRotX;		// ����X
		float fRotY;		// ����Y
		float fRotZ;		// ����Z
		float fPosDiffX;	// �ړI�̈ʒu�܂ł̍���X
		float fPosDiffY;	// �ړI�̈ʒu�܂ł̍���Y
		float fPosDiffZ;	// �ړI�̈ʒu�܂ł̍���Z
		float fRotDiffX;	// �ړI�̌����܂ł̍���X
		float fRotDiffY;	// �ړI�̌����܂ł̍���Y
		float fRotDiffZ;	// �ړI�̌����܂ł̍���Z
	}KEY;

	// �L�[���̍\����
	typedef struct
	{
		int nFrame;			// �Đ��t���[��
		KEY aKey[MAX_FILE];	// �e���f���̃L�[�v�f
	}KEY_INFO;

	// ���[�V�������̍\����
	typedef struct
	{
		bool bLoop;					// ���[�v���邩�ǂ���
		int nNumKey;				// �L�[�̑���
		KEY_INFO aKeyInfo[MAX_KEY];	// �L�[���
	}INFO;

	HRESULT Init(void);
	void Uninit(void);
	void Set(int nType);
	void Update(void);
	int GetType(void) { return m_nType; }
	bool IsFinish(void) { return m_bFinish; }
	void SetInfo(INFO info);
	void SetModel(CModel **ppModel, int nNumModel);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	INFO m_aInfo[MAX_MOTION];	// ���[�V�������
	int m_nNumAll;				// ���[�V�����̑���
	int m_nType;				// ���[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nNumKey;				// �L�[�̑���
	int m_nKey;					// ���݂̃L�[No.
	int m_nCounter;				// ���[�V�����̃J�E���^�[
	bool m_bFinish;				// �I��������ǂ���

	CModel **m_ppModel;			// ���f���ւ̃|�C���^
	int m_nNumModel;			// ���f���̑���
};

#endif