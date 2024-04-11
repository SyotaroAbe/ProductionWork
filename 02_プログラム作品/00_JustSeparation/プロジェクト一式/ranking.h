//=========================================================
//
// ランキング処理 [ranking.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _RANKING_H_     // このマクロ定義がされてなかったら
#define _RANKING_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "number.h"
#include "score.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_RANK		(6)				// 表示するランキング数
#define YOUR_SCORE		(5)				// 今回のスコア

//===============================================
// ランキングクラス
//===============================================
class CRanking
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CRanking();						// デフォルトコンストラクタ
	CRanking(int nPriority = 5);	// オーバーロードされたコンストラクタ
	~CRanking();					// デストラクタ

	static CRanking *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(void);
	void Add(const int nScore);

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CNumber *m_apNumber[MAX_RANK][MAX_NUMBER];		// ナンバークラスのポインタ
	static int m_nIdxTexture;								// 使用するテクスチャの番号

	float m_fSizeX;											// 幅
	float m_fSizeY;											// 高さ
	int m_aScore[MAX_RANK];									// スコアの値
	int m_nYourRank;										// 今回のランク

	float m_fBrightness;				// 明るさの値
	bool m_bFlash;						// 明るくするか暗くするか
};

#endif