//=========================================================
//
// メッシュフィールド描画処理 [meshfield.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MESHFIELD_H_  // このマクロ定義がされてなかったら
#define _MESHFIELD_H_  // ２重インクルード防止のマクロを定義する

#include "objectMesh.h"
#include "fileload.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_FIELD				(36)		// 最大床数

//===============================================
// メッシュフィールドクラス
//===============================================
class CMeshField : CObjectMesh
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CMeshField();					// デフォルトコンストラクタ
	CMeshField(int nPriority = 3);	// オーバーロードされたコンストラクタ
	~CMeshField();					// デストラクタ

	// 床の種類
	enum TYPE
	{
		TYPE_STRAIGHT_H = 0,	// 縦直線
		TYPE_STRAIGHT_W,		// 横直線
		TYPE_CURVE_UPLEFT,		// カーブ（左上角）
		TYPE_CURVE_UPRIGHT,		// カーブ（右上角）
		TYPE_CURVE_DOWNRIGHT,	// カーブ（右下角）
		TYPE_CURVE_DOWNLEFT,	// カーブ（左下角）
		TYPE_TJUNCTION_UP,		// T字路（上）
		TYPE_TJUNCTION_RIGHT,	// T字路（右）
		TYPE_TJUNCTION_DOWN,	// T字路（下）
		TYPE_TJUNCTION_LEFT,	// T字路（左）
		TYPE_CROSSROADS,		// 十字路
		TYPE_DEADEND_UP,		// 行き止まり（上）
		TYPE_DEADEND_RIGHT,		// 行き止まり（右）
		TYPE_DEADEND_DOWN,		// 行き止まり（下）
		TYPE_DEADEND_LEFT,		// 行き止まり（左）
		//TYPE_STRAIGHT_H_S,		// 縦直線
		//TYPE_STRAIGHT_W_S,		// 横直線
		TYPE_MAX
	};

	// 種類別判定
	enum DESTINATION
	{
		DESTINATION_NONE = 0,				// なし
		DESTINATION_STRAIGHT_CURVE,			// 真っすぐ行けて右左曲がることもできる
		DESTINATION_STRAIGHT_RIGHT,			// 真っすぐ行けて右のみ曲がることができる
		DESTINATION_STRAIGHT_LEFT,			// 真っすぐ行けて左のみ曲がることができる
		DESTINATION_STRAIGHT,				// 真っすぐ行けるが曲がれない
		DESTINATION_CURVE,					// 真っすぐいけないが右左曲がれる
		DESTINATION_RIGHT,					// 真っすぐいけないが右には曲がれる
		DESTINATION_LEFT,					// 真っすぐいけないが左には曲がれる
		DESTINATION_DEAD_END,				// 真っすぐいけないし曲がれない（折り返し）
		DESTINATION_MAX
	};

	// ゴミの配置位置
	enum GABAGEPOS
	{
		GABAGEPOS_CENTER = 0,	// 真ん中
		GABAGEPOS_EDGE,			// 端
		GABAGEPOS_SIDE,			// 横並び
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

private:	// 自分のみアクセス可能 [アクセス指定子]
	static char m_aFileName[TYPE_MAX][MAX_NAME];		// テクスチャファイル名を保存
	static int m_nIdxTexture[TYPE_MAX];					// 使用するテクスチャの番号
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	// テクスチャへのポインタ
	static TYPE m_aType[MAX_FIELD];						// 床の種類
	static int m_nNumAll;								// 総数

	D3DXVECTOR3 m_PosMin;				// 位置の最小値
	D3DXVECTOR3 m_PosMax;				// 位置の最大値

	TYPE m_type;					// 種類
	DESTINATION m_destination;		// 種類別判定
	int m_nTimeFever;				// フィーバー状態の時間
	int m_nRandFever;				// フィーバー状態のランダム
};

#endif