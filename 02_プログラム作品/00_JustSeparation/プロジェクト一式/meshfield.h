//=========================================================
//
// ���b�V���t�B�[���h�`�揈�� [meshfield.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MESHFIELD_H_  // ���̃}�N����`������ĂȂ�������
#define _MESHFIELD_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectMesh.h"
#include "fileload.h"

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

	// ���̎��
	enum TYPE
	{
		TYPE_STRAIGHT_H = 0,	// �c����
		TYPE_STRAIGHT_W,		// ������
		TYPE_CURVE_UPLEFT,		// �J�[�u�i����p�j
		TYPE_CURVE_UPRIGHT,		// �J�[�u�i�E��p�j
		TYPE_CURVE_DOWNRIGHT,	// �J�[�u�i�E���p�j
		TYPE_CURVE_DOWNLEFT,	// �J�[�u�i�����p�j
		TYPE_TJUNCTION_UP,		// T���H�i��j
		TYPE_TJUNCTION_RIGHT,	// T���H�i�E�j
		TYPE_TJUNCTION_DOWN,	// T���H�i���j
		TYPE_TJUNCTION_LEFT,	// T���H�i���j
		TYPE_CROSSROADS,		// �\���H
		TYPE_DEADEND_UP,		// �s���~�܂�i��j
		TYPE_DEADEND_RIGHT,		// �s���~�܂�i�E�j
		TYPE_DEADEND_DOWN,		// �s���~�܂�i���j
		TYPE_DEADEND_LEFT,		// �s���~�܂�i���j
		//TYPE_STRAIGHT_H_S,		// �c����
		//TYPE_STRAIGHT_W_S,		// ������
		TYPE_MAX
	};

	// ��ޕʔ���
	enum DESTINATION
	{
		DESTINATION_NONE = 0,				// �Ȃ�
		DESTINATION_STRAIGHT_CURVE,			// �^�������s���ĉE���Ȃ��邱�Ƃ��ł���
		DESTINATION_STRAIGHT_RIGHT,			// �^�������s���ĉE�̂݋Ȃ��邱�Ƃ��ł���
		DESTINATION_STRAIGHT_LEFT,			// �^�������s���č��̂݋Ȃ��邱�Ƃ��ł���
		DESTINATION_STRAIGHT,				// �^�������s���邪�Ȃ���Ȃ�
		DESTINATION_CURVE,					// �^�����������Ȃ����E���Ȃ����
		DESTINATION_RIGHT,					// �^�����������Ȃ����E�ɂ͋Ȃ����
		DESTINATION_LEFT,					// �^�����������Ȃ������ɂ͋Ȃ����
		DESTINATION_DEAD_END,				// �^�����������Ȃ����Ȃ���Ȃ��i�܂�Ԃ��j
		DESTINATION_MAX
	};

	// �S�~�̔z�u�ʒu
	enum GABAGEPOS
	{
		GABAGEPOS_CENTER = 0,	// �^��
		GABAGEPOS_EDGE,			// �[
		GABAGEPOS_SIDE,			// ������
		GABAGEPOS_MAX
	};

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, TYPE nType, int nPriority = 3);
	static void Load(HWND hWnd);
	static void Unload(void);
	static void Script(FILE *pFile);
	static void FieldSet(FILE *pFile);

	static void RandArrange(void);
	static int NextField(int type, int prevTypeX, int prevTypeY, int nCntPrevX);
	static void ConnectCheck(int type, int prevTypeX, int prevTypeY, int nCntPrevX);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	void TurnInfo(float fRot);
	void TurnInput(DESTINATION destination);

	TYPE GetType(void) { return m_type; }
	int GetNumAll(void) { return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static char m_aFileName[TYPE_MAX][MAX_NAME];		// �e�N�X�`���t�@�C������ۑ�
	static int m_nIdxTexture[TYPE_MAX];					// �g�p����e�N�X�`���̔ԍ�
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	// �e�N�X�`���ւ̃|�C���^
	static TYPE m_aType[MAX_FIELD];						// ���̎��
	static int m_nNumAll;								// ����

	D3DXVECTOR3 m_PosMin;				// �ʒu�̍ŏ��l
	D3DXVECTOR3 m_PosMax;				// �ʒu�̍ő�l

	TYPE m_type;					// ���
	DESTINATION m_destination;		// ��ޕʔ���
	int m_nTimeFever;				// �t�B�[�o�[��Ԃ̎���
	int m_nRandFever;				// �t�B�[�o�[��Ԃ̃����_��
};

#endif