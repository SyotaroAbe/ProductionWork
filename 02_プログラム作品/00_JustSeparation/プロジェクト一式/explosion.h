//=========================================================
//
// 爆発処理 [explosion.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _EXPLOSION_H_     // このマクロ定義がされてなかったら
#define _EXPLOSION_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// 爆発クラス
//===============================================
class CExplosion : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CExplosion();						// デフォルトコンストラクタ
	CExplosion(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CExplosion();						// デストラクタ

	static CExplosion *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
	int m_nCounterAnim;			// アニメーションカウンター
	int m_nPatternAnim;			// アニメーションパターンNo.
};

#endif