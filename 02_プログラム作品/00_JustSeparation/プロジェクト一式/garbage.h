//=========================================================
//
// ごみ処理 [garbage.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _GARBAGE_H_     // このマクロ定義がされてなかったら
#define _GARBAGE_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "objectX.h"
#include "particle.h"

//===============================================
// ごみクラス
//===============================================
class CGarbage : public CObjectX
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CGarbage();						// デフォルトコンストラクタ
	CGarbage(int nPriority = 3);	// オーバーロードされたコンストラクタ
	~CGarbage();					// デストラクタ

	// ゴミの種類
	enum TYPE
	{
		// 燃えるゴミ
		TYPE_BOOKOLD = 0,			// 本
		TYPE_BOOT,					// 長靴
		TYPE_BAG,					// 鞄
		// 燃えないゴミ
		TYPE_LIGHT,					// ライト
		TYPE_CAMERA,				// カメラ
		TYPE_BUCKET,				// バケツ
		// 資源ゴミ
		TYPE_BIN,					// 瓶
		TYPE_CAN,					// 缶
		TYPE_DANBORU,				// 段ボール

		TYPE_MAX
	};

	// サイズ
	enum MODELSIZE
	{
		MODELSIZE_NORMAL = 0,	// 通常
		MODELSIZE_SMALL,		// 小さい
		MODELSIZE_BIG,			// 大きい
		MODELSIZE_MAX
	};

	// ゴミの分別
	enum SEPARATION
	{
		SEPARATION_NONE = 0,		// なし
		SEPARATION_BURN,			// 燃えるゴミ
		SEPARATION_NONFLAMMABLE,	// 燃えないゴミ
		SEPARATION_RECYCLABLE,		// 資源ゴミ
		SEPARATION_MAX
	};

	// ゴミの状態
	enum STATE
	{
		STATE_NONE = 0,		// なし
		STATE_NORMAL,		// 通常状態
		STATE_ROCKON,		// ロックオン
		STATE_SEPARATION,	// 分別
		STATE_DISPOSE,		// 捨てる
		STATE_COLLECT,		// 回収
		STATE_INPLAYER,		// 描画しない
		STATE_DUMPSTER,		// ゴミステーション行き
		STATE_DELETE,		// 破棄
		STATE_MAX
	};

	static HRESULT Load(void);
	static void Unload(void);
	static CGarbage *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Separation(SEPARATION separation);

	D3DXVECTOR3 GetMove(void) { return m_move; }
	int GetNumAll(void) { return m_nNumAll; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxXFile[TYPE_MAX];		// 使用するXファイルの番号
	static int m_nNumAll;					// 総数
	static int m_nRandCounter;				// randリセット用

	CParticle::TYPE particleType;			// パーティクルの種類
	D3DXVECTOR3 m_posDest;					// 目的の位置
	D3DXVECTOR3 m_posDiff;					// 目的の位置までの差分
	D3DXVECTOR3 m_nextposDest;				// 次の目的の位置
	TYPE m_type;							// ゴミの種類
	MODELSIZE m_size;						// モデルのサイズ
	STATE m_state;							// ゴミの状態
	SEPARATION m_separation;				// 分別の種類
	SEPARATION m_rockonType;				// ロックオン時の分別
	bool m_bLand;							// 地についているかどうか
	bool m_bJustDust;						// JustDustかどうか
	float m_fMoveDiff;						// 位置の補正係数
	int m_nWaitCounter;						// 静止している時間
	int m_nIdxDumpster;						// ゴミステーションの番号
};

#endif