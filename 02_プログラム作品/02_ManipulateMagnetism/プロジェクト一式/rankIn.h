//=========================================================
//
// ランクイン描画処理 [rankIn.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _RANKIN_H_  // このマクロ定義がされてなかったら
#define _RANKIN_H_  // ２重インクルード防止のマクロを定義する

#include "object2D.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_COLA		(1.0f)		// 最大アルファ値
#define MIN_COLA		(0.1f)		// 最小アルファ値
#define COL_DARK		(-0.025f)	// 少しずつ暗くする
#define COL_LIGHT		(0.025f)	// 少しずつ明るくする

//===============================================
// ランクインクラス
//===============================================
class CRankIn : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CRankIn();						// デフォルトコンストラクタ
	CRankIn(int nPriority = 6);		// オーバーロードされたコンストラクタ
	~CRankIn();						// デストラクタ

	static CRankIn *Create(int nPriority = 6);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nRank);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_pIdxTexture;			// 使用するテクスチャの番号
	float m_fBrightness;				// 明るさの値
	bool m_bFlash;						// 明るくするか暗くするか
	int m_nRank;						// ランキングの位置
};

#endif